#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include"Lexer.h"
#include<vector>
#include"symbolTable.h"
#include"IR.h"
using namespace std;

class Parser {
public:
    bool debug = false;
    void err(const string& s);
    void log(const string& s);
    ofstream outputfile;
    void set_output_file(const string& s) {
        if (debug) {
            outputfile.open(s);
            if (!outputfile.is_open()) {
                cout << "语法分析结果输出文件打开失败\n";
                exit(1);
            }
        }  
    }


    Lexer Grammer_Lexer;
    Token PresentToken;
    symbolTable SymbolTable;
    IR ir;

    int global_type;
    string global_name;
    int const_count;
    int offset;

    vector<Token> SEM;

    Parser(string source_file);
    void Next();

    void Program_sp();//sp——SubProgram 子程序简写，这里是“程序”的递归子程序
    void MainFunc_sp();//sp——SubProgram 子程序简写，这里是“主函数”的递归子程序
    void CompoundStatement_sp();//sp——SubProgram 子程序简写，这里是“复合语句”的递归子程序
    void VariableDeclaration_sp();//sp——SubProgram 子程序简写，这里是“变量说明”的递归子程序
    void VariableDefinition_sp();//sp——SubProgram 子程序简写，这里是“变量定义”的递归子程序
    void StatementColumn_sp();//sp——SubProgram 子程序简写，这里是“语句列”的递归子程序
    void Statement_sp();//sp——SubProgram 子程序简写，这里是“语句”的递归子程序
    void AssignmentStatement_sp();//sp——SubProgram 子程序简写，这里是“赋值语句”的递归子程序
    void Expression_sp();//sp——SubProgram 子程序简写，这里是“表达式”的递归子程序
    void Item_sp();//sp——SubProgram 子程序简写，这里是“项”的递归子程序
    void Factor_sp();//sp——SubProgram 子程序简写，这里是“因子”的递归子程序
    void AddingOperator_sp();//sp——SubProgram 子程序简写，这里是“加法运算符”的递归子程序
    void MultiplyingOperator_sp();//sp——SubProgram 子程序简写，这里是“乘法运算符”的递归子程序
    void Number_sp();//sp——SubProgram 子程序简写，这里是“数”的递归子程序
    void ConditionStatement_sp();  // 条件语句
    void Condition_sp();            // 条件
    void LoopStatement_sp();        // 循环语句
    void RelationaOperator_sp();    // 关系运算符
    void Printf_sp();           // 输出
    void Scanf_sp();            // 输入

    void INSCP();           // 进作用域
    void OTSCP();           // 出作用域



    void UnStated_ERROR();//未声明错误检测     等号左边的未声明
    void Double_UnStated_ERROR();//未声明错误检测
    void RepeatStatement_ERROR();//重复声明错误
};