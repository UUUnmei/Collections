#include "map.h"
#include "config.h"


Map::Map()
{
    // 初始化地图图片
    m_map1.load(MAP_PATH);
    m_map2.load(MAP_PATH);
    // 位置
    m_map1_Y = -WINDOW_HEIGHT;
    m_map2_Y = 0;
    // 速度
    m_scroll_speed = MAP_SCROLL_SPEED;
}

void Map::updateMap()
{
    // 第一张图在上面
    m_map1_Y += m_scroll_speed;
    if(m_map1_Y >= 0){
        m_map1_Y = -WINDOW_HEIGHT;
    }
    // 第二张图在下面
    m_map2_Y += m_scroll_speed;
    if(m_map2_Y >= WINDOW_HEIGHT){
        m_map2_Y = 0;
    }

//   两张图循环向下滚动
//            ------
//            |     |
//            | 图1 |
//            ------
//    ------  ------
//    |     | |     |
//    | 屏  | | 图2  |
//    ------  ------
}
