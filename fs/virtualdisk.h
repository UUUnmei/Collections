#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#pragma once

#include "common.h"
#include <string>
#include <cstdio>


// 超级块
struct SuperBlock {
	uint32_t magic;        // 自定义的magic number 就当是判断是不是咱们这个类型的文件系统

	uint16_t n_inode;      // inode个数.
	uint16_t imapstart;   // inode的bitmap的块号
	uint16_t inodestart;   // 第一个inode的块号

	uint16_t n_data_block;      // 数据块个数
	uint16_t bmapstart;    // 数据块的bitmap的块号
	uint16_t datastart;    // 第一个数据块的块号

	std::array<User, N_USER>users;//存储用户信息

};

// 虚拟磁盘
class VirtualDisk
{
	FILE* handle{ nullptr };

	static constexpr uint32_t BUFFER_SIZE = 30;
	struct Buffer {  // 磁盘块缓存
		bool valid{ false }; // 是否从硬盘读入（data中数据是不是从硬盘读的）
		uint32_t blockno{ 0 };  // 物理块编号
		uint32_t refs{ 0 };   // 引用计数
		char data[BLOCK_SIZE]{ 0 };  // 块

		Buffer* prev{ nullptr };  // LRU 双向链表
		Buffer* next{ nullptr };
	};
	std::array<Buffer, BUFFER_SIZE> buffer;
	Buffer head;  // 多一个链表头节点，方便操作

	// 找blockno号块是否已经在buffer里
	// 如果没在buffer里则找个空buffer
	Buffer* FindBlock(uint32_t blockno);
public:

	// “挂载”磁盘
	bool Mount(const char* disk_path);
	
	// 基本的格式化
	void Format();

	// 根据物理块号读
	Buffer* ReadBlock(uint32_t blockno); 
	
	// 把缓存buf写回
	void WriteBlock(const Buffer* buf);

	// 缓冲中块的引用计数减一
	void ReleaseBlock(Buffer* buf);

	// 这里是 数据块 的分配和释放
	//        用的是绝对的物理块号
	// 分配一个空闲块， 返回值是物理块号   
	uint32_t AllocDataBlock(); 

	// 根据块号释放磁盘块
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

// 系统启动时调用一次并 SetPath
// 之后使用时普通调用即可 不用创建别的VirtualDisk对象
VirtualDisk& GetDisk();

#endif // VIRTUAL_DISK_H