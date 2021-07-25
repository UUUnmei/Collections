#pragma once
#include <iostream>
#include <list>
#include "Token.h"

namespace NFA {
	class NFAEdge;

	class NFAState {
	public:
		std::list<NFAEdge*> _outEdges; // �ɸ�״̬�����ı�
		std::list<NFAEdge*> _inEdges;  // ָ���״̬�ı�

		Token::Type _type;
		bool _end;  // �Ƿ����״̬
		bool _begin; // �Ƿ�Ϊ��ʼ״̬
		bool _valid; // �Ƿ���Ч�������з����ķǿձ�����Ч�� Ҳ��ɾ���ձߺ�Ҫ������״̬
		//bool _ein;   // �Ƿ��п����
		//bool _eout;  // �Ƿ��пճ���

		int _index; // ״̬���

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

		// ��һ��������NFAEdge
		void addEdge(NFAEdge* edge);

		// �ӵ�ǰ״̬��target״̬��ӱ�
		void addEdgeTo(const char& symbol, NFAState* target);
	};


	class NFAEdge {
	public:
		static const char Epsilon = -1; // ��

		char _symbol; // ���ϵ��ַ�  
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

		std::list<NFAState*> _states;  // ��NFA��״̬����
		//std::list<NFAEdge*> _edges;    // �߼��� ������������ڴ棬
		

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
		// �����ʱ�����������ж��㣬�������бߣ�����ʱ�����޾�������
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

		NFAState* _endState;  // ����״̬
		NFAState* _begState;  // ��ʼ��״̬

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
		// ȥ�ձߣ�����Ч״̬��
		void makeNoEmptyEdge(void);

		

	};

}