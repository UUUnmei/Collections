#ifndef ENEMY_H
#define ENEMY_H
#include <QPixmap>
#include "bullet.h"
// 结构上类似Bullet类
class Enemy
{
public:
    Enemy();

    // 更新敌机坐标
    void updatePosition();
    // 敌机射击
    void shoot();

    // 敌机贴图
    QPixmap m_enemy;
    // 敌机左上角坐标
    int m_x;
    int m_y;
    // 敌机移动速度
    int m_speed;
    // 是否闲置
    bool m_free;
    // 碰撞体积
    QRect m_rect;
    // 发射间隔记录
    int m_record;
    // 敌机子弹
    Bullet m_bullets[BULLET_MAX_NUM];
};

#endif // ENEMY_H
