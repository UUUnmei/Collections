#include "syscall.h"
#include "controller.h"
#include "util.h"

#include <iostream>
#include <regex>

namespace {
	Controller ctrl;
	OpenFileTable oft;

	User user_cur;
	std::string path_cur;

	uint32_t DEFAULT_PERMISSION = 0b1101;   //自己可读写 其他人只读
}

//读写权限检查
bool CreatorWrite(uint32_t i) { return (i & 8) == 8; }
bool CreatorRead(uint32_t i) { return (i & 4) == 4; }
bool OthersWrite(uint32_t i) { return (i & 2) == 2; }
bool OthersRead(uint32_t i) { return (i & 1) == 1; }

//文件名合法性检查
static bool CheckFileName(std::string name) {
	if (name.length() > N_DIR_NAME) return false;
	int cnt = 0;
	for (int i = 0; i < name.length(); ++i) {
		if (isalnum(name[i])) {
			;
		}else if (name[i] == '_') {
			;
		}else if (name[i] == '.') {
			cnt++;
			if (cnt > 1) return false;
			if (i == name.length() - 1) return false;
		}
		else {
			return false;
		}
	}
	return true;

	//std::regex fileName("^[a-zA-Z0-9_ ]*\.?[a-zA-Z0-9_ ]+$");
	//return std::regex_match(name, fileName);
}

static bool IsRoot() {
	return ArrayToString(user_cur.username) == "root";
}


//获得当前文件的FileHeader
static FileHeader GetFileHeader(Inode* inode) {
	FileHeader fh;
	char message[FILE_HEADER_SIZE];

	ctrl.ReadInodeFromDisk(inode);
	if (inode->type == (uint16_t)FileType::File) {
		ctrl.ReadInode(inode, message, 0, sizeof(FileHeader));
	}
	else if (inode->type == (uint16_t)FileType::Dir) {
		uint32_t sz = inode->size;
		inode->size = 1024;  //硬改
		ctrl.ReadInode(inode, message, BLOCK_SIZE - 100, sizeof(FileHeader));
		inode->size = sz;
		ctrl.ForceUpdateInode(inode);
	}
	else {
		ASSERT(0);
	}
	std::memmove(&fh, message, sizeof(FileHeader));
	return fh;
}

