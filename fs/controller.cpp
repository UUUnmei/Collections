#include "controller.h"
#include "util.h"

#include <iostream>
#include <vector>



Inode* Controller::FindInode(uint32_t inum)
{
	// �� ����Ӧ�ñ�֤Ҫ�ҵ�inode���ѷ���� 

	Inode* slot = nullptr;
	for (auto& i : itable) {
		if (i.ref > 0 && i.inum == inum) {
			i.ref++;
			//std::cout << "INODE " << i.inum << " ref " << i.ref << '\n';
			return &i;   // �ҵ���
		}
		if (slot == nullptr && i.ref == 0) slot = &i;  // �ҵ���λ
	}

	ASSERT(slot != nullptr);

	// �ҵ���λ 

	slot->inum = inum;
	slot->ref = 1;

	slot->valid = 0; // �����ﲻ����������Ϊ������alloc���շ����inode������ûɶ����

	return slot;
}

void Controller::ReadInodeFromDisk(Inode* inode)
{
	ASSERT(inode);
	if (inode->valid) return;

	auto b = GetDisk().ReadBlock(I2B(inode->inum));
	Dinode* dinode = (Dinode*)(b->data) + inode->inum % IPB;
	inode->type = dinode->type;
	ASSERT(inode->type != 0);  // �����ѷ����
	inode->size = dinode->size;
	inode->n_link = dinode->n_link;
	std::memmove(inode->addr, dinode->addr, sizeof(inode->addr));

	inode->valid = 1;
	
	GetDisk().ReleaseBlock(b);
}

