#ifndef DOUBLE_BULLET_H
#define DOUBLE_BULLET_H
#include "QPixmap"
#include "enemy.h"
#include "config.h"
#include "player.h"

class double_bullet
{
public:
    double_bullet();
    // 双枪包贴图
    QPixmap m_double_bullet;
    // 双枪包的左上角坐标
    int m_x;
    int m_y;
    int record;

    // 双枪包的矩阵框，用于飞机是否吃到双枪包的判断
    QRect m_rect;

    // 是否显示
    bool iswork = false;

    // 显示后更改坐标信息
    void updateposition(Enemy *enemy);

    // 更新信息
    void update_double_bullet(Player *player);
};

#endif // DOUBLE_BULLET_H
