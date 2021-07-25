#include "blood_pack.h"

blood_pack::blood_pack()
{
    // 将血包的图导入，这里仍使用显示血量的图
    m_blood_pack.load(HEART_PATH);
    m_x= 0;
    m_y= 0;
    record = 0;

    // 设置血包的矩阵框
    m_rect.setHeight(m_blood_pack.height());
    m_rect.setWidth(m_blood_pack.width());
    m_rect.moveTo(m_x,m_y);

    iswork = false;
}

void blood_pack::updateposition(Enemy *enemy)
{
    // 当掉血包的飞机被打了之后，即在它gg的地方掉落血包
    iswork = true;
    record = 0;
    m_x = enemy->m_x;
    m_y = enemy->m_y;
    m_rect.moveTo(m_x,m_y);
}

void blood_pack::update_blood_pack(Player *player)
{
    if(iswork == false){ // 如果还未到工作状态，直接返回
        return;
    }
    record ++;
    if(record == HEART_INTERVAL){
        iswork = false;
        return;
    }
    if(this->m_rect.intersects(player->m_rect)){
        // 如果碰到了，则加血，且血包不再显示
        player->heart_num++;
        iswork = false;
        record = 0;
        return;
    }

}
