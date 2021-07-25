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
                cout << "�﷨�����������ļ���ʧ��\n";
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

    void Program_sp();//sp����SubProgram �ӳ����д�������ǡ����򡱵ĵݹ��ӳ���
    void MainFunc_sp();//sp����SubProgram �ӳ����д�������ǡ����������ĵݹ��ӳ���
    void CompoundStatement_sp();//sp����SubProgram �ӳ����д�������ǡ�������䡱�ĵݹ��ӳ���
    void VariableDeclaration_sp();//sp����SubProgram �ӳ����д�������ǡ�����˵�����ĵݹ��ӳ���
    void VariableDefinition_sp();//sp����SubProgram �ӳ����д�������ǡ��������塱�ĵݹ��ӳ���
    void StatementColumn_sp();//sp����SubProgram �ӳ����д�������ǡ�����С��ĵݹ��ӳ���
    void Statement_sp();//sp����SubProgram �ӳ����д�������ǡ���䡱�ĵݹ��ӳ���
    void AssignmentStatement_sp();//sp����SubProgram �ӳ����д�������ǡ���ֵ��䡱�ĵݹ��ӳ���
    void Expression_sp();//sp����SubProgram �ӳ����д�������ǡ����ʽ���ĵݹ��ӳ���
    void Item_sp();//sp����SubProgram �ӳ����д�������ǡ���ĵݹ��ӳ���
    void Factor_sp();//sp����SubProgram �ӳ����д�������ǡ����ӡ��ĵݹ��ӳ���
    void AddingOperator_sp();//sp����SubProgram �ӳ����д�������ǡ��ӷ���������ĵݹ��ӳ���
    void MultiplyingOperator_sp();//sp����SubProgram �ӳ����д�������ǡ��˷���������ĵݹ��ӳ���
    void Number_sp();//sp����SubProgram �ӳ����д�������ǡ������ĵݹ��ӳ���
    void ConditionStatement_sp();  // �������
    void Condition_sp();            // ����
    void LoopStatement_sp();        // ѭ�����
    void RelationaOperator_sp();    // ��ϵ�����
    void Printf_sp();           // ���
    void Scanf_sp();            // ����

    void INSCP();           // ��������
    void OTSCP();           // ��������



    void UnStated_ERROR();//δ����������     �Ⱥ���ߵ�δ����
    void Double_UnStated_ERROR();//δ����������
    void RepeatStatement_ERROR();//�ظ���������
};