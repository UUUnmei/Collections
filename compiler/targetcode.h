#pragma once
#ifndef targetcode_h
#define targetcode_h

#include<vector>
#include<iostream>
#include<stack>
#include<string>
#include <unordered_map>
#include "IR.h"
using namespace std;


struct registers
{
    string name; //�Ĵ�������
    vector<string> Rvalue;//�Ĵ����ڵ�ֵ
    registers(string n) { name = n; }
    registers() {};
    void clearpush(string n) //�����ǰ�Ĵ����ڵı����������µ�ֵ
    {
        Rvalue.clear();
        Rvalue.push_back(n);
    }
    bool isinReg(string n)
    {
        bool flag = false;
        for (int i = 0; i < Rvalue.size(); i++)
        {
            if (Rvalue[i] == n)
                flag = true;
        }
        return flag;
    }
};


struct interdata
{
    string a, b, c;
    interdata() {}
    interdata(string a, string b, string c) :a(a), b(b), c(c) {}
};

class targetcode
{
    unordered_map<string, int> offset;
    std::ofstream outputfile;
    interdata data[1000];
    vector<Quad> qua;
public:
    targetcode() {};
    void input(vector<Quad> q);
    void deal();
    void out(const string& s);

    void print_code_head();
    void print_code_tail();
    //void print_all_variables();
    void cal_offset();
    string bp_addr(const string& s);

};

#endif // !targetcode_h
