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
	bool tag;//true时证明是数，false时证明是string
}NodeRightMessage;

typedef struct DAGTreeNode
{
	int num;//标号，这是第几个节点
	Opcode opcode;//运算类型，用于表示第八章ppt上DAG图中结点左边的运算符
	vector<NodeRightMessage> nodeRightMessage;//用于表示结点右边的信息
	NodeRightMessage TopMessage;//右边最前面的信息

	int roof_num;//父节点的标号，若无则为0
	DAGTreeNode* left_Node;//左孩子
	DAGTreeNode* right_Node;//右孩子

}DAGTreeNode;


class DAG {

public:
	friend bool comp_quad(const Quad& a, const Quad& b);
	DAG();
	int count;//记录有了多少个结点
	vector<DAGTreeNode> Node_sequence;
	vector<Quad> all_quads, last_quads;
	Quad end_quad;//用于存放基本块中最后一个四元式，跳转语句形式的
	bool end_quad_tag;//true-有跳转语句形式的基本块结束点，false-无


	//生成类函数
	void CreateNode(Quad q);
	int NewLeafNode(Var v);//根据Var生成一个新叶节点
	void NewRoofNode(Opcode op, Var B, Var C, Var A);//生成根节点,并与对应子节点相连，适用于 (w,b,c,a)没有空着的地方的四元式
	void LinkNode(Opcode op, Var A, int numB, int numC);//把节点联系起来
	void GenerateDAG(vector<Quad> quads, BasicBlock b);//生成DAG图
	void Recur_Quads(int num_roof);//递归法生成Quads


	//查找类函数
	int FindNode(Var v);//根据Var查找在之前创建好的结点中是否有结点有该VAR值，若有，返回该节点的标号，否则返回-1
	int WhatType(string Str);//根据string判断是什么类型 3——数字 1——系统生成的临时变量 2——用户定义的标识符
	DAGTreeNode* FindNodeByNum(int num);//根据节点标号查找结点
	DAGTreeNode* CheckNode(Opcode op, int numB, int numC);//检查是否有了 相同运算符、相同子结点的结点

	//比较


	//判断该节点中是否有用户定义的变量


	//当对一个结点赋值操作时，检查之前的结点中有无，若有要删去，这是函数
	void DeleteNodeMessage(Var A);

	//赋值
	void Assign(Opcode op, Var B, Var C, Var A);

	//SCANF
	void Scanf(Opcode op, Var B, Var C, Var A);
	//PRINTF
	void Printf(Opcode op, Var B, Var C, Var A);

	//功能性函数
	double stringToNum(const string& str);//string类型转数值
};


#endif // !OPTIMIZE_H