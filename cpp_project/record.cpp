#include "record.h"
#include <stdio.h>
#include <iostream>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QObject>

record::record(int op)
{
    // 如果没有文件，创建文件
    std::ifstream in;
    if(op == 2) filename = GRADE_PATH_2;
    else filename = GRADE_PATH_1;

    in.open(filename,std::ios::in);
    if (!in.is_open()) {
        std::ofstream file(filename, std::fstream::out);
        //if (file) cout << " new file created" << endl;
    }
    num = 0;
}

void record::open_read()
{
    // 这里首先选择的模式是 ：可读模式
    std::ifstream in;
    in.open(filename,std::ios::in);
    if (!in.is_open()) {
        std::cout << "文件不存在" << std::endl;
        exit(1);
    }
    string tmp;
    num = 0;
    while (getline(in, tmp)) {
        int pos = tmp.find(" ");
        name[num] = tmp.substr(0, pos);
        grade[num] = atoi(tmp.substr(pos + 1, tmp.length() - pos).c_str());
        num++;
    }

    in.close();
}

void record::find_position(int score) {
    if (num < 5) { // 如果数量小于5，则该成绩一定能被插入
        //有点类似于插入排序的算法
        int i = num - 1;
        for (; i >= 0; i--) {
            if (grade[i] > score) {
                break;
            }
        }
        pos = ++i;
        num++;
        return;
    }

    // 超过5个
    if (score < grade[4]) { // 该选手的成绩低于排行榜的最低成绩，那么直接退出
        pos = -1;
        return;
    }

    int i = 4;
    for (; i >= 0; i--) {
        if (grade[i] > score) {
            break;
        }
    }
    pos = ++i;
    return;
}

void record::insert(int score)
{
    if (pos == -1) { // 新成绩不在榜单内
        return;
    }

    // 插入排序
    int i = num - 2;
    for (; i >= pos; i--) {
        name[i + 1] = name[i];
        grade[i + 1] = grade[i];
    }

    grade[pos] = score;
}

void record::input_name()
{
    bool ok;
    /*4种回显模式 分别如下：
1、QLineEdit::Normal   输入什么内容就显示什么。这是QLineEdit的默认选项。
2、QLineEdit::NoEcho   不显示任何东西。这可能是适合甚至密码,密码的长度应该是保密的。（没有任何显示）
3、QLineEdit::Password   只要用于密码输入。显示效果根据具体平台相关，一般用“*”替代显示。
4、QLineEdit::PasswordEchoOnEdit  在QLineEdit获取焦点时，正常显示输入的内容。失去焦点时，便以显示密码的方式显示。*/
    // 1 父组件指针   2 对话框标题
    // 3 提示语句     4 输入模式
    // 5 默认字符串   6 接受返回值
    QString text =QInputDialog::getText(NULL,"Input your name","大神玩的也忒6了！\n请留下大神的姓名：",QLineEdit::Normal,"",&ok);

    // 名字给个默认值。。
    name[pos] = "无名氏";
    if(ok && !text.isEmpty()){
        // 将QString 存到string name[6]中
        name[pos] = text.toStdString();
    }
}

void record::write()
{
    std::ofstream out;
    out.open(filename,std::ios::out);
    for (int i = 0; i <num; i++) {
        out<< name[i] <<" "<<grade[i]<<std::endl;
    }

    out.close();
}

void record::operation(int score)
{
    open_read();
    find_position(score);
    if (pos != -1) {
        insert(score);
        input_name();
    }
    record_print();
    write();
}

void record::record_print()
{
//    // 一定需要new出来一个新的，
//    rec_window* re_w = new rec_window;
//    re_w->show();
//    re_w->operation(this);
//    //edit.setText(QString("rec->name[i] << "  " << rec->grade[i]"));
    QString dlgTitle="排行榜";
    QString all;
    QString tmp;
    for(int i=0;i<num;i++){
        tmp = QString("No.")+QString::number(i+1)+QString(":  ")+QString::fromStdString(name[i])+QString("   \t")
                + QString::number(grade[i])+QString("\t")+QString("\n\n");
        all += tmp;
    }
    QString strInfo=all;
    QMessageBox QM;
    QFont font1("宋体", 30, QFont::Bold,true);
//    QM.setFixedHeight(360);
//    QM.setFixedWidth(240);
    QM.setFont(font1);
    QM.QMessageBox::about(NULL, dlgTitle, strInfo);

}

record::~record()
{

}