static int CreateDetail(const std::string& path, FileType type) {

	if (path == "/") return -1;

	auto ret = SplitPath2(path);
	auto parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {  // 父级不存在
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	Inode* child = ctrl.AllocInodeBlock(type);
	child->n_link = 1;
	ctrl.ForceUpdateInode(child);

	bool succ = ctrl.AddItemInDir(parent, ret.second, child->inum);  // 这里就包括了检查重名的问题 但对于"/"不适用，根目录在格式化时单独建立

	if (succ == false) {
		// 添加失败，释放刚分配的inode
		// AddItemInDir实现中保证了添加失败对于父级目录不会有影响
		child->n_link--;
		ctrl.FreeInodeBlock(child);

		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	ctrl.FreeInodeBlock(parent);
	ctrl.FreeInodeBlock(child);

	return 0;

}


int Mount(const std::string& path)
{
	auto& disk = GetDisk();
	bool ret = disk.Mount("fs.img");
	if (ret) {
		std::cout << "磁盘挂载成功" << std::endl;
		return 0;
	}
	std::cout << "磁盘挂载失败" << std::endl;
	return -1;
}

int Create(const std::string& path)
{
	// 都应该先检查路径合法性，或者说在命令行输入命令的地方检查 
	int ret = CreateDetail(path, FileType::File);
	if (ret == -1) {
		std::cout << "创建失败" << std::endl;
		return -1;
	}
	// 写FileHeader
	// 第一个数据块的前100个字节
	Inode* inode = ctrl.PathToInode(path);
	std::string name = SplitPath2(path).second;
	char input[FILE_HEADER_SIZE];
	std::memset(input, 0, sizeof(input));
	for (int i = 0; i < 20 && i < name.size(); i++) {
		input[i] = name[i];
	}
	for (int i = 0; i < 20 && i < user_cur.username.size(); i++) {
		input[i + 20] = user_cur.username[i];
	}
	memcpy(input + 40, &DEFAULT_PERMISSION, sizeof(uint32_t));
	uint32_t num = inode->inum;  // 位数问题，设计的一点瑕疵。。。
	memcpy(input + 44, &num, sizeof(uint32_t));  // 补充一下inum
	ctrl.ReadInodeFromDisk(inode);
	ctrl.WriteInode(inode, input, 0, sizeof(FileHeader));
	inode->size = FILE_HEADER_SIZE;
	ctrl.ForceUpdateInode(inode);
	ctrl.FreeInodeBlock(inode);
	return ret;
}

int MkDir(const std::string& path)
{
	// 都应该先检查路径合法性，或者说在命令行输入命令的地方检查 
	int ret = CreateDetail(path, FileType::Dir);
	if (ret == -1) {
		std::cout << "创建失败" << std::endl;
		return -1;
	}
	// 写FileHeader
	// 第一个数据块的后100个字节
	Inode* inode = ctrl.PathToInode(path);
	std::string name = SplitPath2(path).second;
	char input[FILE_HEADER_SIZE];
	std::memset(input, 0, sizeof(input));
	for (int i = 0; i < 20 && i < name.size(); i++) {
		input[i] = name[i];
	}
	for (int i = 0; i < 20 && i < user_cur.username.size(); i++) {
		input[i + 20] = user_cur.username[i];
	}
	memcpy(input + 40, &DEFAULT_PERMISSION, sizeof(uint32_t));
	uint32_t num = inode->inum;  // 位数问题，设计的一点瑕疵。。。
	memcpy(input + 44, &num, sizeof(uint32_t));  // 补充一下inum
	ctrl.ReadInodeFromDisk(inode);

	// ！！为了让目录的fileheader写进去
	      // 硬改
	inode->size = 1024;  
	ctrl.WriteInode(inode, input, BLOCK_SIZE - 100, sizeof(FileHeader));
	inode->size = 0;
	ctrl.ForceUpdateInode(inode);

	ctrl.FreeInodeBlock(inode);
	return ret;
}

int Format()
{
	// 基本格式化，设置超级块，bitmap，管理员
	GetDisk().Format();

	// 建立根目录 
	auto inode = ctrl.AllocInodeBlock(FileType::Dir);  // 根目录 inode号为0
	inode->n_link = 1;
	ctrl.ForceUpdateInode(inode);
	ctrl.FreeInodeBlock(inode);

	std::cout << "格式化完成" << std::endl;
	return 0;
}

int IsSoftLink(Inode* inode);
int Read(uint32_t fd, char* dst, uint32_t offset, uint32_t cnt)
{
	// 根据fd去打开文件表取inode 以及 检查权限

	ASSERT(fd < oft.table.size());
	Inode* inode = oft.table[fd].inode;
	int ret = IsSoftLink(inode); // 处理同write
	if (ret == -2) {
		return -1;
	}
	bool flag = false;
	if (ret != -1) {
		// 换一下inode
		inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		// 需要再单独控制下ref
		flag = true;
	}

	if (!IsRoot()) {
		FileHeader fh = GetFileHeader(inode);
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorRead(fh.permission) == false) {
				std::cout << "没有读权限" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				return -1;
			}
		}
		else {
			if (OthersRead(fh.permission) == false) {
				std::cout << "没有读权限" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				return -1;
			}
		}
	}
	ctrl.ReadInodeFromDisk(inode);
	int rcnt = ctrl.ReadInode(inode, dst, offset, cnt);
	if (flag) ctrl.FreeInodeBlock(inode);

	return rcnt;
}
//
//int Write(uint32_t fd, const char* src, uint32_t offset, uint32_t cnt)
//{
//	return 0;
//	// 根据fd去打开文件表取inode 以及 检查权限
//	//int ret = ctrl.WriteInode(inode, dst, offset, cnt);
//	//return ret;
//
//	// ！！！ 详见下方交互部分的WriteFile（要区分覆盖写和追加写）
//}

int DeleteFile(const std::string& path)
{
	// 检查权限，且该文件不能被其他“进程”打开
	bool ret = ctrl.DeleteFile(path);
	return (ret == true ? 0 : -1);
}

int DeleteDir(const std::string& path)
{
	if (NormalizedPath(path) == "/"){
		std::cout << "不能删根目录" << std::endl;
		return -1; 
	}

	// 检查权限，且该文件夹不能被其他“进程”打开
	bool ret = ctrl.DeleteDir(path);
	return (ret == true ? 0 : -1);
}