Inode* Controller::AllocInodeBlock(FileType type)
{
	// ��inode��bitmap �ⲿ������ AllocDataBlock

	int inumber = -1;

	auto b = GetDisk().ReadBlock(INODE_BITMAP_BLOCKID);
	int row = 0;
	// inode��8192������ֻռ��512��
	for (; row < N_INODE / 8; ++row) {
		if ((uint8_t)(b->data[row]) != 255u) {   // maybe������΢���
			for (int col = 0, mask = 1; col < 8; ++col, mask <<= 1) {
				if ((b->data[row] & mask) == 0) {
					b->data[row] |= mask;

					GetDisk().ReleaseBlock(b); // �޸�Ӧ��д�ص�������
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
	// û�в���������
	ASSERT(inumber >= 0); // û�п��п�Ҫ����

	b = GetDisk().ReadBlock(I2B(inumber));  // ȡ��Ÿÿ���inode�Ŀ�

	uint16_t offset = inumber % IPB;  // һ�飨16�����еڼ���

	Dinode* dinode = (Dinode*)(b->data) + offset;  // �ҵ���Ӧλ��

	dinode->type = (uint16_t)type;          // �����ļ�����
	dinode->n_link = 0;   // �շ��仹��0 ��Ӧ���ڴ����ļ�֮���ϵͳ����ʱ����
	dinode->size = 0;
	std::memset(dinode->addr, 0, sizeof(dinode->addr));
	auto ret = FindInode(inumber);   // ��ӵ��ڴ�inode��
	GetDisk().WriteBlock(b);  
	ReadInodeFromDisk(ret);     // ǿ��ͬ��һ��
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
			// ��Ŀ���ɾ��
			// �����ͷ���
			ClearInode(inode);
			// ����inode��bitmap
			auto b = GetDisk().ReadBlock(INODE_BITMAP_BLOCKID);

			int row = inode->inum / 8;
			int col = inode->inum % 8;
			int mask = 1 << col;
			ASSERT((b->data[row] & mask) > 0); // ����ʹ�õĿ�

			b->data[row] &= ~mask; // ��λ����

			GetDisk().ReleaseBlock(b);
		}

		// ��inode���Ƴ�
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

	GetDisk().WriteBlock(b);  //����һ��

	GetDisk().ReleaseBlock(b);
}

void Controller::ClearInode(Inode* inode)
{
	// ��������Լ��Ĳ���
	for (int i = 0; i < NDIRECT; ++i) {
		if (inode->addr[i]) {
			GetDisk().FreeDataBlock(inode->addr[i]);
			inode->addr[i] = 0;
		}
	}
	//���������������ˣ���ҲҪ�ͷ�
	if (inode->addr[NDIRECT]) {
		auto b = GetDisk().ReadBlock(inode->addr[NDIRECT]);
		uint32_t* addr = (uint32_t*)b->data;
		for (int i = 0; i < NINDIRECT; ++i) {
			if (addr[i]) {      // ע����������ת��������
				GetDisk().FreeDataBlock(addr[i]);
			}
		}
		GetDisk().ReleaseBlock(b);
		GetDisk().FreeDataBlock(inode->addr[NDIRECT]); //��ӿ鱾�������ݿ�
		inode->addr[NDIRECT] = 0;
	}
	inode->size = 0;
	inode->type = 0;
	ForceUpdateInode(inode);  // ����һ��
}


// ����inode�ĵ�k����������0��ʼ����Ӧ�����ݿ��������
// ���û�������ͷ���
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
	// ��Ҫ�ü��������
	if (inode->addr[NDIRECT] == 0) {
		// û�еĻ��ȷ���һ��
		inode->addr[NDIRECT] = GetDisk().AllocDataBlock();
	}
	auto b = GetDisk().ReadBlock(inode->addr[NDIRECT]);
	uint32_t* ptr = (uint32_t*)b->data;
	if (ptr[k] == 0) {  // ����������Ķ�Ӧλ��
		ptr[k] = GetDisk().AllocDataBlock();
	}
	addr = ptr[k];
	GetDisk().ReleaseBlock(b);

	return addr;
}


int Controller::ReadInode(Inode* inode, char* dst, uint32_t offset, uint32_t cnt)
{
	if (offset > inode->size
		|| offset + cnt < offset) // ���
		return 0;

	if (offset + cnt > inode->size)
		cnt = inode->size - offset;  // ����̫�࣬��������һ�£����ñ���

	uint32_t read_cnt; // ʵ�ʶ�ȡ���������ֽڸ�����

	for (read_cnt = 0; read_cnt < cnt; ) {
		uint32_t k = offset / BLOCK_SIZE; // Ҫ�����ڵڼ������ݿ�
		uint32_t block = GetInodeKthData(inode, k);  // ȡ��k�����������
		auto b = GetDisk().ReadBlock(block);

		uint32_t cur = std::min(cnt - read_cnt, BLOCK_SIZE - offset % BLOCK_SIZE); // ���Ҫ���ĸ���
		std::memmove(dst + read_cnt, b->data + offset % BLOCK_SIZE, cur);   //����ȡ��

		read_cnt += cur;  //���¼�����ƫ��
		offset += cur;

		GetDisk().ReleaseBlock(b);
	}

	return read_cnt;

}

int Controller::WriteInode(Inode* inode, const char* src, uint32_t offset, uint32_t cnt)
{
	if (offset > inode->size
		|| offset + cnt < offset) // ���
		return 0;

	if (offset + cnt > MAX_FILE_SIZE)
		cnt = MAX_FILE_SIZE - offset;  // д��̫�࣬��������һ�£����ñ���

	// ���ƶ�
	uint32_t write_cnt; // ʵ��д���������ֽڸ�����

	for (write_cnt = 0; write_cnt < cnt; ) {
		uint32_t k = offset / BLOCK_SIZE; // Ҫд���ڵڼ������ݿ�
		uint32_t block = GetInodeKthData(inode, k);  // ȡ��k�����������
		auto b = GetDisk().ReadBlock(block);

		uint32_t cur = std::min(cnt - write_cnt, BLOCK_SIZE - offset % BLOCK_SIZE); // ���Ҫд�ĸ���
		std::memmove(b->data + offset % BLOCK_SIZE, src + write_cnt, cur);   // ��д��

		write_cnt += cur;  //���¼�����ƫ��
		offset += cur;

		GetDisk().ReleaseBlock(b);
	}

	if (cnt > 0 && offset > inode->size) inode->size = offset;  //���´�С

	ForceUpdateInode(inode);  //д���̿����漰����inode��Щ�������������һ���Է���һ

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
		// name�϶̣������ֻ�Ƚ���ǰ׺�����ж�һ��
		if (arr[i] != 0) {   // ע���Ŀ¼ʱ����λ����0��
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
		flag = false; //Ϊ�˵���find��ref+1
	}

	if (inode->type != (uint16_t)FileType::Dir // ������inode����Ŀ¼
		|| name.length() > N_DIR_NAME) {  // ���ֳ��Ȳ�����Ҫ��
		if (!flag) FreeInodeBlock(inode);
		return { nullptr, 0 };
	}

	uint32_t offset = 0;
	DirItem it;
	char buf[32];
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int ret = ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (ret == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break; 
		}
		ASSERT(ret == DIR_ITEM_SIZE);

		uint32_t inum;
		std::memmove(&inum, buf + N_DIR_NAME, sizeof(uint32_t));
		// �����Ƿ��ж�Ӧ��
		if (inum == 0) continue;

		CharToArray(it.name, buf);
		if (CmpItemName(it.name, name)) {  // �ҵ���
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

	if (inode->type != (uint16_t)FileType::Dir // ������inode����Ŀ¼
		|| name.length() > N_DIR_NAME    // ���ֳ��Ȳ�����Ҫ��
		|| inode->size / DIR_ITEM_SIZE == N_DIR_ITEM) {   // ����
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	auto ret = LookUpInDir(inode, name);

	if (ret.first != nullptr) {   // Ҫ��ӵ����Ѿ����ڣ�ͬ���ģ�
		FreeInodeBlock(ret.first);   // ����ref--����Ϊlookup���ص��ǵ���find�Ľ����
									 // ����������鵽��ͬ���ļ����ڴ�inode���Ƴ�
		std::cout << "�Ѵ���ͬ���ļ�" << std::endl;

		// �����������ҵ����㣬�������ļ����ļ���
		// �����ļ����ļ���Ҳ����ͬ��
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	uint32_t offset = 0;
	char buf[32];
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		uint32_t num;
		std::memmove(&num, buf + N_DIR_NAME, sizeof(uint32_t));
		// �����Ƿ��п�λ
		if (num == 0) {
			break;
		}
	}

	if (offset == DIR_ITEM_SIZE * N_DIR_ITEM) {
		std::cout << "Ŀ¼����" << std::endl;
		// û��λ��
		if (!flag) FreeInodeBlock(inode);
		return false;
	}

	// ׼������
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

	// ����Ҫ�Ӹ�Ŀ¼��ʼ
	Inode* it = FindInode(ROOT_DIR_INODE);

	if (v.size() == 1) {
		// ֻ�и�Ŀ¼
		return it;
	}

	for (size_t i = 1; i < v.size() - 1; ++i) {
		
		// �������һ������Ȼ��Ŀ¼
		auto ret = LookUpInDir(it, v[i]);

		if (ret.first == nullptr) {  // �м�ĳһ��������
			FreeInodeBlock(it);
			return nullptr;
		}

		ReadInodeFromDisk(ret.first);
		if (ret.first->type != (uint16_t)FileType::Dir) {  // �м�ĳһ������Ŀ¼
			FreeInodeBlock(it);
			FreeInodeBlock(ret.first);
			return nullptr;
		}

		FreeInodeBlock(it);
		it = ret.first;
	}

	// ���ڵ�itӦ��������Ŀ�����ڵ�Ŀ¼
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
		std::cout << "·������" << std::endl;
		return false;
	}

	auto ret = SplitPath2(path);
	auto parent = PathToInode(ret.first);  // �ҵ�Ҫɾ���ļ�����һ��
	
	if (parent == nullptr){     
		FreeInodeBlock(parent);
		std::cout << "·������" << std::endl;
		return false;
	}
	ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		FreeInodeBlock(parent);
		std::cout << "·������" << std::endl;
		return false;
	}

	auto child = LookUpInDir(parent, ret.second);
										
	if (child.first == nullptr) {
		FreeInodeBlock(parent);
		return false;
	}

	ReadInodeFromDisk(child.first);
	if (child.first->type != (uint16_t)FileType::File) {// ɾĿ¼��deletedir
		FreeInodeBlock(parent);
		FreeInodeBlock(child.first);
		return false;
	}
	// ɾchild�Լ�
	child.first->n_link--;
	ForceUpdateInode(child.first);
	FreeInodeBlock(child.first);
	// ���nlink��һ��Ϊ0����δ����ɾ��

	// ��parentĿ¼���Ƴ� 
	char buf[32];
	// ���ڶ�ȡ���ܳ���size������������ţ���Ҫ�����һ����������
	ReadInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);  // �������ڵ����һ��Ŀ¼��
	WriteInode(parent, buf, child.second, DIR_ITEM_SIZE);                 // д���ղŵĿ�λ����ɾ����λ�ã�
	std::memset(buf, 0, sizeof(buf));                                     // �����һ��Ŀ¼����0
	WriteInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);
	parent->size -= DIR_ITEM_SIZE;
	ForceUpdateInode(parent);
	FreeInodeBlock(parent);

	return true;
}

