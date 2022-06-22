#ifndef UTIL_H
#define UTIL_H
#pragma once


#include <vector>
#include <string>
#include <array>

std::vector<std::string> Split(const std::string& str, const std::string& delimiters = " ");

// 传入路径应以 '/' 开始， 结尾不要有 '/'
// 例  /a/b/c  -> vector{/, a, b, c}
//     /a/b/../c  -> vector{/, a, c}
//     /a///b   ->  vector{/, a, b}
//     /./.   ->    vector{ / }
std::vector<std::string> SplitPath(const std::string& path);

// 把SplitPath的结果拼成string
std::string NormalizedPath(const std::string& path);

// 把path分成两份 一个是父亲路径 一个是自己的名字
//  例    /    ->   {/ , /} 
//       /a   ->   {/ , a}
//       /a/b ->   {/a, b}
// 调用者保证路径合法
std::pair<std::string, std::string> SplitPath2(const std::string& path);


template<unsigned N>
std::string ArrayToString(const std::array<char, N>& arr) {
	std::string ret;
	for (int i = 0; i < N && arr[i]; ++i) ret += arr[i];
	return ret;
}


// inode编号转为物理块号
uint32_t I2B(uint32_t inum);

#endif // 0
