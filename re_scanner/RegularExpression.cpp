#include "RegularExpression.h"
#include <cassert>
#include <set>
#include <queue>
#include <string>
#include <map>

namespace RegularExpression {
	// ��
	RegularExpression RegularExpression::Union(RegularExpression other) {
		assert(this->_expression._begState != nullptr);
		if (other._expression._begState == nullptr)
			return *this;
	
		RegularExpression ret;
		NFA::NFAState* st = new NFA::NFAState;
		NFA::NFAState* ed = new NFA::NFAState;

		st->addEdgeTo(NFA::NFAEdge::Epsilon, this->_expression._begState);
		st->addEdgeTo(NFA::NFAEdge::Epsilon, other._expression._begState);

		this->_expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, ed);
		other._expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, ed);

		ret._expression.addState(st);
		ret._expression.addState(ed);
		ret._expression.addStates(this->_expression._states);
		ret._expression.addStates(other._expression._states);
		ret._expression._begState = st;
		ret._expression._endState = ed;

		ret._expression._endState->_type = RegularExpression::_type;

		ed->_end = true;
		st->_begin = true;
		return ret;
	}

	// ����
	RegularExpression RegularExpression::Concat(RegularExpression other) {
		assert(this->_expression._begState != nullptr);
		if (other._expression._begState == nullptr)
			return *this;

		RegularExpression ret;
		this->_expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, other._expression._begState);
		ret._expression.addStates(this->_expression._states);
		ret._expression.addStates(other._expression._states);

		ret._expression._begState = this->_expression._begState;
		ret._expression._endState = other._expression._endState;

		ret._expression._begState->_begin = true;
		ret._expression._endState->_end = true;

		ret._expression._endState->_type = RegularExpression::_type;
		return ret;
	}

	// �Ǳհ�
	RegularExpression RegularExpression::StarClosure(void) {
		assert(this->_expression._begState != nullptr);

		RegularExpression ret;

		this->_expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, this->_expression._begState);

		NFA::NFAState* st = new NFA::NFAState;
		NFA::NFAState* ed = new NFA::NFAState;
		
		st->addEdgeTo(NFA::NFAEdge::Epsilon, this->_expression._begState);
		this->_expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, ed);
		st->addEdgeTo(NFA::NFAEdge::Epsilon, ed);
	
		ret._expression.addStates(this->_expression._states);
		ret._expression.addState(st);
		ret._expression.addState(ed);

		ret._expression._begState = st;
		ret._expression._endState = ed;

		ret._expression._endState->_type = RegularExpression::_type;

		ed->_end = true;
		st->_begin = true;
		return ret;
	}

	// ��  ��д��
	RegularExpression RegularExpression::operator|(RegularExpression other) {
		assert(this->_expression._begState != nullptr);
		if (other._expression._begState == nullptr)
			return *this;
		return this->Union(other);
	}
	

	// ���հ� ������ֹ��ʼ��һ���ձߣ�
	RegularExpression RegularExpression::PosClosure(void) {
		assert(this->_expression._begState != nullptr);

		this->_expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, this->_expression._begState);
		this->_expression._endState->_end = true;
		this->_expression._begState->_begin = true;

		this->_expression._endState->_type = RegularExpression::_type;
		return *this;
	}

	// this | epsilon
	RegularExpression RegularExpression::Optional(void) {
		assert(this->_expression._begState != nullptr);

		this->_expression._begState->addEdgeTo(NFA::NFAEdge::Epsilon, this->_expression._endState);

		this->_expression._begState->_end = true;
		this->_expression._begState->_begin = true;
		this->_expression._endState->_end = true;

		this->_expression._endState->_type = RegularExpression::_type;
		return *this;
	}

	// �ַ��� ��Ӧ�ؼ���
	// ��ʵ���ɸ�char RE�������ɣ���������ʡ�����еĿձ�
	RegularExpression::RegularExpression(const std::string& s) {
		assert(s.length() != 0);

		auto it = s.begin();
		RegularExpression ret(*it);
		it++;

		while (it != s.end()) {
			NFA::NFAState* st = new NFA::NFAState;
			ret._expression._endState->addEdgeTo(*it, st);
			ret._expression._endState = st;
			ret._expression.addState(st);
			it++;
		}
		ret._expression._endState->_end = true;
		ret._expression._endState->_type = RegularExpression::_type;

		this->_expression = ret._expression;
	}


	// ���ַ� l �� r �Ĳ��� ԭ����ֻ���� 0~9 a~z A~Z ��Χ��
	// ûֱ��ȫ��Union ��ÿձ�̫��
	RegularExpression::RegularExpression(char l, char r) {
		assert(l <= r);

		if (l == r)
			*this = RegularExpression(l);

		// ��������
		RegularExpression ret = RegularExpression(l).Union(RegularExpression((char)(l + 1)));
		for (char c = l + 2; c <= r; ++c) {
			RegularExpression t(c);
			ret._expression._begState->addEdgeTo(NFA::NFAEdge::Epsilon, t._expression._begState);
			t._expression._endState->addEdgeTo(NFA::NFAEdge::Epsilon, ret._expression._endState);

			ret._expression.addStates(t._expression._states);
		}
		ret._expression._endState->_end = true;
		ret._expression._endState->_type = RegularExpression::_type;
		*this = ret;
	}

	//size_t hasher(std::set<NFA::NFAState*>& x) {
	//	std::string s;
	//	for (const auto& it : x) {
	//		s += std::to_string(it->_index) + "a";
	//	}
	//	return std::hash<std::string>()(s);
	//}

	//bool eq(std::set<NFA::NFAState*>& x, std::set<NFA::NFAState*>& y) {
	//	return hasher(x) == hasher(y);
	//}

	DFA::DFA RegularExpression::toDFA(void) {
		assert(this->_expression._begState != nullptr);

		this->_expression.makeNoEmptyEdge();
		// ��ʱ�Ѿ����޿ձ��޶���״̬��NFA

		int cnt = 1;
		DFA::DFA dfa;
		dfa.addState(cnt, this->_expression._begState->_end);
		//std::cout << this->_expression._begState->_index << " --> " << cnt << std::endl;

		//std::unordered_map<std::set<NFA::NFAState*>, int, decltype(hasher)*, decltype(eq)*> sofar;
		std::map<std::set<NFA::NFAState*>, int> sofar;
		sofar[{this->_expression._begState}] = cnt++;
		//std::set<std::set<NFA::NFAState*>> sofar; // һ�����ϣ�ÿ��Ԫ�ض�����Ӧһ��DFAState��һ��Ԫ��������NFAState��
		//sofar.insert({ this->_expression._begState });
		std::queue<std::set<NFA::NFAState*>> q;
		q.push({ this->_expression._begState });

		while (!q.empty()) {
			auto cur = q.front();
			q.pop();
			// ��NFA״̬���ϳ����ϵ��ַ��ļ���
			std::set<char> acceptedChar;
			for(const auto& state : cur)
				for (const auto& x : state->_outEdges)
					acceptedChar.insert(x->_symbol);
			// ����ÿ���ַ����ҽ���״̬�ļ���
			for (auto c : acceptedChar) {
				std::set<NFA::NFAState*> t;
				bool _end = false;
				Token::Type _type = Token::Type::None;
				for (const auto& state : cur) 
					for (const auto& x : state->_outEdges) 
						if (x->_symbol == c) {
							t.insert(x->_targetState);
							_end |= x->_targetState->_end;	// ������nfa�Ľ���̬���״̬Ҳ��dfa�Ľ���̬
							if (_type == Token::Type::None && x->_targetState->_end && x->_targetState->_type != Token::Type::None)
								_type = x->_targetState->_type;
						}		

				if (sofar.find(t) == sofar.end()) {
					
					//// nfa��dfa��״̬��Ӧ
					//for (const auto& x : t) {
					//	std::cout << x->_index << ' ';
					//}
					//std::cout << "--> " << cnt << std::endl;
					//if(_end)
					//	std::cout << "!!!!!!!!!" << cnt << std::endl;

					dfa.addState(cnt, _end);
					if(dfa._type.find(cnt) == dfa._type.end())
						dfa._type[cnt] = _type;
					sofar[t] = cnt++;
					q.push(t);

				}

				// ���״̬ת��
				//dfa._T[std::make_pair(dfa._States[sofar[cur]], c)] = dfa._States[sofar[t]];
				//dfa._T[{sofar[cur], c}] = sofar[t];
				//std::cout << sofar[cur] << ' ' << sofar[t] << std::endl;
				dfa._T.insert({ {sofar[cur], (char)c} , sofar[t] });
			}

		}

		return dfa;

	}

	Token::Type RegularExpression::_type; 
	// !!!!!!!!! �������к���Ҫ����
	//https://blog.csdn.net/jackystudio/article/details/11482067
	//https://blog.csdn.net/echonupt/article/details/8099967

	void RegularExpression::setType(Token::Type _t) {
		_type = _t;
	}

}