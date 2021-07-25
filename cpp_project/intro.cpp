#include "intro.h"
#include "config.h"
#include "ui_intro.h"  // 不清楚这东西是什么情况， ui要用
// 为 游戏说明 窗口手动添加 .ui 文件
// 另外，intro.ui 文件中的 objectName 要改为类名 即 Intro
Intro::Intro(QWidget *parent) : QMainWindow(parent), ui(new Ui::Intro)
{
    ui->setupUi(this);


    setFixedSize(640, 480);
    // 设置标题
    setWindowTitle("PlaneWar游戏说明");
    // 设置图标
    setWindowIcon(QIcon(GAME_ICO_PATH));
}


void Intro::closeEvent(QCloseEvent *){
    emit ExitWin();
}
