
#pragma once

#include <windows.h>
typedef enum // 等级从低到高排列
{
    Normal,
    Silver,
    Gold,
    Platinum,
    Diamond,
} Level;


typedef enum
{
    Invalid, // 挂失
    Valid,
} AccountState;

typedef enum
{
    Register,  // 注册
    LogIn,      // 登录
    LogOut,     // 登出
    Consumption,    // 正常消费
    Charge,     // 续费
    ChangePassword, // 改密码
    ChangeLevel, // 改等级
    Refund, // 退卡
    ReportLoss, // 挂失
    Recover, // 挂失后恢复
} Operation;

typedef enum
{
    HEAD, //头结点
    COMMON_NODE, //一般节点
    ADMIN, // 管理员 但实际上不在链表中
} NodeType;


#define PASSWORD_MAX_LENGTH 20
#define NAME_MAX_LENGTH 20
#define INDICES_INITIAL_LENGTH 50
#define RECORD_INITIAL_LENGTH 100
#define SAFEGUARD_MAX_LENGTH 20

typedef struct{
    int id; // 序号
    char name[NAME_MAX_LENGTH + 1]; // 名
    int op_count; //索引个数
    int op_capacity; // 当前索引最大个数
    //int indices[INDICES_INITIAL_LENGTH + 1]; // 大表中的索引
    int *indices; // 为了realloc
    char password[PASSWORD_MAX_LENGTH + 1]; // 密码
    Level lvl; // 等级
    double money; //余额
    double sum; // 累计充值数
    int register_time; // 注册时间
    AccountState validity; // 有效性
    int question_choose; //选择的密保问题编号
    char secretguard[SAFEGUARD_MAX_LENGTH + 1]; // 密保 存放密保问题的答案
} User;

typedef struct Node{
    NodeType type;  // 分头结点，一般节点
    union{
        User user; //对于一般节点记录用户
        int size;
    };
    struct Node *next;
} Node;


typedef struct{
    User user;
    Operation op;
    int time; // 操作的时间
    double change; // 余额的变化
    AccountState validity;  // 表示操作记录的有效性，直接用账号有效性了（一般是有效的，退卡后无效）
} Row; // 大表的每行

typedef struct{
    //Row rows[RECORD_INITIAL_LENGTH];
    Row *rows; // 为了realloc
    int size;
    int capacity; 
} Record; //大表

