#ifndef UTIL_H
#define UTIL_H
#pragma once


#include <vector>
#include <string>
#include <array>

std::vector<std::string> Split(const std::string& str, const std::string& delimiters = " ");

// ����·��Ӧ�� '/' ��ʼ�� ��β��Ҫ�� '/'
// ��  /a/b/c  -> vector{/, a, b, c}
//     /a/b/../c  -> vector{/, a, c}
//     /a///b   ->  vector{/, a, b}
//     /./.   ->    vector{ / }
std::vector<std::string> SplitPath(const std::string& path);

// ��SplitPath�Ľ��ƴ��string
std::string NormalizedPath(const std::string& path);

// ��path�ֳ����� һ���Ǹ���·�� һ�����Լ�������
//  ��    /    ->   {/ , /} 
//       /a   ->   {/ , a}
//       /a/b ->   {/a, b}
// �����߱�֤·���Ϸ�
std::pair<std::string, std::string> SplitPath2(const std::string& path);


template<unsigned N>
std::string ArrayToString(const std::array<char, N>& arr) {
	std::string ret;
	for (int i = 0; i < N && arr[i]; ++i) ret += arr[i];
	return ret;
}


// inode���תΪ������
uint32_t I2B(uint32_t inum);

#endif // 0
