#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <string>
#include "player.h"
#include <fstream>
using std::string;
class record
{
public:
//    FILE* fp;  // 文件的指针
    string filename;
    int num = 0; // 文件中的信息数量
    int pos = -1; //  新加入的成绩应该所处的位置->这里指下标
    string name[6]; // 用六个便于排序时的处理
    int grade[6];
public:
    record(int);
    void open_read(); // 打开文件的函数 & 读取文件中的信息
    void find_position(int score); // 找到正确的插入位置
    void insert(int score);
    void input_name();
    void record_print();
    void write();
    void operation(int score); // 友好接口
    ~record();
};

#endif // RECORD_H
