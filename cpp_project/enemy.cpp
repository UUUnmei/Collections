#include "enemy.h"
#include "config.h"

Enemy::Enemy()
{
    m_enemy.load(ENEMY_PATH1);

    m_x = m_y = 0; // 先随便来一个位置

    m_free = true;

    m_speed = ENEMY_SPEED;

    m_record = 0;

    m_rect.setWidth(m_enemy.width());
    m_rect.setHeight(m_enemy.height());
    m_rect.moveTo(m_x, m_y);


    for(int i = 0; i < BULLET_MAX_NUM; ++i){
        m_bullets[i].m_direction = {0, -1};
        m_bullets[i].adjust();  // 转为qt坐标系中的方向
        m_bullets[i].m_bullet.load(ENEMY_BULLET_PATH);
    }

}

void Enemy::updatePosition()
{
    if(m_free){
        return;
    }
    // 这里是默认向下运动
    m_y += m_speed;
    m_rect.moveTo(m_x, m_y);

    // 出屏幕则恢复空闲
    if(m_y >= WINDOW_HEIGHT){
        m_free = true;
    }
}

void Enemy::shoot()
{
    ++m_record;
    if(m_record < ENEMY_FIRE_INTERVAL){
        return;
    }

    m_record = 0;
    for(int i = 0; i < BULLET_MAX_NUM; ++i){
        // 找空闲子弹
        if(m_bullets[i].m_free){
            m_bullets[i].m_free = false;
            m_bullets[i].m_x = m_x + 27;
            m_bullets[i].m_y = m_y + 45;
            break;
        }
    }
}
