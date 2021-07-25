#include <set>
#include<fstream>
#include"targetcode.h"

int p;
int j = 1;
int n;

registers reg("AX");
struct Var RDL = { "",false };
stack<int> stk;
stack<string> lb_stk;
Opcode last_cmp_op = Opcode::Undefined;
string get_label() {
	static int cnt = 0;
	return "L" + to_string(cnt++);
}
Opcode get_rev_op(Opcode op) {
	switch (op)
	{
	case Opcode::LT:
		return Opcode::GE;
	case Opcode::GT:
		return Opcode::LE;
	case Opcode::LE:
		return Opcode::GT;
	case Opcode::GE:
		return Opcode::LT;
	case Opcode::EQ:
		return Opcode::NE;
	case Opcode::NE:
		return Opcode::EQ;
	
	default:
		return Opcode::Undefined;
	}
}

string get_conditional_jmp(Opcode op) {
	switch (op)
	{
	case Opcode::LT:
		return "JL";
	case Opcode::GT:
		return "JG";
	case Opcode::LE:
		return "JLE";
	case Opcode::GE:
		return "JGE";
	case Opcode::EQ:
		return "JE";
	case Opcode::NE:
		return "JNE";

	default:
		return "";
	}
}

bool is_number(const string& s) {
	if (s[0] == '-' || s[0] == '+' || (s[0] >= '0' && s[0] <= '9'))
		return true;
	return false;
}


void targetcode::input(vector<Quad> q)
{
	j = 1;
	n = q.size();
	qua.resize(n);
	for (int i = 0; i < n; ++i)
		qua[i] = q[i];
}

