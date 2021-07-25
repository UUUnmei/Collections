
#include"optimize.h"
DAG::DAG()
{
	this->count = 0;
	this->end_quad_tag = false;
}

void DAG::CreateNode(Quad q) {

	switch (q.op)
	{
	case Opcode::Add:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::Sub:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::Mul:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::Div:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::Mod:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::NE:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::LT:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::GT:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::LE:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::GE:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::EQ:NewRoofNode(q.op, q.B, q.C, q.A);
		break;
	case Opcode::Assign:Assign(q.op, q.B, q.C, q.A);
		break;
	case Opcode::IF:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;////////////////////////////////////////ֻ��һ��B
	case Opcode::EL:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;//�޲�
	case Opcode::IE:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;//�޲�
	case Opcode::WH:
		this->all_quads.push_back(q);
		break;///�޲�
	case Opcode::DO:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;////////////////////////////////////////ֻ��һ��B
	case Opcode::WE:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;//�޲�
	case Opcode::dw_LB:
		this->all_quads.push_back(q);
		break;//�޲�
	case Opcode::dw_WH:
		this->all_quads.push_back(q);
		break;;///�޲�
	case Opcode::dw_DO:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;/////////////////////////////////////ֻ��һ��B
	case Opcode::fr_FR:
		this->all_quads.push_back(q);
		break;;///�޲�
	case Opcode::fr_LB:
		this->all_quads.push_back(q);
		break;;///�޲�
	case Opcode::fr_DO:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;/////////////////////////////////////ֻ��һ��B
	case Opcode::fr_FE:
		this->end_quad = q;
		this->end_quad.num = 99999;
		this->end_quad_tag = true;
		break;//�޲�
	case Opcode::Undefined:
		this->all_quads.push_back(q);
		break;//�޲�
	case Opcode::SCANF://(SCANF, _, _, dest)
		Scanf(q.op, q.B, q.C, q.A);
		break;
	case Opcode::PRINTF://(PRINTF, SEM[m], _ , _ )
		Printf(q.op, q.B, q.C, q.A);
		break;

	}
}

int DAG::WhatType(string Str)//����string�ж���ʲô���� 3�������� 1����ϵͳ���ɵ���ʱ���� 2�����û�����ı�ʶ�� 0������
{
	if ((Str[0] <= '9' && Str[0] >= '0') || (Str[0] == '-' && Str[1] >= '0' && Str[1] <= '9'))//���ַ�������
	{
		return 3;
	}
	else if (Str.size() == '1' && Str == "_")//ֻ��һ��"_"��Ϊ��
	{
		return 0;
	}
	else if (Str.find('_') != Str.npos)//�ܲ��ҵ�"_"���û������
	{
		return 2;
	}
	else//ʣ��ľ���ϵͳ���ɵ�
	{
		return 1;
	}
}

int DAG::FindNode(Var v)//����Var������֮ǰ�����õĽ�����Ƿ��н���и�VARֵ�����У����ظýڵ�ı�ţ����򷵻�-1
{

	int i, j;

	for (i = 0; i != this->Node_sequence.size(); i++)
	{
		if ((this->Node_sequence[i].TopMessage.tag == true && this->Node_sequence[i].TopMessage.num == stringToNum(v.name)))
		{
			return this->Node_sequence[i].num;
		}
		else if ((this->Node_sequence[i].TopMessage.tag == false && this->Node_sequence[i].TopMessage.VariateName == v.name))
		{
			return this->Node_sequence[i].num;
		}
		else
		{
			for (j = 0; j != this->Node_sequence[i].nodeRightMessage.size(); j++)
			{
				if ((this->Node_sequence[i].nodeRightMessage[j].tag == true && this->Node_sequence[i].nodeRightMessage[j].num == stringToNum(v.name)))
				{
					return this->Node_sequence[i].num;
				}
				else if ((this->Node_sequence[i].nodeRightMessage[j].tag == false && this->Node_sequence[i].nodeRightMessage[j].VariateName == v.name))
				{
					return this->Node_sequence[i].num;
				}
				else
				{

				}
			}
		}

	}
	return -1;
}


