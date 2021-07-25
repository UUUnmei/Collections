#include "NFA.h"
#include <set>
#include <cassert>
namespace NFA {
	void NFAState::addEdge(NFAEdge* edge) {
		edge->_sourceState->_end = false; // 这个其实不绝对，见PosClosure的情况
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
		// 鉴于没实现深拷贝，这个如果作为析构会报错，
		// 比如 RE re = RE('0');
		// 这句执行完，右边的析构，而其实re里面的也已经没了。。。

		for (auto x = _states.begin(); x != _states.end(); x++) {
			//状态的所有边都删
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

	// 从结束状态开始反向添加结束标记
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

	// 求cur的空闭包
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

	// 去空边
	void NFA::makeNoEmptyEdge(void) {
		std::set<NFAState*> vis;
		_addEndTag(this->_endState, this->_begState, vis);
		vis.clear();
		
		this->_begState->_valid = true; // 开始状态认为是有效的
		for (const auto& s : _states) {
			//std::cout << s->_index << ": ";
			if (s->_valid == true) {
				auto ret = getEClosure(s); // 求从有效状态出发的空闭包
				for (const auto& x : ret) {  // 对于其中每个状态
					//std::cout << x->_index << ' ';
					for (const auto& e : x->_outEdges) {  //对于每个出边
						if (e->_symbol != NFAEdge::Epsilon) {  // 如果是非空边
							// 此时加边不应影响到状态是否为开始/结束状态
							bool ed = s->_end;
							bool st = e->_targetState->_begin;
							s->addEdgeTo(e->_symbol, e->_targetState);  // 补充边
							s->_end = ed;
							e->_targetState->_begin = st;
						}
					}
				}
			}
			//std::cout << std::endl;
		}

		// 真正删
		// 空边
		for (const auto& x : _states) {
			for (auto e = x->_outEdges.begin(); e != x->_outEdges.end();) {
				if ((*e)->_symbol == NFAEdge::Epsilon) {

					delete *e; // inEdge和outEdge 里存的边是总体上是指向相同内存空间的，只删一遍即可
					e = x->_outEdges.erase(e);
				}else {
					e++;
				}
			}
		}
		// 无用状态
		for (auto x = _states.begin(); x != _states.end(); ) {
			if ((*x)->_valid == false) {
				//无效状态的所有边都删
				for (auto e = (*x)->_outEdges.begin(); e != (*x)->_outEdges.end(); e++) {
					delete *e;
				}
				(*x)->_outEdges.clear();
				(*x)->_inEdges.clear();

				delete *x;
				x = _states.erase(x);
			}else {
				//// 有效状态的出入边有已经删除的没用边，要从list中删除
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
				// 有效状态仅保留出边
				(*x)->_inEdges.clear();
				x++;

			}
		}

	}

}