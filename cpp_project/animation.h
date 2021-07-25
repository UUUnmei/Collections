#ifndef ANIMATION_H
#define ANIMATION_H

#include <QLabel>
#include <QMovie>

class Animation
{
public:
    QLabel *label;
    QMovie *movie;
    bool running = false;
    QWidget *current_widget;


    Animation(QWidget *, const QString&);
    ~Animation();

    void start();
    void stop();
    void setDoublePlayerPosition(); // 双人游戏时的位置，主要是双人游戏窗口变宽了，
};


#endif // ANIMATION_H