void targetcode::deal()
{
	cal_offset();

	for (int a = 0; a < n; a++)
	{
		if (int(qua[a].op) == 0)//	add
		{

			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV",  qua[a].B.name, reg.name };
				}
				data[j++] = { "ADD", reg.name, qua[a].C.name };
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV",  qua[a].C.name, reg.name };
				}
				data[j++] = { "ADD", reg.name, qua[a].B.name };
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV", RDL.name, reg.name };//cout << "ST " << reg.name << "," << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << "," << qua.B.name << endl;
				data[j++] = { "ADD", reg.name, qua[a].C.name };//cout << "ADD " << reg.name << "," << qua.C.name << endl;
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 1) //sub
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name  };
				}
				data[j++] = { "SUB", reg.name, qua[a].C.name };
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			//else if (reg.isinReg(qua[a].C.name))
			//{
			//	if (qua[a].C.alive)
			//	{
			//		data[j++] = { "MOV", qua[a].C.name, reg.name  };//cout << "ST" << reg.name << "," << qua.C.name << endl;
			//	}
			//	//data[j++] = { "ST", reg.name, qua[a].C.name };//cout << "ST " << reg.name << "," << qua.C.name << endl;
			//	data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << "," << qua.B.name << endl;
			//	data[j++] = { "SUB", reg.name, qua[a].C.name };//cout << "SUB " << reg.name << "," << qua.C.name << endl;
			//	reg.clearpush(qua[a].A.name);
			//	RDL = qua[a].A;
			//}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",  RDL.name , reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				data[j++] = { "SUB", reg.name, qua[a].C.name }; //cout << "SUB " << reg.name << qua.C.name << endl;
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}

		}

		else if (int(qua[a].op) == 2) //mul
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name  };
				}
				//data[j++] = { "MUL", reg.name, qua[a].C.name }; //cout << "MUL " << reg.name << "," << qua.C.name << endl;
				if (is_number(qua[a].C.name)) {
					data[j++] = { "MOV", "BX", qua[a].C.name };
					data[j++] = { "IMUL", "", "BX" };
				}
				else {
					data[j++] = { "IMUL", "", qua[a].C.name };
				}
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV", qua[a].C.name, reg.name };
				}
				//data[j++] = { "MUL", reg.name, qua[a].B.name }; //cout << "MUL " << reg.name << "," << qua.B.name << endl;
				if (is_number(qua[a].B.name)) {
					data[j++] = { "MOV", "BX", qua[a].B.name };
					data[j++] = { "IMUL", "", "BX" };
				}
				else {
					data[j++] = { "IMUL", "", qua[a].B.name };
				}
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV", RDL.name, reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "MUL", reg.name, qua[a].C.name }; //cout << "MUL " << reg.name << qua.C.name << endl;
				if (is_number(qua[a].C.name)) {
					data[j++] = { "MOV", "BX", qua[a].C.name };
					data[j++] = { "IMUL", "", "BX" };
				}
				else {
					data[j++] = { "IMUL", "", qua[a].C.name };
				}

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 3) //div
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV",qua[a].B.name, reg.name  };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "DIV", reg.name, qua[a].C.name }; //cout << "DIV " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CWD", "", "" };
				if (is_number(qua[a].C.name)) {
					data[j++] = { "MOV", "BX", qua[a].C.name };
					data[j++] = { "IDIV", "", "BX" };
				}
				else {
					data[j++] = { "IDIV", "", qua[a].C.name };
				}
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			//else if (reg.isinReg(qua[a].C.name))
			//{
			//	if (qua[a].C.alive)
			//	{
			//		data[j++] = { "MOV", qua[a].C.name, reg.name  };//cout << "ST" << reg.name << "," << qua.C.name << endl;
			//	}
			//	//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
			//	data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
			//	//data[j++] = { "DIV", reg.name, qua[a].C.name }; //cout << "DIV " << reg.name << "," << qua.C.name << endl;
			//	data[j++] = { "CWD", "", "" };
			//	if (is_number(qua[a].C.name)) {
			//		data[j++] = { "MOV", "BX", qua[a].C.name };
			//		data[j++] = { "IDIV", "", "BX" };
			//	}
			//	else {
			//		data[j++] = { "IDIV", "", qua[a].C.name };
			//	}
			//	reg.clearpush(qua[a].A.name);
			//	RDL = qua[a].A;
			//}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name  }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "DIV", reg.name, qua[a].C.name }; //cout << "DIV " << reg.name << qua.C.name << endl;
				data[j++] = { "CWD", "", "" };
				if (is_number(qua[a].C.name)) {
					data[j++] = { "MOV", "BX", qua[a].C.name };
					data[j++] = { "IDIV", "", "BX" };
				}
				else {
					data[j++] = { "IDIV", "", qua[a].C.name };
				}
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}

		}

		else if (int(qua[a].op) == 4) //mod
		{
			{
				if (reg.isinReg(qua[a].B.name))
				{
					if (qua[a].B.alive)
					{
						data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
					}
					//data[j++] = { "MOD", reg.name, qua[a].C.name }; //cout << "MOD " << reg.name << "," << qua.C.name << endl;
					data[j++] = { "CWD", "", "" };
					if (is_number(qua[a].C.name)) {
						data[j++] = { "MOV", "BX", qua[a].C.name };
						data[j++] = { "IDIV", "", "BX" };
					}
					else {
						data[j++] = { "IDIV", "", qua[a].C.name };
					}
					data[j++] = { "MOV", "AX", "DX" };
					reg.clearpush(qua[a].A.name);
					RDL = qua[a].A;
				}
				//else if (reg.isinReg(qua[a].C.name))
				//{
				//	if (qua[a].C.alive)
				//	{
				//		data[j++] = { "MOV", qua[a].C.name, reg.name};//cout << "ST" << reg.name << "," << qua.C.name << endl;
				//	}
				//	//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				//	data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//	//data[j++] = { "MOD", reg.name, qua[a].C.name }; //cout << "MOD " << reg.name << "," << qua.C.name << endl;
				//	data[j++] = { "CWD", "", "" };
				//	if (is_number(qua[a].C.name)) {
				//		data[j++] = { "MOV", "BX", qua[a].C.name };
				//		data[j++] = { "IDIV", "", "BX" };
				//	}
				//	else {
				//		data[j++] = { "IDIV", "", qua[a].C.name };
				//	}
				//	data[j++] = { "MOV", "AX", "DX" };
				//	reg.clearpush(qua[a].A.name);
				//	RDL = qua[a].A;
				//}
				else
				{
					if (RDL.alive)
					{
						data[j++] = { "MOV", RDL.name , reg.name}; //cout << "ST " << reg.name << RDL.name << endl;
					}
					data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
					//data[j++] = { "MOD", reg.name, qua[a].C.name }; //cout << "MOD " << reg.name << qua.C.name << endl;
					data[j++] = { "CWD", "", "" };
					if (is_number(qua[a].C.name)) {
						data[j++] = { "MOV", "BX", qua[a].C.name };
						data[j++] = { "IDIV", "", "BX" };
					}
					else {
						data[j++] = { "IDIV", "", qua[a].C.name };
					}
					data[j++] = { "MOV", "AX", "DX" };
					reg.clearpush(qua[a].A.name);
					RDL = qua[a].A;
				}
			}
		}

		else if (int(qua[a].op) == 7) //lt<
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "LT", reg.name, qua[a].C.name }; //cout << "LT " << reg.name << "," << qua.C.name << endl;
				
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LT;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV",qua[a].C.name, reg.name  };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "LT", reg.name, qua[a].C.name }; //cout << "LT " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LT;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name , reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "LT", reg.name, qua[a].C.name }; //cout << "LT " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LT;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 8)  //gt>
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "GT", reg.name, qua[a].C.name }; //cout << "GT " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GT;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV",  qua[a].C.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "GT", reg.name, qua[a].C.name }; //cout << "GT " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GT;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV", RDL.name, reg.name}; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "GT", reg.name, qua[a].C.name }; //cout << "GT " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GT;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 9)  //le<=
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "LE", reg.name, qua[a].C.name }; //cout << "LE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LE;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV",  qua[a].C.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "LE", reg.name, qua[a].C.name }; //cout << "LE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LE;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV", RDL.name, reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "LE", reg.name, qua[a].C.name }; //cout << "LE " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::LE;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 10) //ge>=
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "GE", reg.name, qua[a].C.name }; //cout << "GE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GE;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV", qua[a].C.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "GE", reg.name, qua[a].C.name }; //cout << "GE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GE;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV", RDL.name, reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "GE", reg.name, qua[a].C.name }; //cout << "GE " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::GE;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 11)  //eq==
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "EQ", reg.name, qua[a].C.name }; //cout << "EQ " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::EQ;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV", qua[a].C.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "EQ", reg.name, qua[a].C.name }; //cout << "EQ " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::EQ;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "EQ", reg.name, qua[a].C.name }; //cout << "EQ " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::EQ;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 12)  //ne!=
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "NE", reg.name, qua[a].C.name };// cout << "NE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::NE;
				
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else if (reg.isinReg(qua[a].C.name))
			{
				if (qua[a].C.alive)
				{
					data[j++] = { "MOV", qua[a].C.name, reg.name };//cout << "ST" << reg.name << "," << qua.C.name << endl;
				}
				//data[j++] = { "ST", reg.name, qua[a].C.name }; //cout << "ST " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << "," << qua.B.name << endl;
				//data[j++] = { "NE", reg.name, qua[a].C.name }; //cout << "NE " << reg.name << "," << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::NE;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name }; //cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name }; //cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "NE", reg.name, qua[a].C.name }; //cout << "NE " << reg.name << qua.C.name << endl;
				data[j++] = { "CMP", reg.name, qua[a].C.name };
				last_cmp_op = Opcode::NE;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 13)  //assign=优化后理论上不会出现
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };
				}
				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}

			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name };//cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << qua.B.name << endl;

				reg.clearpush(qua[a].A.name);
				RDL = qua[a].A;
			}
		}

		else if (int(qua[a].op) == 18)//IF
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };
				}
				//data[j++] = { "FJ", reg.name, "" };
				data[j++] = { get_conditional_jmp(get_rev_op(last_cmp_op)), "", "" };

				reg.Rvalue.clear();
				stk.push(j - 1);
				RDL = { "",false };
			}
			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name };//cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "FJ", reg.name, "" };
				data[j++] = { get_conditional_jmp(get_rev_op(last_cmp_op)), "", "" };
				stk.push(j - 1);
				reg.Rvalue.clear();
				RDL = { "",false };
			}
		}

		else if (int(qua[a].op) == 19) //el
		{
			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			p = stk.top();
			stk.pop();

			//data[p].c = to_string(j + 1);
			data[j++] = { "JMP","_","" };
			//stk.push(j - 1);
			
			string t = get_label();
			data[p].c = t;
			data[j++] = { t + ":","_","_" };
			stk.push(j - 2);
			reg.Rvalue.clear();
			RDL = { "",false };
		}

		else if (int(qua[a].op) == 20) //ie
		{
			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			p = stk.top();
			stk.pop();
			//data[p].c = to_string(j);
			string t = get_label();
			data[p].c = t;
			data[j++] = { t + ":","_","_" };
			reg.Rvalue.clear();
			RDL = { "",false };
		}
		else if (int(qua[a].op) == 21 || qua[a].op == Opcode::fr_LB || qua[a].op == Opcode::dw_LB) //wh
		{
			//stk.push(j);

			string t = get_label();
			data[j++] = { t + ":","_","_" };
			lb_stk.push(t);
		}

		else if (int(qua[a].op) == 22 || qua[a].op == Opcode::fr_DO) //do
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };
				}
				//data[j++] = { "FJ", reg.name, "" };
				data[j++] = { get_conditional_jmp(get_rev_op(last_cmp_op)), "", "" };
				reg.Rvalue.clear();
				stk.push(j - 1);
				RDL = { "",false };
			}

			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name };//cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << qua.B.name << endl;
				//data[j++] = { "FJ", reg.name, "" };
				data[j++] = { get_conditional_jmp(get_rev_op(last_cmp_op)), "", "" };
				stk.push(j - 1);
				reg.Rvalue.clear();
				RDL = { "",false };
			}
		}

		else if (int(qua[a].op) == 23 || qua[a].op == Opcode::fr_FE) //we
		{
			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			reg.Rvalue.clear();
			RDL = { "",false };
			//p = stk.top();
			//stk.pop();
			//data[p].c = to_string(j + 1);
			//p = stk.top();
			//stk.pop();
			//data[j++] = { "JMP","_",to_string(p) };
			
			data[j++] = { "JMP","_",lb_stk.top() };
			lb_stk.pop();
			p = stk.top();
			stk.pop();
			string t = get_label();
			data[p].c = t;
			data[j++] = { t + ":","_","_" };

			reg.Rvalue.clear();
			RDL = { "",false };
		}
		else if (qua[a].op == Opcode::SCANF) {

			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			//data[j++] = { "SCANF","_","_" };
			data[j++] = { "CALL","scanf","_" };
			//data[j++] = { "CALL","nextRow","_" };
			reg.clearpush(qua[a].A.name);
			RDL = qua[a].A;
		}
		else if (qua[a].op == Opcode::PRINTF) {
			//data[j++] = { "PRINTF","_","_" };
			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			data[j++] = { "MOV", reg.name, qua[a].B.name };
			data[j++] = { "CALL","printf","_" };
			data[j++] = { "CALL","nextRow","_" };
			reg.Rvalue.clear();
			RDL = { "",false };
		}
		else if (qua[a].op == Opcode::fr_FR || qua[a].op == Opcode::dw_WH) {
			if (RDL.name != "" && RDL.alive)
			{
				data[j++] = { "MOV",RDL.name, reg.name };
			}
			reg.Rvalue.clear();
			RDL = { "",false };
		}
		else if (int(qua[a].op) == 26) //DW_DO SEM
		{
			if (reg.isinReg(qua[a].B.name))
			{
				if (qua[a].B.alive)
				{
					data[j++] = { "MOV", qua[a].B.name, reg.name };
				}
				//p = stk.top();
				//stk.pop();
				//data[j++] = { "TJ", reg.name, to_string(p) };

				//data[j++] = { "TJ", reg.name,lb_stk.top() };
				data[j++] = { get_conditional_jmp(last_cmp_op), "", lb_stk.top() };
				lb_stk.pop();

				reg.Rvalue.clear();
				RDL = { "",false };
			}

			else
			{
				if (RDL.alive)
				{
					data[j++] = { "MOV",RDL.name, reg.name };//cout << "ST " << reg.name << RDL.name << endl;
				}
				data[j++] = { "MOV", reg.name, qua[a].B.name };//cout << "MOV " << reg.name << qua.B.name << endl;
				//p = stk.top();
				//stk.pop();
				//data[j++] = { "TJ", reg.name, to_string(p) };

				//data[j++] = { "TJ", reg.name,lb_stk.top() };
				data[j++] = { get_conditional_jmp(last_cmp_op), "", lb_stk.top() };
				lb_stk.pop();

				reg.Rvalue.clear();
				RDL = { "",false };
			}

		}
		else
		{

			//cout << "四元式为空" << endl;

		}
	}
}



