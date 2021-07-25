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
    dw_DO,      // (dw_DO, SEM[m], _, _)    ��������ȥLB������˳������


    // for
    fr_FR,      // (fr_FR, _, _, _)
    fr_LB,      // ����������ʵ��while������һ��
    fr_DO,
    fr_FE,

    // scanf
    SCANF,      // (SCANF, _ , _, dest)   scanf(dest)  dest�Ǹ���ʶ��
    // printf
    PRINTF,      // (PRINTF, SEM[m], _ , _ )   printf(exp)  exp�Ǹ����ʽ


    Undefined,   // ����Ԫʽ
};

struct Var {
	string name;  // ������
	bool alive;  // ��Ծ��Ϣ

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

    int jmp_to; // ��������ת����Ԫʽ

    int num;//�����Ż�ʱ������

	// ��������Ԫʽ
	Quad(Opcode op, const string& b, const string& c, const string& a)
        :op(op), B(b), C(c), A(a), jmp_to(-1) {
        id = cnt++;
    }
    // ��(= a _ b)����ʽ
    Quad(Opcode op, const string& b, const string& a)
        :op(op), B(b), C(), A(a), jmp_to(-1) {
        id = cnt++;
    }
    // �� (do, SEM[m], _, _) ����ʽ
    Quad(Opcode op, const string& b)
        :op(op), B(b), C(), A(), jmp_to(-1) {
        id = cnt++;
    }
    // ��  (wh, _, _, _)����ʽ
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

   


    // �����Ծ��Ϣ
    void cal_active_info(BasicBlock& b);  // ��һ�����ڣ���Ҫ������

      // ������ת��λ�ù�ϵ
    void exec_jmp_target();
public:
    
    vector<Quad> all_quads;
    std::map<int, BasicBlock> all_blocks;
    std::map<int, BasicBlock>::iterator cur_block;

    void set_output_file(const string& s) {
        outputfile.open(s);
        if (!outputfile.is_open()) {
            std::cout << "��Ԫʽ����ļ���ʧ��\n";
            exit(1);
        }
    }

    IR() : cur_block(all_blocks.begin()) {  
        // ������cur_block��ʼ�����ƺ�������Ч�ģ������Ҫʹ�üǵ���һ��ʼ cur_block = all_blocks.begin();
        all_quads.clear();
        all_blocks.clear();
    }

    bool next_block(BasicBlock & b);
    vector<Quad> get_all_quads();
    int get_cur_quad_position();
    void move_backward(int b, int e);   // all_quads : �±�be֮�������ƶ���ĩβ

    
    // �����Ԫʽ���Ҫ��һ�� 
    //�����磺
    //ir.add_quad(Opcode::GT, "a_", "b_", "t1");
    //ir.add_quad(Opcode::IF, "t1");
    //ir.add_quad(Opcode::Add, "a_", "b_", "t2");
    //ir.add_quad(Opcode::Mul, "t2", "c_", "x_");
    //ir.add_quad(Opcode::EL);
    //ir.add_quad(Opcode::Mul, "a_", "b_", "t3");
    //ir.add_quad(Opcode::Sub, "5_", "t3", "x_");
    //ir.add_quad(Opcode::IE);
    //ir.add_quad(Opcode::Undefined);  ��������
    // �����Ԫʽ��4��������Ӧ��Ԫʽ��4�����캯��
    void add_quad(Opcode op, const string& b, const string& c, const string& a) {
        all_quads.emplace_back(op, b, c, a);
    }
    void add_quad(Opcode op, const string& b, const string& a) {
        all_quads.emplace_back(op, b, a);
    }
    void add_quad(Opcode op, const string& b) {
        all_quads.emplace_back(op, b);
        if (op == Opcode::SCANF) {
            // ����������⴦��һ�£�
            // Ϊ�˱���Ӱ���Ծ��Ϣ����������identifier��һ�θ�ֵ
            // (SCANF, _, _, identifier) 
            all_quads.back().A.name = b;
            all_quads.back().B.name = "";
        }
    }
    void add_quad(Opcode op) {
        all_quads.emplace_back(op);
    }

    // �����Ԫʽ��Ϣ�ͻ����黮�����
    void debug();

    // �����黮��
    void divide_blocks();

    // �����Ծ��Ϣ
    void cal_active_info_all();          // �����п��

};

#endif // !IR_H