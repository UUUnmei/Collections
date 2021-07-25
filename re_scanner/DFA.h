#pragma once
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <cassert>
#include "Token.h"

namespace DFA {
	



	// 类型的定义还需改进！！！
	

	//struct DFAState {
	//	int _id;
	//	bool _end;
	//};
	//size_t hasher(const DFAState& x);
	//bool eq(const DFAState& x, const DFAState& y);

	// 配合unordered_map<pair<int, char>, int> （对于关键字，需要提供相应的哈希方法）
	size_t hasher(const std::pair<int, char>& x);
	bool eq(const std::pair<int, char>& x, const std::pair<int, char>& y);

	class DFA {
	public:
		//static std::vector<char> _basicCharSet;
		
		//std::unordered_map<int, DFAState, decltype(hasher)*, decltype(eq)*> _States;
		//std::unordered_map<std::pair<DFAState, char>, DFAState, decltype(hasher)*, decltype(eq)*> _T;
		std::unordered_set<int> _states;
		std::unordered_map<int, bool> _end;
		std::unordered_map<int, Token::Type> _type;
		using transMatrix = std::unordered_map<std::pair<int, char>, int, decltype(hasher)*, decltype(eq)*>;
		transMatrix _T;
		DFA() {
			_states.clear();
			_end.clear();
			_type.clear();
			_T = transMatrix(100, hasher, eq);
		}
		void addState(int x, bool end);


		// todo
		// minimize
		// createCompressedCharSet




	};
}