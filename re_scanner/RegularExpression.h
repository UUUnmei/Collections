#pragma once
#include "NFA.h"
#include "DFA.h"
#include "Token.h"

namespace RegularExpression {

	class RegularExpression {
    public:
        NFA::NFA _expression; 
        static Token::Type _type; 
        static void setType(Token::Type _t);


        RegularExpression() {}
        // 根据一个字符构造
        explicit RegularExpression(const char& exp) :_expression(exp, _type) {}
        //RegularExpression(const RegularExpression& o) : _expression(o._expression) {}
        ~RegularExpression() = default;

        // 基本变换
        RegularExpression Union(RegularExpression other);
        RegularExpression Concat(RegularExpression other);
        RegularExpression StarClosure(void);
        // 其他变换
        RegularExpression operator|(RegularExpression other);
        explicit RegularExpression(const std::string& s);
        explicit RegularExpression(char l, char r);
        RegularExpression PosClosure(void);
        RegularExpression Optional(void);

        DFA::DFA toDFA(void);
        
	};

}