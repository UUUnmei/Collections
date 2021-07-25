#include "maingame.h"
#include "config.h"

#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>
#include <ctime>
#include <QMessageBox>
#include <QCoreApplication>
#include "mainwindow.h"
#include <QString>
#include "animation.h"


MainGame::MainGame(int player_number, QWidget *parent) : QMainWindow(parent), m_player_num(player_number)
                                                         , record_single(1), record_double(2)
{
    // 敌机出场横坐标的随机数种子
    srand( (unsigned int)time(NULL) );

    initWindow();
    playGame();

    setAttribute(Qt::WA_DeleteOnClose);


}


// 用于恢复父窗口
void MainGame::closeEvent(QCloseEvent *e){
    qDebug() << "发送退出信号";
    // 发射一个信号
    emit ExitWin();
    e->accept();
}


MainGame::~MainGame()
{
    qDebug() << "退出游戏！" ;

}
// 初始化窗口
void MainGame::initWindow(void)
{
    // 设置窗口大小，且无法修改

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    Bullet::m_x_bound = WINDOW_WIDTH;
    if(m_player_num == 2){
        setFixedSize(700, WINDOW_HEIGHT);// 700 是背景图片的宽度。。
        m_player[0].m_x_bound = m_player[1].m_x_bound = 700;
        Bullet::m_x_bound = 700;
        m_boss.m_x = (700 - m_boss.m_pix.width()) * 0.5;
        m_player[1].m_plane.load(PLANE_PATH2);
    }
    Bullet::m_y_bound = WINDOW_HEIGHT;



    // 设置标题
    setWindowTitle(GAME_TITEL);
    // 设置图标
    setWindowIcon(QIcon(GAME_ICO_PATH));

    // 初始化定时器
    m_timer.setInterval(GAME_FPS);

    // 敌机出场间隔初始化
    m_recorder = 0;

    // 敌机正常入场
    enemy_appear = true;

    // boss出场计时(10s)
    count_down = 1000;

    // 血量图片
    m_heart.load(HEART_PATH);
    m_heart_lose.load(HEART_LOSE_PATH);

    if(m_player_num == 1)
        m_player[0].m_x = (WINDOW_WIDTH - m_player[0].m_plane.width()) / 2;
    else{
        // 双人
        int k = (700 - 2 * m_player[0].m_plane.width()) / 3;
        m_player[0].m_x = k;
        m_player[1].m_x = 2 * k + m_player[0].m_plane.width();
    }

    between_check_point = 0;
    m_map_id = 0;
    last = -1;
}


