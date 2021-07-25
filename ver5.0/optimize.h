#pragma once
#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include<iostream>
#include<string>
#include<sstream>
#include"IR.h"
#include"Lexer.h"
using namespace std;

typedef struct NodeRightMessage
{
	double num;
	string VariateName;
	bool tag;//trueʱ֤��������falseʱ֤����string
}NodeRightMessage;

typedef struct DAGTreeNode
{
	int num;//��ţ����ǵڼ����ڵ�
	Opcode opcode;//�������ͣ����ڱ�ʾ�ڰ���ppt��DAGͼ�н����ߵ������
	vector<NodeRightMessage> nodeRightMessage;//���ڱ�ʾ����ұߵ���Ϣ
	NodeRightMessage TopMessage;//�ұ���ǰ�����Ϣ

	int roof_num;//���ڵ�ı�ţ�������Ϊ0
	DAGTreeNode* left_Node;//����
	DAGTreeNode* right_Node;//�Һ���

}DAGTreeNode;


class DAG {

public:
	friend bool comp_quad(const Quad& a, const Quad& b);
	DAG();
	int count;//��¼���˶��ٸ����
	vector<DAGTreeNode> Node_sequence;
	vector<Quad> all_quads, last_quads;
	Quad end_quad;//���ڴ�Ż����������һ����Ԫʽ����ת�����ʽ��
	bool end_quad_tag;//true-����ת�����ʽ�Ļ���������㣬false-��


	//�����ຯ��
	void CreateNode(Quad q);
	int NewLeafNode(Var v);//����Var����һ����Ҷ�ڵ�
	void NewRoofNode(Opcode op, Var B, Var C, Var A);//���ɸ��ڵ�,�����Ӧ�ӽڵ������������� (w,b,c,a)û�п��ŵĵط�����Ԫʽ
	void LinkNode(Opcode op, Var A, int numB, int numC);//�ѽڵ���ϵ����
	void GenerateDAG(vector<Quad> quads, BasicBlock b);//����DAGͼ
	void Recur_Quads(int num_roof);//�ݹ鷨����Quads


	//�����ຯ��
	int FindNode(Var v);//����Var������֮ǰ�����õĽ�����Ƿ��н���и�VARֵ�����У����ظýڵ�ı�ţ����򷵻�-1
	int WhatType(string Str);//����string�ж���ʲô���� 3�������� 1����ϵͳ���ɵ���ʱ���� 2�����û�����ı�ʶ��
	DAGTreeNode* FindNodeByNum(int num);//���ݽڵ��Ų��ҽ��
	DAGTreeNode* CheckNode(Opcode op, int numB, int numC);//����Ƿ����� ��ͬ���������ͬ�ӽ��Ľ��

	//�Ƚ�


	//�жϸýڵ����Ƿ����û�����ı���


	//����һ����㸳ֵ����ʱ�����֮ǰ�Ľ�������ޣ�����Ҫɾȥ�����Ǻ���
	void DeleteNodeMessage(Var A);

	//��ֵ
	void Assign(Opcode op, Var B, Var C, Var A);

	//SCANF
	void Scanf(Opcode op, Var B, Var C, Var A);
	//PRINTF
	void Printf(Opcode op, Var B, Var C, Var A);

	//�����Ժ���
	double stringToNum(const string& str);//string����ת��ֵ
};


#endif // !OPTIMIZE_H