// 在打开文件表中查找，找到ref+1，或找空位，或报错
//   返回文件描述符
static int FindOFT(Inode* inode) {
	int pos = -1;
	for (int i = 0; i < oft.table.size(); ++i) {
		if (oft.table[i].inode == inode) {
			return i;
		}
		if (pos == -1 && oft.table[i].inode == nullptr) {
			pos = i;
		}
	}

	ASSERT(pos != -1);
	oft.table[pos].inode = inode;
	return pos;
}

int Open(const std::string& path) {
	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "路径无效" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(inode);
	if (inode->type != (uint16_t)FileType::File) {
		std::cout << "打开的不是文件" << std::endl;
		ctrl.FreeInodeBlock(inode);
		return -1;
	}
	int fd = FindOFT(inode);
	oft.table[fd].ref++;
	return fd;
}

int Close(uint32_t fd) {
	ASSERT(fd < oft.table.size());
	ASSERT(oft.table[fd].ref >= 1);
	oft.table[fd].ref--;
	if (oft.table[fd].ref == 0) {
		ctrl.FreeInodeBlock(oft.table[fd].inode);
		oft.table[fd].inode = nullptr; 
	}
	return 0;
}

int HardLink(const std::string& target_path, const std::string& link_path)
{
	if (target_path == "/") {
		std::cout << "禁止对根目录建立链接" << std::endl;
		return -1;
	}
	// 暂未考虑权限问题

	Inode* target = ctrl.PathToInode(target_path);
	if (target == nullptr) {
		std::cout << "被链接路径无效" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "不能链接目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(link_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "链接名不合法" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "目标路径不存在" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "目标的上一级需要是目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "目标目录下已经存在同名文件" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("该目录下文件已满");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// 可以建了
	target->n_link++;
	ctrl.ForceUpdateInode(target);
	ctrl.AddItemInDir(parent, ret.second, target->inum); // 新的文件名和原本的inum （其实这个也做了上面的一部分检查，主要是想延后真正分配的时机）

	ctrl.FreeInodeBlock(target);
	ctrl.FreeInodeBlock(parent);

	return 0;
}



int SoftLink(const std::string& target_path, const std::string& link_path)
{
	if (target_path == "/") {
		std::cout << "禁止对根目录建立链接" << std::endl;
		return -1;
	}
	// 暂未考虑权限
	// 一开始这些检查条件和硬链接一致
	Inode* target = ctrl.PathToInode(target_path);
	if (target == nullptr) {
		std::cout << "被链接路径无效" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "不能链接目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(link_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "链接名不合法" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "目标路径不存在" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "目标的上一级需要是目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "目标目录下已经存在同名文件" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("该目录下文件已满");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// 可以建了
	ASSERT(Create(link_path) == 0);
	
	// 修改fileheader中的inum
	Inode* link = ctrl.PathToInode(link_path);
	FileHeader fh = GetFileHeader(link);
	fh.inum = target->inum; // 建立链接
	ctrl.WriteInode(link, (char*)&fh, 0, sizeof(FileHeader));
	ctrl.FreeInodeBlock(link);

	ctrl.FreeInodeBlock(target);
	ctrl.FreeInodeBlock(parent);
	return 0;
}

// 为了检查软链接准备的特别版
///  主要是原本会在读取硬盘的时候顺便查一下是不是已分配的
//   但考虑到软链接，删除原文件后链接失效的情况，会让那里的ASSERT触发，系统就寄了
static void ReadInodeFromDiskUncheck(Inode* inode) {
	ASSERT(inode);
	if (inode->valid) return;

	auto b = GetDisk().ReadBlock(I2B(inode->inum));
	Dinode* dinode = (Dinode*)(b->data) + inode->inum % IPB;
	inode->type = dinode->type;
	inode->size = dinode->size;
	inode->n_link = dinode->n_link;
	std::memmove(inode->addr, dinode->addr, sizeof(inode->addr));

	inode->valid = 1;

	GetDisk().ReleaseBlock(b);
}

// 返回-2表示路径无效，-1表示不是软链接，否则返回链接对象的inum
int IsSoftLink(Inode* inode) {
	int ret = inode->inum;
	// 检查到底  // 但硬链接不考虑
	int cnt = 1;
	while (1) {  
		Inode* cur = ctrl.FindInode(ret);
		ReadInodeFromDiskUncheck(cur);
		if (cur->type == 0) {
			std::cout << "链接文件已失效" << std::endl;
			cur->ref--;   // 这时的cur是未分配的，不能用FreeInode 里面检查bitmap了
			cur->inum = 0;
			cur->valid = 0;
			return -2;
		}
		FileHeader fh = GetFileHeader(cur);
		ctrl.FreeInodeBlock(cur);
		if (ret != fh.inum) {
			ret = fh.inum;  // 是软链接，继续
			cnt++;
		}
		else {
			if (cnt == 1) {
				return -1;  // 不是链接
			}
			else {
				// 找到最后了
				break;
			}
		}
		if (cnt > 10) {
			std::cout << "链接层级过多\n";
			return -2;
		}
	}

	return ret;
}


struct StatInfo {
	std::string name;
	uint32_t inum;
	uint32_t nlink;
	uint32_t size;

	// D/F/L WRWR
	uint32_t type;
	uint32_t permission;
};
static bool IsRoot();
int Stat(const std::string& path)
{

	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "路径无效" << std::endl;
		return -1;
	}
	StatInfo info;
	ctrl.ReadInodeFromDisk(inode);
	info.inum = inode->inum;
	info.nlink = inode->n_link;
	info.name = SplitPath2(path).second;
	info.size = inode->size;
	if (inode->type == (uint16_t)FileType::File) info.size -= FILE_HEADER_SIZE;
	info.type = inode->type;

	FileHeader fh = GetFileHeader(inode);
	bool iscreator = ArrayToString(fh.username) == ArrayToString(user_cur.username);
	info.permission = fh.permission;

	// 考虑 显示一下软链接
	std::string linkname;
	int ret = IsSoftLink(inode);
	if (ret == -2) {
		linkname = "?";
	}
	if (ret != -1 && ret != -2) {
		// 换一下inode
		Inode* inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		FileHeader fh2 = GetFileHeader(inode);
		linkname = ArrayToString(fh2.filename);
		ctrl.FreeInodeBlock(inode);
	}

	// 考虑输出格式
	// 文件类型及权限
	printf("%s", info.name.c_str());
	if (linkname.empty() == false) {
		printf(" -> %s", linkname.c_str());  // 结构所限，并不方便展示被链接文件的全部路径，就显示名
	}
	printf("\n");
	printf("%c%c%c%c%c\n",
		info.type == FileType::File ? (linkname.empty() ? 'F' : 'L') : 'D',
		CreatorWrite(info.permission) ? 'W' : '-',
		CreatorRead(info.permission) ? 'R' : '-',
		OthersWrite(info.permission) ? 'W' : '-',
		OthersRead(info.permission) ? 'R' : '-'
	);
	printf("size: %d\n", info.size);
	printf("inum: %d\n", info.inum);
	printf("nlink: %d\n", info.nlink);


	ctrl.FreeInodeBlock(inode);
	return 0;
}


int Move(const std::string& old_path, const std::string& new_path)
{
	if (old_path == "/") {
		std::cout << "禁止移动根目录" << std::endl;
		return -1;
	}
	// 基本只是修改目录
	// 没考虑权限

	Inode* target = ctrl.PathToInode(old_path);
	if (target == nullptr) {
		std::cout << "要移动的文件无效" << std::endl;
		return -1;
	}
	auto ret = SplitPath2(new_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "目标文件名不合法" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "目标路径不存在" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "目标的上一级需要是目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "目标目录下已经存在同名文件" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("该目录下文件已满");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// 可以修改目录了
	// 在目标路径中添加
	ctrl.AddItemInDir(parent, ret.second, target->inum);
	ctrl.FreeInodeBlock(parent);

	ctrl.FreeInodeBlock(target);
	// 把现有的删了
	Inode* target_parent = ctrl.PathToInodeParent(old_path);
	auto target_name = SplitPath2(old_path).second;
	auto target_file = ctrl.LookUpInDir(target_parent, target_name);

	// 从parent目录中移除 见controller里delete的实现
	ctrl.ReadInode(target_parent, buf, target_parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);  
	ctrl.WriteInode(target_parent, buf, target_file.second, DIR_ITEM_SIZE);                
	std::memset(buf, 0, sizeof(buf));                                  
	ctrl.WriteInode(target_parent, buf, target_parent->size - DIR_ITEM_SIZE, DIR_ITEM_SIZE);
	target_parent->size -= DIR_ITEM_SIZE;
	ctrl.ForceUpdateInode(target_parent);
	ctrl.FreeInodeBlock(target_parent);
	ctrl.FreeInodeBlock(target_file.first);
	return 0;
}

int Copy(const std::string& old_path, const std::string& new_path)
{
	if (old_path == "/") {
		std::cout << "禁止复制根目录" << std::endl;
		return -1;
	}
	// 暂未考虑总空间够不够
	// 权限也没考虑
	
	// 一系列检查，和建立软硬链接的差不多
	Inode* target = ctrl.PathToInode(old_path);
	if (target == nullptr) {
		std::cout << "要复制的文件的路径无效" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "暂不能复制目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(new_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "目标文件不合法" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "目标路径不存在" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "目标的上一级需要是目录" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "目标目录下已经存在同名文件" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("该目录下文件已满");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	ctrl.FreeInodeBlock(parent);

	// 可以复制了
	ASSERT(Create(new_path) == 0);
	Inode* cur = ctrl.PathToInode(new_path);
	ctrl.ReadInodeFromDisk(cur);
	char buf2[BLOCK_SIZE];
	offset = FILE_HEADER_SIZE;
	while (1) {
		int rcnt = ctrl.ReadInode(target, buf2, offset, BLOCK_SIZE);
		if (rcnt == 0) break;
		int wcnt = ctrl.WriteInode(cur, buf2, offset, rcnt);
		ASSERT(rcnt == wcnt);
		offset += rcnt;
	}

	ctrl.FreeInodeBlock(cur);
	ctrl.FreeInodeBlock(target);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下和交互有关


//系统初始化
static void Init() {
	path_cur = "/";  //这里放的是根目录名称

	for (int i = 0; i < oft.table.size(); ++i) {
		oft.table[i].inode = nullptr;
		oft.table[i].ref = 0;
	}
}

//登录函数
static bool Login(const std::string& name, const std::string& password) {
	SuperBlock super = GetDisk().GetSuperBlock();

	for (int i = 0; i < super.users.size(); i++) {
		std::string n = ArrayToString(super.users[i].username);
		std::string p = ArrayToString(super.users[i].password);

		if (name == n && password == p) {
			user_cur.username = super.users[i].username;
			user_cur.password = super.users[i].password;
			return true;  //登录成功
		}
	}
	return false;  //登陆失败
}

// 展示当前所在目录 
static void ShowDir() {
	Inode* inode = ctrl.PathToInode(path_cur);
	ASSERT(inode); // 当前所在路径应该总是有效的
	ctrl.ReadInodeFromDisk(inode);
	char buf[32];
	uint32_t offset = 0, cnt = 0;
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// 遍历该目录下每个项
		int ret = ctrl.ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (ret == 0) {
			// 这里返回0认为是目录项还没有那么多，
			break;
		}
		ASSERT(ret == DIR_ITEM_SIZE);

		std::string name;
		for (int i = 0; i < N_DIR_NAME && buf[i]; ++i) name += buf[i];
		std::cout << name << ' ';
		cnt++;
		if (cnt % 5 == 0) {  // 输出的时候一行显示5个吧
			std::cout << std::endl;
		}
	}
	ctrl.FreeInodeBlock(inode);
}

// cd
static void Chdir(const std::string& path) {
	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "路径无效" << std::endl;
		return;
	}

	ctrl.ReadInodeFromDisk(inode);
	if (inode->type != (uint16_t)FileType::Dir) {
		std::cout << "目标不是文件夹" << std::endl;
		ctrl.FreeInodeBlock(inode);
		return;
	}

	path_cur = NormalizedPath(path);	
	ctrl.FreeInodeBlock(inode);
}

//向文件写入信息add
//文件path，写入信息src，mode=0覆盖=1追加
void WriteFile(const std::string& path, int mode, const std::string& content) {

	int fd = Open(path);
	if (fd < 0) return;
	Inode* inode = oft.table[fd].inode;
	
	int ret = IsSoftLink(inode);
	if (ret == -2) {
		ASSERT(Close(fd) == 0);
		return;
	}
	bool flag = false;
	if (ret != -1) { 
		// 换一下inode
		inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		// 需要再单独控制下ref
		flag = true;
	}

	if (!IsRoot()) {   // 所以这里是已原文件的权限为准了
		FileHeader fh = GetFileHeader(inode);
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorWrite(fh.permission) == false) {
				std::cout << "没有写权限" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				ASSERT(Close(fd) == 0);
				return;
			}
		}
		else {
			if (OthersWrite(fh.permission) == false) {
				std::cout << "没有写权限" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				ASSERT(Close(fd) == 0);
				return;
			}
		}
	}
	ctrl.ReadInodeFromDisk(inode);
	if (mode == 0) {
		FileHeader fh = GetFileHeader(inode);
		uint16_t type = inode->type;
		ctrl.ClearInode(inode);
		inode->type = type;
		ctrl.ForceUpdateInode(inode);
		int wcnt = ctrl.WriteInode(inode, (char*)&fh, 0, FILE_HEADER_SIZE);
		ASSERT(wcnt == FILE_HEADER_SIZE);
		wcnt = ctrl.WriteInode(inode, content.c_str(), FILE_HEADER_SIZE, content.length());
		ASSERT(wcnt == content.length());

		inode->size = FILE_HEADER_SIZE + wcnt;  // 文件大小=header+数据
		ctrl.ForceUpdateInode(inode);
	}
	else if(mode == 1){
		std::string t(inode->size == FILE_HEADER_SIZE ? content : ('\n' + content));
		int wcnt = ctrl.WriteInode(inode, t.c_str(), inode->size, t.length());
		ASSERT(wcnt == t.length());
	}
	else {
		ASSERT(0);
	}

	if (flag) ctrl.FreeInodeBlock(inode);
	ASSERT(Close(fd) == 0);
	return;
}

