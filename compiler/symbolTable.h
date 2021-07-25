#pragma once
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<utility>
#define INTLENGTH 4 //4 byte
#define DOUBLELENGTH 8 //8 byte
#define NULLITEM -1 //表项没有指向

using std::unordered_map;
using std::pair;
using std::string;
using std::vector;

#pragma region

enum class cattType//变量类型CAT
{
	CONSTT,//常量
	VAR//变量
};



enum class constType//常数类型
{
	INTC,//整常数
	DOUBLEC//浮点常数
};

#pragma endregion

#pragma region
struct mainTableItem {
	string _a_name;//实际名字
	int _type;//类型表表项指针暂定1整数2浮点
	cattType _cat;//变量类型，用以判断填哪个表
	int _addr_pos;//const表

	mainTableItem() = default;
	mainTableItem(string a_name, int type, cattType cat, int addr_pos)
		:_a_name(a_name), _type(type), _cat(cat), _addr_pos(addr_pos) {};

};//主表表项


struct constTableItem
{
	constType _const_type;
	int _const_pos;
};//常数表表项



#pragma endregion


class symbolTable
{
private:



	int _const_table_pos_;//常数表的下一个可填位置
	int _const_table_int_pos_;//常数表整型表的下一个可填位置
	int _const_table_double_pos_;//常数表浮点型表的下一个可填位置
	int _const_count_;//常数计数器
	int _temp_count_;//临时变量计数器


	unordered_map<string, mainTableItem> main_table;//符号表主表
	vector<constTableItem> const_table;//常数表
	vector<int> const_table_int;//常数表整数存储位置
	vector<double> const_table_double;//常数表整数存储位置
	vector<vector<pair<int, int>>>_in_function_scope; //作用域表
	vector<pair<int, int>> _scope_stack;//作用域栈
	pair<int, int>_last_scope;//上一个作用域
	pair<int, int>_now_scope;//当前作用域
public:
	bool insert_symbol(string& _a_name, int _ver_type, cattType _var_cat);//插入变量
	bool insert_const(int _ver_type, int _var_value, string& _const_name);//插入整数常量
	bool insert_const(int _ver_type, double _var_value, string& _const_name);//插入浮点常量
	bool new_temp(int _ver_type, string& _temp_name);//新建临时变量
	bool delete_temp_symbol(string _name);//删除符号
	bool serach_symbol(string _a_name, string& _name);//_a_name to _name		用改后的名字查。。。。，返回的也是一个改后的名字
	bool serach_symbol(string _name, cattType& _var_cat);///查询符号类型
	bool inc_scope(int& level, int& count);//进作用域
	bool dec_scope(int& level, int& count);//出作用域
	bool initialization();
	bool ini_scopes();//初始化作用域
	void var_name_to_info(string _var_name, string& _a_name, string& _funciton_name, int& level, int& count);
	bool print_all_table(const string& s);
	bool print_all_scope(const string& s);//打表

};

