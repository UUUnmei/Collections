#ifndef INTRO_H
#define INTRO_H

#include <QMainWindow>
#include <QCloseEvent>

// 为 游戏说明 窗口手动添加 .ui 文件
namespace Ui { class Intro; }


class Intro : public QMainWindow
{
    Q_OBJECT
public:
    explicit Intro(QWidget *parent = nullptr);


    void closeEvent(QCloseEvent *);
signals:
    void ExitWin();


// 为 游戏说明 窗口手动添加 .ui 文件
private:
    Ui::Intro *ui;

};

#endif // INTRO_H