//读取文件内容
static void ReadFile(const std::string& path) {
	int fd = Open(path);
	if (fd < 0) return;
	char buf[BLOCK_SIZE];
	uint32_t offset = FILE_HEADER_SIZE;
	while (1) {
		int rcnt = Read(fd, buf, offset, BLOCK_SIZE);
		if (rcnt <= 0) break;
		for (int i = 0; i < rcnt; ++i) std::cout << buf[i];   // 直接输出
		offset += rcnt;
	}
	ASSERT(Close(fd) == 0);
	return;
}

void Delete(const std::string& path) {
	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "路径无效" << std::endl;
		return;
	}

	if (!IsRoot()) {
		Inode* p = ctrl.PathToInodeParent(path);
		FileHeader fh = GetFileHeader(p);  // ！认为删除文件需要其父目录的写权限，与其自身无关
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorWrite(fh.permission) == false) {
				std::cout << "没有写权限" << std::endl;
				ctrl.FreeInodeBlock(inode);
				ctrl.FreeInodeBlock(p);
				return;
			}
		}
		else {
			if (OthersWrite(fh.permission) == false) {
				std::cout << "没有写权限" << std::endl;
				ctrl.FreeInodeBlock(inode);
				ctrl.FreeInodeBlock(p);
				return;
			}
		}
		ctrl.FreeInodeBlock(p);
	}
	ctrl.ReadInodeFromDisk(inode);
	int ret;
	if (inode->type == (uint16_t)FileType::Dir) {
		ctrl.FreeInodeBlock(inode);  // 这里ref--对应开头的PathToInode，免得影响实际的Delete
		ret = DeleteDir(path);
	}
	else if (inode->type == (uint16_t)FileType::File) {
		ctrl.FreeInodeBlock(inode);
		ret = DeleteFile(path);
	}
	else {
		ASSERT(0);
	}

	if (ret == 0) {
		std::cout << "删除成功" << std::endl;
	}
	else {
		std::cout << "删除失败" << std::endl;
	}
}

