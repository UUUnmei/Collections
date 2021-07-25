#pragma once

#ifndef IR_H
#define IR_H


#include <vector>
#include <iostream>
#include <map>
#include <fstream>
using std::string;
using std::vector;

enum class Opcode {
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

    
    IF,         //  (if, SEM[m], _, _)
    EL,         //  (el, _, _, _)
    IE,         //  (ie, _, _, _)

    WH,         //  (wh, _, _, _)
    DO,         //  (do, SEM[m], _, _)
    WE,         //  (we, _, _, _)

    // do-while
    dw_LB,      // (dw_LB, _, _, _)
    dw_WH,      // (dw_WH, _, _, _)
    dw_DO,      // (dw_DO, SEM[m], _, _)    满足条件去LB，否则顺序向下


    // for
    fr_FR,      // (fr_FR, _, _, _)
    fr_LB,      // 下面中三其实和while的三个一样
    fr_DO,
    fr_FE,

    // scanf
    SCANF,      // (SCANF, _ , _, dest)   scanf(dest)  dest是个标识符
    // printf
    PRINTF,      // (PRINTF, SEM[m], _ , _ )   printf(exp)  exp是个表达式


    Undefined,   // 空四元式
};

struct Var {
	string name;  // 变量名
	bool alive;  // 活跃信息

    Var() = default;
	Var(const string& s, bool liv = true) : name(s), alive(liv) { }
};


struct Quad {
    static int cnt;
    int id;

	// (op, B, C, A)
	Opcode op;  
	Var B;  
	Var C;
	Var A;

    int jmp_to; // 对于有跳转的四元式

    int num;//用于优化时排序用

	// 完整的四元式
	Quad(Opcode op, const string& b, const string& c, const string& a)
        :op(op), B(b), C(c), A(a), jmp_to(-1) {
        id = cnt++;
    }
    // 如(= a _ b)的形式
    Quad(Opcode op, const string& b, const string& a)
        :op(op), B(b), C(), A(a), jmp_to(-1) {
        id = cnt++;
    }
    // 如 (do, SEM[m], _, _) 的形式
    Quad(Opcode op, const string& b)
        :op(op), B(b), C(), A(), jmp_to(-1) {
        id = cnt++;
    }
    // 如  (wh, _, _, _)的形式
    Quad(Opcode op)
        :op(op), B(), C(), A(), jmp_to(-1) {
        id = cnt++;
    }

    Quad()
        :op(Opcode::Undefined), B(), C(), A(), jmp_to(-1) {
        id = cnt++;
    }
};

bool comp_quad(const Quad& a, const Quad& b);

struct BasicBlock {
    int start;
    int end;
    BasicBlock() = default;
    BasicBlock(int s, int e) : start(s), end(e) { }
};

class IR
{
    std::ofstream outputfile;

    void debug_all_quads();
    void debug_all_blocks();
    void debug_all_quads_with_active();

   


    // 计算活跃信息
    void cal_active_info(BasicBlock& b);  // 求一个块内，不要单独用

      // 处理跳转的位置关系
    void exec_jmp_target();
public:
    
    vector<Quad> all_quads;
    std::map<int, BasicBlock> all_blocks;
    std::map<int, BasicBlock>::iterator cur_block;

    void set_output_file(const string& s) {
        outputfile.open(s);
        if (!outputfile.is_open()) {
            std::cout << "四元式输出文件打开失败\n";
            exit(1);
        }
    }

    IR() : cur_block(all_blocks.begin()) {  
        // ！这里cur_block初始化后似乎还是无效的，如果需要使用记得在一开始 cur_block = all_blocks.begin();
        all_quads.clear();
        all_blocks.clear();
    }

    bool next_block(BasicBlock & b);
    vector<Quad> get_all_quads();
    int get_cur_quad_position();
    void move_backward(int b, int e);   // all_quads : 下标be之间内容移动到末尾

    
    // 添加四元式最后要补一个 
    //样例如：
    //ir.add_quad(Opcode::GT, "a_", "b_", "t1");
    //ir.add_quad(Opcode::IF, "t1");
    //ir.add_quad(Opcode::Add, "a_", "b_", "t2");
    //ir.add_quad(Opcode::Mul, "t2", "c_", "x_");
    //ir.add_quad(Opcode::EL);
    //ir.add_quad(Opcode::Mul, "a_", "b_", "t3");
    //ir.add_quad(Opcode::Sub, "5_", "t3", "x_");
    //ir.add_quad(Opcode::IE);
    //ir.add_quad(Opcode::Undefined);  ！！！！
    // 添加四元式的4个方法对应四元式的4个构造函数
    void add_quad(Opcode op, const string& b, const string& c, const string& a) {
        all_quads.emplace_back(op, b, c, a);
    }
    void add_quad(Opcode op, const string& b, const string& a) {
        all_quads.emplace_back(op, b, a);
    }
    void add_quad(Opcode op, const string& b) {
        all_quads.emplace_back(op, b);
        if (op == Opcode::SCANF) {
            // 输入语句特殊处理一下，
            // 为了避免影响活跃信息，视作对于identifier的一次赋值
            // (SCANF, _, _, identifier) 
            all_quads.back().A.name = b;
            all_quads.back().B.name = "";
        }
    }
    void add_quad(Opcode op) {
        all_quads.emplace_back(op);
    }

    // 输出四元式信息和基本块划分情况
    void debug();

    // 基本块划分
    void divide_blocks();

    // 计算活跃信息
    void cal_active_info_all();          // 求所有块的

};

#endif // !IR_H