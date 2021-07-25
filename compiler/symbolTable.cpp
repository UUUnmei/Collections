#include "symbolTable.h"
#include <fstream>
using namespace std;
ostream& operator << (ostream& out, cattType cT)
{
	switch (cT)
	{
	case cattType::CONSTT:
		out << " CONSTT "; break;
	case cattType::VAR:
		out << " VAR "; break;
	}
	return out;
}
ostream& operator << (ostream& out, constType cT)
{
	switch (cT)
	{
	case constType::DOUBLEC:
		out << " DOUBLEC "; break;
	case constType::INTC:
		out << " INTC "; break;
	}
	return out;
}
bool symbolTable::insert_symbol(string& _a_name, int _ver_type, cattType _var_cat)
{
	string _name = _a_name + "_main_" + to_string(_now_scope.first) + "_" + to_string(_now_scope.second);
	if (main_table.find(_name) == main_table.end())
	{
		mainTableItem temp;
		temp._a_name = _a_name;
		temp._type = _ver_type;
		temp._cat = _var_cat;
		temp._addr_pos = NULLITEM;
		main_table[_name] = temp;
		_a_name = _name;
		return true;
	}
	else
	{
		return false;
	}
}

bool symbolTable::insert_const(int _ver_type, int _var_value, string& _const_name)
{
	string _name = "const" + to_string(_const_count_);
	_const_name = _name;
	_const_count_++;
	if (main_table.find(_name) == main_table.end())
	{
		mainTableItem temp;
		temp._a_name = _name;
		temp._type = _ver_type;
		temp._cat = cattType::CONSTT;
		temp._addr_pos = _const_table_pos_;
		_const_table_pos_++;

		main_table[_name] = temp;

		if (_ver_type == 1)
		{
			constTableItem tempC;
			tempC._const_pos = _const_table_int_pos_;
			_const_table_int_pos_++;
			const_table_int.push_back(_var_value);
			tempC._const_type = constType::INTC;
			const_table.push_back(tempC);
		}

		return true;
	}
	else
	{
		return false;
	}
}