//string --> Array<char,20> 
std::array<char, 20> StringToArray(std::string s) {
	std::array<char, 20> c;
	c.fill('\0');
	for (int i = 0; i < 20 && i < s.size(); i++) {
		c[i] = s[i];
	}
	return c;
}

//用户注册，需要root权限，
void UserAdd(std::string userAdd, std::string passwordAdd) {
	SuperBlock super = GetDisk().GetSuperBlock();
	std::array<char, 20> root = StringToArray("root");

	for (int i = 0; i < 20; i++) {
		if (user_cur.username[i] != root[i]) {
			printf("无权限");
			return;
		}
	}

	for (int i = 0; i < N_USER; i++) {
		if (userAdd == ArrayToString(super.users[i].username)) {
			printf("用户已存在！");
			return;
		}
	}
	int i;
	for (i = 1; i < N_USER; i++) {
		if (ArrayToString(super.users[i].username).empty()) {
			break;
		}
	}
	if (i == N_USER) {
		printf("用户表已满！");
		return;
	}
	super.users[i].username = StringToArray(userAdd);
	super.users[i].password = StringToArray(passwordAdd);
	GetDisk().WriteSuperBlock(super);
	printf("用户添加成功");
}

//删除用户
void UserDelete(std::string userDelete) {
	SuperBlock super = GetDisk().GetSuperBlock();
	std::array<char, 20>root = StringToArray("root");
	for (int i = 0; i < 20; i++) {
		if (user_cur.username[i] != root[i]) {
			printf("无权限");
			return;
		}
	}
	int i;
	for (i = 1; i < 10; i++) {
		if (ArrayToString(super.users[i].username) == userDelete) {
			break;
		}
	}
	if (i == N_USER) {
		printf("不存在该用户！");  // root这里算是不存在
		return;
	}
	super.users[i].username = StringToArray("");
	super.users[i].password = StringToArray("");
	GetDisk().WriteSuperBlock(super);
	printf("用户删除成功");
}


