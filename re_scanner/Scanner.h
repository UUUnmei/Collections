// complier.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once


#include "RegularExpression.h"
#include "Token.h"


namespace Scanner {
	
	class Scanner {
	public:
		DFA::DFA _trans;
		Scanner() = default;
		~Scanner() = default;
 		void init(void);
		std::vector<Token::Token> readFile(const char* filename);
	};
}