#include "boss.h"

Boss::Boss()
{
    //  加载boss图片
    m_pix.load(BOSS_PATH); //暂定196×512

    m_x = (WINDOW_WIDTH - m_pix.width()) * 0.5;
    m_y = -m_pix.height();
    Offset1_x = 158;
    Offset3_x = 468;
    Offset1_y = Offset3_y = 100;

    m_rect.setWidth(m_pix.width());
    m_rect.setHeight(m_pix.height());
    m_rect.moveTo(m_x,m_y);

    hit_point1.setWidth(IMPACT_WIDTH);
    hit_point1.setHeight(IMPACT_HEIGHT);
    m_rect.moveTo(m_x + Offset1_x,m_y + Offset1_y);

    hit_point2.setWidth(IMPACT_WIDTH);
    hit_point2.setHeight(IMPACT_HEIGHT);
    hit_point2.moveTo(m_x + m_pix.width() * 0.5,m_y + m_pix.height() * 0.9);

    hit_point3.setWidth(IMPACT_WIDTH);
    hit_point3.setHeight(IMPACT_HEIGHT);
    m_rect.moveTo(m_x + Offset3_x,m_y + Offset3_y);

    main_interval = 0;
    minor_interval = 0;
    m_free = true;
    unrivaled = true;
    support_free = true;
    support_record = 0;


    for(int i = 0; i < BULLET_MAX_NUM; ++i){
        m_bullets1[i].m_direction = {0, -1};
        m_bullets1[i].adjust();
        m_bullets1[i].m_bullet.load(ENEMY_BULLET_PATH);
        m_bullets2[i].m_direction = {0, -1};
        m_bullets2[i].adjust();
        m_bullets2[i].m_bullet.load(ENEMY_BULLET_PATH);
    }

    m_speedY = 1;
    m_speedX = 5;
}


void Boss::updatePosition()
{
    if(m_y < 0)
    {
        m_y += m_speedY;
        m_rect.moveTo(m_x,m_y);
        hit_point1.moveTo(m_x + Offset1_x,m_y + Offset1_y);
        hit_point2.moveTo(m_x + m_rect.width() * 0.5,m_y + m_rect.width() * 0.9);
        hit_point3.moveTo(m_x + Offset3_x,m_y + Offset3_y);
    }
    else
    {
        unrivaled = false;
    }
}

void Boss::mainShoot()
{
    ++main_interval;
    if(main_interval < ENEMY_FIRE_INTERVAL){
        return;
    }

    main_interval = 0;
    for(int i = 0; i < BULLET_MAX_NUM; ++i){

        if(m_bullets2[i].m_free){
            m_bullets2[i].m_free = false;
            m_bullets2[i].m_x = m_x + m_rect.width() * 0.5;
            m_bullets2[i].m_y = m_y + m_rect.height() * 0.9;
            break;

        }
    }

}

void Boss::assistShoot()
{
    ++minor_interval;
    if(minor_interval < 5 * ENEMY_FIRE_INTERVAL){
        return;
    }

    minor_interval = 0;
    for(int i = 0; i < BULLET_MAX_NUM; ++i){

        if(m_bullets1[i].m_free){
            m_bullets1[i].m_free = false;
            m_bullets1[i].m_x = m_x + Offset1_x;
            m_bullets1[i].m_y = m_y + Offset1_y;
            break;

        }
    }

    for(int i = 0; i < BULLET_MAX_NUM; ++i){

        if(m_bullets1[i].m_free){
            m_bullets1[i].m_free = false;
            m_bullets1[i].m_x = m_x + Offset3_x;
            m_bullets1[i].m_y = m_y + Offset3_y;
            break;

        }
    }
}
