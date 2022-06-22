#pragma once

#include "virtualdisk.h"

// 主要负责 inode
class Controller {
    static constexpr uint16_t TABLE_SIZE = 30;
    std::array<Inode, TABLE_SIZE> itable;  // 内存中的inode表，TODO 大小可能要改

public:

    // 在inode表中查（或为新分配的inode找表中空位）
    //   ！！不管如何 查的应该是已分配的inode
    Inode* FindInode(uint32_t inum);

    // 把inode本身读过来，可使valid有效
    //   如果inode未被分配则报错（这里通过type判断）
    void ReadInodeFromDisk(Inode* inode);

    // 分配一个inode （并添加到inode表）
    //   返回值的valid应为0
    Inode* AllocInodeBlock(FileType type);

    // 释放inode, ref--
    //   如果ref等于0，则要从内存inode表中移除
    //   如果nlink也为0， 则真正释放inode
    void FreeInodeBlock(Inode* inode);

    // 更新对应的dinode，保证写回
    void ForceUpdateInode(const Inode* inode);

    // 清空inode，但没释放inode本身
    void ClearInode(Inode* inode);

public:
    // 读inode对应的数据块
    //   dst “目的地址”  offset相对于开头的偏移 cnt要读的字节数
    //   调用者应保证dst的内存空间已存在/申请
    //   返回实际读取数量
    int ReadInode(Inode* inode, char* dst, uint32_t offset, uint32_t cnt);

    // 写inode对应的数据块
    //   src “源数据地址”  offset相对于开头的偏移 cnt要写的字节数
    //   调用者保证src有效
    //   返回实际写入数量
    int WriteInode(Inode* inode, const char* src, uint32_t offset, uint32_t cnt);

public:
    // 和目录相关
    
    // 删除单个文件 nlink---
    //   在调用之前应该检查一下权限！
    bool DeleteFile(const std::string& path);

    // 递归删除目录 nlink--
    //   在调用之前应该检查一下权限！
    bool DeleteDir(const std::string& path);

    // 找到path对应的inode， 不存在的话返回空指针
    //    string主要是想避免char*的一点麻烦
    //    path要绝对路径
    //    对应的inode的ref+1
    Inode* PathToInode(const std::string& path);

    // 找path对应的inode的上一级inode
    //   其他方面同PathToInode
    //   ！注意，这个其实只是忽略了path的最后一段，去调用PathToInode，没有检查完整的path是否存在！
    Inode* PathToInodeParent(const std::string& path);

    // 首先有一个目录对应的inode，在其中找name文件（夹）
    //    找到的话返回那个文件的inode和它在现在目录inode中的偏移 --给删除确定位置
    //    否则空指针
    std::pair<Inode*, uint32_t> LookUpInDir(Inode* inode, const std::string& name);

    // 首先有一个目录对应的inode，在其中添加一个文件name inum是这个文件的inode号（要保证是有效的）
    //   返回值是否成功
    //   不影响ref
    bool AddItemInDir(Inode* inode, const std::string& name, uint32_t inum);
};


// 使用原则
// 1、Alloc、PathToInode之后要Free对应的Inode一次，尤其注意函数中提前return时
// 2、修改Inode的type、n_link、size、addr时跟上ForceUpdate
// 3、PathToInode、LookUpInDir之类的调用之后，如果要访问type、n_link、size、addr，记得ReadFromDisk