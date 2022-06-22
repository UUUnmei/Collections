#pragma once

#include "virtualdisk.h"

// ��Ҫ���� inode
class Controller {
    static constexpr uint16_t TABLE_SIZE = 30;
    std::array<Inode, TABLE_SIZE> itable;  // �ڴ��е�inode��TODO ��С����Ҫ��

public:

    // ��inode���в飨��Ϊ�·����inode�ұ��п�λ��
    //   ����������� ���Ӧ�����ѷ����inode
    Inode* FindInode(uint32_t inum);

    // ��inode�������������ʹvalid��Ч
    //   ���inodeδ�������򱨴�����ͨ��type�жϣ�
    void ReadInodeFromDisk(Inode* inode);

    // ����һ��inode ������ӵ�inode��
    //   ����ֵ��validӦΪ0
    Inode* AllocInodeBlock(FileType type);

    // �ͷ�inode, ref--
    //   ���ref����0����Ҫ���ڴ�inode�����Ƴ�
    //   ���nlinkҲΪ0�� �������ͷ�inode
    void FreeInodeBlock(Inode* inode);

    // ���¶�Ӧ��dinode����֤д��
    void ForceUpdateInode(const Inode* inode);

    // ���inode����û�ͷ�inode����
    void ClearInode(Inode* inode);

public:
    // ��inode��Ӧ�����ݿ�
    //   dst ��Ŀ�ĵ�ַ��  offset����ڿ�ͷ��ƫ�� cntҪ�����ֽ���
    //   ������Ӧ��֤dst���ڴ�ռ��Ѵ���/����
    //   ����ʵ�ʶ�ȡ����
    int ReadInode(Inode* inode, char* dst, uint32_t offset, uint32_t cnt);

    // дinode��Ӧ�����ݿ�
    //   src ��Դ���ݵ�ַ��  offset����ڿ�ͷ��ƫ�� cntҪд���ֽ���
    //   �����߱�֤src��Ч
    //   ����ʵ��д������
    int WriteInode(Inode* inode, const char* src, uint32_t offset, uint32_t cnt);

public:
    // ��Ŀ¼���
    
    // ɾ�������ļ� nlink---
    //   �ڵ���֮ǰӦ�ü��һ��Ȩ�ޣ�
    bool DeleteFile(const std::string& path);

    // �ݹ�ɾ��Ŀ¼ nlink--
    //   �ڵ���֮ǰӦ�ü��һ��Ȩ�ޣ�
    bool DeleteDir(const std::string& path);

    // �ҵ�path��Ӧ��inode�� �����ڵĻ����ؿ�ָ��
    //    string��Ҫ�������char*��һ���鷳
    //    pathҪ����·��
    //    ��Ӧ��inode��ref+1
    Inode* PathToInode(const std::string& path);

    // ��path��Ӧ��inode����һ��inode
    //   ��������ͬPathToInode
    //   ��ע�⣬�����ʵֻ�Ǻ�����path�����һ�Σ�ȥ����PathToInode��û�м��������path�Ƿ���ڣ�
    Inode* PathToInodeParent(const std::string& path);

    // ������һ��Ŀ¼��Ӧ��inode����������name�ļ����У�
    //    �ҵ��Ļ������Ǹ��ļ���inode����������Ŀ¼inode�е�ƫ�� --��ɾ��ȷ��λ��
    //    �����ָ��
    std::pair<Inode*, uint32_t> LookUpInDir(Inode* inode, const std::string& name);

    // ������һ��Ŀ¼��Ӧ��inode�����������һ���ļ�name inum������ļ���inode�ţ�Ҫ��֤����Ч�ģ�
    //   ����ֵ�Ƿ�ɹ�
    //   ��Ӱ��ref
    bool AddItemInDir(Inode* inode, const std::string& name, uint32_t inum);
};


// ʹ��ԭ��
// 1��Alloc��PathToInode֮��ҪFree��Ӧ��Inodeһ�Σ�����ע�⺯������ǰreturnʱ
// 2���޸�Inode��type��n_link��size��addrʱ����ForceUpdate
// 3��PathToInode��LookUpInDir֮��ĵ���֮�����Ҫ����type��n_link��size��addr���ǵ�ReadFromDisk