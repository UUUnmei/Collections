#include "Lexer.h"
#include <string>

bool Lexer::next_word(Token& tok) {
    if (_it == _tokens.end())
        return false;
    tok = *_it++;
    return true;
}

bool Lexer::prev_word(Token& tok) {

    if (_it == _tokens.begin())
        return false;
    tok = *--_it;
    return true;
}

bool Lexer::is_number(Token& t) {
    return t._type == TokenType::Integer
        || t._type == TokenType::Double;
}


void Lexer::debug(void) {
    Token t;
    std::ofstream out("output.txt");
    while (next_word(t))
        out << t._value << ' ' << t._row << ' ' << t._col << '\n';
    out.close();
}

int Lexer::get_used_token_num(void) {
    return _it - _tokens.begin();
}


// 从平均计时结果来看，这样的调用比手动读文件然后用带参数版本的函数慢一些
void Lexer::init(void) {
    if (_source_file.empty()) {
        std::cout << "no file to read" << std::endl;
        exit(1);
    }
    std::ifstream in(_source_file);
    if (!in) {
        std::cout << "read error" << std::endl;
        exit(1);
    }
    string buf;
    string source;
    while (std::getline(in, buf))
        source += buf + "\n";
    in.close();

    _tokens =  Lexer::generate_tokens(source);
    _tokens.emplace_back("#", 0, 0, TokenType::UnDefined);
    // 设置指针
    _it = _tokens.begin();
}

