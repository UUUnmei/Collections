#ifndef BULLET_H
#define BULLET_H
#include "config.h"
#include <QPixmap>


class Bullet
{
public:
    Bullet();
    // 更新子弹坐标
    void updatePosition();

    // 子弹贴图
    QPixmap m_bullet;
    // 飞机左上角坐标
    int m_x;
    int m_y;
    // 子弹移动速度
    int m_speed;
    // 是否闲置
    bool m_free;
    // 碰撞体积
    QRect m_rect;


    // 子弹方向
    // 设定时考虑一般的xOy坐标系方向即可，再转成qt中的坐标
    QPair<int, int> m_direction;

    void adjust();

    static int m_x_bound;
    static int m_y_bound;

};

#endif // BULLET_H
