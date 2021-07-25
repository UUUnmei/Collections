
#pragma once
#include <vector>
#include <unordered_map>
namespace Token {
    enum Type {

        None,
        // class
        Int,
        Double,
        StringConstant,
        Identifier,
        Keyword,
        Operator,
        Delimiter,

        ////Keyword
        //WHILE, IF, ELSE, FOR, BREAK, CASE, SWITCH, CONTINUE, 
        //DEFAULT, INT, DOUBLE, MAIN, RETURN, VOID,
        ////Operator
        //EQ_EQ, N_EQ, LE, GE, ARROW, PLUS_EQ, MINUS_EQ, MUL_EQ, DIV_EQ,
        //REM_EQ, XOR_EQ, OR_OR, AND_AND, PLUS_PLUS, MINUS_MINUS, TANHAO, REV,
        //OR, AND, PLUS, MINUS, MUL, DIV, REM, XOR, G, EQ, L,
        ////Delimiter
        //LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
        //COMMA, SEMICOLON
    };  
    const std::vector<std::string> TypeName{ "None", "Int", "Double", "StringConstant", 
        "Identifier", "Keyword", "Operator", "Delimiter" };

    /*class Token {
    public:
        Type _type;
        std::string _exp;
    };*/
    struct Token {
        Type _type;
        std::string _exp;
        int _row;
        int _col;
    };

    //const std::vector<std::unordered_map<std::string, Type>> exactName{
    //    // keyword
    //    {
    //        {"while", WHILE}, {"if", IF}, {"else", ELSE}, {"for", FOR}, {"break", BREAK},
    //        {"case", CASE}, {"switch", SWITCH}, {"continue", CONTINUE}, {"default", DEFAULT}, {"int", INT},
    //        {"double", DOUBLE}, {"main", MAIN}, {"return", RETURN}, {"void", VOID}
    //    },
    //    // Operator
    //    {
    //        {"==", EQ_EQ}, {"!=", N_EQ}, {"<=", LE}, {">=", GE}, {"->", ARROW},
    //        {"+=", PLUS_EQ}, {"-=", MINUS_EQ}, {"*=", MUL_EQ}, {"/=", DIV_EQ}, {"%=", REM_EQ}, {"^=", XOR_EQ},
    //        {"||", OR_OR}, {"&&", AND_AND}, {"++", PLUS_PLUS}, {"--", MINUS_MINUS}, {'!', TANHAO}, {'~', REV},
    //        {'|', OR}, {'&', AND}, {'+', PLUS}, {'-', MINUS}, {'*', MUL}, {'/', DIV}, {'%', REM}, {'^', XOR},
    //        {'>', G}, {'=', EQ}, {'<', L}
    //    },
    //    // Delimiter
    //    {
    //        {"(", LEFT_PAREN}, { ")", RIGHT_PAREN }, { "{", LEFT_BRACE }, { "}", RIGHT_BRACE }, { "[", LEFT_BRACKET },
    //        { "]", RIGHT_BRACKET }, { ";", SEMICOLON }, { ",", COMMA }
    //    }

    //};
}