double DAG::stringToNum(const string& str)
{
	istringstream iss(str);
	double num;
	iss >> num;
	return num;
}

int DAG::NewLeafNode(Var v)
{
	this->count++;//��������һ
	DAGTreeNode dagTreeNode;
	NodeRightMessage leaf_node_mess;
	//�򵥵ĸ�ֵ����
	dagTreeNode.num = this->count;
	dagTreeNode.left_Node = NULL;
	dagTreeNode.right_Node = NULL;
	dagTreeNode.roof_num = 0;
	dagTreeNode.opcode = Opcode::Undefined;

	if (WhatType(v.name) == 3)
	{
		//leaf_node_mess.num = stringToNum(v.name);
		leaf_node_mess.num = stoi(v.name);
		leaf_node_mess.VariateName = v.name;
		leaf_node_mess.tag = true;
	}
	else
	{
		leaf_node_mess.num = 0;
		leaf_node_mess.VariateName = v.name;
		leaf_node_mess.tag = false;
	}
	dagTreeNode.TopMessage.num = leaf_node_mess.num;
	dagTreeNode.TopMessage.tag = leaf_node_mess.tag;
	dagTreeNode.TopMessage.VariateName = leaf_node_mess.VariateName;

	//�������
	this->Node_sequence.push_back(dagTreeNode);
	return dagTreeNode.num;
}

void DAG::LinkNode(Opcode op, Var A, int numB, int numC)
{
	this->count++;//��������һ
	DAGTreeNode dagTreeNode;
	dagTreeNode.left_Node = new (DAGTreeNode);
	dagTreeNode.left_Node->left_Node = FindNodeByNum(numB)->left_Node;
	dagTreeNode.left_Node->nodeRightMessage = FindNodeByNum(numB)->nodeRightMessage;
	dagTreeNode.left_Node->num = FindNodeByNum(numB)->num;
	dagTreeNode.left_Node->opcode = FindNodeByNum(numB)->opcode;
	dagTreeNode.left_Node->right_Node = FindNodeByNum(numB)->right_Node;
	dagTreeNode.left_Node->roof_num = FindNodeByNum(numB)->roof_num;
	dagTreeNode.left_Node->TopMessage = FindNodeByNum(numB)->TopMessage;
	dagTreeNode.right_Node = new (DAGTreeNode);
	dagTreeNode.right_Node->left_Node = FindNodeByNum(numC)->left_Node;
	dagTreeNode.right_Node->nodeRightMessage = FindNodeByNum(numC)->nodeRightMessage;
	dagTreeNode.right_Node->num = FindNodeByNum(numC)->num;
	dagTreeNode.right_Node->opcode = FindNodeByNum(numC)->opcode;
	dagTreeNode.right_Node->right_Node = FindNodeByNum(numC)->right_Node;
	dagTreeNode.right_Node->roof_num = FindNodeByNum(numC)->roof_num;
	dagTreeNode.right_Node->TopMessage = FindNodeByNum(numC)->TopMessage;
	dagTreeNode.num = this->count;
	FindNodeByNum(numB)->roof_num = dagTreeNode.num;
	FindNodeByNum(numC)->roof_num = dagTreeNode.num;
	dagTreeNode.roof_num = 0;
	dagTreeNode.opcode = op;
	NodeRightMessage roof_node_mess;
	//�򵥵ĸ�ֵ����
	roof_node_mess.num = 0;
	roof_node_mess.VariateName = A.name;
	roof_node_mess.tag = false;

	dagTreeNode.TopMessage.num = roof_node_mess.num;
	dagTreeNode.TopMessage.tag = roof_node_mess.tag;
	dagTreeNode.TopMessage.VariateName = roof_node_mess.VariateName;


	//�������
	this->Node_sequence.push_back(dagTreeNode);
}


DAGTreeNode* DAG::FindNodeByNum(int num)//���ݽڵ��Ų��ҽ��
{
	int i;
	for (i = 0; i != this->Node_sequence.size(); i++)
	{
		if (this->Node_sequence[i].num == num)
		{
			return &this->Node_sequence[i];
		}
	}
}

