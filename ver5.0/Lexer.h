
#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <cstdio>
#include <cctype>

using std::string;
using std::vector;


enum class TokenType
{
    Integer,
    Double,
    Char,
    StringConstant,
    Identifier,

    // KEYWORD
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    STRUCT,
    MAIN,
    VOID,
    IF,
    ELSE,
    FOR,
    DO,
    WHILE,
    CONTINUE,
    BREAK,
    RETURN,
    CONST,
    CASE,
    SWITCH,
    DEFAULT,
    SCANF,
    PRINTF,

    // Operator or Delimiter
    LeftBracket,  // (
    RightBracket, // )
    LeftSquare,   // [
    RightSquare,  // ]
    LeftBrace,    // {
    RightBrace,   // }
    Comma,        // ,
    SemiColon,    // ;
    Dot,          // .
    Arrow,        // ->

    Add,          // +
    Sub,          // -
    Mul,          // *
    Div,          // /
    Mod,          // %
    Inc,          // ++
    Dec,          // --

    LT,           // <
    GT,           // >
    LE,           // <=
    GE,           // >=
    EQ,           // ==
    NE,           // !=

    Assign,       // =

    And,          // &
    Or,           // |
    Inv,          // ~
    Xor,          // ^

    Not,          // !
    AndAnd,       // &&
    OrOr,         // ||

    Comment,      //  �� // ��ͷ�� ����

    UnDefined,
};

struct Token {
    string _value;
    int _row;
    int _col;
    TokenType _type;

    Token() = default;
    Token(const string& v, int r, int c, TokenType t)
        : _value(v), _row(r), _col(c), _type(t) { }
};

class Lexer {

public:
    string _source_file;
    Lexer() { }
    Lexer(const string& filename)
        : _source_file(filename) { }
    void setSourceFile(const string& filename) {
        _source_file = filename;
    }

    // ���ȫ��token���ļ�
    void debug(void);
    // ��ȫɨһ������ȫ����token
    void init(void);
    // ����ֻ��¶һ��һ����token�Ľӿ�
    bool next_word(Token& tok);
    bool prev_word(Token& tok);

    // һЩ���ߺ���
    int get_used_token_num(void);
    static bool is_number(Token& t);

private:
    vector<Token>::iterator _it;
    vector<Token> _tokens;
    static vector<Token> generate_tokens(const string&);
};


#endif // !LEXER_H