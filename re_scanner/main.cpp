// complier.cpp: 定义应用程序的入口点。
//



	//cout << "*******************" << endl;
//	//cout << "测试" << endl;
//	//cout << re._expression._states.size() << endl;
//	//set<NFA::NFAState*> vis;
//	//dfs(re._expression._begState, vis, re._expression._endState);
//	//vis.clear();
//	//cout << "*******************" << endl;
//void dfs(NFA::NFAState* cur, set<NFA::NFAState*> &vis, NFA::NFAState* END) {
//	cout << "STATE:" << cur->_index << endl;
//	printf("valid:%d beg:%d end:%d\n", cur->_valid, cur->_begin, cur->_end);
//	if (cur == END)return;
//	if (vis.find(cur) == vis.end())
//		vis.insert(cur);
//	else
//		return;
//	
//	for (const auto& v : cur->_outEdges) {
//		cout << (int)v->_symbol << endl;
//		dfs(v->_targetState, vis, END);
//	}
//}

#include <ctime>
clock_t start, finish;
double duration;
#define BEGIN_CLK start = clock();
#define END_CLK  finish = clock(); \
				 duration = ((double)finish - start) / CLOCKS_PER_SEC; \
				 printf("%f seconds\n", duration); \

#include "Scanner.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {


	//RE re = RE('a').Union(RE('b'));
	//RE re = (RE('a') | RE('b')).StarClosure();

	//RE re = (RE('a').PosClosure() | RE('b')).Concat(RE('c'));
	//RE re = RE("while");
	//RE re = RE('0', '2');
	//RE re = RE('a').Optional();
	//RE re = RE('a').PosClosure();

	//RE re = RE("ab") | RE("cd");
	//re._expression.makeNoEmptyEdge();
	
	// 无符号整数
	//RE::_type = Token::Type::Int;
	
	
	//for (const auto& it : dfa._T) {
	//	cout << it.first.first << ' ' << it.first.second << ' ' << it.second << endl;
	//	cout << Token::TypeName[dfa._type[it.first.first]] << endl;

	//}
	//cout << "*************************" << endl;



	//////////////////////////// 以上初始化///////////////////////////////

	////命令行。
	//while (1) {
	//	bool ok = true;
	//	int curState = 1;
	//	cout << "> ";
	//	string input;
	//	cin >> input;
	//	for (const auto& c : input) {
	//		if (dfa._T.find({ curState, c }) == dfa._T.end()) {
	//			ok = false;
	//			break;
	//		}
	//		curState = dfa._T[{curState, c}];
	//	}
	//	//cout << (ok && dfa._end[curState] ? "Accept" : "Error") << endl;
	//	if (ok && dfa._end[curState]) {
	//		cout << "Accept " << Token::TypeName[dfa._type[curState]] << endl;
	//	}else{
	//		cout << "Error" << endl;
	//	}
	//}


	Scanner::Scanner sc;
	std::ofstream out("D:\\Visual Studio\\complier\\out.txt");
	if (!out)
		return 0;


	BEGIN_CLK
	sc.init();
	END_CLK

	std::vector<Token::Token> tokenlist;

	BEGIN_CLK
	tokenlist = sc.readFile("D:\\Visual Studio\\complier\\in.txt");
	END_CLK

	for (const auto& x : tokenlist) {
		out << x._exp << ' ' << Token::TypeName[x._type] << ' ' << x._row << ' ' << x._col << std::endl;
	}
	out.close();
	return 0;
}
