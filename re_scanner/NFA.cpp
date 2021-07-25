#include "NFA.h"
#include <set>
#include <cassert>
namespace NFA {
	void NFAState::addEdge(NFAEdge* edge) {
		edge->_sourceState->_end = false; // �����ʵ�����ԣ���PosClosure�����
		edge->_targetState->_begin = false;
		if (edge->_symbol != NFAEdge::Epsilon) {
			edge->_targetState->_valid = true;
		}
		this->_outEdges.push_back(edge);
		edge->_targetState->_inEdges.push_back(edge);

	}

	void NFAState::addEdgeTo(const char& symbol, NFAState* target) {
		this->addEdge(new NFAEdge(symbol, this, target));
	}

	void NFA::showAllEdges(void) {
		for (const auto& x : _states) {
			for (const auto& e : x->_outEdges) {
				std::cout << x->_index << ' '
					<< (e->_symbol == NFAEdge::Epsilon ? '#' : (char)e->_symbol) << ' '
					<< e->_targetState->_index << std::endl;
			}
		}
	}

	void NFA::freeMem(void) {
		// ����ûʵ���������������Ϊ�����ᱨ��
		// ���� RE re = RE('0');
		// ���ִ���꣬�ұߵ�����������ʵre�����Ҳ�Ѿ�û�ˡ�����

		for (auto x = _states.begin(); x != _states.end(); x++) {
			//״̬�����б߶�ɾ
			for (auto e = (*x)->_outEdges.begin(); e != (*x)->_outEdges.end(); e++) {
				delete *e;
			}
			(*x)->_outEdges.clear();
			(*x)->_inEdges.clear();

			delete *x;
		}
		_states.clear();

		std::cout << "NFA Destructed" << std::endl;
	}

	//NFAState::NFAState(const NFAState& o) : _index(o._index), _end(o._end) {
	//	/*_outEdges = o._outEdges;
	//	_inEdges = o._inEdges;*/
	//	for (const auto& e : o._outEdges)
	//		_outEdges.emplace_back(new NFAEdge(*e));
	//	for (const auto& e : o._inEdges)
	//		_inEdges.emplace_back(new NFAEdge(*e));
	//}

	// �ӽ���״̬��ʼ������ӽ������
	static void _addEndTag(NFAState* cur, NFAState* END, std::set<NFAState*> &vis) {
		cur->_end = true;
		if (cur == END)return;
		if (vis.find(cur) == vis.end())
			vis.insert(cur);
		else
			return;
		for (const auto& x : cur->_inEdges) {
			if (x->_symbol == NFAEdge::Epsilon) {

				if (cur->_type != Token::Type::None && x->_sourceState->_type==Token::Type::None)
					x->_sourceState->_type = cur->_type;

				_addEndTag(x->_sourceState, END, vis);
			}
		}
	}

	// ��cur�Ŀձհ�
	static std::set<NFAState*> getEClosure(NFAState* cur) {
		std::set<NFAState*> ret;
		for (const auto& x : cur->_outEdges) {
			if (x->_symbol == NFAEdge::Epsilon) {
				ret.insert(x->_targetState);
				auto r = getEClosure(x->_targetState);
				ret.insert(r.begin(), r.end());
			}
		}
		return ret;
	}

	// ȥ�ձ�
	void NFA::makeNoEmptyEdge(void) {
		std::set<NFAState*> vis;
		_addEndTag(this->_endState, this->_begState, vis);
		vis.clear();
		
		this->_begState->_valid = true; // ��ʼ״̬��Ϊ����Ч��
		for (const auto& s : _states) {
			//std::cout << s->_index << ": ";
			if (s->_valid == true) {
				auto ret = getEClosure(s); // �����Ч״̬�����Ŀձհ�
				for (const auto& x : ret) {  // ��������ÿ��״̬
					//std::cout << x->_index << ' ';
					for (const auto& e : x->_outEdges) {  //����ÿ������
						if (e->_symbol != NFAEdge::Epsilon) {  // ����Ƿǿձ�
							// ��ʱ�ӱ߲�ӦӰ�쵽״̬�Ƿ�Ϊ��ʼ/����״̬
							bool ed = s->_end;
							bool st = e->_targetState->_begin;
							s->addEdgeTo(e->_symbol, e->_targetState);  // �����
							s->_end = ed;
							e->_targetState->_begin = st;
						}
					}
				}
			}
			//std::cout << std::endl;
		}

		// ����ɾ
		// �ձ�
		for (const auto& x : _states) {
			for (auto e = x->_outEdges.begin(); e != x->_outEdges.end();) {
				if ((*e)->_symbol == NFAEdge::Epsilon) {

					delete *e; // inEdge��outEdge ���ı�����������ָ����ͬ�ڴ�ռ�ģ�ֻɾһ�鼴��
					e = x->_outEdges.erase(e);
				}else {
					e++;
				}
			}
		}
		// ����״̬
		for (auto x = _states.begin(); x != _states.end(); ) {
			if ((*x)->_valid == false) {
				//��Ч״̬�����б߶�ɾ
				for (auto e = (*x)->_outEdges.begin(); e != (*x)->_outEdges.end(); e++) {
					delete *e;
				}
				(*x)->_outEdges.clear();
				(*x)->_inEdges.clear();

				delete *x;
				x = _states.erase(x);
			}else {
				//// ��Ч״̬�ĳ�������Ѿ�ɾ����û�ñߣ�Ҫ��list��ɾ��
				//for (auto e = (*x)->_inEdges.begin(); e != (*x)->_inEdges.end(); ) {
				//	if (*e == nullptr) {
				//		e = (*x)->_inEdges.erase(e);
				//	}else {
				//		e++;
				//	}
				//}
				//for (auto e = (*x)->_outEdges.begin(); e != (*x)->_outEdges.end(); ) {
				//	if (*e == nullptr) {
				//		e = (*x)->_outEdges.erase(e);
				//	}else {
				//		e++;
				//	}
				//}
				// ��Ч״̬����������
				(*x)->_inEdges.clear();
				x++;

			}
		}

	}

}