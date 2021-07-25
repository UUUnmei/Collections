#include "DFA.h"
#include <string>
namespace DFA {
	//std::vector<char> DFA::_basicCharSet{
	//'0','1','2','3','4','5','6','7','8','9',
	//'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	//'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	//'+','-','*','/','%','^','&','|','~','(',')','.','=','!','>','<','\"','\''
	//};

	//void DFA::addState(int x, bool _end) {
	//	DFAState a{ x, _end };
	//	_States[x] = a;
	//}
	//size_t hasher(const DFAState& x) {
	//	return std::hash<std::string>()(std::to_string(x._id));
	//}
	//bool eq(const DFAState& x, const DFAState& y) {
	//	return x._id == y._id;
	//}
	size_t hasher(const std::pair<int, char>& x) {
		return std::hash<std::string>()(std::to_string(x.first) + x.second);
	}
	bool eq(const std::pair<int, char>& x, const std::pair<int, char>& y) {
		return x.first == y.first && x.second == y.second;
	}

	void DFA::addState(int x, bool end) {
		_states.insert(x);
		_end[x] = end;
	}
	
}