bool is_useful(const string &s) {
	if (s.empty() || s == "_")
		return false;
	return true;
}
//
//void targetcode::print_all_variables() {
//	set<string> s;
//	for (int i = 0; i < n; ++i) {
//		if (!is_number(qua[i].B.name) && is_useful(qua[i].B.name))
//			s.insert(qua[i].B.name);
//		if (!is_number(qua[i].C.name) && is_useful(qua[i].C.name))
//			s.insert(qua[i].C.name);
//		if (!is_number(qua[i].A.name) && is_useful(qua[i].A.name))
//			s.insert(qua[i].A.name);
//	}
//	outputfile << "READ_BUFFER\tdw 0\n";
//	for (auto it = s.begin(); it != s.end(); ++it) {
//		outputfile << *it << '\t' << "dw 0\n";
//	}
//}

static const string io_base = R"(
;10进制输出     
; 递归
; 相当于
; void show(int x){
;     if(!x)
;         return;
;     show(x / 10);
;     cout << x % 10;
; }   
printf_ PROC
        test ax, ax
        jz non
        push ax
        push dx 
        xor dx, dx      ; 这里我用的双字(dxax / bx)除以字，但实际上只需字除以字，须将dx置零
        div bx
        call printf_
        add dl, 30h
        mov ah, 02h
        int 21h
        pop dx
        pop ax
