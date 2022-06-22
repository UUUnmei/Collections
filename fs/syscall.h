#ifndef SYSCALL_H
#define SYSCALL_H

#pragma once

#include <string>
#include "virtualdisk.h"

// ������
void ProcessCommand();

// ����ֵ������˵����Ϊ��
//   ����0������-1ʧ�� ��������ʶ��һ���������ϲ㲻̫���������ʲôԭ��

// ���ش���
int Mount(const std::string& path);

// �����ļ�
int Create(const std::string& path);

// �����ļ���
int MkDir(const std::string& path);

// ��ʽ������
int Format();

// ���ļ�
//    ����ʵ�ʶ�ȡ����
//    ��Ҫ�ȴ��ļ���ȡ�ļ�������fd
int Read(uint32_t fd, char* dst, uint32_t offset, uint32_t cnt);

// д�ļ�
//    ����ʵ��д�����
//    ��Ҫ�ȴ��ļ���ȡ�ļ�������fd
//int Write(uint32_t fd, const char* src, uint32_t offset, uint32_t cnt);
void WriteFile(const std::string& path, int mode, const std::string& content);

// ɾ���ļ�
int DeleteFile(const std::string& path);

// ɾ���ļ���
int DeleteDir(const std::string& path);

// ���ļ�
//   �ɹ������ļ��������� ʧ��-1
int Open(const std::string& path);

// �ر��ļ�
int Close(uint32_t fd);

// �޸��ļ����У�Ȩ��
int Chmod(const std::string& path, uint32_t new_permission);

/*
 ����/�ƶ��ļ����У�
   ������������������
   ����path������������·�� 
 Ŀ¼->Ŀ¼ --- �ƶ�+������
 �ļ�->�ļ� --- �ƶ�+������
*/ 
int Move(const std::string& old_path, const std::string& new_path);

/*
 �����ļ�
   old_path��ԭ�ļ���new_path��Ŀ��Ŀ¼����Ҫ����
 */
int Copy(const std::string& old_path, const std::string& new_path);

// �����ӣ��������ļ�
//  target_pathָ�����ӵ��ļ�·����link_path��ָ�����ġ����ӡ���λ��
//   Ӳ����ͬ
int SoftLink(const std::string& target_path, const std::string& link_path);

// Ӳ����
int HardLink(const std::string& target_path, const std::string& link_path);

// ��ȡ�ļ���Ϣ
int Stat(const std::string& path);



#endif