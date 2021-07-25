#pragma once
#include <iostream>
#include <list>
#include "Token.h"

namespace NFA {
	class NFAEdge;

	class NFAState {
	public:
		std::list<NFAEdge*> _outEdges; // 由该状态引出的边
		std::list<NFAEdge*> _inEdges;  // 指向该状态的边

		Token::Type _type;
		bool _end;  // 是否结束状态
		bool _begin; // 是否为开始状态
		bool _valid; // 是否有效，（若有发出的非空边则有效） 也是删除空边后要保留的状态
		//bool _ein;   // 是否有空入边
		//bool _eout;  // 是否有空出边

		int _index; // 状态序号

		~NFAState() = default;
		//~NFAState() {
		//	for (auto x : _outEdges)
		//		delete x;
		//	/*for (auto x : _inEdges)
		//		delete x;*/
		//	std::cout << "destruct NFAState" << std::endl;
		//}
		NFAState(): _index(0), _end(false), _begin(false), _valid(false), _type(Token::Type::None) {
			_outEdges.clear();
			_inEdges.clear();
		}
		//NFAState(const NFAState& o);

		// 加一条完整的NFAEdge
		void addEdge(NFAEdge* edge);

		// 从当前状态向target状态添加边
		void addEdgeTo(const char& symbol, NFAState* target);
	};


	class NFAEdge {
	public:
		static const char Epsilon = -1; // ε

		char _symbol; // 边上的字符  
		NFAState* _sourceState;
		NFAState* _targetState;

		NFAEdge(): _sourceState(nullptr), _targetState(nullptr), _symbol(0) { }
		/*NFAEdge(const NFAEdge& o): _symbol(o._symbol){ 
			_sourceState = new NFAState(*o._sourceState);
			_targetState = new NFAState(*o._targetState);
		}*/
		
		~NFAEdge() = default;
		/*~NFAEdge() {
			std::cout << "destruct NFAEdge" << std::endl;
		}*/


		NFAEdge(const char& symbol, NFAState* source, NFAState* target)
			:_symbol(symbol),_sourceState(source), _targetState(target) {}
	};
	

	class NFA {
	public:

		std::list<NFAState*> _states;  // 该NFA中状态集合
		//std::list<NFAEdge*> _edges;    // 边集， 这俩方便管理内存，
		

		NFA(): _begState(nullptr), _endState(nullptr) {
			_states.clear();
			//_edges.clear();
		}
		explicit NFA(const char& exp, Token::Type _t) {
			auto s1 = new NFAState;
			auto s2 = new NFAState;
			auto e = new NFAEdge(exp, s1, s2);
			s1->addEdge(e);
			s1->_begin = true;
			s2->_end = true;
			addState(s1);
			addState(s2);

			s2->_type = _t;
			
			_begState = s1;
			_endState = s2;
		}
		// 深拷贝暂时放弃，边里有顶点，顶点里有边，复制时无穷无尽。。。
		/*NFA(const NFA& o):_states(o._states) {
			_begState = new NFAState(*o._begState);
			_endState = new NFAState(*o._endState);
			for (const auto& x : o._states) {
				_states.emplace_back(new NFAState(*x));
			}
		}*/

		~NFA() = default;
		/*~NFA() {
			for (auto x : _states)
				delete x;
			std::cout << "destruct NFA" << std::endl;
		}*/

		NFAState* _endState;  // 最后的状态
		NFAState* _begState;  // 开始的状态

		void addState(NFAState* state) {
			state->_index = (int)_states.size() + 1;
			_states.push_back(state);
		}

		void addStates(const std::list<NFAState*>& states) {
			for (const auto& x : states)
				addState(x);
		}

		void freeMem(void);
		void showAllEdges(void);
		// 去空边（及无效状态）
		void makeNoEmptyEdge(void);

		

	};

}