#include "structure.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "tools.h"
#include "string.h"

extern int key;
extern int ID;
extern int loginday;
extern int day_lasttime;
extern Record *record;
extern Node *head;
extern const char *level_name[];
extern const char *operation_name[];
extern const int admin_id;
extern const char admin_pwd[];
extern const COORD MenuPosition[];
extern const int DAYS_EACH_MONTH[12];

/**
 * 新建大表
 */
Record* create_record(void){
    Record *t = (Record *)malloc(sizeof(Record));
    t->size = 0;
    t->capacity = RECORD_INITIAL_LENGTH;
    t->rows = (Row *)malloc(sizeof(Row) * t->capacity);
    return t;
}


/**
 * 新建头结点
 */
Node* create_head(void){
    Node *t = (Node *)malloc(sizeof(Node));
    t->type = HEAD;
    t->size = 0;
    t->next = NULL;
    return t;
}

extern int ID;
/**
 * 新建空节点，并赋id
 */
Node* create_new_empty_user(void){
    Node *t = (Node *)malloc(sizeof(Node));

    t->user.indices = (int *)malloc(sizeof(int) * INDICES_INITIAL_LENGTH);
    memset(t->user.indices, -1, sizeof(int) * INDICES_INITIAL_LENGTH);
    t->user.lvl = Normal;
    t->user.money = 0;
    t->user.sum = 0;
    t->user.validity = Valid;

    t->type = COMMON_NODE;
    t->user.op_count = 0;
    t->user.op_capacity = INDICES_INITIAL_LENGTH;
    t->user.id = ID++;
    t->next = NULL;
    return t;
}


/**
 * 初始化姓名
 */
static bool initialize_name(char *c){
    char buffer[100];
    printf("请输入会员姓名(不超过%d个字符):\n", NAME_MAX_LENGTH);
    //gets(buffer);
    scanf("%s", buffer);
    size_t len = strlen(buffer);
    if(len >= NAME_MAX_LENGTH){
        printf("请不要超过%d个字符!\n", NAME_MAX_LENGTH);
        return false;
    }
    memcpy(c, buffer, len + 1);
    return true;
}

/**
 * 初始化密码 （读入密码）
 */
static bool initialize_pwd(char *c){
	char buffer[100];
    printf("请输入会员卡密码(不超过%d个字符):\n", PASSWORD_MAX_LENGTH);
    //gets(buffer);
    scanf("%s", buffer);
    size_t len = strlen(buffer);
    if(len >= PASSWORD_MAX_LENGTH){
        printf("请不要超过%d个字符!\n", PASSWORD_MAX_LENGTH);
        return false;
    }
    memcpy(c, buffer, len + 1);
    return true;
}


/**
 * 初始化注册时间
 * @param time Node中user的register_time
 */
bool initialize_register_time(int *time){
    Date date;
    if(!read_date(&date))
        return false;
    *time = date2time(date);
    return true;
}

/**
 * 设置密保问题
 * @param num 问题序号
 * @param c  答案内容
 */
bool initialize_secretguard(int *num,char *c)
{   
    printf("请输入您选择的密保问题：\n");
    printf("1.您父亲的姓名\n");
    printf("2.您母亲的姓名\n");
    printf("3.您小学班主任的姓名\n");
    printf("4.您最近一次旅游去过的地方\n");
    
    int choose;

    while(!read_int(&choose, NULL, "请输入1~4之间的整数!\n请重新输入：")){
        if(choose > 4 || choose < 1){
            printf("请输入1~4之间的整数!\n");
            printf("请重新输入：");
        }
    }

    *num = choose;

    char buffer[100];
    printf("请输入您的密保问题答案(不超过%d个字符):\n", PASSWORD_MAX_LENGTH);
    //gets(buffer);
    scanf("%s", buffer);
    size_t len = strlen(buffer);
    if (len >= PASSWORD_MAX_LENGTH) {
        printf("请不要超过%d个字符!\n", PASSWORD_MAX_LENGTH);
        return false;
    }
    memcpy(c, buffer, len + 1);
    return true;
}

/**
 * 对于空node 初始化
 * @param node 指向待初始化的node指针
 */
bool initialize_user(Node* node) {
    if (initialize_register_time(&node->user.register_time)) {
        if (node->user.register_time < day_lasttime) {
            printf("您输入的时间早于上次操作时间，有误！");
            return false;
        }
        else {
            loginday = node->user.register_time;
            day_lasttime = node->user.register_time;
        }
    }
    else {
        return false;
    }
    if (initialize_name(node->user.name)
        && initialize_pwd(node->user.password)
        && initialize_secretguard(&node->user.question_choose,node->user.secretguard)) {
        return true;
    }
    return false;
    
}

/**
 * 插入用户到链表，头插
 * @param head 链表头结点
 * @param user 待插入的用户
 */
void add_new_user(Node **head, Node *user){
    user->next = (*head)->next;
    (*head)->next = user;
    (*head)->size++;
}

