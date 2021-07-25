#pragma once
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<utility>
#define INTLENGTH 4 //4 byte
#define DOUBLELENGTH 8 //8 byte
#define NULLITEM -1 //����û��ָ��

using std::unordered_map;
using std::pair;
using std::string;
using std::vector;

#pragma region

enum class cattType//��������CAT
{
	CONSTT,//����
	VAR//����
};



enum class constType//��������
{
	INTC,//������
	DOUBLEC//���㳣��
};

#pragma endregion

#pragma region
struct mainTableItem {
	string _a_name;//ʵ������
	int _type;//���ͱ����ָ���ݶ�1����2����
	cattType _cat;//�������ͣ������ж����ĸ���
	int _addr_pos;//const��

	mainTableItem() = default;
	mainTableItem(string a_name, int type, cattType cat, int addr_pos)
		:_a_name(a_name), _type(type), _cat(cat), _addr_pos(addr_pos) {};

};//�������


struct constTableItem
{
	constType _const_type;
	int _const_pos;
};//���������



#pragma endregion


class symbolTable
{
private:



	int _const_table_pos_;//���������һ������λ��
	int _const_table_int_pos_;//���������ͱ����һ������λ��
	int _const_table_double_pos_;//���������ͱ����һ������λ��
	int _const_count_;//����������
	int _temp_count_;//��ʱ����������


	unordered_map<string, mainTableItem> main_table;//���ű�����
	vector<constTableItem> const_table;//������
	vector<int> const_table_int;//�����������洢λ��
	vector<double> const_table_double;//�����������洢λ��
	vector<vector<pair<int, int>>>_in_function_scope; //�������
	vector<pair<int, int>> _scope_stack;//������ջ
	pair<int, int>_last_scope;//��һ��������
	pair<int, int>_now_scope;//��ǰ������
public:
	bool insert_symbol(string& _a_name, int _ver_type, cattType _var_cat);//�������
	bool insert_const(int _ver_type, int _var_value, string& _const_name);//������������
	bool insert_const(int _ver_type, double _var_value, string& _const_name);//���븡�㳣��
	bool new_temp(int _ver_type, string& _temp_name);//�½���ʱ����
	bool delete_temp_symbol(string _name);//ɾ������
	bool serach_symbol(string _a_name, string& _name);//_a_name to _name		�øĺ�����ֲ顣�����������ص�Ҳ��һ���ĺ������
	bool serach_symbol(string _name, cattType& _var_cat);///��ѯ��������
	bool inc_scope(int& level, int& count);//��������
	bool dec_scope(int& level, int& count);//��������
	bool initialization();
	bool ini_scopes();//��ʼ��������
	void var_name_to_info(string _var_name, string& _a_name, string& _funciton_name, int& level, int& count);
	bool print_all_table(const string& s);
	bool print_all_scope(const string& s);//���

};

