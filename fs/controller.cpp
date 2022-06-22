#include "controller.h"
#include "util.h"

#include <iostream>
#include <vector>



Inode* Controller::FindInode(uint32_t inum)
{
	// ！ 这里应该保证要找的inode是已分配的 

	Inode* slot = nullptr;
	for (auto& i : itable) {
		if (i.ref > 0 && i.inum == inum) {
			i.ref++;
			//std::cout << "INODE " << i.inum << " ref " << i.ref << '\n';
			return &i;   // 找到了
		}
		if (slot == nullptr && i.ref == 0) slot = &i;  // 找到空位
	}

	ASSERT(slot != nullptr);

	// 找到空位 

	slot->inum = inum;
	slot->ref = 1;

	slot->valid = 0; // 在这里不主动读，因为可能是alloc，刚分配的inode的数据没啥意义

	return slot;
}

void Controller::ReadInodeFromDisk(Inode* inode)
{
	ASSERT(inode);
	if (inode->valid) return;

	auto b = GetDisk().ReadBlock(I2B(inode->inum));
	Dinode* dinode = (Dinode*)(b->data) + inode->inum % IPB;
	inode->type = dinode->type;
	ASSERT(inode->type != 0);  // 得是已分配的
	inode->size = dinode->size;
	inode->n_link = dinode->n_link;
	std::memmove(inode->addr, dinode->addr, sizeof(inode->addr));

	inode->valid = 1;
	
	GetDisk().ReleaseBlock(b);
}

Inode* Controller::AllocInodeBlock(FileType type)
{
	// 查inode的bitmap 这部分类似 AllocDataBlock

	int inumber = -1;

	auto b = GetDisk().ReadBlock(INODE_BITMAP_BLOCKID);
	int row = 0;
	// inode有8192个，但只占了512块
	for (; row < N_INODE / 8; ++row) {
		if ((uint8_t)(b->data[row]) != 255u) {   // maybe可以稍微快点
			for (int col = 0, mask = 1; col < 8; ++col, mask <<= 1) {
				if ((b->data[row] & mask) == 0) {
					b->data[row] |= mask;

					GetDisk().ReleaseBlock(b); // 修改应该写回到磁盘了
					inumber = row * 8 + col;
					break;
				}
			}
			if (inumber != -1) break;
		}
	}
	if (inumber == -1) {
		GetDisk().ReleaseBlock(b);
	}
	// 没有不完整的行
	ASSERT(inumber >= 0); // 没有空闲块要报错

	b = GetDisk().ReadBlock(I2B(inumber));  // 取存放该空闲inode的块

	uint16_t offset = inumber % IPB;  // 一块（16个）中第几个

	Dinode* dinode = (Dinode*)(b->data) + offset;  // 找到对应位置

	dinode->type = (uint16_t)type;          // 设置文件类型
	dinode->n_link = 0;   // 刚分配还是0 ，应该在创建文件之类的系统调用时更新
	dinode->size = 0;
	std::memset(dinode->addr, 0, sizeof(dinode->addr));
	auto ret = FindInode(inumber);   // 添加到内存inode表
	GetDisk().WriteBlock(b);  
	ReadInodeFromDisk(ret);     // 强行同步一下
	GetDisk().ReleaseBlock(b);

	return ret;
}

void Controller::FreeInodeBlock(Inode* inode)
{
	ASSERT(inode);

	ASSERT(inode->ref >= 1);
	inode->ref--;
	//std::cout << "INODE " << inode->inum << " ref " << inode->ref << '\n';
	if (inode->ref == 0 && inode->valid) {

		if (inode->n_link == 0) {
			// 真的可以删了
			// 可以释放了
			ClearInode(inode);
			// 更新inode的bitmap
			auto b = GetDisk().ReadBlock(INODE_BITMAP_BLOCKID);

			int row = inode->inum / 8;
			int col = inode->inum % 8;
			int mask = 1 << col;
			ASSERT((b->data[row] & mask) > 0); // 是已使用的块

			b->data[row] &= ~mask; // 该位清零

			GetDisk().ReleaseBlock(b);
		}

		// 从inode表移除
		//ForceUpdateInode(inode);
		inode->valid = 0;
		inode->inum = 0;

	}
}

void Controller::ForceUpdateInode(const Inode* inode)
{
	ASSERT(inode);
	//ASSERT(inode->valid);

	auto b = GetDisk().ReadBlock(I2B(inode->inum));
	Dinode* dinode = (Dinode*)(b->data) + inode->inum % IPB;

	dinode->type = inode->type;
	dinode->size = inode->size;
	dinode->n_link = inode->n_link;
	std::memmove(dinode->addr, inode->addr, sizeof(inode->addr));

	GetDisk().WriteBlock(b);  //保险一点

	GetDisk().ReleaseBlock(b);
}