// 游戏启动
void MainGame::playGame()
{
    // 开场倒计时相关
    Animation *count_down = new Animation(this, COUNT_DOWN_GIF_PATH);
    if(m_player_num == 2)
        count_down->setDoublePlayerPosition();
    count_down->start();

    // singleshot实际上是异步执行的。。。
    // 不过对于现在来说，只要m_timer不计数，画面就不会有变化，就有了同步阻塞的效果（大概。。
    // 这里捕获得是值捕获，在表达式创建时捕获（引用捕获是在调用时）,主要是对于movie和waiting这俩，m_timer似乎不太影响
    // 3400ms和选用的gif图相关
    QTimer::singleShot(3400, this, [=]{
        qDebug() << "计时结束";

        count_down->stop();
        delete count_down;

         // 启动定时器
        m_timer.start();
    });

    // 启动定时器
    //m_timer.start();

    // 监听定时器的信号
    connect(&m_timer, &QTimer::timeout, [=]{
        // 更新并绘制
        enemyToScene();

        updateAll();

        update(); // 调用paintEvent

        collisionDetection();
    });
}
// 更新所有元素坐标
void MainGame::updateAll()
{
    m_map.updateMap();

    // 发射子弹
    for(int i = 0; i < m_player_num; ++i){
        if(m_player[i].heart_num == 0) continue;
        if(m_player[i].m_fire)
            m_player[i].shoot();

        for(int j = 0; j < BULLET_MAX_NUM; ++j){
            if(m_player[i].m_bullets[j].m_free == false){
                m_player[i].m_bullets[j].updatePosition();
                //qDebug() << "shoot2";
            }
        }
    }


    // 更新玩家位置
    for(int i = 0; i < m_player_num; ++i)
        if(m_player[i].heart_num > 0){
            // 更新血包状态
            bloodpack.update_blood_pack(&this->m_player[i]);

            // 更新双枪包状态
            doublebullet.update_double_bullet(&this->m_player[i]);

            m_player[i].update();

            m_player[i].updateByKey();
        }


    // 敌机出场
    for(int i = 0; i < ENEMY_MAX_NUM; ++i){
        // 找非空闲敌机
        if(m_enemys[i].m_free == false){
            m_enemys[i].updatePosition();
            m_enemys[i].shoot();
            // 敌机发射
            for(int j = 0; j < BULLET_MAX_NUM; ++j){
                if(m_enemys[i].m_bullets[j].m_free == false){
                    m_enemys[i].m_bullets[j].updatePosition();
                }
            }
        }
    }




    // 爆炸效果
    for(int i = 0; i < BOMB_NUM; ++i){
        if(m_bombs[i].m_Free == false){
            m_bombs[i].updateInfo();
        }
    }
    if(m_boss.boss_heart > 0){
        // boss出场倒计时
        if(m_boss.m_free)
        {
            if(count_down == 200)
            {
                enemy_appear = false;
            }

            if(count_down == 0)
            {
                m_boss.m_free = false;
            }
            else
                count_down--;
        }

        // boss状态更新
        else
        {
            m_boss.updatePosition();
            m_boss.mainShoot();
            m_boss.assistShoot();

            for(int i = 0; i < BULLET_MAX_NUM; ++i){
                if(m_boss.m_bullets1[i].m_free == false){
                    m_boss.m_bullets1[i].updatePosition();
                }
            }
            for(int i = 0; i < BULLET_MAX_NUM; ++i){
                if(m_boss.m_bullets2[i].m_free == false){
                    m_boss.m_bullets2[i].updatePosition();
                }
            }

            if(m_boss.boss_heart * 2 <= BOSS_LIFE && m_boss.support_free)
            {
                m_boss.unrivaled = true;
                if(m_boss.m_x > -m_boss.m_rect.width())
                {
                    m_boss.m_x -= m_boss.m_speedX;
                }
                else
                {
                    enemy_appear = true;
                    m_boss.support_record++;
                    if(m_boss.support_record == 500)
                    {
                        m_boss.support_record = 0;
                        m_boss.support_free = false;
                        enemy_appear = false;
                        m_boss.m_y = -m_boss.m_rect.height();
                        if(m_player_num == 2)
                            m_boss.m_x = (700 - m_boss.m_rect.width()) * 0.5;
                        else
                            m_boss.m_x = (WINDOW_WIDTH - m_boss.m_rect.width()) * 0.5;
                    }
                }
            }
        }
    }
    else
    {
        pass();
    }
}
// 绘制
void MainGame::paintEvent(QPaintEvent *)
{
    //在当前窗口绘制
    QPainter painter(this);

    // 绘制地图
    painter.drawPixmap(0, m_map.m_map1_Y, m_map.m_map1);
    painter.drawPixmap(0, m_map.m_map2_Y, m_map.m_map2);

    // 绘制飞机
    //qDebug() << "飞机位置: " << m_player.m_x << ' ' << m_player.m_y;
    for(int i = 0; i < m_player_num; ++i)
        if(m_player[i].heart_num > 0)
            painter.drawPixmap(m_player[i].m_x, m_player[i].m_y, m_player[i].m_plane);



    // 绘制子弹
    //painter.drawPixmap(m_bullets.m_x, m_bullets.m_y, m_bullets.m_bullet);
    for(int j = 0; j < m_player_num; ++j){
        if(m_player[j].heart_num == 0) continue;

        for(int i = 0; i < BULLET_MAX_NUM; ++i){
            if(m_player[j].m_bullets[i].m_free == false){
                painter.drawPixmap(m_player[j].m_bullets[i].m_x,
                                   m_player[j].m_bullets[i].m_y,
                                   m_player[j].m_bullets[i].m_bullet);
            }
        }
    }

    // 绘制血包
    if(bloodpack.iswork == true){ // 只有工作状态才能绘制
        painter.drawPixmap(bloodpack.m_x,bloodpack.m_y,bloodpack.m_blood_pack);
    }

    // 绘制双枪包
    if(doublebullet.iswork == true){
        painter.drawPixmap(doublebullet.m_x,doublebullet.m_y,doublebullet.m_double_bullet);
    }

    // 绘制敌机
    for(int i = 0; i < ENEMY_MAX_NUM; ++i){
        // 找非空闲敌机
        if(m_enemys[i].m_free == false){
            painter.drawPixmap(m_enemys[i].m_x,
                               m_enemys[i].m_y,
                               m_enemys[i].m_enemy);
            //绘制敌机的子弹
            for(int j = 0; j < BULLET_MAX_NUM; ++j){
                if(m_enemys[i].m_bullets[j].m_free == false){
                    painter.drawPixmap(m_enemys[i].m_bullets[j].m_x,
                                       m_enemys[i].m_bullets[j].m_y,
                                       m_enemys[i].m_bullets[j].m_bullet);
                }
            }
        }
    }
    // 绘制boss
    if(m_boss.m_free == false){
        painter.drawPixmap(m_boss.m_x,m_boss.m_y,m_boss.m_pix);

        //绘制boss的子弹
        for(int i = 0; i < BULLET_MAX_NUM; ++i){
            if(m_boss.m_bullets1[i].m_free == false){
                painter.drawPixmap(m_boss.m_bullets1[i].m_x,
                                   m_boss.m_bullets1[i].m_y,
                                   m_boss.m_bullets1[i].m_bullet);
            }
            if(m_boss.m_bullets2[i].m_free == false){
                painter.drawPixmap(m_boss.m_bullets2[i].m_x,
                                   m_boss.m_bullets2[i].m_y,
                                   m_boss.m_bullets2[i].m_bullet);
            }
        }
    }

    // 绘制爆炸图片
    for(int i = 0; i < BOMB_NUM; ++i){
        if(m_bombs[i].m_Free == false){
            painter.drawPixmap(m_bombs[i].m_X,
                               m_bombs[i].m_Y,
                               m_bombs[i].m_pixArr[m_bombs[i].m_Index]);
        }
    }

    // 绘制血量
    for(int j = 0; j < m_player_num; ++j){
        for(int i = m_player[j].heart_num; i < LIFE; ++i)
            painter.drawPixmap(i * HEART_WIDTH, j * HEART_WIDTH, m_heart_lose);
        for(int i = 0; i < m_player[j].heart_num; ++i)
            painter.drawPixmap(i * HEART_WIDTH, j * HEART_WIDTH, m_heart);
    }

    // 打印分数
    QFont font1("宋体", 10, QFont::Bold,true);
    // 设置文字样式
    font1.setUnderline(true); // 设置下划线
    painter.setFont(font1); //使用字体
    painter.setPen(Qt::white); // 设置为白色
//    int xx = (m_player_num == 1 ? 380 : 570);
//    QString msg("玩家%1的分数：");
//    for(int i = 0; i < m_player_num; ++i){
//        QString text = msg.arg(i + 1) + QString::number(m_player[i].score);
//        painter.drawText(xx , 30 * (i + 1), text);
//    }
    if(m_player_num == 1){
        painter.drawText(LIFE * HEART_WIDTH + 10 , HEART_WIDTH / 2, QString("血量"));
        painter.drawText(420, 30, QString("分数：") + QString::number(m_player[0].score));
    }else{
        painter.drawText(LIFE * HEART_WIDTH + 10 , HEART_WIDTH / 2, QString("玩家1血量"));
        painter.drawText(LIFE * HEART_WIDTH + 10 , HEART_WIDTH / 2 + HEART_WIDTH, QString("玩家2血量"));
        QString msg("玩家%1的分数：");
        for(int i = 0; i < m_player_num; ++i){
            QString text = msg.arg(i + 1) + QString::number(m_player[i].score);
            painter.drawText(570 , 30 * (i + 1), text);
        }
    }




}