vector<Token> Lexer::generate_tokens(const string& source) {
    if (source.empty())
        return vector<Token>();

    vector<Token> ret; // 存放结果

    // 准备， 模拟自动机
    enum class State
    {
        Begin,
        Integer,
        Double,
        Char,
        StringConstant,
        Identifier,
        Comment,
        preComment,
    };
    const char* curChar = source.c_str();
    const char* begin = nullptr;
    const char* rowBegin = curChar;

    int rowCount = 1;
    State curState = State::Begin;

    auto addToken = [&](int size, TokenType type) {
        // 注释不管
        if (type == TokenType::Comment) {
            return;
        }
        // 拿到具体内容
        auto tokenBegin = begin ? begin : curChar;
        string v(tokenBegin, tokenBegin + size);

        switch (type) {

        case TokenType::Identifier: {
            type =
                v == "int" ? TokenType::INT :
                v == "float" ? TokenType::FLOAT :
                v == "double" ? TokenType::DOUBLE :
                v == "char" ? TokenType::CHAR :
                v == "struct" ? TokenType::STRUCT :
                v == "main" ? TokenType::MAIN :
                v == "void" ? TokenType::VOID :
                v == "if" ? TokenType::IF :
                v == "else" ? TokenType::ELSE :
                v == "for" ? TokenType::FOR :
                v == "do" ? TokenType::DO :
                v == "while" ? TokenType::WHILE :
                v == "continue" ? TokenType::CONTINUE :
                v == "break" ? TokenType::BREAK :
                v == "return" ? TokenType::RETURN :
                v == "scanf" ? TokenType::SCANF :
                v == "printf" ? TokenType::PRINTF :
                v == "const" ? TokenType::CONST :
                v == "case" ? TokenType::CASE :
                v == "switch" ? TokenType::SWITCH :
                v == "default" ? TokenType::DEFAULT :
                TokenType::Identifier;
            break;
        }
        default:
            break;
        }

        ret.push_back(Token(v, rowCount, tokenBegin - rowBegin + 1, type));
    };

    // 运行

    while (char c = *curChar) {

        switch (curState) {

        case State::Begin: {
            switch (c) {
            case '(':
                addToken(1, TokenType::LeftBracket);
                break;
            case ')':
                addToken(1, TokenType::RightBracket);
                break;
            case '{':
                addToken(1, TokenType::LeftBrace);
                break;
            case '}':
                addToken(1, TokenType::RightBrace);
                break;
            case '[':
                addToken(1, TokenType::LeftSquare);
                break;
            case ']':
                addToken(1, TokenType::RightSquare);
                break;
            case ';':
                addToken(1, TokenType::SemiColon);
                break;
            case ',':
                addToken(1, TokenType::Comma);
                break;
            case '-': {
                if (curChar[1] && curChar[1] == '-')
                    addToken(2, TokenType::Dec), curChar++;
                else if (curChar[1] && curChar[1] == '>')
                    addToken(2, TokenType::Arrow), curChar++;
                else
                    addToken(1, TokenType::Sub);
                break;
            }
            case '+': {
                if (curChar[1] && curChar[1] == '+')
                    addToken(2, TokenType::Inc), curChar++;
                else
                    addToken(1, TokenType::Add);
                break;
            }
            case '*':
                addToken(1, TokenType::Mul);
                break;
            case '/':
                begin = curChar;
                curState = State::preComment;
                break;
            case '%':
                addToken(1, TokenType::Mod);
                break;
            case '=': {
                if (curChar[1] && curChar[1] == '=')
                    addToken(2, TokenType::EQ), curChar++;
                else
                    addToken(1, TokenType::Assign);
                break;
            }
            case '.':
                addToken(1, TokenType::Dot);
                break;
            case '&': {
                if (curChar[1] && curChar[1] == '&')
                    addToken(2, TokenType::AndAnd), curChar++;
                else
                    addToken(1, TokenType::And);
                break;
            }
            case '|': {
                if (curChar[1] && curChar[1] == '|')
                    addToken(2, TokenType::OrOr), curChar++;
                else
                    addToken(1, TokenType::Or);
            }
            case '^':
                addToken(1, TokenType::Xor);
                break;
            case '~':
                addToken(1, TokenType::Inv);
                break;
            case '>': {
                if (curChar[1] && curChar[1] == '=')
                    addToken(2, TokenType::GE), curChar++;
                else
                    addToken(1, TokenType::GT);
                break;
            }
            case '<': {
                if (curChar[1] && curChar[1] == '=')
                    addToken(2, TokenType::LE), curChar++;
                else
                    addToken(1, TokenType::LT);
                break;
            }
            case '!': {
                if (curChar[1] && curChar[1] == '=')
                    addToken(2, TokenType::NE), curChar++;
                else {
                    addToken(1, TokenType::Not);
                }
                break;
            }

            case '\n':
                rowCount++;
                rowBegin = curChar + 1;
                break;
            case '\r':
            case ' ':
            case '\t':
                break;

            case '\"':
                begin = curChar;
                curState = State::StringConstant;
                break;

            case '\'':
                begin = curChar;
                curState = State::Char;
                break;

            default: {
                if (isdigit(c)) {
                    begin = curChar;
                    curState = State::Integer;
                }
                else if (isalpha(c) || c == '_') {
                    begin = curChar;
                    curState = State::Identifier;
                }
                else {
                    printf("error  %c  at row %d\n", rowCount, c);
                    exit(0);
                }
            }
            }
            break;
        }

        case State::StringConstant: {
            switch (c)
            {
            case '\"':
                begin++;
                addToken(curChar - begin, TokenType::StringConstant);
                begin = nullptr;
                curState = State::Begin;
                break;
            case '\n':
                printf("error  \\n  at row  %d, 应为单行字符串\n", rowCount);
                exit(1);
            }
            break;
        }

        case State::Char: {
            if (c == '\'') {
                begin++;
                if (curChar - begin != 1) {
                    // 字符应该长度为1
                    printf("error  use of \'  at row  %d\n", rowCount);
                    exit(1);
                }
                addToken(curChar - begin, TokenType::Char);
                begin = nullptr;
                curState = State::Begin;
                break;
            }
            break;
        }

        case State::preComment: {
            switch (c) {
            case '/':
                curState = State::Comment;
                break;
            default:
                addToken(1, TokenType::Div);
                curState = State::Begin;
                begin = nullptr;
                curChar--;
            }
            break;
        }
        case State::Comment: {
            switch (c) {
                // 目前只有单行注释
            case '\n':
                addToken(curChar - begin, TokenType::Comment);
                curState = State::Begin;
                begin = nullptr;
                curChar--;
            }
            break;
        }

        case State::Identifier: {
            if (isalpha(c) || c == '_' || isdigit(c)) {
                // do nothing

            }
            else {
                addToken(curChar - begin, TokenType::Identifier);
                curState = State::Begin;
                begin = nullptr;
                curChar--;
            }

            break;
        }
        case State::Integer: {
            if (isdigit(c)) {
                // do nothing
            }
            else if (c == '.' && curChar[1] && isdigit(curChar[1])) {
                curState = State::Double;
            }
            else if (c == 'e' && curChar[1] && (curChar[1] == '+' || curChar[1] == '-' || isdigit(curChar[1]))) {
                curState = State::Double;
                curChar++;
            }
            else {
                addToken(curChar - begin, TokenType::Integer);
                curState = State::Begin;
                begin = nullptr;
                curChar--;
            }
            break;
        }
        case State::Double: {
            if (isdigit(c)) {
                // do nothing
            }
            else if (c == 'e' && curChar[1] && (curChar[1] == '+' || curChar[1] == '-' || isdigit(curChar[1]))) {
                curChar++;
            }
            else {
                addToken(curChar - begin, TokenType::Double);
                curState = State::Begin;
                begin = nullptr;
                curChar--;
            }
            break;
        }

        }
        curChar++;
    }

    // 收尾
    if (begin) {
        switch (curState) {
        case State::Integer:
            addToken(curChar - begin, TokenType::Integer);
            break;
        case State::Double:
            addToken(curChar - begin, TokenType::Double);
            break;
        case State::Identifier:
            addToken(curChar - begin, TokenType::Identifier);
            break;
        case State::StringConstant:
        case State::Char:
            printf("char or string should in a single line, error at row %d\n", rowCount);
            exit(1);
            break;
        default:
            break;
        }
    }
    return ret;
}