void Controller::ClearInode(Inode* inode)
{
	// 先清空他自己的部分
	for (int i = 0; i < NDIRECT; ++i) {
		if (inode->addr[i]) {
			GetDisk().FreeDataBlock(inode->addr[i]);
			inode->addr[i] = 0;
		}
	}
	//如果间接索引用上了，那也要释放
	if (inode->addr[NDIRECT]) {
		auto b = GetDisk().ReadBlock(inode->addr[NDIRECT]);
		uint32_t* addr = (uint32_t*)b->data;
		for (int i = 0; i < NINDIRECT; ++i) {
			if (addr[i]) {      // 注意这里类型转换的作用
				GetDisk().FreeDataBlock(addr[i]);
			}
		}
		GetDisk().ReleaseBlock(b);
		GetDisk().FreeDataBlock(inode->addr[NDIRECT]); //间接块本身是数据块
		inode->addr[NDIRECT] = 0;
	}
	inode->size = 0;
	inode->type = 0;
	ForceUpdateInode(inode);  // 更新一波
}


// 返回inode的第k个索引（从0开始）对应的数据块的物理块号
// 如果没有这个块就分配
static uint32_t GetInodeKthData(Inode* inode, uint32_t k) {

	ASSERT(inode);
	ASSERT(inode->valid);

	ASSERT(k < NDIRECT + NINDIRECT);

	uint32_t addr;
	if (k < NDIRECT) {
		if (inode->addr[k] == 0) {
			inode->addr[k] = GetDisk().AllocDataBlock();
		}
		addr = inode->addr[k];
		return addr;
	}
	ASSERT(k >= NDIRECT);
	k -= NDIRECT;
	ASSERT(k < NINDIRECT);
	// 需要用间接索引了
	if (inode->addr[NDIRECT] == 0) {
		// 没有的话先分配一个
		inode->addr[NDIRECT] = GetDisk().AllocDataBlock();
	}
	auto b = GetDisk().ReadBlock(inode->addr[NDIRECT]);
	uint32_t* ptr = (uint32_t*)b->data;
	if (ptr[k] == 0) {  // 看间接索引的对应位置
		ptr[k] = GetDisk().AllocDataBlock();
	}
	addr = ptr[k];
	GetDisk().ReleaseBlock(b);

	return addr;
}


int Controller::ReadInode(Inode* inode, char* dst, uint32_t offset, uint32_t cnt)
{
	if (offset > inode->size
		|| offset + cnt < offset) // 溢出
		return 0;

	if (offset + cnt > inode->size)
		cnt = inode->size - offset;  // 读的太多，这里修正一下，不用报错

	uint32_t read_cnt; // 实际读取的数量（字节个数）

	for (read_cnt = 0; read_cnt < cnt; ) {
		uint32_t k = offset / BLOCK_SIZE; // 要读的在第几个数据块
		uint32_t block = GetInodeKthData(inode, k);  // 取第k个块的物理块号
		auto b = GetDisk().ReadBlock(block);

		uint32_t cur = std::min(cnt - read_cnt, BLOCK_SIZE - offset % BLOCK_SIZE); // 这次要读的个数
		std::memmove(dst + read_cnt, b->data + offset % BLOCK_SIZE, cur);   //“读取”

		read_cnt += cur;  //更新计数和偏移
		offset += cur;

		GetDisk().ReleaseBlock(b);
	}

	return read_cnt;

}

int Controller::WriteInode(Inode* inode, const char* src, uint32_t offset, uint32_t cnt)
{
	if (offset > inode->size
		|| offset + cnt < offset) // 溢出
		return 0;

	if (offset + cnt > MAX_FILE_SIZE)
		cnt = MAX_FILE_SIZE - offset;  // 写的太多，这里修正一下，不用报错

	// 类似读
	uint32_t write_cnt; // 实际写的数量（字节个数）

	for (write_cnt = 0; write_cnt < cnt; ) {
		uint32_t k = offset / BLOCK_SIZE; // 要写的在第几个数据块
		uint32_t block = GetInodeKthData(inode, k);  // 取第k个块的物理块号
		auto b = GetDisk().ReadBlock(block);

		uint32_t cur = std::min(cnt - write_cnt, BLOCK_SIZE - offset % BLOCK_SIZE); // 这次要写的个数
		std::memmove(b->data + offset % BLOCK_SIZE, src + write_cnt, cur);   // “写”

		write_cnt += cur;  //更新计数和偏移
		offset += cur;

		GetDisk().ReleaseBlock(b);
	}

	if (cnt > 0 && offset > inode->size) inode->size = offset;  //更新大小

	ForceUpdateInode(inode);  //写过程可能涉及更新inode那些索引，这里更新一下以防万一

	return write_cnt;
}