void MainGame::keyPressEvent(QKeyEvent *event)
{
    // 键盘控制
    // WSAD 上下左右
    // space 射击

    if(event->isAutoRepeat()) return;   // !!!!

    if(event->key() == Qt::Key_W){
        m_player[0].m_mov_up = true;
        qDebug() << "W Press";
    }

    if(event->key() == Qt::Key_S)
        m_player[0].m_mov_down = true;
    if(event->key() == Qt::Key_A)
        m_player[0].m_mov_left = true;
    if(event->key() == Qt::Key_D)
        m_player[0].m_mov_right = true;
    if(event->key() == Qt::Key_Space)
        m_player[0].m_fire = true;


    if(event->key() == Qt::Key_Up)
        m_player[1].m_mov_up = true;
    if(event->key() == Qt::Key_Down)
        m_player[1].m_mov_down = true;
    if(event->key() == Qt::Key_Left)
        m_player[1].m_mov_left = true;
    if(event->key() == Qt::Key_Right)
        m_player[1].m_mov_right = true;
    if(event->key() == Qt::Key_Return){
        // 大回车键，不是小键盘的回车
        //qDebug() << "2 fire";
        m_player[1].m_fire = true;
    }


//    if(event->key() == Qt::Key_I)
//        m_player[2].m_mov_up = true;
//    if(event->key() == Qt::Key_K)
//        m_player[2].m_mov_down = true;
//    if(event->key() == Qt::Key_J)
//        m_player[2].m_mov_left = true;
//    if(event->key() == Qt::Key_L)
//        m_player[2].m_mov_right = true;
//    if(event->key() == Qt::Key_Semicolon){
//        // 大回车键，不是小键盘的回车
//        //qDebug() << "2 fire";
//        m_player[2].m_fire = true;
//    }

    //qDebug() << (int)event->key();
    // m_player.updateByKey(); 放在了updateAll()中
}