DAGTreeNode* DAG::CheckNode(Opcode op, int numB, int numC)//����Ƿ����� ͬ�������ͬ���Ľ��
{
	int i;
	for (i = 0; i != this->Node_sequence.size(); i++)
	{
		if (this->Node_sequence[i].opcode == op && this->Node_sequence[i].left_Node == FindNodeByNum(numB) && this->Node_sequence[i].right_Node == FindNodeByNum(numC))
		{
			return &this->Node_sequence[i];
		}
	}
	return NULL;
}

void DAG::NewRoofNode(Opcode op, Var B, Var C, Var A)//���ɸ��ڵ�
{
	int numB, numC;
	DAGTreeNode* dagTreeNode;
	NodeRightMessage roof_node_mess;
	if (FindNode(B) != -1)
	{
		numB = FindNode(B);
	}
	else
	{
		numB = NewLeafNode(B);
	}
	if (FindNode(C) != -1)
	{
		numC = FindNode(C);
	}
	else
	{
		numC = NewLeafNode(C);
	}
	dagTreeNode = CheckNode(op, numB, numC);
	if (dagTreeNode == NULL)//֮ǰ�޽��
	{
		LinkNode(op, A, numB, numC);
	}
	else
	{
		roof_node_mess.VariateName = A.name;
		roof_node_mess.num = 0;
		roof_node_mess.tag = false;
		dagTreeNode->nodeRightMessage.push_back(roof_node_mess);
		if (WhatType(dagTreeNode->TopMessage.VariateName) < WhatType(A.name))//�û������Ҫǿ��ϵͳ���ɵ�
		{
			dagTreeNode->TopMessage.VariateName = A.name;
		}
	}
}

void DAG::Assign(Opcode op, Var B, Var C, Var A)
{

	int numB;
	DAGTreeNode* dagTreeNode;
	NodeRightMessage assign_node_mess;
	if (FindNode(B) != -1)
	{
		numB = FindNode(B);
	}
	else
	{
		numB = NewLeafNode(B);
	}
	DeleteNodeMessage(A);
	dagTreeNode = FindNodeByNum(numB);
	if (WhatType(dagTreeNode->TopMessage.VariateName) < WhatType(A.name))
	{
		dagTreeNode->nodeRightMessage.push_back(dagTreeNode->TopMessage);
		dagTreeNode->TopMessage.num = 0;
		dagTreeNode->TopMessage.tag = false;
		dagTreeNode->TopMessage.VariateName = A.name;
	}
	else
	{
		assign_node_mess.tag = false;
		assign_node_mess.VariateName = A.name;
		assign_node_mess.num = 0;
		dagTreeNode->nodeRightMessage.push_back(assign_node_mess);
	}
}

void DAG::DeleteNodeMessage(Var A)
{
	int numA;
	DAGTreeNode* dagTreeNode;
	bool tag = false;
	if (FindNode(A) != -1)//֮ǰ�Ľ�����н����Message�к���A��Ҫ����
	{
		numA = FindNode(A);
		dagTreeNode = FindNodeByNum(numA);
		if (dagTreeNode->TopMessage.VariateName == A.name)//��������ϲ�ɾ
		{
			return;
		}
		else
		{
			int i;
			for (i = 0; i != dagTreeNode->nodeRightMessage.size(); i++)
			{
				if (dagTreeNode->nodeRightMessage[i].VariateName == A.name)
				{
					tag = true;
					break;
				}
			}
			if (tag == true)
			{
				dagTreeNode->nodeRightMessage.erase(dagTreeNode->nodeRightMessage.begin() + i);
			}

		}
	}
}

void DAG::GenerateDAG(vector<Quad> quads, BasicBlock b)//����DAGͼ
{
	int i;
	for (i = b.start; i != b.end + 1; i++)
	{
		CreateNode(quads[i]);
	}
	for (int i = 1; i <= this->count; i++)
	{
		if (FindNodeByNum(i)->roof_num == 0)//�ҵ����ڵ�
		{
			Recur_Quads(i);
		}
	}
	sort(this->all_quads.begin(), this->all_quads.end(), comp_quad);
	if (this->end_quad_tag == true)
	{
		this->all_quads.push_back(this->end_quad);
	}

	for (i = 0; i != this->all_quads.size(); i++)
	{
		this->last_quads.push_back(this->all_quads[i]);
	}
	this->all_quads.clear();
	this->Node_sequence.clear();
	this->count = 0;
	this->end_quad_tag = false;
}