/**
 * 根据id查找用户, 返回node*不是直接的user
 * @param head 链表头结点
 * @param id 用户id
 */
Node* find_user_by_id(Node *head, int id){
    Node *t = head->next;
    while(t){
        if(t->user.id == id){
            return t;
        }
        t = t->next;
    }
    return NULL;
}

/**
 * 改变（增大）用户indices的容量
 * @param u 指向待修改用户的user *
 */
void resize_user_indices(User *u){
    // node *版本
    // 默认将容量翻倍
    // u->user.op_capacity *= 2;
    // int *t = u->user.indices;
    // do {
    //     u->user.indices = (int *)realloc(u->user.indices, sizeof(int) * u->user.op_capacity);
    //     if(u->user.indices){
    //         printf("用户索引记录扩容成功！\n");
    //         t = NULL;
    //     }else{
    //         printf("用户索引记录扩容失败！\n");
    //         u->user.indices = t;
    //     }
    // } while (t); // 扩容成功的话 t==NULL， 结束循环，没成功就一直尝试
    u->op_capacity *= 2;
    int *t = u->indices;
    do {
        u->indices = (int *)realloc(u->indices, sizeof(int) * u->op_capacity);
        if(u->indices){
            printf("用户索引记录扩容成功！\n");
            t = NULL;
        }else{
            printf("用户索引记录扩容失败！\n");
            u->indices = t;
        }
    } while (t); // 扩容成功的话 t==NULL， 结束循环，没成功就一直尝试
}

/**
 * 改变（增大）大表的容量
 * @param rec 指向待修改的record
 */
void resize_record(Record *rec){
    // 默认将容量翻倍
    rec->capacity *= 2;
    Row *t = rec->rows;
    // 与 resize_user_indices 类似
    do {
        rec->rows = (Row *)realloc(rec->rows, sizeof(Row) * rec->capacity);
        if(rec->rows){
            printf("大表扩容成功！\n");
            t = NULL;
        }else{
            printf("大表扩容失败！\n");
            rec->rows = t;
        }
    } while (t);
}

/**
 * 给用户增加操作的条数
 * 和 update_record 搭配使用
 */
void add_user_op_index(Node *u){
    if (u->user.op_count == u->user.op_capacity)
        resize_user_indices(&(u->user));
    u->user.indices[u->user.op_count++] = record->size;
}

/**
 * 复制一份user
 * @param x 目标
 * @param u 源数据
 */
static void clone_user(User *x, User *u){
    x->id = u->id;
    x->lvl = u->lvl;
    x->money = u->money;
    x->sum = u->sum;
    x->op_count = u->op_count;
    x->validity = u->validity;
    x->op_capacity = u->op_capacity;
    x->register_time = u->register_time;
    strcpy(x->name, u->name);
    strcpy(x->password, u->password);
    x->indices = (int *)malloc(sizeof(int) * u->op_capacity);
    x->question_choose = u->question_choose;
    strcpy(x->secretguard, u->secretguard);
    memcpy(x->indices, u->indices, sizeof(int) * u->op_capacity);
}

/**
 * 更新大表
 */
void update_record(Record *rec, User u, Operation op, int time, double change){
    if(rec->size == rec->capacity)
        resize_record(rec);
    clone_user(&(rec->rows[rec->size].user), &u);
    rec->rows[rec->size].op = op;
    rec->rows[rec->size].change = change;
    rec->rows[rec->size].time = time;
    rec->rows[rec->size].validity = Valid;
    rec->size++;
}

/**
 * 显示用户信息
 * 用于 用户查看个人信息 和 管理员查看用户列表
 * 根据debug_user_info暂定
 * 参数c用来调整输出形式，'\n'是每个用户属性占一行 '\t‘每个用户占一行，属性间为若干空格
 */
void user_iter(User *u, const char c){
    
    Date ret = civil_from_days(u->register_time);

    printf("id = %05d%c" 
           "姓名 = %s%c"
           "密码 = %s%c"
           "余额 = %.2f%c"
           "累计 = %.2f%c"
           "注册时间 = %d-%d-%d%c"
           "有效性 = %s%c",
           u->id, c, u->name, c, u->password, c, u->money, c, u->sum, c,
           ret.year, ret.month, ret.day, c, (u->validity ? "YES" : "NO"),
           c);
    printf("等级 = %s%c", level_name[(int)u->lvl], c);
    printf("密保问题 = %d%c密报答案 = %s\n", u->question_choose, c, u->secretguard);
}



/**
 * 类似 user_iter
 * 用来看record的每一条
 */
void row_iter(Row *row){
    Date ret = civil_from_days(row->time);
    user_iter(&(row->user), ' ');

    printf("%s ", operation_name[(int)(row->op)]);
    printf("操作时间 = %d-%d-%d ", ret.year, ret.month, ret.day);
    printf("余额变化 = %.2f ", row->change);
    printf("有效性 = %s\n", (row->validity ? "YES" : "NO"));
    printf("\n");
}