void MainGame::keyReleaseEvent(QKeyEvent *event)
{
    // 松开按键，标志恢复
    if(event->isAutoRepeat()) return;  // !!!!

    if(event->key() == Qt::Key_W){
        m_player[0].m_mov_up = false;
        qDebug() << "W Release";
    }

    if(event->key() == Qt::Key_S)
        m_player[0].m_mov_down = false;
    if(event->key() == Qt::Key_A)
        m_player[0].m_mov_left = false;
    if(event->key() == Qt::Key_D)
        m_player[0].m_mov_right = false;
    if(event->key() == Qt::Key_Space)
        m_player[0].m_fire = false;

    if(event->key() == Qt::Key_Up)
        m_player[1].m_mov_up = false;
    if(event->key() == Qt::Key_Down)
        m_player[1].m_mov_down = false;
    if(event->key() == Qt::Key_Left)
        m_player[1].m_mov_left = false;
    if(event->key() == Qt::Key_Right)
        m_player[1].m_mov_right = false;
    if(event->key() == Qt::Key_Return){
        // 大回车键，不是小键盘的回车
        //qDebug() << "2 not fire";
        m_player[1].m_fire = false;
    }

//    if(event->key() == Qt::Key_I)
//            m_player[2].m_mov_up = false;
//        if(event->key() == Qt::Key_K)
//            m_player[2].m_mov_down = false;
//        if(event->key() == Qt::Key_J)
//            m_player[2].m_mov_left = false;
//        if(event->key() == Qt::Key_L)
//            m_player[2].m_mov_right = false;
//        if(event->key() == Qt::Key_Semicolon){
//            // 大回车键，不是小键盘的回车
//            //qDebug() << "2 fire";
//            m_player[2].m_fire = false;
//        }

}

