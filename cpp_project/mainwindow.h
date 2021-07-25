#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    关于重开游戏大致是这样的：
//    在单人/双人游戏中gameOver后，首先子窗口close，但更像是hide，并未真正退出
//    在剩下的运行时间中
//    在子窗口中拿到指向父窗口的指针，并调用restartGame函数
//    在该函数中根据参数发出相应的信号：
//        id == 1 -> 重启单人游戏
//        id == 2 -> 重启双人游戏
//    而后利用在MainWindow构造函数中准备的connect就可以执行相应的槽函数，也就是让相应的按钮按下
//    就相当于再次开启了单人/双人游戏
//     ！！！ 不过还是有一些困惑，关于close后到底何时结束，，singlegame类中有count、id等相关调试信息，
    void restartGame(int id);


signals:
    //重启单人游戏
    void restart_single();
    //重启双人游戏
    void restart_double();

private slots:
    // 单人游戏按钮
    void on_pushButton_clicked();
    // 双人游戏按钮
    void on_pushButton_2_clicked();
    // 游戏说明按钮
    void on_pushButton_3_clicked();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
