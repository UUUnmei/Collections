#ifndef CONFIG_H
#define CONFIG_H
// 游戏配置文件

constexpr int WINDOW_WIDTH = 512;
constexpr int WINDOW_HEIGHT = 768;

constexpr auto GAME_TITEL = "PlaneWar";
constexpr auto GAME_RES_PATH = "./plane.rcc";
constexpr auto GAME_ICO_PATH = ":/resource/app.ico";
constexpr int GAME_FPS = 10;       // 多少毫秒刷新一次画面

constexpr auto MAP_PATH = ":/resource/background1.png";
constexpr auto MAP_PATH_2 = ":/resource/background2.png";
constexpr int MAP_SCROLL_SPEED = 2;

constexpr auto PLANE_PATH1 = ":/resource/icons8-fighter-96.png";
constexpr auto PLANE_PATH2 = ":/resource/icons8-fighter-96-2.png";
constexpr int PLANE_SPEED = 10;

constexpr auto BULLET_PATH = ":/resource/bullet2.png";
constexpr int BULLET_SPEED = 10;
constexpr int BULLET_MAX_NUM = 30;

constexpr auto ENEMY_PATH1 = ":/resource/icons8-fighter-jet-80 (1).png";
constexpr int ENEMY_SPEED = 5;
constexpr int ENEMY_MAX_NUM = 20;
constexpr int ENEMY_INTERVAL = 30; // 敌机出场间隔
constexpr int ENEMY_FIRE_INTERVAL = 20; // 敌机开火间隔
constexpr auto ENEMY_BULLET_PATH = ":/resource/enemy_bullet1.png";

#define BOMB_PATH ":/resource/bomb-%1.png"
constexpr int BOMB_NUM = 20;    // 最大爆炸数量
constexpr int BOMB_MAX = 7;        // 爆炸效果的图片数
constexpr int BOMB_INTERVAL = 20;   // 更新爆炸图片的间隔

constexpr int HEART_WIDTH = 40; // 红心宽度
constexpr auto HEART_PATH = ":/resource/heart_2.png";
constexpr auto HEART_LOSE_PATH = ":/resource/heart_lose_2.png";
constexpr int LIFE = 3;    //初始血量
constexpr int SCORE = 0;    //得分

constexpr auto COUNT_DOWN_GIF_PATH = ":/resource/321_transparent.gif";


constexpr auto GRADE_PATH_1 = "Grade1.txt";
constexpr auto GRADE_PATH_2 = "Grade2.txt";

constexpr auto BOSS_PATH =":/resource/boss.png";
constexpr int BOSS_LIFE = 20;   //boss血量
constexpr int IMPACT_WIDTH = 30;    //打击点宽度
constexpr int IMPACT_HEIGHT = 28;   //打击点高度

constexpr int HEART_INTERVAL = 400; // 血包最多停留4s
constexpr int ENEMY_BLOOD_PACK = 3; // 该飞机被打死之后会掉落血包

constexpr int ENEMY_DOUBLE_BULLET = 1; // 该飞机被打死后会掉落双枪包
constexpr int DOUBLE_INTERVAL = 300; // 双枪攻击最多停留3s
// 双枪包的图标
constexpr auto DOUBLE_BULLET_PATH = ":/resource/icons8-airport-64.png";


constexpr auto BLACK_BETWEEN_CHECK_POINT = ":/resource/black_2.gif";

#endif // CONFIG_H
