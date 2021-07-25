#include "mainwindow.h"
#include "config.h"
#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 注册二进制资源文件
    // .qrc 和 .rcc 取其一即可
    //QResource::registerResource(GAME_RES_PATH);
    MainWindow w;
    w.show();
    return a.exec();
}