non:    ret
printf_ ENDP

;10进制输出ax
; 范围 -32768 ~ 32767
; 准备
printf PROC
        push ax
        test ax, ax
        jz nn                ; 0要特判
        jns poss
        neg ax
        push ax
        mov dl, '-' 
        mov ah, 02h
        int 21h
        pop ax
poss:   mov bx, 10
        xor dx, dx
        call printf_
        jmp gg
nn:     mov dl, 30h
        mov ah, 02h
        int 21h
gg:     pop ax
        ret 
printf ENDP



;换行，不需要参数
nextRow PROC
    push ax
    mov dl, 0ah
    mov ah, 02h
    int 21h
    pop ax
    ret
nextRow ENDP


; 输入到ax
; 不过没啥错误处理，输入不合法也不管的，，
; 不过反而有一些有意思的现象， -123- 成了123  123-4成了-1234
scanf PROC
        ; 读双字节数
        mov cx, 6               ; 考虑数据范围-32768~32767，最多就6个字符
        mov bx, 1
        lea si, READ_BUFFER             ; !! 在数据段特别准备一个dw READ_BUFFER吧，感觉没啥办法
		mov word ptr [si], 0			; ！！一定要清0，不然多次输入就不对了
again:  mov ah, 01h
        int 21h
        cmp al, 0Dh     ; 0D 'enter'      
        je FINISH  
        cmp al, 2Dh     ; 2D '-'
        jne POS         
        neg bx
        jmp again
