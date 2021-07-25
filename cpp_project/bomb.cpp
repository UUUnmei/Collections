#include "bomb.h"

Bomb::Bomb()
{
    for(int i = 1;i <= BOMB_MAX;i++)
    {
        // 修改占位符  ":/resource/bomb-%1.png" -> ":/resource/bomb-1.png" ->  ":/resource/bomb-2.png" .....
        QString str = QString(BOMB_PATH).arg(i);
        m_pixArr.push_back(QPixmap(str));
    }

    m_X = 0;
    m_Y = 0;

    m_Free = true;
     // 当前爆炸图片下标
    m_Index = 0;
    // 爆炸间隔记录
    m_Recorder = 0;
}

void Bomb::updateInfo()
{
    if(m_Free)
    {
        return;
    }

    m_Recorder++;
    if(m_Recorder < BOMB_INTERVAL)
    {
        return;
    }
    m_Recorder = 0;


    m_Index++;
    //数组中爆炸效果图片0~6
    if(m_Index >= BOMB_MAX)
    {
        m_Index = 0;
        m_Free = true;
    }
}
