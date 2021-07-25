#include "double_bullet.h"

double_bullet::double_bullet()
{
    // 将血包的图导入，这里仍使用显示血量的图
    m_double_bullet.load(DOUBLE_BULLET_PATH);
    m_x= 0;
    m_y= 0;
    record = 0;

    // 设置血包的矩阵框
    m_rect.setHeight(m_double_bullet.height());
    m_rect.setWidth(m_double_bullet.width());
    m_rect.moveTo(m_x,m_y);

    iswork = false;
}

void double_bullet::updateposition(Enemy *enemy)
{
    // 当掉血包的飞机被打了之后，即在它gg的地方掉落血包
    iswork = true;
    record = 0;
    m_x = enemy->m_x;
    m_y = enemy->m_y;
    m_rect.moveTo(m_x,m_y);
}

void double_bullet::update_double_bullet(Player *player)
{
    if(iswork == false){ // 如果还未到工作状态，直接返回
        return;
    }
    record ++;
    if(record == HEART_INTERVAL){
        // 这里仍然是 4s不吃，则双枪包直接消失
        iswork = false;
        return;
    }
    if(this->m_rect.intersects(player->m_rect)){
        // 如果碰到了，则变为双枪模式
        player->single_double = true;
        player->change_to_double = true;
        iswork = false;
        record = 0;
        return;
    }
}