POS:    sub al, 30h
        mov dx, [si]     ; 存一份
        shl word ptr [si], 1       ; 8x
        shl word ptr [si], 1
        shl word ptr [si], 1
        shl dx, 1       ; 2x
        add [si], dx    
        cbw
        add [si], ax 
        loop again

FINISH: mov ax, [si]
        imul bx    

        ret    
scanf ENDP)";

void targetcode::print_code_head(){
	//cout << "HEAD!!\n";
	outputfile << R"(
SSEG SEGMENT STACK
SKTOP	db 80h dup(0)
SSEG ENDS

DSEG SEGMENT
READ_BUFFER dw 0)" << '\n';
	//print_all_variables();

	outputfile << R"(
DSEG ENDS

CSEG SEGMENT
		ASSUME CS : CSEG, DS : DSEG, SS : SSEG)" << '\n';

	outputfile << io_base << '\n';

	outputfile << R"(
MAIN:   MOV AX, DSEG
		MOV DS, AX
		MOV AX, SSEG
		MOV SS, AX
		MOV SP, LENGTH SKTOP)" << '\n';
}
void targetcode::print_code_tail() {
	//cout << "TAIL!!\n";
	outputfile << R"(		
		MOV AX, 4C00h
		INT 21h
CSEG ENDS
		END MAIN)" << '\n';
}

