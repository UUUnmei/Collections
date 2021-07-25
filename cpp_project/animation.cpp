#include "animation.h"

Animation::Animation(QWidget *w, const QString &s) : current_widget(w)
{
    // 实际是播放gif，label用来确定位置
    label = new QLabel(w);	//创建label
    label->setGeometry(0, 0, 512, 768);//设置位置和大小
    label->setAlignment(Qt::AlignCenter);//设置居中（动图在label的位置）
    label->setStyleSheet("background:transparent");//叫背景颜色透明（当然动图背景也要选透明的）
    movie = new QMovie(s);   // gif资源文件
    movie->setScaledSize(QSize(800, 800));//设置动图大小
    label->setMovie(movie);// movie 放入
}

Animation::~Animation()
{
    //qDebug() << "~Animation()";
    delete movie;
    delete label;
}

void Animation::start()
{
    running = true;
    movie->start(); // 开始
    label->show();
}

void Animation::stop()
{
    if(!running) return;
    running = false;
    movie->stop();
    label->close();
}

void Animation::setDoublePlayerPosition()
{
    label->setGeometry(0, 0, 700, 768);//设置位置和大小
}