bool Controller::DeleteDir(const std::string& path)
{
	// ��ʼ��һϵ�м�����ͬDeleteFile
	if (TestPathBasic(path) == false) {
		return false;
	}

	auto ret = SplitPath2(path);
	auto parent = PathToInode(ret.first);  // �ҵ�Ҫɾ���ļ�����һ��

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

	if (child.first == nullptr   //��Ӧ��Ϊ�գ���û����
	|| child.first->inum == ROOT_DIR_INODE) {    // 0�Ŷ�Ӧ��Ŀ¼������ɾ��
		FreeInodeBlock(parent);
		return false;
	}
	ReadInodeFromDisk(child.first);
	if (child.first->type == (uint16_t)FileType::File) { //ʵ��ɾ������ͨ�ļ�
		bool succ = DeleteFile(path);
		FreeInodeBlock(child.first);
		FreeInodeBlock(parent);
		return succ;
	}
	
	// child.first ��Ŀ¼

	char buf[32];
	uint32_t offset = 0;
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ReadInode(child.first, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
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
		bool succ = DeleteDir(next_path); // �ݹ�

		if (!succ) {
			FreeInodeBlock(child.first);
			FreeInodeBlock(parent);
			return succ;
		}
	}

	child.first->n_link--;
	ForceUpdateInode(child.first);
	FreeInodeBlock(child.first);

	// ���ڶ�ȡ���ܳ���size������������ţ���Ҫ�����һ����������
	ReadInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);  // �������ڵ����һ��Ŀ¼��
	WriteInode(parent, buf, child.second, DIR_ITEM_SIZE);                 // д���ղŵĿ�λ����ɾ����λ�ã�
	std::memset(buf, 0, sizeof(buf));                                     // �����һ��Ŀ¼����0
	WriteInode(parent, buf, parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);
	parent->size -= DIR_ITEM_SIZE;
	ForceUpdateInode(parent);
	FreeInodeBlock(parent);

	return true;
}