template<unsigned N>
static void CharToArray(std::array<char, N>& arr, const char* src) {
	std::memmove(arr.data(), src, N);
}

static bool CmpItemName(const std::array<char, N_DIR_NAME>& arr, const std::string& name) {
	ASSERT(name.length() <= N_DIR_NAME);
	size_t i;
	for (i = 0; i < name.length(); ++i) {
		if (name[i] != arr[i]) {
			return false;
		}
	}
	if (i < N_DIR_NAME) {
		// name较短，上面就只比较了前缀，再判断一下
		if (arr[i] != 0) {   // 注意存目录时多余位置清0！
			return false;
		}
	}
	return true;
}

std::pair<Inode*, uint32_t> Controller::LookUpInDir(Inode* inode, const std::string& name)
{
	ASSERT(inode);
	bool flag = true;
	if (inode->valid == 0) {
		ReadInodeFromDisk(FindInode(inode->inum));
		flag = false; //为了抵消find对ref+1
	}

	if (inode->type != (uint16_t)FileType::Dir // 传来的inode不是目录
		|| name.length() > N_DIR_NAME) {  // 名字长度不符合要求
		if (!flag) FreeInodeBlock(inode);
		return { nullptr, 0 };
	}

	uint32_t offset = 0;
	DirItem it;
	char buf[32];
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int ret = ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (ret == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break; 
		}
		ASSERT(ret == DIR_ITEM_SIZE);

		uint32_t inum;
		std::memmove(&inum, buf + N_DIR_NAME, sizeof(uint32_t));
		// 看下是否有对应的
		if (inum == 0) continue;

		CharToArray(it.name, buf);
		if (CmpItemName(it.name, name)) {  // 找到了
			if (!flag) FreeInodeBlock(inode);
			return { FindInode(inum), offset };
		}

	}
	if (!flag) FreeInodeBlock(inode);
	return { nullptr, 0 };
}

bool Controller::AddItemInDir(Inode* inode, const std::string& name, uint32_t inum)
{
	ASSERT(inode);
	bool flag = true;
	if (inode->valid == 0) {
		ReadInodeFromDisk(FindInode(inode->inum));
		flag = false;
	}

	if (inode->type != (uint16_t)FileType::Dir // 传来的inode不是目录
		|| name.length() > N_DIR_NAME    // 名字长度不符合要求
		|| inode->size / DIR_ITEM_SIZE == N_DIR_ITEM) {   // 满了
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	auto ret = LookUpInDir(inode, name);

	if (ret.first != nullptr) {   // 要添加的项已经存在（同名的）
		FreeInodeBlock(ret.first);   // 这里ref--，因为lookup返回的是调用find的结果，
									 // 这样把这个查到的同名文件从内存inode表移除
		std::cout << "已存在同名文件" << std::endl;

		// 另外这里是找到就算，不区分文件和文件夹
		// 所以文件和文件夹也不能同名
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	uint32_t offset = 0;
	char buf[32];
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		uint32_t num;
		std::memmove(&num, buf + N_DIR_NAME, sizeof(uint32_t));
		// 看下是否有空位
		if (num == 0) {
			break;
		}
	}

	if (offset == DIR_ITEM_SIZE * N_DIR_ITEM) {
		std::cout << "目录已满" << std::endl;
		// 没空位了
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	// 准备数据
	std::memset(buf, 0, sizeof(buf));
	std::memmove(buf, name.data(), name.length());
	std::memmove(buf + N_DIR_NAME, &inum, sizeof(uint32_t));

	int wcnt = WriteInode(inode, buf, offset, DIR_ITEM_SIZE);
	ASSERT(wcnt == DIR_ITEM_SIZE);
	if (!flag) FreeInodeBlock(inode);
	return true;
}

static bool TestPathBasic(const std::string& path) {
	if (path.empty()
		|| path.front() != '/'
		|| (path.length() > 1 && path.back() == '/')) {
		return false;
	}
	return true;
}

Inode* Controller::PathToInode(const std::string& path)
{
	if (TestPathBasic(path) == false) {
		return nullptr;
	}

	auto v = SplitPath(path);

	// 总是要从根目录开始
	Inode* it = FindInode(ROOT_DIR_INODE);

	if (v.size() == 1) {
		// 只有根目录
		return it;
	}

	for (size_t i = 1; i < v.size() - 1; ++i) {
		
		// 除了最后一个都必然是目录
		auto ret = LookUpInDir(it, v[i]);

		if (ret.first == nullptr) {  // 中间某一级不存在
			FreeInodeBlock(it);
			return nullptr;
		}

		ReadInodeFromDisk(ret.first);
		if (ret.first->type != (uint16_t)FileType::Dir) {  // 中间某一级不是目录
			FreeInodeBlock(it);
			FreeInodeBlock(ret.first);
			return nullptr;
		}

		FreeInodeBlock(it);
		it = ret.first;
	}

	// 现在的it应该是最终目标所在的目录
	auto ret = LookUpInDir(it, v.back());
	if (ret.first == nullptr) {
		FreeInodeBlock(it);
		return nullptr;
	}
	FreeInodeBlock(it);

	return ret.first;
}

Inode* Controller::PathToInodeParent(const std::string& path)
{
	if (TestPathBasic(path) == false) {
		return nullptr;
	}

	auto ret = SplitPath2(path);
	return PathToInode(ret.first);
}

bool Controller::DeleteFile(const std::string& path)
{
	if (TestPathBasic(path) == false) {
		std::cout << "路径错误" << std::endl;
		return false;
	}

	auto ret = SplitPath2(path);
	auto parent = PathToInode(ret.first);  // 找到要删的文件的上一级
	
	if (parent == nullptr){     
		FreeInodeBlock(parent);
		std::cout << "路径错误" << std::endl;
		return false;
	}
	ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		FreeInodeBlock(parent);
		std::cout << "路径错误" << std::endl;
		return false;
	}

	auto child = LookUpInDir(parent, ret.second);
										
	if (child.first == nullptr) {
		FreeInodeBlock(parent);
		return false;
	}

	ReadInodeFromDisk(child.first);
	if (child.first->type != (uint16_t)FileType::File) {// 删目录用deletedir
		FreeInodeBlock(parent);
		FreeInodeBlock(child.first);
		return false;
	}
	// 删child自己
	child.first->n_link--;
	ForceUpdateInode(child.first);
	FreeInodeBlock(child.first);
	// 如果nlink减一后不为0，则并未真正删除

	// 从parent目录中移除 
	char buf[32];
	// 由于读取不能超过size，必须连续存放，需要把最后一个交换过来
	ReadInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);  // 读来现在的最后一个目录项
	WriteInode(parent, buf, child.second, DIR_ITEM_SIZE);                 // 写到刚才的空位（被删除的位置）
	std::memset(buf, 0, sizeof(buf));                                     // 把最后一个目录项清0
	WriteInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);
	parent->size -= DIR_ITEM_SIZE;
	ForceUpdateInode(parent);
	FreeInodeBlock(parent);

	return true;
}

