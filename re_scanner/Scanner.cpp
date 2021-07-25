#include "Scanner.h"
#include <cstdio>
#include <fstream>
#include <string>
#include <stack>


namespace Scanner {
	using RE = RegularExpression::RegularExpression;

	void Scanner::init(void) {
		RE::setType(Token::Type::Int);
		RE UINT = RE('0') | (RE('1', '9').Concat(RE('0', '9').StarClosure()));


		// 无符号浮点数
		//RE::_type = Token::Type::Double;
		RE::setType(Token::Type::Double);
		RE UDOUBLE = (RE('0') | (RE('1', '9').Concat(RE('0', '9').StarClosure())))
			.Concat((RE('.').Concat(RE('0', '9').StarClosure())).Optional());

		RE::setType(Token::Type::StringConstant);
		RE STRING = RE('\"')
			.Concat((RE('0', '9') | RE('a', 'z') | RE('A', 'Z') | RE('_')).StarClosure())
			.Concat(RE('\"'));

		RE::setType(Token::Type::Keyword); // 顺序靠前的优先级高（原因见toDFA中的_type更新后就不再更新），这样关键字不会被认为是标识符
		RE KEY = RE("while") | RE("if") | RE("else") | RE("for") | RE("break")
			| RE("case") | RE("switch") | RE("continue") | RE("default")
			| RE("int") | RE("double") | RE("main") | RE("return") | RE("void");


		RE::setType(Token::Type::Identifier);
		RE IDENTIFIER = (RE('a', 'z') | RE('A', 'Z') | RE('_'))
			.Concat((RE('0', '9') | RE('a', 'z') | RE('A', 'Z') | RE('_')).StarClosure());

		RE::setType(Token::Type::Operator);
		RE OP = RE("==") | RE("!=") | RE("<=") | RE(">=") | RE("->") | RE('.')
			| RE("+=") | RE("-=") | RE("*=") | RE("/=") | RE("%=") | RE("^=")
			| RE("||") | RE("&&") | RE("++") | RE("--") | RE('!') | RE('~') 
			| RE('|') | RE('&') | RE('+') | RE('-') | RE('*') | RE('/') | RE('%') | RE('^')
			| RE('>') | RE('=') | RE('<');

		RE::setType(Token::Type::Delimiter);
		RE DE = RE('(') | RE(')') | RE('{') | RE('}') | RE('[') | RE(']')
			| RE(';') | RE(','); //空格不纳入考虑


		//// 无符号浮点数，（必须带e的 如1.2e3）
		//RE UeDOUBLE = (RE('0') | (RE('1', '9').Concat(RE('0', '9').StarClosure())))
		//	.Concat((RE('.').Concat(RE('0', '9').StarClosure())).Optional())
		//	.Concat(RE('e'))
		//	.Concat((RE('+') | RE('-')).Optional())
		//	.Concat(RE('0') | (RE('1', '9').Concat(RE('0', '9').StarClosure())));


		RE::setType(Token::Type::None);
		RE re = UINT | UDOUBLE | STRING | IDENTIFIER | KEY | OP | DE;
		std::cout << "RE constructed" << std::endl;
		std::cout << "NFA constructed" << std::endl;
		std::cout << re._expression._states.size() << std::endl;
	
		this->_trans = re.toDFA();
		re._expression.freeMem();
		std::cout << "DFA constructed" << std::endl;
		std::cout << _trans._states.size() << std::endl;
		std::cout << "Scanner constructed" << std::endl;
	}

	std::vector<Token::Token> Scanner::readFile(const char* filename) {
		std::ifstream in(filename);
		if (!in) {
			std:: cout << "read error" << std::endl;
			return {};
		}

		std::vector<Token::Token> tokenlist;

		std::string buf;
		int row = 0;

		// 全都存在一个string中
		//std::string source;
		//while (std::getline(in, buf))
		//	source += buf + "\n";
		//in.close();

		//int col = 0;
		//std::stack<int> rec;
		//rec.push(0);
		//auto it = source.begin();
		//while (true) {
		//	while (!rec.empty()) rec.pop();

		//	int curState = 1;

		//	std::string lexme = "";
		//	char c;
		//	while (true) {

		//		if (it == source.end())
		//			break;
		//		c = *it;
		//		++it, ++col;
		//		lexme += c;

		//		if (_trans._end[curState] == true) {
		//			// 清空栈
		//			while (!rec.empty()) rec.pop();
		//		}
		//		rec.push(curState);
		//		if (_trans._T.find({ curState, c }) == _trans._T.end()) {
		//			curState = 1;  // 任意正整数
		//			break;
		//		}
		//		curState = _trans._T[{curState, c}];
		//	}
		//	while (!rec.empty() && curState && !_trans._end[curState]) {
		//		curState = rec.top();
		//		rec.pop();

		//		lexme.pop_back();
		//		--it, --col;
		//	}
		//	if (curState && _trans._end[curState]) {
		//		//out << lexme << ' ' << Token::TypeName[dfa._type[curState]] << " row: " << row << " col: " << col << endl;

		//		tokenlist.push_back({ _trans._type[curState] , lexme , row, col });
		//	}
		//	else if (c == ' ') {
		//		if (it != source.end()) ++it, ++col;
		//	}
		//	else if (c == '\n') {
		//		++row, ++it, col = 0;
		//	}
		//	else {
		//		std::cout << "Error at row: " << row << " col: " << col << std::endl;
		//		exit(EXIT_FAILURE);
		//	}
		//	//std::cout << row << '\n';
		//	if (it == source.end())break;
		//}


		// 一行一行的
		while (getline(in, buf)) {

			++row;
			if (buf.empty()) continue;

			int col = 0;
			std::stack<int> rec;
			rec.push(0);
			auto it = buf.begin();
			while (true) {
				while (!rec.empty()) rec.pop();

				int curState = 1;

				std::string lexme = "";
				char c;
				while (true) {

					if (it == buf.end())
						break;
					c = *it;
					++it, ++col;
					lexme += c;

					if (_trans._end[curState] == true) {
						// 清空栈
						while (!rec.empty()) rec.pop();
					}
					rec.push(curState);
					if (_trans._T.find({ curState, c }) == _trans._T.end()) {
						curState = 1;  // 任意正整数
						break;
					}
					curState = _trans._T[{curState, c}];
				}
				while (!rec.empty() && curState && !_trans._end[curState]) {
					curState = rec.top();
					rec.pop();

					lexme.pop_back();
					--it, --col;
				}
				if (curState && _trans._end[curState]) {
					//out << lexme << ' ' << Token::TypeName[dfa._type[curState]] << " row: " << row << " col: " << col << endl;

					tokenlist.push_back({ _trans._type[curState] , lexme , row, col });
				}
				else if (c == ' ') {
					if (it != buf.end()) ++it, ++col;
				}
				else {
					std::cout << "Error at row: " << row << " col: " << col << std::endl;
					exit(EXIT_FAILURE);
				}

				if (it == buf.end())break;
			}

		}
		in.close();
		return tokenlist;
	}


	
}