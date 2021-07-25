#ifndef BLOOD_PACK_H
#define BLOOD_PACK_H
//#include <qpixmap.h>

//#include <config.h>
//#include <player.h>
//#include <enemy.h>
#include "enemy.h"
#include "player.h"
#include "config.h"
#include <QPixmap>

class blood_pack
{
public:
    blood_pack();
    // 血包贴图
    QPixmap m_blood_pack;
    // 血包的左上角坐标
    int m_x;
    int m_y;
    int record;

    // 血包的矩阵框，用于飞机是否吃到血包的判断
    QRect m_rect;

    // 是否显示
    bool iswork = false;

    // 显示后更改坐标信息
    void updateposition(Enemy *enemy);

    // 更新信息
    void update_blood_pack(Player *player);
};

#endif // BLOOD_PACK_H
