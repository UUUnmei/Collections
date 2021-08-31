#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    MainWindow w;
    w.setWindowTitle("最小生成树");
    w.show();

    return a.exec();
}
