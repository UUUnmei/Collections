#ifndef COMMON_H
#define COMMON_H

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <array>

constexpr uint32_t DISK_SIZE = 8 * 1024 * 1024;      // ������̴�С 8MB
constexpr uint32_t BLOCK_SIZE = 1024;                // ���̿��С���ֽ�
constexpr uint32_t NBLOCK = DISK_SIZE / BLOCK_SIZE;  // ������
constexpr uint32_t NDIRECT = 13;  // inode��ֱ����������
constexpr uint32_t NINDIRECT = BLOCK_SIZE / sizeof(uint32_t);  // inode��Ϊ�������ʱ�����������256����ַ
constexpr uint32_t MAX_FILE_SIZE = BLOCK_SIZE * (NDIRECT + NINDIRECT);  // һ���ļ�������С

constexpr uint32_t MAGIC_NUMBER = 0x01145140;
constexpr uint16_t SUPERBLOCK_ID = 0;  // ������Ŀ��   // һ��ʹ��ʱ����Ϊ0�ſ�����Ч���!��
constexpr uint16_t INODE_BITMAP_BLOCKID = 1;  // inode��bitmap���ڿ��
constexpr uint16_t DATA_BITMAP_BLOCKID = 2;   // ���ݿ�bitmap���ڿ��
constexpr uint16_t N_INODE = 8192;            // inode����
constexpr uint16_t INODE_START_BLOCK = 3;     // inode��һ����
constexpr uint16_t N_INODE_BLOCK = 512;  // inode������  sizeof(dinode)*N_INODE/N_INODE
constexpr uint16_t DATA_START_BLOCK = INODE_START_BLOCK + N_INODE_BLOCK;  // ���ݿ��һ����
constexpr uint16_t N_DATA_BLOCK = NBLOCK - DATA_START_BLOCK;  // ���ݿ�����

constexpr uint32_t IPB = BLOCK_SIZE / 64;  // ÿ�����̿��м���inode��inode��С64B
constexpr uint32_t BIT_PER_BLOCK = 8 * BLOCK_SIZE;  //ÿ��bitmap���ж���bit

constexpr uint8_t N_DIR_ITEM = 16;  // ÿ��Ŀ¼���������
constexpr uint8_t N_DIR_NAME = 20;  // Ŀ¼��/�ļ�����󳤶ȣ���\0)  �ݶ������ﲢû����һ��
constexpr uint8_t DIR_ITEM_SIZE = N_DIR_NAME + sizeof(uint32_t);  // һ��Ŀ¼��ռ�Ĵ�С

constexpr uint32_t FILE_HEADER_SIZE = 100;  //FileHeaderռ�Ĺ̶���С
constexpr uint8_t N_USER = 10;   // ����û�

constexpr uint32_t ROOT_DIR_INODE = 0;  // ��Ŀ¼��inode


#define ASSERT(expression)                                                \
    do {                                                                  \
        if (!(expression)) {                                              \
            printf("ASSERT FAIL: [%s][line:%d] %s\n", __FILE__, __LINE__, \
                   #expression);                                          \
            exit(1);                                                      \
        }                                                                 \
    } while (0);

enum FileType {  // û��enum class��Ҫ���Ƿ�������ת���ˣ�
	File = 1,
	Dir
};

struct Dinode {
	uint16_t type{ 0 };    //�ļ����ͣ�Ŀ¼ or ��ͨ�ļ���
	uint16_t n_link{ 0 };  // inode��������ü��������ӣ�
	uint32_t size{ 0 };  // �ļ���С  ����Ŀ¼����С����Ŀ¼�����*ITEMSIZE�����Ǳ���Ŀ¼��ʱ��Ҫ����С��Ŀ¼��洢��һ��������
	uint32_t addr[NDIRECT + 1]{ 0 };  // ���������ݿ��ַ��NDIRECT��ֱ��������1���������ָ����һ��inode����һ��ȫ����Ϊ�ǵ�ַ��
			 // ���ݿ�ĵ�ַ��ʵ�������ݿ���
};

struct Inode {
	uint8_t valid{ 0 };  // inode�Ƿ��Ѿ���Ӳ�̶���
	uint16_t ref{ 0 };   // �ڴ������ü���
	uint16_t inum{ 0 };  //  ��inode��� �������� - ��һ��inode���)

	// ��Ӧ����inode�Ŀ���
	uint16_t type{ 0 };
	uint16_t n_link{ 0 };
	uint32_t size{ 0 };
	uint32_t addr[NDIRECT + 1]{ 0 };
};

// ������Щ�����еĳ����������С���д���һ���̶�

struct DirItem {
	std::array<char, N_DIR_NAME> name;  //�ļ��� ����������ĸ�����»��ߺ�'.'���� . ����һ�����������
	uint32_t inum;                      // ��Ӧinode��
};

struct Directory {
	std::array<DirItem, N_DIR_ITEM> items;  // ÿ��Ŀ¼�µ��ļ����У�
};

struct FileHeader {
	// �ļ���Ԫ���ݣ����û���� �ĸ��û����ģ���дȨ��֮���
	// Ҫ���С�̶�  �������ļ�����Ĵ�С
	std::array<char, 20> filename;
	std::array<char, 20> username;
	uint32_t permission;
	uint32_t inum; //���ļ�ʵ��ָ���inode�ţ�emmһ�������Ӧ�ú�Ŀ¼�ж�Ӧ��inum��ͬ�������ͬ��˵�����Ǹ��������ļ���
};

struct File {
	//FileHeader header;
	Inode* inode;
	uint32_t ref;  // ���ļ�����
};

// ϵͳ�򿪱�������Ҫ�û�����һ���򿪱��ˣ�
struct OpenFileTable {
	std::array<struct File, 30> table;
};

// �û���Ϣ�����ڳ�������
struct User {
	std::array<char, 20> username;
	std::array<char, 20> password;
};

#endif  // COMMON_H