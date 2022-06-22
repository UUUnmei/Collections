#include "util.h"
#include "common.h"

std::vector<std::string> Split(const std::string& str, const std::string& delimiters)
{
	std::vector<std::string> ret;
	std::string::size_type last = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, last);
	while (std::string::npos != pos || std::string::npos != last) {
		ret.emplace_back(str.substr(last, pos - last));
		last = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, last);
	}
	return ret;
}

// ����·��Ӧ�� '/' ��ʼ�� ��β��Ҫ�� '/'
// ��  /a/b/c  -> vector{/, a, b, c}
//     /a/b/../c  -> vector{/, a, c}
//     /a///b   ->  vector{/, a, b}
//     /./.   ->    vector{ / }
std::vector<std::string> SplitPath(const std::string& path) {
	std::vector<std::string> ret{ "/" };

	int n = path.length();
	for (int i = 1, j; i < n; i = j + 1) {
		for (j = i; j < n && path[j] != '/'; ++j)
			;

		std::string cur(path.substr(i, j - i));
		if (cur.empty()  // j == i,  ·������ '//'
			|| cur == ".") {
			continue;
		}
		else if (cur == "..") {
			if (ret.size() > 1)  // �����ǰ�ڸ�Ŀ¼����ô�����ϼ�Ŀ¼���Ǹ�Ŀ¼
				ret.pop_back();
			continue;
		}

		ret.push_back(cur);
	}

	return ret;
}

std::string NormalizedPath(const std::string& path)
{
	auto vs = SplitPath(path);
	if (vs.size() == 1) return "/";
	std::string ret;
	for (int i = 1; i < vs.size(); ++i) ret += "/" + vs[i];
	return ret;
}


// ��path�ֳ����� һ���Ǹ���·�� һ�����Լ�������
//  ��    /    ->   {/ , /} 
//       /a   ->   {/ , a}
//       /a/b ->   {/a, b}
// �����߱�֤·���Ϸ�
std::pair<std::string, std::string> SplitPath2(const std::string& path) {
	if (path == "/") return { "/", "/" };
	auto p = path.find_last_of('/');
	return { path.substr(0, std::max(p, 1ull)), path.substr(p + 1) };
}

uint32_t I2B(uint32_t inum) {
	return inum / IPB + INODE_START_BLOCK;
}