void DAG::Recur_Quads(int num_roof)
{
	if (FindNodeByNum(num_roof)->left_Node != NULL && FindNodeByNum(num_roof)->right_Node != NULL)
	{
		DAGTreeNode* dagTreeNode_roof, * dagTreeNode_left, * dagTreeNode_right;
		dagTreeNode_roof = FindNodeByNum(num_roof);
		dagTreeNode_left = FindNodeByNum(dagTreeNode_roof->left_Node->num);
		dagTreeNode_right = FindNodeByNum(dagTreeNode_roof->right_Node->num);
		Quad quad(dagTreeNode_roof->opcode, dagTreeNode_left->TopMessage.VariateName, dagTreeNode_right->TopMessage.VariateName, dagTreeNode_roof->TopMessage.VariateName);
		quad.num = num_roof;
		this->all_quads.push_back(quad);
		Recur_Quads(dagTreeNode_roof->left_Node->num);
		Recur_Quads(dagTreeNode_roof->right_Node->num);
	}
	else if (FindNodeByNum(num_roof)->opcode == Opcode::SCANF)
	{
		Quad quad(Opcode::SCANF, "_", "_", FindNodeByNum(num_roof)->TopMessage.VariateName);
		quad.num = num_roof;
		this->all_quads.push_back(quad);
	}
	else if (FindNodeByNum(num_roof)->opcode == Opcode::PRINTF)
	{
		Quad quad(Opcode::PRINTF, FindNodeByNum(num_roof)->TopMessage.VariateName, "_", "_");
		quad.num = num_roof;
		this->all_quads.push_back(quad);
	}
	else if ((FindNodeByNum(num_roof)->TopMessage.tag = true || WhatType(FindNodeByNum(num_roof)->TopMessage.VariateName) == 2))
	{
		int i;
		for (i = 0; i != FindNodeByNum(num_roof)->nodeRightMessage.size(); i++)
		{
			if (WhatType(FindNodeByNum(num_roof)->nodeRightMessage[i].VariateName) == 2)
			{
				Quad quad(Opcode::Assign, FindNodeByNum(num_roof)->TopMessage.VariateName, "_", FindNodeByNum(num_roof)->nodeRightMessage[i].VariateName);
				quad.num = num_roof;
				this->all_quads.push_back(quad);
			}
		}
	}

	else
	{
		return;
	}
}


void DAG::Scanf(Opcode op, Var B, Var C, Var A)
{
	int numA;
	DAGTreeNode* dagTreeNode;
	NodeRightMessage Scanf_node_mess;
	numA = NewLeafNode(A);
	dagTreeNode = FindNodeByNum(numA);
	dagTreeNode->left_Node = NULL;
	dagTreeNode->right_Node = NULL;
	dagTreeNode->TopMessage.num = 0;
	dagTreeNode->TopMessage.tag = false;
	dagTreeNode->TopMessage.VariateName = A.name;
	dagTreeNode->num = this->count;
	dagTreeNode->roof_num = 0;
	dagTreeNode->opcode = Opcode::SCANF;
}
void DAG::Printf(Opcode op, Var B, Var C, Var A)
{
	int numB;
	DAGTreeNode* dagTreeNode;
	NodeRightMessage Scanf_node_mess;
	numB = NewLeafNode(B);
	dagTreeNode = FindNodeByNum(numB);
	dagTreeNode->left_Node = NULL;
	dagTreeNode->right_Node = NULL;
	dagTreeNode->TopMessage.num = 0;
	dagTreeNode->TopMessage.tag = false;
	dagTreeNode->TopMessage.VariateName = B.name;
	dagTreeNode->num = this->count;
	dagTreeNode->roof_num = 0;
	dagTreeNode->opcode = Opcode::PRINTF;
}