void targetcode::out(const string& s)
{
	outputfile.open(s);
	if (!outputfile.is_open()) {
		cout << "汇编输出文件打开失败\n";
		exit(1);
	}

	print_code_head();
	for (int a = 1; a < j; a++)
	{
		//cout << a << ": " << data[a].a << " " << data[a].b << "," << data[a].c << endl;
		outputfile << data[a].a << '\t';
		if (is_useful(data[a].b) && is_useful(data[a].c)) {
			outputfile << data[a].b << ",\t" << data[a].c << '\n';
		}
		else if (is_useful(data[a].b)) {
			outputfile << data[a].b << '\n';
		}
		else if (is_useful(data[a].c)) {
			outputfile << data[a].c << '\n';
		}
		else {
			outputfile << '\n';
		}
	}
	print_code_tail();

}

string targetcode::bp_addr(const string& s) {
	if (is_number(s)) return s;

	return "word ptr [BP - " + to_string(offset[s]) + "]";
}
void targetcode::cal_offset() {
	set<string> s;
	for (int i = 0; i < n; ++i) {
		if (!is_number(qua[i].B.name) && is_useful(qua[i].B.name))
			s.insert(qua[i].B.name);
		if (!is_number(qua[i].C.name) && is_useful(qua[i].C.name))
			s.insert(qua[i].C.name);
		if (!is_number(qua[i].A.name) && is_useful(qua[i].A.name))
			s.insert(qua[i].A.name);
	}
	int p = 2;
	for (auto it = s.begin(); it != s.end(); ++it) {
		offset[*it] = p;
		p += 2;
	}
	for (int i = 0; i < n; ++i) {
		if (!is_number(qua[i].B.name) && is_useful(qua[i].B.name))
			qua[i].B.name = bp_addr(qua[i].B.name);
		if (!is_number(qua[i].C.name) && is_useful(qua[i].C.name))
			qua[i].C.name = bp_addr(qua[i].C.name);
		if (!is_number(qua[i].A.name) && is_useful(qua[i].A.name))
			qua[i].A.name = bp_addr(qua[i].A.name);
	}
}