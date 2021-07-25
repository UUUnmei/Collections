#ifndef BOSS_H
#define BOSS_H
#include "enemy.h"

class Boss : public QObject
{
    Q_OBJECT
public:
    Boss();

    //子弹击打位置
    QRect hit_point1;
    QRect hit_point2;
    QRect hit_point3;

    int Offset1_x;
    int Offset3_x;
    int Offset1_y;
    int Offset3_y;

    void updatePosition();

    void mainShoot();

    void assistShoot();

    // boss贴图
    QPixmap m_pix;
    // boss状态
    bool m_free;
    // boss坐标
    int m_x;
    int m_y;
    //碰撞体积
    QRect m_rect;
    // 主武器发射间隔
    int main_interval;
    // 次武器发射间隔
    int minor_interval;
    // boss移动速度(纵向)
    int m_speedX;
    // boss移动速度(横向)
    int m_speedY;
    // 1号子弹
    Bullet m_bullets1[BULLET_MAX_NUM];
    // 2号子弹
    Bullet m_bullets2[BULLET_MAX_NUM];
    // boss血量
    int boss_heart = BOSS_LIFE;
    // 无敌状态
    bool unrivaled;
    // 可支援状态
    bool support_free;
    // 支援持续时间
    int support_record;
};

#endif // BOSS_H