bool Controller::DeleteDir(const std::string& path)
{
	// 开始的一系列检查基本同DeleteFile
	if (TestPathBasic(path) == false) {
		return false;
	}

	auto ret = SplitPath2(path);
	auto parent = PathToInode(ret.first);  // 找到要删的文件的上一级

	if (parent == nullptr) {
		FreeInodeBlock(parent);
		return false;
	}
	ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		FreeInodeBlock(parent);
		return false;
	}

	auto child = LookUpInDir(parent, ret.second);

	if (child.first == nullptr   //不应该为空，但没报错
	|| child.first->inum == ROOT_DIR_INODE) {    // 0号对应根目录，不能删！
		FreeInodeBlock(parent);
		return false;
	}
	ReadInodeFromDisk(child.first);
	if (child.first->type == (uint16_t)FileType::File) { //实际删的是普通文件
		bool succ = DeleteFile(path);
		FreeInodeBlock(child.first);
		FreeInodeBlock(parent);
		return succ;
	}
	
	// child.first 是目录

	char buf[32];
	uint32_t offset = 0;
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ReadInode(child.first, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		uint32_t num;
		std::memmove(&num, buf + N_DIR_NAME, sizeof(uint32_t));
		if (num == 0) {
			continue;
		}

		std::string next_path = path + "/";
		for (int i = 0; i < N_DIR_NAME && buf[i]; ++i) {
			next_path += buf[i];
		}
		bool succ = DeleteDir(next_path); // 递归

		if (!succ) {
			FreeInodeBlock(child.first);
			FreeInodeBlock(parent);
			return succ;
		}
	}

	child.first->n_link--;
	ForceUpdateInode(child.first);
	FreeInodeBlock(child.first);

	// 由于读取不能超过size，必须连续存放，需要把最后一个交换过来
	ReadInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);  // 读来现在的最后一个目录项
	WriteInode(parent, buf, child.second, DIR_ITEM_SIZE);                 // 写到刚才的空位（被删除的位置）
	std::memset(buf, 0, sizeof(buf));                                     // 把最后一个目录项清0
	WriteInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);
	parent->size -= DIR_ITEM_SIZE;
	ForceUpdateInode(parent);
	FreeInodeBlock(parent);

	return true;
}

