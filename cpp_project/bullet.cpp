#include "bullet.h"
#include <QtMath>
Bullet::Bullet()
{
    //  贴图
    m_bullet.load(BULLET_PATH);
    // 速度
    m_speed = BULLET_SPEED;
    // 位置
    m_x = (WINDOW_WIDTH - m_bullet.width()) / 2;

    m_y = WINDOW_HEIGHT;
    // 初始闲置
    m_free = true;
    // 碰撞体积
    m_rect.setWidth(m_bullet.width());
    m_rect.setHeight(m_bullet.height());
    m_rect.moveTo(m_x, m_y);

}

int Bullet::m_x_bound;
int Bullet::m_y_bound;

static bool isOut(int x, int y, int xl, int xr, int yl, int yr){
    return x < xl || x > xr || y < yl || y > yr;
}


void Bullet::updatePosition()
{
    // 空闲的子弹，无需更新
    if(m_free){
        return;
    }
//    // 这里是默认向上运动，还没用到m_direction
//    m_y -= m_speed;

    if(m_direction.first == 0){
        m_y += (m_direction.second > 0 ? 1 : -1) * m_speed;
    }else{

        double d = sqrt(m_direction.first * m_direction.first + m_direction.second * m_direction.second);
        int dx = m_direction.first / d * m_speed;
        int dy = m_direction.second / d * m_speed;
        m_x += dx;
        m_y += dy;
    }


    m_rect.moveTo(m_x, m_y);

    // 出屏幕则恢复空闲
    if(isOut(m_x, m_y, 0, m_x_bound, 0, m_y_bound)){
        m_free = true;
    }

}

void Bullet::adjust()
{
    // 变换
    m_direction.second = -m_direction.second;
}