bool symbolTable::insert_const(int _ver_type, double _var_value, string& _const_name)
{
	string _name = "const" + to_string(_const_count_);
	_const_name = _name;
	_const_count_++;
	if (main_table.find(_name) == main_table.end())
	{
		mainTableItem temp;
		temp._a_name = _name;
		temp._type = _ver_type;
		temp._cat = cattType::CONSTT;
		temp._addr_pos = _const_table_pos_;
		_const_table_pos_++;

		main_table[_name] = temp;

		if (_ver_type == 2)
		{
			constTableItem tempC;
			tempC._const_pos = _const_table_double_pos_;
			_const_table_double_pos_++;
			const_table_double.push_back(_var_value);
			tempC._const_type = constType::DOUBLEC;
			const_table.push_back(tempC);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool symbolTable::new_temp(int _ver_type, string& _temp_name)
{
	string _name = "t" + to_string(_temp_count_);
	_temp_count_++;
	_temp_name = _name;
	mainTableItem temp;
	temp._a_name = _name;
	temp._type = _ver_type;
	temp._cat = cattType::VAR;
	temp._addr_pos = NULLITEM;
	main_table[_name] = temp;
	return true;
}

bool symbolTable::delete_temp_symbol(string _name)
{
	auto iter = main_table.find(_name);
	main_table.erase(iter);
	return true;
}


bool symbolTable::serach_symbol(string _var_name, string& _name)
{
	string _temp_var_name = _var_name;
	string __a_name;
	string _funciton_name;
	pair<int, int>_now_scope;
	pair<int, int>_last_scope;
	auto _function_refer_to = main_table.find(_funciton_name);
	var_name_to_info(_var_name, __a_name, _funciton_name, _now_scope.first, _now_scope.second);
	auto _var_refer_to = main_table.find(_temp_var_name);
	while (_var_refer_to == main_table.end())
	{
		_last_scope = _in_function_scope[_now_scope.first][_now_scope.second];
		if (_now_scope == _last_scope)
		{
			return false;
		}
		_now_scope = _last_scope;
		_temp_var_name = __a_name + "_" + _funciton_name + "_" + to_string(_now_scope.first) + "_" + to_string(_now_scope.second);
		_var_refer_to = main_table.find(_temp_var_name);
	}
	_name = _temp_var_name;
	return true;
}

bool symbolTable::serach_symbol(string _var_name, cattType& _var_cat)
{
	string _temp_var_name = _var_name;
	string __a_name;
	string _funciton_name;
	pair<int, int>_now_scope;
	pair<int, int>_last_scope;
	auto _function_refer_to = main_table.find(_funciton_name);
	var_name_to_info(_var_name, __a_name, _funciton_name, _now_scope.first, _now_scope.second);
	auto _var_refer_to = main_table.find(_temp_var_name);
	while (_var_refer_to == main_table.end())
	{
		_last_scope = _in_function_scope[_now_scope.first][_now_scope.second];
		if (_now_scope == _last_scope)
		{
			return false;
		}
		_now_scope = _last_scope;
		_temp_var_name = __a_name + "_" + _funciton_name + "_" + to_string(_now_scope.first) + "_" + to_string(_now_scope.second);
		_var_refer_to = main_table.find(_temp_var_name);
	}
	_var_cat = _var_refer_to->second._cat;
	return true;
}

bool symbolTable::initialization()
{
	_const_table_pos_ = 0;
	_const_table_int_pos_ = 0;
	_const_table_double_pos_ = 0;
	_const_count_ = 0;
	main_table.reserve(15000);
	const_table.reserve(50);
	const_table_int.reserve(50);
	const_table_double.reserve(50);
	return true;
}

bool symbolTable::print_all_table(const string& s)
{
	ofstream outputfile(s);
	if (!outputfile.is_open()) {
		cout << "符号表输出文件打开失败\n";
		exit(1);
	}
	outputfile << "main_table" << endl;
	for (auto&& [a, b] : main_table)
	{
		outputfile << a << " " << b._a_name << " " << b._cat << " " << b._type << " " << b._addr_pos << " " << endl;
	}
	outputfile << "const_table" << endl;
	for (auto&& a : const_table)
	{
		outputfile << a._const_type << " " << a._const_pos << " " << endl;
	}
	outputfile << "const_table_int" << endl;
	for (auto&& a : const_table_int)
	{
		outputfile << a << " " << endl;
	}
	outputfile << "const_table_double" << endl;
	for (auto&& a : const_table_double)
	{
		outputfile << a << " " << endl;
	}
	return true;
}

bool symbolTable::ini_scopes()
{
	_in_function_scope.resize(50);
	for (auto i = _in_function_scope.begin(); i != _in_function_scope.end(); i++)
	{
		i->reserve(50);
	}
	_scope_stack.reserve(50);
	_now_scope = pair<int, int>(0, 0);
	_last_scope = _now_scope;
	_in_function_scope[0].push_back(_now_scope);
	_scope_stack.push_back(_now_scope);
	return true;
}
bool symbolTable::inc_scope(int& level, int& count)
{
	_last_scope = _now_scope;
	_now_scope.first++;
	_in_function_scope[_now_scope.first].push_back(_last_scope);
	_now_scope.second = _in_function_scope[_now_scope.first].size() - 1;
	_scope_stack.push_back(_now_scope);

	level = _now_scope.first;
	count = _now_scope.second;
	return true;
}

bool symbolTable::dec_scope(int& level, int& count)
{
	_scope_stack.pop_back();
	_now_scope = _scope_stack.back();
	_last_scope = _in_function_scope[_now_scope.first][_now_scope.second];

	level = _now_scope.first;
	count = _now_scope.second;
	return true;
}
bool symbolTable::print_all_scope(const string& s)
{
	ofstream outputfile(s);
	if (!outputfile.is_open()) {
		cout << "作用域输出文件打开失败\n";
		exit(1);
	}
	for (unsigned i = 0; i < _in_function_scope.size(); i++)
	{
		for (unsigned j = 0; j < _in_function_scope[i].size(); j++)
		{
			outputfile << i << " " << j << " " << ":";
			outputfile << "<" << _in_function_scope[i][j].first << "," << _in_function_scope[i][j].second << ">" << endl;
		}
	}
	return true;
}
void symbolTable::var_name_to_info(string _var_name, string& _a_name, string& _funciton_name, int& level, int& count)
{
	vector<int> _pos;
	_pos.reserve(3);
	for (unsigned i = 0; i <= _var_name.length() - 1; i++)
	{
		if (_var_name[i] == '_')
			_pos.push_back(i);
	}
	_a_name = _var_name.substr(0, _pos[0]);
	_funciton_name = _var_name.substr(_pos[0] + 1, _pos[1] - _pos[0] - 1);
	level = atoi(_var_name.substr(_pos[1] + 1, _pos[2] - _pos[1]).c_str());
	count = atoi(_var_name.substr(_pos[2] + 1).c_str());
}
//int main()
//{
//	symbolTable a;
//	a.initialization();
//	a.ini_scopes();
//	string b = "doremifaso";
//	a.insert_symbol(b, 1, cattType::VAR);
//	a.insert_symbol(b, 1, cattType::VAR);
//	string name;
//	a.insert_const(1, 2, name);
//	cout << name << endl;
//	a.insert_const(2, 2.3, name);
//	cout << name << endl;
//	a.insert_const(1, 2, name);
//	cout << name << endl;
//	a.insert_const(2, 2.3, name);
//	cout << name << endl;
//	a.insert_const(1, 2, name);
//	cout << name << endl;
//	a.insert_const(2, 2.3, name);
//	cout << name << endl;
//	a.insert_const(1, 2, name);
//	cout << name << endl;
//	a.insert_const(2, 2.3, name);
//	cout << name << endl;
//	string ada1;
//	a.inc_scope();
//	a.dec_scope();
//	a.serach_symbol("doremifaso_main_1_0", ada1);
//	cout << ada1;
//	string  ada;
//	a.new_temp(2, ada);
//	a.new_temp(1, ada);
//	cout << ada1;
//	a.print_all_table();
//	a.print_all_scope();
//}