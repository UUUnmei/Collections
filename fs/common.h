#ifndef COMMON_H
#define COMMON_H

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <array>

constexpr uint32_t DISK_SIZE = 8 * 1024 * 1024;      // 虚拟磁盘大小 8MB
constexpr uint32_t BLOCK_SIZE = 1024;                // 磁盘块大小，字节
constexpr uint32_t NBLOCK = DISK_SIZE / BLOCK_SIZE;  // 块总数
constexpr uint32_t NDIRECT = 13;  // inode里直接索引个数
constexpr uint32_t NINDIRECT = BLOCK_SIZE / sizeof(uint32_t);  // inode作为间接索引时的最大容量，256个地址
constexpr uint32_t MAX_FILE_SIZE = BLOCK_SIZE * (NDIRECT + NINDIRECT);  // 一个文件的最大大小

constexpr uint32_t MAGIC_NUMBER = 0x01145140;
constexpr uint16_t SUPERBLOCK_ID = 0;  // 超级块的块号   // 一般使用时，认为0号块是无效块号!，
constexpr uint16_t INODE_BITMAP_BLOCKID = 1;  // inode的bitmap所在块号
constexpr uint16_t DATA_BITMAP_BLOCKID = 2;   // 数据块bitmap所在块号
constexpr uint16_t N_INODE = 8192;            // inode总数
constexpr uint16_t INODE_START_BLOCK = 3;     // inode第一块块号
constexpr uint16_t N_INODE_BLOCK = 512;  // inode块总数  sizeof(dinode)*N_INODE/N_INODE
constexpr uint16_t DATA_START_BLOCK = INODE_START_BLOCK + N_INODE_BLOCK;  // 数据块第一块块号
constexpr uint16_t N_DATA_BLOCK = NBLOCK - DATA_START_BLOCK;  // 数据块总数

constexpr uint32_t IPB = BLOCK_SIZE / 64;  // 每个磁盘块有几个inode，inode大小64B
constexpr uint32_t BIT_PER_BLOCK = 8 * BLOCK_SIZE;  //每个bitmap块有多少bit

constexpr uint8_t N_DIR_ITEM = 16;  // 每个目录下最多项数
constexpr uint8_t N_DIR_NAME = 20;  // 目录名/文件名最大长度（含\0)  暂定，这里并没用完一块
constexpr uint8_t DIR_ITEM_SIZE = N_DIR_NAME + sizeof(uint32_t);  // 一个目录项占的大小

constexpr uint32_t FILE_HEADER_SIZE = 100;  //FileHeader占的固定大小
constexpr uint8_t N_USER = 10;   // 最多用户

constexpr uint32_t ROOT_DIR_INODE = 0;  // 根目录的inode


#define ASSERT(expression)                                                \
    do {                                                                  \
        if (!(expression)) {                                              \
            printf("ASSERT FAIL: [%s][line:%d] %s\n", __FILE__, __LINE__, \
                   #expression);                                          \
            exit(1);                                                      \
        }                                                                 \
    } while (0);

enum FileType {  // 没用enum class主要考虑方便类型转换了，
	File = 1,
	Dir
};

struct Dinode {
	uint16_t type{ 0 };    //文件类型（目录 or 普通文件）
	uint16_t n_link{ 0 };  // inode级别的引用计数（链接）
	uint32_t size{ 0 };  // 文件大小  对于目录，大小等于目录项个数*ITEMSIZE，但是遍历目录的时候不要看大小（目录项存储不一定连续）
	uint32_t addr[NDIRECT + 1]{ 0 };  // 关联的数据块地址（NDIRECT个直接索引，1个间接索引指向另一个inode，那一块全都认为是地址）
			 // 数据块的地址其实就是数据块块号
};

struct Inode {
	uint8_t valid{ 0 };  // inode是否已经从硬盘读入
	uint16_t ref{ 0 };   // 内存中引用计数
	uint16_t inum{ 0 };  //  该inode编号 （物理块号 - 第一个inode块号)

	// 对应磁盘inode的拷贝
	uint16_t type{ 0 };
	uint16_t n_link{ 0 };
	uint32_t size{ 0 };
	uint32_t addr[NDIRECT + 1]{ 0 };
};

// 下面这些定义中的常量（数组大小）有待进一步商定

struct DirItem {
	std::array<char, N_DIR_NAME> name;  //文件名 ！仅允许字母数字下划线和'.'（且 . 仅有一个不能在最后）
	uint32_t inum;                      // 对应inode号
};

struct Directory {
	std::array<DirItem, N_DIR_ITEM> items;  // 每个目录下的文件（夹）
};

struct FileHeader {
	// 文件的元数据，存用户相关 哪个用户建的，读写权限之类的
	// 要求大小固定  不计入文件本身的大小
	std::array<char, 20> filename;
	std::array<char, 20> username;
	uint32_t permission;
	uint32_t inum; //该文件实际指向的inode号，emm一般情况下应该和目录中对应的inum相同，如果不同，说明这是个软链接文件！
};

struct File {
	//FileHeader header;
	Inode* inode;
	uint32_t ref;  // 打开文件计数
};

// 系统打开表，（不需要用户单独一个打开表了）
struct OpenFileTable {
	std::array<struct File, 30> table;
};

// 用户信息，存在超级块中
struct User {
	std::array<char, 20> username;
	std::array<char, 20> password;
};

#endif  // COMMON_H