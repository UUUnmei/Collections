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

	uint32_t DEFAULT_PERMISSION = 0b1101;   //�Լ��ɶ�д ������ֻ��
}

//��дȨ�޼��
bool CreatorWrite(uint32_t i) { return (i & 8) == 8; }
bool CreatorRead(uint32_t i) { return (i & 4) == 4; }
bool OthersWrite(uint32_t i) { return (i & 2) == 2; }
bool OthersRead(uint32_t i) { return (i & 1) == 1; }

//�ļ����Ϸ��Լ��
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


//��õ�ǰ�ļ���FileHeader
static FileHeader GetFileHeader(Inode* inode) {
	FileHeader fh;
	char message[FILE_HEADER_SIZE];

	ctrl.ReadInodeFromDisk(inode);
	if (inode->type == (uint16_t)FileType::File) {
		ctrl.ReadInode(inode, message, 0, sizeof(FileHeader));
	}
	else if (inode->type == (uint16_t)FileType::Dir) {
		uint32_t sz = inode->size;
		inode->size = 1024;  //Ӳ��
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
	if (parent == nullptr) {  // ����������
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	Inode* child = ctrl.AllocInodeBlock(type);
	child->n_link = 1;
	ctrl.ForceUpdateInode(child);

	bool succ = ctrl.AddItemInDir(parent, ret.second, child->inum);  // ����Ͱ����˼������������ ������"/"�����ã���Ŀ¼�ڸ�ʽ��ʱ��������

	if (succ == false) {
		// ���ʧ�ܣ��ͷŸշ����inode
		// AddItemInDirʵ���б�֤�����ʧ�ܶ��ڸ���Ŀ¼������Ӱ��
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
		std::cout << "���̹��سɹ�" << std::endl;
		return 0;
	}
	std::cout << "���̹���ʧ��" << std::endl;
	return -1;
}

int Create(const std::string& path)
{
	// ��Ӧ���ȼ��·���Ϸ��ԣ�����˵����������������ĵط���� 
	int ret = CreateDetail(path, FileType::File);
	if (ret == -1) {
		std::cout << "����ʧ��" << std::endl;
		return -1;
	}
	// дFileHeader
	// ��һ�����ݿ��ǰ100���ֽ�
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
	uint32_t num = inode->inum;  // λ�����⣬��Ƶ�һ��覴á�����
	memcpy(input + 44, &num, sizeof(uint32_t));  // ����һ��inum
	ctrl.ReadInodeFromDisk(inode);
	ctrl.WriteInode(inode, input, 0, sizeof(FileHeader));
	inode->size = FILE_HEADER_SIZE;
	ctrl.ForceUpdateInode(inode);
	ctrl.FreeInodeBlock(inode);
	return ret;
}

int MkDir(const std::string& path)
{
	// ��Ӧ���ȼ��·���Ϸ��ԣ�����˵����������������ĵط���� 
	int ret = CreateDetail(path, FileType::Dir);
	if (ret == -1) {
		std::cout << "����ʧ��" << std::endl;
		return -1;
	}
	// дFileHeader
	// ��һ�����ݿ�ĺ�100���ֽ�
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
	uint32_t num = inode->inum;  // λ�����⣬��Ƶ�һ��覴á�����
	memcpy(input + 44, &num, sizeof(uint32_t));  // ����һ��inum
	ctrl.ReadInodeFromDisk(inode);

	// ����Ϊ����Ŀ¼��fileheaderд��ȥ
	      // Ӳ��
	inode->size = 1024;  
	ctrl.WriteInode(inode, input, BLOCK_SIZE - 100, sizeof(FileHeader));
	inode->size = 0;
	ctrl.ForceUpdateInode(inode);

	ctrl.FreeInodeBlock(inode);
	return ret;
}

int Format()
{
	// ������ʽ�������ó����飬bitmap������Ա
	GetDisk().Format();

	// ������Ŀ¼ 
	auto inode = ctrl.AllocInodeBlock(FileType::Dir);  // ��Ŀ¼ inode��Ϊ0
	inode->n_link = 1;
	ctrl.ForceUpdateInode(inode);
	ctrl.FreeInodeBlock(inode);

	std::cout << "��ʽ�����" << std::endl;
	return 0;
}

int IsSoftLink(Inode* inode);
int Read(uint32_t fd, char* dst, uint32_t offset, uint32_t cnt)
{
	// ����fdȥ���ļ���ȡinode �Լ� ���Ȩ��

	ASSERT(fd < oft.table.size());
	Inode* inode = oft.table[fd].inode;
	int ret = IsSoftLink(inode); // ����ͬwrite
	if (ret == -2) {
		return -1;
	}
	bool flag = false;
	if (ret != -1) {
		// ��һ��inode
		inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		// ��Ҫ�ٵ���������ref
		flag = true;
	}

	if (!IsRoot()) {
		FileHeader fh = GetFileHeader(inode);
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorRead(fh.permission) == false) {
				std::cout << "û�ж�Ȩ��" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				return -1;
			}
		}
		else {
			if (OthersRead(fh.permission) == false) {
				std::cout << "û�ж�Ȩ��" << std::endl;
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
//	// ����fdȥ���ļ���ȡinode �Լ� ���Ȩ��
//	//int ret = ctrl.WriteInode(inode, dst, offset, cnt);
//	//return ret;
//
//	// ������ ����·��������ֵ�WriteFile��Ҫ���ָ���д��׷��д��
//}

int DeleteFile(const std::string& path)
{
	// ���Ȩ�ޣ��Ҹ��ļ����ܱ����������̡���
	bool ret = ctrl.DeleteFile(path);
	return (ret == true ? 0 : -1);
}

int DeleteDir(const std::string& path)
{
	if (NormalizedPath(path) == "/"){
		std::cout << "����ɾ��Ŀ¼" << std::endl;
		return -1; 
	}

	// ���Ȩ�ޣ��Ҹ��ļ��в��ܱ����������̡���
	bool ret = ctrl.DeleteDir(path);
	return (ret == true ? 0 : -1);
}


// �ڴ��ļ����в��ң��ҵ�ref+1�����ҿ�λ���򱨴�
//   �����ļ�������
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
		std::cout << "·����Ч" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(inode);
	if (inode->type != (uint16_t)FileType::File) {
		std::cout << "�򿪵Ĳ����ļ�" << std::endl;
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
		std::cout << "��ֹ�Ը�Ŀ¼��������" << std::endl;
		return -1;
	}
	// ��δ����Ȩ������

	Inode* target = ctrl.PathToInode(target_path);
	if (target == nullptr) {
		std::cout << "������·����Ч" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "��������Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(link_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "���������Ϸ�" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "Ŀ��·��������" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "Ŀ�����һ����Ҫ��Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "Ŀ��Ŀ¼���Ѿ�����ͬ���ļ�" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("��Ŀ¼���ļ�����");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// ���Խ���
	target->n_link++;
	ctrl.ForceUpdateInode(target);
	ctrl.AddItemInDir(parent, ret.second, target->inum); // �µ��ļ�����ԭ����inum ����ʵ���Ҳ���������һ���ּ�飬��Ҫ�����Ӻ����������ʱ����

	ctrl.FreeInodeBlock(target);
	ctrl.FreeInodeBlock(parent);

	return 0;
}



int SoftLink(const std::string& target_path, const std::string& link_path)
{
	if (target_path == "/") {
		std::cout << "��ֹ�Ը�Ŀ¼��������" << std::endl;
		return -1;
	}
	// ��δ����Ȩ��
	// һ��ʼ��Щ���������Ӳ����һ��
	Inode* target = ctrl.PathToInode(target_path);
	if (target == nullptr) {
		std::cout << "������·����Ч" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "��������Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(link_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "���������Ϸ�" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "Ŀ��·��������" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "Ŀ�����һ����Ҫ��Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "Ŀ��Ŀ¼���Ѿ�����ͬ���ļ�" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("��Ŀ¼���ļ�����");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// ���Խ���
	ASSERT(Create(link_path) == 0);
	
	// �޸�fileheader�е�inum
	Inode* link = ctrl.PathToInode(link_path);
	FileHeader fh = GetFileHeader(link);
	fh.inum = target->inum; // ��������
	ctrl.WriteInode(link, (char*)&fh, 0, sizeof(FileHeader));
	ctrl.FreeInodeBlock(link);

	ctrl.FreeInodeBlock(target);
	ctrl.FreeInodeBlock(parent);
	return 0;
}

// Ϊ�˼��������׼�����ر��
///  ��Ҫ��ԭ�����ڶ�ȡӲ�̵�ʱ��˳���һ���ǲ����ѷ����
//   �����ǵ������ӣ�ɾ��ԭ�ļ�������ʧЧ����������������ASSERT������ϵͳ�ͼ���
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

// ����-2��ʾ·����Ч��-1��ʾ���������ӣ����򷵻����Ӷ����inum
int IsSoftLink(Inode* inode) {
	int ret = inode->inum;
	// ��鵽��  // ��Ӳ���Ӳ�����
	int cnt = 1;
	while (1) {  
		Inode* cur = ctrl.FindInode(ret);
		ReadInodeFromDiskUncheck(cur);
		if (cur->type == 0) {
			std::cout << "�����ļ���ʧЧ" << std::endl;
			cur->ref--;   // ��ʱ��cur��δ����ģ�������FreeInode ������bitmap��
			cur->inum = 0;
			cur->valid = 0;
			return -2;
		}
		FileHeader fh = GetFileHeader(cur);
		ctrl.FreeInodeBlock(cur);
		if (ret != fh.inum) {
			ret = fh.inum;  // �������ӣ�����
			cnt++;
		}
		else {
			if (cnt == 1) {
				return -1;  // ��������
			}
			else {
				// �ҵ������
				break;
			}
		}
		if (cnt > 10) {
			std::cout << "���Ӳ㼶����\n";
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
		std::cout << "·����Ч" << std::endl;
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

	// ���� ��ʾһ��������
	std::string linkname;
	int ret = IsSoftLink(inode);
	if (ret == -2) {
		linkname = "?";
	}
	if (ret != -1 && ret != -2) {
		// ��һ��inode
		Inode* inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		FileHeader fh2 = GetFileHeader(inode);
		linkname = ArrayToString(fh2.filename);
		ctrl.FreeInodeBlock(inode);
	}

	// ���������ʽ
	// �ļ����ͼ�Ȩ��
	printf("%s", info.name.c_str());
	if (linkname.empty() == false) {
		printf(" -> %s", linkname.c_str());  // �ṹ���ޣ���������չʾ�������ļ���ȫ��·��������ʾ��
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
		std::cout << "��ֹ�ƶ���Ŀ¼" << std::endl;
		return -1;
	}
	// ����ֻ���޸�Ŀ¼
	// û����Ȩ��

	Inode* target = ctrl.PathToInode(old_path);
	if (target == nullptr) {
		std::cout << "Ҫ�ƶ����ļ���Ч" << std::endl;
		return -1;
	}
	auto ret = SplitPath2(new_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "Ŀ���ļ������Ϸ�" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "Ŀ��·��������" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "Ŀ�����һ����Ҫ��Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "Ŀ��Ŀ¼���Ѿ�����ͬ���ļ�" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("��Ŀ¼���ļ�����");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}

	// �����޸�Ŀ¼��
	// ��Ŀ��·�������
	ctrl.AddItemInDir(parent, ret.second, target->inum);
	ctrl.FreeInodeBlock(parent);

	ctrl.FreeInodeBlock(target);
	// �����е�ɾ��
	Inode* target_parent = ctrl.PathToInodeParent(old_path);
	auto target_name = SplitPath2(old_path).second;
	auto target_file = ctrl.LookUpInDir(target_parent, target_name);

	// ��parentĿ¼���Ƴ� ��controller��delete��ʵ��
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
		std::cout << "��ֹ���Ƹ�Ŀ¼" << std::endl;
		return -1;
	}
	// ��δ�����ܿռ乻����
	// Ȩ��Ҳû����
	
	// һϵ�м�飬�ͽ�����Ӳ���ӵĲ��
	Inode* target = ctrl.PathToInode(old_path);
	if (target == nullptr) {
		std::cout << "Ҫ���Ƶ��ļ���·����Ч" << std::endl;
		return -1;
	}
	ctrl.ReadInodeFromDisk(target);
	if (target->type != (uint16_t)FileType::File) {
		std::cout << "�ݲ��ܸ���Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	auto ret = SplitPath2(new_path);
	if (CheckFileName(ret.second) == false) {
		std::cout << "Ŀ���ļ����Ϸ�" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	Inode* parent = ctrl.PathToInode(ret.first);
	if (parent == nullptr) {
		std::cout << "Ŀ��·��������" << std::endl;
		ctrl.FreeInodeBlock(target);
		return -1;
	}
	ctrl.ReadInodeFromDisk(parent);
	if (parent->type != (uint16_t)FileType::Dir) {
		std::cout << "Ŀ�����һ����Ҫ��Ŀ¼" << std::endl;
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	char buf[32];
	uint32_t offset = 0;
	int i = 0;
	for (i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int rcnt = ctrl.ReadInode(parent, buf, offset, DIR_ITEM_SIZE);
		if (rcnt == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(rcnt == DIR_ITEM_SIZE);

		std::string name;
		for (int j = 0; j < N_DIR_NAME && buf[j]; ++j) name += buf[j];
		if (name == ret.second) {
			std::cout << "Ŀ��Ŀ¼���Ѿ�����ͬ���ļ�" << std::endl;
			ctrl.FreeInodeBlock(target);
			ctrl.FreeInodeBlock(parent);
			return -1;
		}
	}
	if (i == N_DIR_ITEM) {
		printf("��Ŀ¼���ļ�����");
		ctrl.FreeInodeBlock(target);
		ctrl.FreeInodeBlock(parent);
		return -1;
	}
	ctrl.FreeInodeBlock(parent);

	// ���Ը�����
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
// ���ºͽ����й�


//ϵͳ��ʼ��
static void Init() {
	path_cur = "/";  //����ŵ��Ǹ�Ŀ¼����

	for (int i = 0; i < oft.table.size(); ++i) {
		oft.table[i].inode = nullptr;
		oft.table[i].ref = 0;
	}
}

//��¼����
static bool Login(const std::string& name, const std::string& password) {
	SuperBlock super = GetDisk().GetSuperBlock();

	for (int i = 0; i < super.users.size(); i++) {
		std::string n = ArrayToString(super.users[i].username);
		std::string p = ArrayToString(super.users[i].password);

		if (name == n && password == p) {
			user_cur.username = super.users[i].username;
			user_cur.password = super.users[i].password;
			return true;  //��¼�ɹ�
		}
	}
	return false;  //��½ʧ��
}

// չʾ��ǰ����Ŀ¼ 
static void ShowDir() {
	Inode* inode = ctrl.PathToInode(path_cur);
	ASSERT(inode); // ��ǰ����·��Ӧ��������Ч��
	ctrl.ReadInodeFromDisk(inode);
	char buf[32];
	uint32_t offset = 0, cnt = 0;
	for (int i = 0; i < N_DIR_ITEM; ++i, offset += DIR_ITEM_SIZE) {
		// ������Ŀ¼��ÿ����
		int ret = ctrl.ReadInode(inode, buf, offset, DIR_ITEM_SIZE);
		if (ret == 0) {
			// ���ﷵ��0��Ϊ��Ŀ¼�û����ô�࣬
			break;
		}
		ASSERT(ret == DIR_ITEM_SIZE);

		std::string name;
		for (int i = 0; i < N_DIR_NAME && buf[i]; ++i) name += buf[i];
		std::cout << name << ' ';
		cnt++;
		if (cnt % 5 == 0) {  // �����ʱ��һ����ʾ5����
			std::cout << std::endl;
		}
	}
	ctrl.FreeInodeBlock(inode);
}

// cd
static void Chdir(const std::string& path) {
	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "·����Ч" << std::endl;
		return;
	}

	ctrl.ReadInodeFromDisk(inode);
	if (inode->type != (uint16_t)FileType::Dir) {
		std::cout << "Ŀ�겻���ļ���" << std::endl;
		ctrl.FreeInodeBlock(inode);
		return;
	}

	path_cur = NormalizedPath(path);	
	ctrl.FreeInodeBlock(inode);
}

//���ļ�д����Ϣadd
//�ļ�path��д����Ϣsrc��mode=0����=1׷��
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
		// ��һ��inode
		inode = ctrl.FindInode(ret);
		ctrl.ReadInodeFromDisk(inode);
		// ��Ҫ�ٵ���������ref
		flag = true;
	}

	if (!IsRoot()) {   // ������������ԭ�ļ���Ȩ��Ϊ׼��
		FileHeader fh = GetFileHeader(inode);
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorWrite(fh.permission) == false) {
				std::cout << "û��дȨ��" << std::endl;
				if (flag) ctrl.FreeInodeBlock(inode);
				ASSERT(Close(fd) == 0);
				return;
			}
		}
		else {
			if (OthersWrite(fh.permission) == false) {
				std::cout << "û��дȨ��" << std::endl;
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

		inode->size = FILE_HEADER_SIZE + wcnt;  // �ļ���С=header+����
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

//��ȡ�ļ�����
static void ReadFile(const std::string& path) {
	int fd = Open(path);
	if (fd < 0) return;
	char buf[BLOCK_SIZE];
	uint32_t offset = FILE_HEADER_SIZE;
	while (1) {
		int rcnt = Read(fd, buf, offset, BLOCK_SIZE);
		if (rcnt <= 0) break;
		for (int i = 0; i < rcnt; ++i) std::cout << buf[i];   // ֱ�����
		offset += rcnt;
	}
	ASSERT(Close(fd) == 0);
	return;
}

void Delete(const std::string& path) {
	Inode* inode = ctrl.PathToInode(path);
	if (inode == nullptr) {
		std::cout << "·����Ч" << std::endl;
		return;
	}

	if (!IsRoot()) {
		Inode* p = ctrl.PathToInodeParent(path);
		FileHeader fh = GetFileHeader(p);  // ����Ϊɾ���ļ���Ҫ�丸Ŀ¼��дȨ�ޣ����������޹�
		if (ArrayToString(fh.username) == ArrayToString(user_cur.username)) {
			if (CreatorWrite(fh.permission) == false) {
				std::cout << "û��дȨ��" << std::endl;
				ctrl.FreeInodeBlock(inode);
				ctrl.FreeInodeBlock(p);
				return;
			}
		}
		else {
			if (OthersWrite(fh.permission) == false) {
				std::cout << "û��дȨ��" << std::endl;
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
		ctrl.FreeInodeBlock(inode);  // ����ref--��Ӧ��ͷ��PathToInode�����Ӱ��ʵ�ʵ�Delete
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
		std::cout << "ɾ���ɹ�" << std::endl;
	}
	else {
		std::cout << "ɾ��ʧ��" << std::endl;
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

//�û�ע�ᣬ��ҪrootȨ�ޣ�
void UserAdd(std::string userAdd, std::string passwordAdd) {
	SuperBlock super = GetDisk().GetSuperBlock();
	std::array<char, 20> root = StringToArray("root");

	for (int i = 0; i < 20; i++) {
		if (user_cur.username[i] != root[i]) {
			printf("��Ȩ��");
			return;
		}
	}

	for (int i = 0; i < N_USER; i++) {
		if (userAdd == ArrayToString(super.users[i].username)) {
			printf("�û��Ѵ��ڣ�");
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
		printf("�û���������");
		return;
	}
	super.users[i].username = StringToArray(userAdd);
	super.users[i].password = StringToArray(passwordAdd);
	GetDisk().WriteSuperBlock(super);
	printf("�û���ӳɹ�");
}

//ɾ���û�
void UserDelete(std::string userDelete) {
	SuperBlock super = GetDisk().GetSuperBlock();
	std::array<char, 20>root = StringToArray("root");
	for (int i = 0; i < 20; i++) {
		if (user_cur.username[i] != root[i]) {
			printf("��Ȩ��");
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
		printf("�����ڸ��û���");  // root�������ǲ�����
		return;
	}
	super.users[i].username = StringToArray("");
	super.users[i].password = StringToArray("");
	GetDisk().WriteSuperBlock(super);
	printf("�û�ɾ���ɹ�");
}


//����FileHeader��Ϣ
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

	//root�û����ò�Ȩ��
	if (!IsRoot()) {
		if ((ArrayToString(user_cur.username) == ArrayToString(fh.filename) && CreatorWrite(fh.permission)) || \
			((ArrayToString(user_cur.username) != ArrayToString(fh.filename) && OthersWrite(fh.permission)))) {
			//��дȨ��
		}
		else {
			printf("�޸�ʧ�ܣ���Ȩ��");
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
		std::cout << "��¼�ɹ�" << std::endl;
		std::string name = commond1;
		std::getline(std::cin, commond1); // ��һ�»س�

		while (1) {
			std::cout << "[" << name << "@" << path_cur << "] $ > ";
			
			std::getline(std::cin, commond1);
			auto v = Split(commond1);
			if (v.empty()) continue;
			if (v[0] == "dir" || v[0] == "cd") {
				if (v.size() == 2) {
					if (v[1].front() == '/') {
						// ��Ϊ�Ǿ���·��
						Chdir(v[1]);
					}
					else {
						// ���·��
						Chdir(path_cur + "/" + v[1]);
					}
				}
				else {
					std::cout << "����Ƿ��� " << "cd/dir [path]" << std::endl;
				}
			}
			else if (v[0] == "ls") {
				if (v.size() == 1) {
					ShowDir();
				}
				else {
					std::cout << "����Ƿ��� " << "������������" << std::endl;
				}
			}
			else if (v[0] == "format") {
				if (v.size() == 1) {
					if (ArrayToString(user_cur.username) == "root") {
						Format();
					}
					else {
						std::cout << "Ȩ�޲���" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "������������" << std::endl;
				}
			}
			else if (v[0] == "touch") {
				if (v.size() == 2) {
					// ��������ֺϷ���
					if (CheckFileName(v[1])) {
						Create(NormalizedPath(path_cur + "/" + v[1]));
					}
					else {
						std::cout << "���ֲ��Ϸ�" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "touch [path]" << std::endl;
				}
			}
			else if (v[0] == "mkdir") {
				if (v.size() == 2) {
					// ��������ֺϷ���
					if (CheckFileName(v[1])) {
						MkDir(NormalizedPath(path_cur + "/" + v[1]));
					}
					else {
						std::cout << "���ֲ��Ϸ�" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "mkdir [path]" << std::endl;
				}
			}
			else if (v[0] == "cat") {
				if (v.size() == 2) {
					ReadFile(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "����Ƿ��� " << "cat [path]" << std::endl;
				}
			}
			else if (v[0] == "rm") {
				if (v.size() == 2) {
					Delete(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "����Ƿ��� " << "rm [path]" << std::endl;
				}
			}
			else if (v[0] == "stat") {
				if (v.size() == 2) {
					Stat(NormalizedPath(path_cur + "/" + v[1]));
				}
				else {
					std::cout << "����Ƿ��� " << "stat [path]" << std::endl;
				}
			}
			else if (v[0] == "chmod") {
				if (v.size() == 3) {
					if (v[2].length() == 2
						&& v[2][0] >= '0' && v[2][0] <= '3'
						&& v[2][1] >= '0' && v[2][1] <= '3') {
						       // ʹ����λ���ֱ�ʾ ÿλ0~3��Ӧ��д����λ��00��01��10��11
						Chmod(NormalizedPath(path_cur + "/" + v[1]), ((v[2][0] - '0') << 2) | (v[2][1] - '0'));
					}
					else {
						std::cout << "�������� " << "Ȩ������λ������ʾ��ÿλ0~3\n��Ӧ00~11��ǰһλд��һλ����1����0��ֹ��" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "chmod [path] [permission]" << std::endl;
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
					std::cout << "����Ƿ��� " << "ln (-s) [target] [link]" << std::endl;
				}
			}
			else if (v[0] == "cp") {
				if (v.size() == 3) {
					Copy(NormalizedPath(path_cur + "/" + v[1]), NormalizedPath(path_cur + "/" + v[2]));
				}
				else {
					std::cout << "����Ƿ��� " << "cp [old] [new]" << std::endl;
				}
			}
			else if (v[0] == "mv") {
				if (v.size() == 3) {
					Move(NormalizedPath(path_cur + "/" + v[1]), NormalizedPath(path_cur + "/" + v[2]));
				}
				else {
					std::cout << "����Ƿ��� " << "mv [old] [new]" << std::endl;
				}
			}
			else if (v[0] == "\\q") {
				std::cout << "�˳�ϵͳ" << std::endl;
				break;
			}
			else if (v[0] == "useradd") {
				if (v.size() == 3) {
					if (v[1].length() <= 20 && v[2].length() <= 20) {
						UserAdd(v[1], v[2]);
					}
					else {
						std::cout << "���ֲ��Ϸ�" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "useradd [username] [password]" << std::endl;
				}
			}
			else if (v[0] == "userdel") {
				if (v.size() == 2) {
					if (v[1].length() <= 20) {
						UserDelete(v[1]);
					}
					else {
						std::cout << "���ֲ��Ϸ�" << std::endl;
					}
				}
				else {
					std::cout << "����Ƿ��� " << "userdel [username]" << std::endl;
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
					std::cout << "����Ƿ��� " << "д�ļ���[path] </<< [content]" << std::endl;
				}

			}
			else {
				std::cout << "��Чָ��" << std::endl;
			}

			std::cout << std::endl;
		}
	}
	else {
		std::cout << "��¼ʧ�ܣ�����ϵͳ......" << std::endl;
	}
}