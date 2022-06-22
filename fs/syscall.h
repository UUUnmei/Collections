#ifndef SYSCALL_H
#define SYSCALL_H

#pragma once

#include <string>
#include "virtualdisk.h"

// 命令行
void ProcessCommand();

// 返回值无特殊说明均为：
//   返回0正常，-1失败 （不过意识到一个问题是上层不太清楚具体是什么原因）

// 挂载磁盘
int Mount(const std::string& path);

// 创建文件
int Create(const std::string& path);

// 创建文件夹
int MkDir(const std::string& path);

// 格式化磁盘
int Format();

// 读文件
//    返回实际读取个数
//    需要先打开文件获取文件描述符fd
int Read(uint32_t fd, char* dst, uint32_t offset, uint32_t cnt);

// 写文件
//    返回实际写入个数
//    需要先打开文件获取文件描述符fd
//int Write(uint32_t fd, const char* src, uint32_t offset, uint32_t cnt);
void WriteFile(const std::string& path, int mode, const std::string& content);

// 删除文件
int DeleteFile(const std::string& path);

// 删除文件夹
int DeleteDir(const std::string& path);

// 打开文件
//   成功返回文件描述符， 失败-1
int Open(const std::string& path);

// 关闭文件
int Close(uint32_t fd);

// 修改文件（夹）权限
int Chmod(const std::string& path, uint32_t new_permission);

/*
 剪切/移动文件（夹）
   包含了重命名的作用
   两个path都得是完整的路径 
 目录->目录 --- 移动+重命名
 文件->文件 --- 移动+重命名
*/ 
int Move(const std::string& old_path, const std::string& new_path);

/*
 复制文件
   old_path是原文件，new_path是目标目录，都要存在
 */
int Copy(const std::string& old_path, const std::string& new_path);

// 软链接，仅考虑文件
//  target_path指被链接的文件路径，link_path是指产生的“链接”的位置
//   硬链接同
int SoftLink(const std::string& target_path, const std::string& link_path);

// 硬链接
int HardLink(const std::string& target_path, const std::string& link_path);

// 获取文件信息
int Stat(const std::string& path);



#endif