#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#pragma once

#include "common.h"
#include <string>
#include <cstdio>


// ������
struct SuperBlock {
	uint32_t magic;        // �Զ����magic number �͵����ж��ǲ�������������͵��ļ�ϵͳ

	uint16_t n_inode;      // inode����.
	uint16_t imapstart;   // inode��bitmap�Ŀ��
	uint16_t inodestart;   // ��һ��inode�Ŀ��

	uint16_t n_data_block;      // ���ݿ����
	uint16_t bmapstart;    // ���ݿ��bitmap�Ŀ��
	uint16_t datastart;    // ��һ�����ݿ�Ŀ��

	std::array<User, N_USER>users;//�洢�û���Ϣ

};

// �������
class VirtualDisk
{
	FILE* handle{ nullptr };

	static constexpr uint32_t BUFFER_SIZE = 30;
	struct Buffer {  // ���̿黺��
		bool valid{ false }; // �Ƿ��Ӳ�̶��루data�������ǲ��Ǵ�Ӳ�̶��ģ�
		uint32_t blockno{ 0 };  // �������
		uint32_t refs{ 0 };   // ���ü���
		char data[BLOCK_SIZE]{ 0 };  // ��

		Buffer* prev{ nullptr };  // LRU ˫������
		Buffer* next{ nullptr };
	};
	std::array<Buffer, BUFFER_SIZE> buffer;
	Buffer head;  // ��һ������ͷ�ڵ㣬�������

	// ��blockno�ſ��Ƿ��Ѿ���buffer��
	// ���û��buffer�����Ҹ���buffer
	Buffer* FindBlock(uint32_t blockno);
public:

	// �����ء�����
	bool Mount(const char* disk_path);
	
	// �����ĸ�ʽ��
	void Format();

	// ���������Ŷ�
	Buffer* ReadBlock(uint32_t blockno); 
	
	// �ѻ���bufд��
	void WriteBlock(const Buffer* buf);

	// �����п�����ü�����һ
	void ReleaseBlock(Buffer* buf);

	// ������ ���ݿ� �ķ�����ͷ�
	//        �õ��Ǿ��Ե�������
	// ����һ�����п飬 ����ֵ��������   
	uint32_t AllocDataBlock(); 

	// ���ݿ���ͷŴ��̿�
	void FreeDataBlock(uint32_t blockno);

	SuperBlock GetSuperBlock();
	void WriteSuperBlock(const SuperBlock& super);

public:
	VirtualDisk();
	VirtualDisk(const VirtualDisk& other) = delete;
	VirtualDisk(VirtualDisk&& other) = delete;
	VirtualDisk& operator=(const VirtualDisk& other) = delete;
	VirtualDisk& operator=(VirtualDisk&& other) = delete;

	~VirtualDisk();
};

// ϵͳ����ʱ����һ�β� SetPath
// ֮��ʹ��ʱ��ͨ���ü��� ���ô������VirtualDisk����
VirtualDisk& GetDisk();

#endif // VIRTUAL_DISK_H