//更新FileHeader信息
void WriteFileHeader(Inode* inode, std::array<char, 20>filename, std::array<char, 20>username, uint32_t permission, uint32_t inum) {
	FileHeader fh;
	char message[FILE_HEADER_SIZE];
	int i;
	for (i = 0; i < 20; i++) {
		message[i] = filename[i];
	}
	for (i; i < 40; i++) {
		message[i] = username[i - 20];
	}

	std::memmove(message + 40, &permission, sizeof(uint32_t));
	std::memmove(message + 44, &inum, sizeof(uint32_t));

	ctrl.ReadInodeFromDisk(inode);
	if (inode->type == (uint16_t)FileType::File) {
		ctrl.WriteInode(inode, message, 0, sizeof(FileHeader));
	}
	else if (inode->type == (uint16_t)FileType::Dir) {
		ctrl.WriteInode(inode, message, BLOCK_SIZE - 100, sizeof(FileHeader));
	}
	else {
		ASSERT(0);
	}
	ctrl.ForceUpdateInode(inode);
}


int Chmod(const std::string& path, uint32_t permission) {
	Inode* inode = ctrl.PathToInode(path);
	FileHeader fh = GetFileHeader(inode);

	//root用户不用查权限
	if (!IsRoot()) {
		if ((ArrayToString(user_cur.username) == ArrayToString(fh.filename) && CreatorWrite(fh.permission)) || \
			((ArrayToString(user_cur.username) != ArrayToString(fh.filename) && OthersWrite(fh.permission)))) {
			//有写权限
		}
		else {
			printf("修改失败，无权限");
			ctrl.FreeInodeBlock(inode);
			return -1;
		}
	}
	fh.permission = permission;
	WriteFileHeader(inode, fh.filename, fh.username, fh.permission, inode->inum);
	ctrl.FreeInodeBlock(inode);
	return 0;
}

