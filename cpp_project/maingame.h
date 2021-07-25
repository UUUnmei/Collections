#ifndef MAINGAME_H
#define MAINGAME_H


#include <QTimer>
#include "map.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "bomb.h"
#include "record.h"
#include "boss.h"
#include "blood_pack.h"
#include "double_bullet.h"
#include <QMainWindow>

#include <QCloseEvent>


class MainGame : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainGame(int player_number, QWidget *parent = nullptr);  // 也许这个参数可以 把个数直接换成 Player的vector之类的
    ~MainGame();


    // 初始化窗口
    void initWindow();

    // 游戏启动
    void playGame();
    // 更新所有元素坐标
    void updateAll();
    // 绘制   函数名不能改
    void paintEvent(QPaintEvent *);
    // 键盘事件 控制飞机移动
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    // 敌机出场
    void enemyToScene();
    // 碰撞检测
    void collisionDetection();

    // 游戏退出、结束
    void gameOver();

    // 检查是否要结束
    void isGameOver();


    // 进入下一关
    void pass();

    // 初始化部分配置
        void InitScene();

    // 存储数据
    record record_single;
    record record_double;

    // 地图
    Map m_map;

    // 第几张地图（就两张来回换。。）
    int m_map_id;


    //玩家个数
    int m_player_num;
    // 玩家飞机
    Player m_player[2];
    // 敌机
    Enemy m_enemys[ENEMY_MAX_NUM];

    int m_recorder;
    // 定时器
    QTimer m_timer;

    // 爆炸效果
    Bomb m_bombs[BOMB_NUM];

    // 红心
    QPixmap m_heart;
    // 黑白心
    QPixmap m_heart_lose;

    // 敌机是否入场
    bool enemy_appear;

    // boss出场倒计时
    int count_down;

    // boss
    Boss m_boss;

    // 血包
    blood_pack bloodpack;

    // 上一架空闲飞机
    int last;

    // 双枪包
    double_bullet doublebullet;

    // 是否处于两个关卡之间的阶段 0 不在 >= 1在
    int between_check_point;

    // 用于恢复父窗口
    void closeEvent(QCloseEvent *);
signals:
    void ExitWin();
};

#endif // MAINGAME_H
