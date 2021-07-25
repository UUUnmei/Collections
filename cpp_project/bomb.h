#ifndef BOMB_H
#define BOMB_H
#include "config.h"
#include <QPixmap>
#include <QVector>

class Bomb
{
public:
    Bomb();

    //播放图片下标、播放间隔
    void updateInfo();

public:

    //爆炸资源数组
    QVector<QPixmap> m_pixArr;

    //爆炸位置
    int m_X;
    int m_Y;

    //爆炸状态
    bool m_Free;

    //爆炸切图时间间隔
    int m_Recorder;

    //爆炸时加载的图片下标
    int m_Index;
};

#endif // BOMB_H