void ProcessCommand()
{
	Init();
	std::string commond1;
	std::string commond2;
	std::cout << "Input username: ";
	std::cin >> commond1;
	std::cout << "password: ";
	std::cin >> commond2;
	if (Login(commond1, commond2)) {
		std::cout << "登录成功" << std::endl;
		std::string name = commond1;
		std::getline(std::cin, commond1); // 读一下回车

		while (1) {
			std::cout << "[" << name << "@" << path_cur << "] $ > ";
			
			std::getline(std::cin, commond1);
			auto v = Split(commond1);
			if (v.empty()) continue;
			if (v[0] == "dir" || v[0] == "cd") {
				if (v.size() == 2) {
					if (v[1].front() == '/') {
						// 认为是绝对路径
						Chdir(v[1]);
					}
					else {
						// 相对路径
						Chdir(path_cur + "/" + v[1]);
					}
				}
				else {
					std::cout << "命令非法： " << "cd/dir [path]" << std::endl;
				}
			}
			else if (v[0] == "ls") {
				if (v.size() == 1) {
					ShowDir();
				}
				else {
					std::cout << "命令非法： " << "无需其他参数" << std::endl;
				}
			}
			else if (v[0] == "format") {
				if (v.size() == 1) {
					if (ArrayToString(user_cur.username) == "root") {
						Format();
					}
					else {
						std::cout << "权限不够" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "无需其他参数" << std::endl;
				}
			}
			else if (v[0] == "touch") {
				if (v.size() == 2) {
					// ！检查名字合法性
					if (CheckFileName(v[1])) {
						Create(NormalizedPath(path_cur + "/" + v[1]));
					}
					else {
						std::cout << "名字不合法" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "touch [path]" << std::endl;
				}
			}
			else if (v[0] == "mkdir") {
				if (v.size() == 2) {
					// ！检查名字合法性
					if (CheckFileName(v[1])) {
						MkDir(NormalizedPath(path_cur + "/" + v[1]));
					}
					else {
						std::cout << "名字不合法" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "mkdir [path]" << std::endl;
				}
			}
			else if (v[0] == "cat") {
				if (v.size() == 2) {
					ReadFile(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "命令非法： " << "cat [path]" << std::endl;
				}
			}
			else if (v[0] == "rm") {
				if (v.size() == 2) {
					Delete(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "命令非法： " << "rm [path]" << std::endl;
				}
			}
			else if (v[0] == "stat") {
				if (v.size() == 2) {
					Stat(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "命令非法： " << "stat [path]" << std::endl;
				}
			}
			else if (v[0] == "chmod") {
				if (v.size() == 3) {
					if (v[2].length() == 2
						&& v[2][0] >= '0' && v[2][0] <= '3'
						&& v[2][1] >= '0' && v[2][1] <= '3') {
						       // 使用两位数字表示 每位0~3对应读写的两位，00，01，10，11
						Chmod(NormalizedPath(path_cur + "/" + v[1]), ((v[2][0] - '0') << 2) | (v[2][1] - '0'));
					}
					else {
						std::cout << "参数错误： " << "权限用两位整数表示，每位0~3\n对应00~11（前一位写后一位读，1允许0禁止）" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "chmod [path] [permission]" << std::endl;
				}
			}
			else if (v[0] == "ln") {
				if (v.size() == 3) {
					HardLink(NormalizedPath(path_cur + "/" + v[1]), NormalizedPath(path_cur + "/" + v[2]));
				}
				else if (v.size() == 4 && v[1] == "-s") {
					SoftLink(NormalizedPath(path_cur + "/" + v[2]), NormalizedPath(path_cur + "/" + v[3]));
				}
				else {
					std::cout << "命令非法： " << "ln (-s) [target] [link]" << std::endl;
				}
			}
			else if (v[0] == "cp") {
				if (v.size() == 3) {
					Copy(NormalizedPath(path_cur + "/" + v[1]), NormalizedPath(path_cur + "/" + v[2]));
				}
				else {
					std::cout << "命令非法： " << "cp [old] [new]" << std::endl;
				}
			}
			else if (v[0] == "mv") {
				if (v.size() == 3) {
					Move(NormalizedPath(path_cur + "/" + v[1]), NormalizedPath(path_cur + "/" + v[2]));
				}
				else {
					std::cout << "命令非法： " << "mv [old] [new]" << std::endl;
				}
			}
			else if (v[0] == "\\q") {
				std::cout << "退出系统" << std::endl;
				break;
			}
			else if (v[0] == "useradd") {
				if (v.size() == 3) {
					if (v[1].length() <= 20 && v[2].length() <= 20) {
						UserAdd(v[1], v[2]);
					}
					else {
						std::cout << "名字不合法" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "useradd [username] [password]" << std::endl;
				}
			}
			else if (v[0] == "userdel") {
				if (v.size() == 2) {
					if (v[1].length() <= 20) {
						UserDelete(v[1]);
					}
					else {
						std::cout << "名字不合法" << std::endl;
					}
				}
				else {
					std::cout << "命令非法： " << "userdel [username]" << std::endl;
				}
			}
			else if (v.size() == 3) {  
				if (v[1] == "<") {
					WriteFile(path_cur + "/" + v[0], 0, v[2]);
				}
				else if(v[1] == "<<"){
					WriteFile(path_cur + "/" + v[0], 1, v[2]);
				}
				else {
					std::cout << "命令非法： " << "写文件：[path] </<< [content]" << std::endl;
				}

			}
			else {
				std::cout << "无效指令" << std::endl;
			}

			std::cout << std::endl;
		}
	}
	else {
		std::cout << "登录失败，重启系统......" << std::endl;
	}
}