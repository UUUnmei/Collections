#include "IR.h"

#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

int Quad::cnt = 0;

bool comp_quad(const Quad& a, const Quad& b)
{
	return a.num < b.num;
}
static const char* OPCODE_NAME[] = { 
	"Add",          // +
	"Sub",          // -
	"Mul",          // *
	"Div",          // /
	"Mod",          // %
	"Inc",          // ++
	"Dec",          // --

	"LT",           // <
	"GT",           // >
	"LE",           // <=
	"GE",           // >=
	"EQ",           // ==
	"NE",           // !=

	"Assign",       // =

	"And",          // &
	"Or",           // |
	"Inv",          // ~
	"Xor",          // ^


	"IF",         //  (if, SEM[m], _, _)
	"EL",         //  (el, _, _, _)
	"IE",         //  (ie, _, _, _)

	"WH",         //  (wh, _, _, _)
	"DO",         //  (do, SEM[m], _, _)
	"WE",         //  (we, _, _, _)

	// do-while
	"dw_LB",
	"dw_WH",
	"dw_DO",

	// for
	"fr_FR",
	"fr_LB",
	"fr_DO",
	"fr_FE",

	
	"SCANF",
	"PRINTF",

	"Undefined" 
};
std::ostream& operator<<(std::ostream& os, Opcode op) {
	return os << OPCODE_NAME[(int)op];
}


void IR::debug_all_quads() {
	auto func = [&](const Var& v) {
		if (!v.name.empty()) {
			outputfile << v.name << '\t';
		}
		else {
			outputfile << "_\t";
		}
	};

	for (const auto& q : all_quads) {
		outputfile << q.id << ": ";
		outputfile << q.op << '\t';
		func(q.B);
		func(q.C);
		func(q.A);
		outputfile << q.jmp_to << '\n';
	}
}

void IR::debug_all_blocks() {
	// C++ 17
	for (const auto& [x, y] : all_blocks) {
		outputfile << y.start << ' ' << y.end << '\n';
	}
}

void IR::debug_all_quads_with_active() {
	auto p = [&](bool alive) {
		outputfile << (alive ? "YES\t" : "NO\t");
	};
	auto func = [&](const Var& v) {
		if (!v.name.empty()) {
			outputfile << v.name << '\t';

		}
		else {
			outputfile << "_\t";
		}
		p(v.alive);
	};
	for (const auto& q : all_quads) {
		outputfile << q.id << ": ";
		outputfile << q.op << '\t';

		func(q.B);
		func(q.C);
		func(q.A);
		outputfile << q.jmp_to << '\n';
	}
}


void IR::debug() {
	outputfile << "Quads:\n";
	//debug_all_quads();
	debug_all_quads_with_active();
	outputfile << "Blocks:\n";
	debug_all_blocks();

	outputfile.close();
}



bool IR::next_block(BasicBlock &b) {
	if (cur_block == all_blocks.end())
		return false;
	b = cur_block->second;
	cur_block++;
	return true;
}

void IR::exec_jmp_target() {

	vector<int> stk;
	for (int i = 0; i < all_quads.size(); ++i) {
		switch (all_quads[i].op) {
		case Opcode::IF:
		case Opcode::WH:
		case Opcode::DO:
		case Opcode::dw_LB:
		case Opcode::fr_LB:
		case Opcode::fr_DO:
			stk.push_back(i);
			break;
		case Opcode::EL:
			all_quads[stk.back()].jmp_to = i + 1;
			stk.pop_back();
			stk.push_back(i);
			break;
		case Opcode::IE:
			all_quads[stk.back()].jmp_to = i + 1;
			stk.pop_back();
			break;
		case Opcode::WE:
		case Opcode::fr_FE:
			all_quads[stk.back()].jmp_to = i + 1;
			stk.pop_back();
			all_quads[i].jmp_to = stk.back();
			stk.pop_back();
			break;
		case Opcode::dw_DO:
			all_quads[i].jmp_to = stk.back();
			stk.pop_back();
			break;
		default:
			break;
		}
	}
}


int IR::get_cur_quad_position()
{
	return all_quads.size() - 1;
}

void IR::move_backward(int b, int e)
{
	for (int i = b; i <= e; i++)
		all_quads.push_back(all_quads[i]);
	all_quads.erase(all_quads.begin() + b, all_quads.begin() + e + 1);

	// 调整序号
	for (auto p = all_quads.begin() + b; p != all_quads.end(); ++p)
		p->id = b++;
}

void IR::divide_blocks() {
	if(all_quads.rbegin()->op != Opcode::Undefined)
		add_quad(Opcode::Undefined);

	// 处理四元式中跳转的对应关系，
	exec_jmp_target();


	std::set<int> leader;  //基本块入口
	leader.insert(0);
	all_blocks.insert({ 0, BasicBlock(0, -1) });
	// 求所有基本块入口
	for (int i = 0; i < all_quads.size(); ++i) {
		if (all_quads[i].jmp_to != -1) {
			leader.insert(i + 1);
			leader.insert(all_quads[i].jmp_to);
			if (all_blocks.find(i + 1) == all_blocks.end())
				all_blocks.insert({ i + 1, BasicBlock(i + 1, -1) });
			if (all_blocks.find(all_quads[i].jmp_to) == all_blocks.end())
				all_blocks.insert({ all_quads[i].jmp_to, BasicBlock(all_quads[i].jmp_to, -1) });
		}
		else if (all_quads[i].op == Opcode::dw_WH || all_quads[i].op == Opcode::fr_FR) {
			// do{}while() 不在一个基本块， 
			leader.insert(i);
			if (all_blocks.find(i) == all_blocks.end())
				all_blocks.insert({ i, BasicBlock(i, -1) });
		}
	}
	if (leader.size() == 1) {
		all_blocks.begin()->second.end = all_quads.size() - 1;

	}
	else {
		// 填基本块出口
		for (auto it = all_blocks.begin(); std::next(it) != all_blocks.end(); ++it)
			it->second.end = std::next(it)->second.start - 1;
		all_blocks.rbegin()->second.end = all_blocks.rbegin()->second.start;
	}
	

}

vector<Quad> IR::get_all_quads()
{
	return all_quads;
}

void IR::cal_active_info(BasicBlock &b) {
	std::unordered_set<string> identifier, temp;
	std::unordered_map<string, bool> active_table;
	auto func = [&identifier, &temp, &active_table](Var& v, bool new_active_info) {
		string& s = v.name;
		if (s.empty())
			return;
		if (s.find('_') == string::npos) {
			// 临时变量。 按设计只有临时变量的名不含下划线
			if (temp.find(s) == temp.end()) {
				temp.insert(s);
				v.alive = false;
			}
			else {
				v.alive = active_table[s];
			}
		}
		else {
			// 用户定义标识符
			if (identifier.find(s) == identifier.end()) {
				identifier.insert(s);
				v.alive = true;
			}
			else {
				v.alive = active_table[s];
			}
		}
		active_table[s] = new_active_info;
	};
	// 逆序处理活跃信息
	for (int i = b.end; i >= b.start; --i) {
		auto& q = all_quads[i];
		func(q.B, true);
		func(q.C, true);
		func(q.A, false);
	}
}


void IR::cal_active_info_all() {
	BasicBlock b;
	cur_block = all_blocks.begin();
	while (next_block(b)) {
		cal_active_info(b);
	}
}