void MainGame::enemyToScene()
{
    if(enemy_appear == 0 || between_check_point ){
        return;
    }

    // 敌机登场
    ++m_recorder;
    if(m_recorder < ENEMY_INTERVAL){
        return;
    }

    m_recorder = 0;
    for(int i = last + 1; i < ENEMY_MAX_NUM; ++i){
        // 找空闲敌机
        if(m_enemys[i].m_free){
            last = i;
            m_enemys[i].m_free = false;

            // 随机出场坐标
            if(m_player_num == 2)
                m_enemys[i].m_x = rand() % (700 - m_enemys[i].m_enemy.width());
            else
                m_enemys[i].m_x = rand() % (WINDOW_WIDTH - m_enemys[i].m_enemy.width());

            m_enemys[i].m_y = -m_enemys[i].m_enemy.height();

            break;
        }
    }
    if(last == ENEMY_MAX_NUM - 1)
        last = -1;

}

void MainGame::collisionDetection()
{
    // 碰撞检测
    for(int i = 0; i < ENEMY_MAX_NUM; ++i){

        // 遍历非空闲敌机
        if(m_enemys[i].m_free == false){

            for(int k = 0; k < m_player_num; ++k){

                // 主要考虑双人模式下，玩家死后不再进行相关处理
                if(m_player[k].heart_num == 0)
                    continue;

                // 如果玩家撞上敌机 ， 玩家gg
                if(m_enemys[i].m_rect.intersects(m_player[k].m_rect)){
                    m_player[k].loseFullLife();
                    isGameOver();
                }


                // 遍历玩家的非空闲子弹 -- 玩家射击敌机
                for(int j = 0; j < BULLET_MAX_NUM; ++j){
                    if(m_player[k].m_bullets[j].m_free == false){

                        // 矩形是否相交
                        if(m_enemys[i].m_rect.intersects(m_player[k].m_bullets[j].m_rect)){
                                // 碰上了令敌机和该子弹消失
                                // 暂不考虑血量之类的
                                 m_player[k].score += 10;

                                //m_enemys[i].m_free = true;
                                m_player[k].m_bullets[j].m_free = true;

                                // 该敌机的子弹也消失
                                for(int k = 0; k < BULLET_MAX_NUM; ++k)
                                    m_enemys[i].m_bullets[k].m_free = true;

                                // 播放爆炸效果
                                for(int k = 0;k < BOMB_NUM;k++){
                                    if(m_bombs[k].m_Free) {
                                        m_bombs[k].m_Free = false;
                                        m_bombs[k].m_X = m_enemys[i].m_x;
                                        m_bombs[k].m_Y = m_enemys[i].m_y;
                                        break;
                                    }
                                }

                                if(i == ENEMY_BLOOD_PACK){ // 是掉落血包的飞机
                                    // 在飞机GG的地方掉落血包
                                    if(!bloodpack.iswork){
                                        // 血包出现在地图上的话，就不需要更新血包的位置了
                                        bloodpack.updateposition(&m_enemys[i]);
                                    }
                                }else if(i == ENEMY_DOUBLE_BULLET){// 是掉落双枪包的飞机
                                    qDebug() << "ccccccccccccccccccccccccccccc";
                                    if(!doublebullet.iswork){
                                        qDebug() << "dddddddddddddddd";
                                        doublebullet.updateposition(&m_enemys[i]);
                                    }
                                }
                                m_enemys[i].m_free = true;
                        }


                    }
                }

                // 遍历敌机的非空闲子弹  --- 敌机射击玩家
                for(int j = 0; j < BULLET_MAX_NUM; ++j){
                    if(m_enemys[i].m_bullets[j].m_free == false){
                        if(m_enemys[i].m_bullets[j].m_rect.intersects(m_player[k].m_rect))
                        {
                            // 玩家被击中，扣除生命值
                            m_player[k].loseLife();
                            isGameOver();
                            m_enemys[i].m_bullets[j].m_free = true;
                        }
                    }
                }
            }

        }
    }


    if(m_boss.m_free == false)
    {
        for(int i = 0;i < m_player_num;++i)
        {
            if(m_player[i].heart_num == 0)
                continue;

            // 如果玩家撞上boss，玩家gg
            if(m_boss.m_rect.intersects(m_player[i].m_rect)){
                m_player[i].loseFullLife();
                isGameOver();
            }

            for(int j = 0;j < BULLET_MAX_NUM;++j)
            {
                if(m_boss.m_bullets1[j].m_free == false){
                    if(m_boss.m_bullets1[j].m_rect.intersects((m_player[i].m_rect)))
                    {
                        m_player[i].loseLife();
                        isGameOver();
                        m_boss.m_bullets1[j].m_free = true;
                    }
                }
            }
            for(int j = 0;j < BULLET_MAX_NUM;++j)
            {
                if(m_boss.m_bullets2[j].m_free == false){
                    if(m_boss.m_bullets2[j].m_rect.intersects((m_player[i].m_rect)))
                    {
                        m_player[i].loseLife(2);
                        isGameOver();
                        m_boss.m_bullets2[j].m_free = true;
                    }
                }
            }

            if(m_boss.unrivaled == false)
            {
                for(int j = 0;j < BULLET_MAX_NUM; ++j)
                {
                    if(m_player[i].m_bullets[j].m_free == false){
                        if(m_player[i].m_bullets[j].m_rect.intersects(m_boss.hit_point1))
                        {
                            m_boss.boss_heart--;
                            qDebug()<<"boss当前血量:"<<m_boss.boss_heart;

                            m_player[i].m_bullets[j].m_free = true;
                            // 播放爆炸效果
                            for(int k = 0;k < BOMB_NUM;k++){
                                if(m_bombs[k].m_Free) {
                                    m_bombs[k].m_Free = false;
                                    m_bombs[k].m_X = m_boss.m_x + m_boss.Offset1_x;
                                    m_bombs[k].m_Y = m_boss.m_y + m_boss.Offset1_y;
                                    break;
                                }
                            }

                            if(m_boss.boss_heart <= 0)
                            {
                                // boss爆炸...
                                qDebug()<<"You Win!";
                                m_boss.m_free = true;
                                m_boss.unrivaled = true;
                                m_player[i].score += 100;

                                for(int k = 0; k < BULLET_MAX_NUM; ++k)
                                {
                                    m_boss.m_bullets1[k].m_free = true;
                                    m_boss.m_bullets2[k].m_free = true;
                                }

                            }
                        }
                        if(m_player[i].m_bullets[j].m_rect.intersects(m_boss.hit_point2))
                        {
                            m_boss.boss_heart -= 2;
                            qDebug()<<"boss当前血量:"<<m_boss.boss_heart;

                            // 播放爆炸效果
                            for(int k = 0;k < BOMB_NUM;k++){
                                if(m_bombs[k].m_Free) {
                                    m_bombs[k].m_Free = false;
                                    m_bombs[k].m_X = m_boss.m_x + m_boss.m_rect.width() * 0.5;
                                    m_bombs[k].m_Y = m_boss.m_y + m_boss.m_rect.height() * 0.8;
                                    break;
                                }
                            }

                            m_player[i].m_bullets[j].m_free = true;

                            if(m_boss.boss_heart <= 0)
                            {
                                // boss爆炸...
                                qDebug()<<"You Win!";
                                m_boss.m_free = true;
                                m_boss.unrivaled = true;
                                m_player[i].score += 100;

                                for(int k = 0; k < BULLET_MAX_NUM; ++k)
                                {
                                    m_boss.m_bullets1[k].m_free = true;
                                    m_boss.m_bullets2[k].m_free = true;
                                }

                            }
                        }
                        if(m_player[i].m_bullets[j].m_rect.intersects(m_boss.hit_point3))
                        {
                            m_boss.boss_heart--;
                            qDebug()<<"boss当前血量:"<<m_boss.boss_heart;

                            // 播放爆炸效果
                            for(int k = 0;k < BOMB_NUM;k++){
                                if(m_bombs[k].m_Free) {
                                    m_bombs[k].m_Free = false;
                                    m_bombs[k].m_X = m_boss.m_x + m_boss.Offset3_x;
                                    m_bombs[k].m_Y = m_boss.m_y + m_boss.Offset3_y;
                                    break;
                                }
                            }

                            m_player[i].m_bullets[j].m_free = true;

                            if(m_boss.boss_heart <= 0)
                            {
                                // boss爆炸...
                                qDebug()<<"You Win!";
                                m_boss.m_free = true;
                                m_boss.unrivaled = true;

                                m_player[i].score += 100;

                                for(int k = 0; k < BULLET_MAX_NUM; ++k)
                                {
                                    m_boss.m_bullets1[k].m_free = true;
                                    m_boss.m_bullets2[k].m_free = true;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}

void MainGame::pass()
{
    if(between_check_point == 1){
        between_check_point++;

        // 游戏结束之后需要判断是否记录
        if(m_player_num == 1)
            record_single.operation(m_player[0].score);
        else
            record_double.operation(m_player[0].score + m_player[1].score);

        auto result = QMessageBox::question(this, "恭喜通关！", "是否开始下一关？", QMessageBox::Yes|QMessageBox::No);

        if(result == QMessageBox::Yes){
            InitScene();
        }else{
            this->close();
        }
    }
    if(between_check_point == 0){
        between_check_point = 1;
    }


}

void MainGame::InitScene()
{
    Animation *interlude = new Animation(this, BLACK_BETWEEN_CHECK_POINT);
    if(m_player_num == 2)
        interlude->setDoublePlayerPosition();
    interlude->start();

    QTimer::singleShot(2000, this, [&]{
        if(m_map_id == 0){
            m_map_id = 1;
            m_map.m_map1.load(MAP_PATH_2);
            m_map.m_map2.load(MAP_PATH_2);
        }else{
            m_map_id = 0;
            m_map.m_map1.load(MAP_PATH);
            m_map.m_map2.load(MAP_PATH);
        }


        between_check_point = 0;

        // 敌机出场间隔初始化
        m_recorder = 0;

        // 敌机正常入场
        enemy_appear = true;

        // boss出场计时(10s)
        count_down = 1000;
        // boss 重置
        m_boss.m_free = true;
        m_boss.main_interval = 0;
        m_boss.minor_interval = 0;
        m_boss.m_free = true;
        m_boss.unrivaled = true;
        m_boss.support_free = true;
        m_boss.support_record = 0;
        if(m_player_num == 2)
            m_boss.m_x = (700 - m_boss.m_pix.width()) * 0.5;
        else
            m_boss.m_x = (WINDOW_WIDTH - m_boss.m_pix.width()) * 0.5;
        m_boss.m_y = -m_boss.m_pix.height();

        m_boss.boss_heart = BOSS_LIFE;

        for(int i = 0; i < m_player_num; ++i){
            m_player[i].m_mov_down = false;
            m_player[i].m_mov_up = false;
            m_player[i].m_mov_left = false;
            m_player[i].m_mov_right = false;
            m_player[i].m_fire = false;
        }
    });

}


void MainGame::isGameOver()
{
    bool gg = true;
    for(int i = 0; i < m_player_num; ++i)
        if(m_player[i].heart_num > 0)
            gg = false;
    if(gg)
        gameOver();
}


void MainGame::gameOver()
{
    // 游戏结束之后需要判断是否记录
    if(m_player_num == 1)
        record_single.operation(m_player[0].score);
    else
        record_double.operation(m_player[0].score + m_player[1].score);

    auto result = QMessageBox::question(this, "游戏结束", "本次游戏结束\n是否重新开始", QMessageBox::Yes|QMessageBox::No);

    m_timer.stop();
    this->close();

    if(result == QMessageBox::Yes){
        auto p = (MainWindow *)parentWidget();
        p->restartGame(m_player_num);
    }
}

