#ifndef MAP_H
#define MAP_H
#include <QPixmap>

// 地图类，主要实现背景的滚动
class Map
{

public:
    // 构造函数
    Map();

    // 更新（滚动）
    void updateMap();

    // 图片
    QPixmap m_map1;
    QPixmap m_map2;

    // 分别的y轴坐标
    int m_map1_Y;
    int m_map2_Y;

    // 滚动速度
    int m_scroll_speed;

};

#endif // MAP_H
