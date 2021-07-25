
#include "lib.h"
#include "tools.h"
#include <stdio.h>
#include "structure.h"

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
 * 用户注册
 */
bool user_register(void){
    Node *t = create_new_empty_user();
    if(initialize_user(t)){
        add_new_user(&head, t);
        printf("你的id是：%d\n", t->user.id);
        printf("您的密码是：%s\n", t->user.password);
        printf("望牢记以上信息\n");
        add_user_op_index(t);
        update_record(record, t->user, Register, t->user.register_time, 0);
        return true;
    }
    free(t);
    return false;
}

static bool findback_pwd(Node *p)
{   
    printf("请输入以下问题的答案：\n");
    switch (p->user.question_choose){
    case 1: {
        printf("您父亲的姓名\n");
        break;
    }
    case 2: {
        printf("您母亲的姓名\n");
        break;
    }
    case 3: {
        printf("您小学班主任的姓名\n");
        break;
    }
    case 4: {
        printf("您最近一次旅游去过的地方\n");
        break;
    }
    default:
        break;
    }
    char buffer[100];
    scanf("%s", buffer);
    if (strlen(p->user.secretguard)!=strlen(buffer) || strncmp(buffer, p->user.secretguard, strlen(p->user.secretguard))) {
        printf("密保问题错误！\n");
        return false;
    }
    else {
        printf("您的密码是：%s\n", p->user.password);
        printf("请及时更换您的密码！\n");
        return true;
    }
}
// 挂失后恢复
void recover(Node* p) {
    p->user.validity = Valid;
    add_user_op_index(p);                      
    update_record(record, p->user, Recover, loginday, 0);
}


bool findback_id(int *id){   
    int cnt = 0; // 记录该姓名的个数
    Node* t[100];
    printf("请输入您的姓名：");
    char buffer[100];
    scanf("%s", buffer);

    for (Node* temp = head->next; temp; temp = temp->next) {
        if (strlen(temp->user.name)==strlen(buffer) && !strncmp(buffer, temp->user.name, strlen(temp->user.name))) {
            if (!cnt) { // 第一次找到的时候
                printf("找到符合的ID如下：\n");
            }
            t[cnt++] = temp;
            printf("%d : ID = %d\n", cnt, temp->user.id);
        }
    }

    if (!cnt) { // 未找到
        printf("未找到与您姓名匹配的ID!\n");
        return false;
    }
    else { // 多个匹配
        printf("请输入您注册时选择的密保问题：\n");
        printf("1.您父亲的姓名\n");
        printf("2.您母亲的姓名\n");
        printf("3.您小学班主任的姓名\n");
        printf("4.您最近一次旅游去过的地方\n");
        int choose;
        read_int(&choose, NULL, NULL);

        printf("请输入您的密保问题答案：\n");
        char buffer[100];
        scanf("%s", buffer);
        for (int i = 0; i < cnt; i++) {
            if ((t[i]->user.question_choose == choose)
                && (strlen(t[i]->user.secretguard)==strlen(buffer) && !strncmp(buffer, t[i]->user.secretguard, strlen(t[i]->user.secretguard))) ){
                printf("\n找回您的ID为 ：%d\n", t[i]->user.id);
                *id = t[i]->user.id;
                return true;
            }
        }
        printf("密保问题或答案匹配失败！\n");
        return false;
    }
}

/**
 * 登录
 * @return 返回登录的账号id
 */
int log_in(NodeType type) {
    Date d;

    int id;
    if (!read_date(&d))
        return 0; //登录失败

    if (date2time(d) < day_lasttime) {
        printf("您输入的时间早于上次操作时间，有误！");
        return 0;
    }
    else {
        loginday = date2time(d);
        day_lasttime = date2time(d);
    }

    if (!read_int(&id, "请输入登录id：\n", "输入不合法!\n")) {
        return 0;
    } 
    // else if (id > head->size && id != admin_id) {
    //     printf("该id号不存在！\n");
    //     return 0;
    // }

    if (type == ADMIN) {
        if (id == admin_id) {
            char buffer[100];
            printf("请输入管理员密码：\n");
            scanf("%s", buffer);
            if (strlen(admin_pwd)!=strlen(buffer) || strncmp(buffer, admin_pwd, strlen(admin_pwd))) {
                printf("密码错误！\n");
                return 0;
            }
            return id; // 管理员登录
        }
        else {
            printf("id错误！\n");
            return 0;
        }
    }

    Node* t = find_user_by_id(head, id);
    if (!t) {
        int choose;
        printf("id错误！\n");
        printf("您是否想要找回ID？  1-是 0-否\n");
        if (!read_int(&choose, NULL, NULL) || !choose) { // 选择 否
            return 0;
        }
        if (!findback_id(&id)) { //找回ID失败
            return 0;
        }
        else {
            t = find_user_by_id(head, id);
        }
        
    }
    if (t->user.validity == Invalid) {
        printf("会员卡已失效，无法登录\n");
        return 0;
    }

    char buffer[100];
    printf("请输入您的会员卡密码：\n");
    scanf("%s", buffer);
    if (strlen(t->user.password)!=strlen(buffer) || strncmp(buffer, t->user.password, strlen(t->user.password))) {
        printf("密码错误！\n");
        printf("您是否想要通过密保找回密码？  1-是 0-否\n");
        int choose;

        if (!read_int(&choose, NULL, NULL) || !choose) {
            return 0;
        }else {
            if (!findback_pwd(t)) { // 密保问题回答错误
                return 0; 
            }
            else { //再给一次登录的机会
                printf("\n请输入您的会员卡密码：\n");
                scanf("%s", buffer);
                if (strlen(t->user.password)!=strlen(buffer) || strncmp(buffer, t->user.password, strlen(t->user.password))) {
                    return 0;
                }
            }
        }
    }

    add_user_op_index(t);
    update_record(record, t->user, LogIn, loginday, 0);


    return id; // 用户登录
}


/**
 * 给用户退卡
 */
bool refund_card(void){

    int id;
    if(!read_int(&id, "请输入要退卡会员的id：\n", "输入不合法!\n"))
        return false; //登录失败
    if(id == admin_id){
        printf("管理员账号不可撤销\n");
        return false;
    }
    Node *t = head->next;
    Node *pre = head;
    while (t){
        if(t->user.id == id){
            break;
        }
        pre = t;
        t = t->next;
    }

    if(!t){
        printf("id不存在！\n");
        return false;
    }
    if(t->user.validity == Invalid){
        printf("会员卡处于不可操作状态（已挂失）\n");
        return false;
    }
    // 以上确认要退卡的id合法

    // 单链表删除
    pre->next = t->next;

    // 添加退卡的操作记录，但最后还是会变成无效的
    add_user_op_index(t);
    update_record(record, t->user, Refund, loginday, 0);
    // 对于退卡的用户， 其操作记录变为无效
    for (int i = 0; i < t->user.op_count;++i){
        record->rows[t->user.indices[i]].validity = Invalid;
        record->rows[t->user.indices[i]].user.validity = Invalid;
    }
        
    // 无需让其账号变成Invalid， 毕竟已经没了。。
    free(t);
    return true;
}

/**
 * 挂失(管理员端)
 */
bool admin_report_loss(void){

    int id;
    if(!read_int(&id, "请输入要挂失会员的id：\n", "输入不合法!\n"))
        return false; //登录失败
    if(id == admin_id){
        printf("管理员账号不可挂失\n");
        return false;
    }

    Node *t = find_user_by_id(head, id);

    if(!t){
        printf("id不存在！\n");
        return false;
    }
    if(t->user.validity == Invalid){
        printf("会员卡处于不可操作状态（已挂失）\n");
        return false;
    }
    // 以上确认要挂失的id合法

    add_user_op_index(t);
    update_record(record, t->user, ReportLoss, loginday, 0);

    t->user.validity = Invalid;
    return true;
}

/**
 * 挂失（用户端）
 * @param u 用户
 */
bool client_report_loss(Node *u){
    if(u->user.validity == Invalid){ // 要挂失的对象已经挂失了，正常来说不会发生（由登录保证）
        printf("会员卡处于不可操作状态（已挂失）\n");
        return false;
    }

    u->user.validity = Invalid;
    add_user_op_index(u);
    update_record(record, u->user, ReportLoss, loginday, 0);
    return true;
}

/**
 * 续费
 * @param u 用户
 * @param money 金额
 * @param time 时间
 */
void recharge(Node* u, double money) {
    u->user.money += money;//余额增加
    u->user.sum += money;//总额增加
    printf("您本次充值%.2f元。\n",money);
    printf("现在卡里余额%.2f元。\n",u->user.money);
    add_user_op_index(u);
    update_record(record, u->user, Charge, loginday, money);
    if(change_Level(u)){
        add_user_op_index(u);
        update_record(record, u->user, ChangeLevel, loginday, 0);
    }
}
//根据用户等级确定折扣
static double discount(Level lvl) {
    double disc;
    switch (lvl) {
    case Diamond:
        disc = 0.6; break;
    case Platinum:
        disc = 0.7; break;
    case Gold:
        disc = 0.8; break;
    case Silver:
        disc = 0.9; break;
    default: disc = 1;
    }
    return disc;
}


/**
 * 消费
 * @param u 用户
 * @param money 金额
 */
void consume(Node* u, double money) {
    double disc = discount(u->user.lvl);
    if (money * disc > u->user.money) {
        printf("余额不足，请充值！\n");
        int choose = 0;
        bool flag = true; // 第一次为true, 其余情况为false
        bool isrecharge = false;
        while (1){
            if(flag){
                flag = false;
            }else{
                printf("您的输入有误！\n");
            }
            printf("请进行选择：\n");
            printf("1.立即充值\n");
            printf("2.退出\n");
            scanf("%d",&choose);
            if( choose == 1 ){
                double some;
                if(read_double(&some, "请输入续费金额：\n", "金额不合法：\n")){
                    recharge(u, some);
                    isrecharge = true;
                }
                break;
            }else if( choose == 2 ) {
                break;
            }
        }
        if(!isrecharge){
            return;
        }
    }
    u->user.money -= money * disc;//余额减少
    printf("您本次购买所有物品的总价格为%.2f元。\n",money);
    printf("您的等级为%s,本次共打折%.2f元。\n",level_name[u->user.lvl],money*(1-disc));
    printf("您本次共花费%.2f元。\n",money*disc);
    printf("您现在卡里的余额为%.2f元。\n",u->user.money);
    add_user_op_index(u);
    update_record(record, u->user, Consumption, loginday, -money * disc);
}

/**
 * (ver 1.0)
 * 先对于链表的结点进行ID的匹配
 * 如果存在该ID，则给予用户至多三次机会
 * 修改与之前不相同的密码，返回true；其余情况返回false
 * @param head 链表头结点
 * @param id_input 用户输入的id
 */
// bool change_password(Node* head,int id_input){   

//     Node *p = find_user_by_id(head, id_input);
    
//     if(!p){ // 没找到
//         printf("未找到您输入的ID\n修改密码失败!\n");
//         return false;
//     }

/**
 * (ver 2.0)
 * 由操作流程保证待修改用户存在
 * @param p 指向用户的node指针
 */
bool change_password(Node *p){

    char password_input[100];
    bool flag = false;
    for (int cnt = 0; cnt < 3; cnt++){
        if (cnt){
            printf("输入密码有误！\n");
        }
        printf("请输入您的会员卡的原密码：\n");
        scanf("%s", password_input);
        if (strlen(p->user.password)==strlen(password_input) && !strncmp(password_input, p->user.password, strlen(p->user.password))){
            flag = true;
            break;
        }
    }
    if (!flag){ // 找到但是原密码错误
        printf("输入密码有误！\n输入密码次数达到3次！修改密码失败!\n");
    }else{ //说明找到了对应的ID，且密码匹配成功
        printf("请输入您的会员卡的新密码：\n");
        scanf("%s",p->user.password);
        if(strlen(p->user.password)==strlen(password_input) && !strncmp(password_input, p->user.password, strlen(p->user.password))){
            flag = false; // 新设置的密码与原密码相同，修改密码失败
            printf("新密码与原密码相同\n修改密码失败!\n");
        }else{
            printf("修改密码成功！\n");
        }
    } 
    return flag;
}

/**
 * 先利用链表的结点进行ID的匹配
 * 根据如今的续费总额划分如今的等级，并与原等级进行判断
 * 等级改变返回true；等级未发生改变返回false
 * @param head 链表头结点
 * @param id 用户的id
 */
bool change_Level(Node* p){   
    bool flag = false; //如果没有变等级，返回false；如果变了等级，返回true 
    Level newlevel;

    if( p->user.sum <= 500){
        newlevel = Normal;
    }else if( p->user.sum <= 1000){
        newlevel = Silver;
    }else if( p->user.sum <= 2000){
        newlevel = Gold;
    }else if( p->user.sum <= 5000){
        newlevel = Platinum;
    }else{
        newlevel = Diamond;
    }

    if( p->user.lvl == newlevel){
        printf("等级没有发生改变\n");
        flag = false;
    }else{
        flag = true;
        printf("等级由%s转变为%s\n",level_name[p->user.lvl],level_name[newlevel]);
        p->user.lvl = newlevel;
    }
    
    return flag;
}


/*********************************************** 排   序 ***********************************************/
// 比较id大小 升
static bool cmp_id(const User *a, const User *b){
    return a->id <= b->id;
}
// 比较id大小 降
static bool cmp_id_r(const User *a, const User *b){
    return a->id >= b->id;
}
// 比较名字长度 升
static bool cmp_name_length(const User *a, const User *b){
    return strlen(a->name) <= strlen(b->name);
}
// 比较名字长度 降
static bool cmp_name_length_r(const User *a, const User *b){
    return strlen(a->name) >= strlen(b->name);
}
// 比较名字字典序 升
static bool cmp_name_lex(const User *a, const User *b){
    int r = strcmp(a->name, b->name);
    if(r <= 0)
        return true;
    return false;
}
// 比较名字字典序 降
static bool cmp_name_lex_r(const User *a, const User *b){
    int r = strcmp(a->name, b->name);
    if(r >= 0)
        return true;
    return false;
}
// 比较索引个数 升
static bool cmp_op_count(const User *a, const User *b){
    return a->op_count <= b->op_count;
}
// 比较索引个数 降
static bool cmp_op_count_r(const User *a, const User *b){
    return a->op_count >= b->op_count;
}
// 比较注册时间 升
static bool cmp_reg_time(const User *a, const User *b){
    return a->register_time <= b->register_time;
}
// 比较注册时间 降
static bool cmp_reg_time_r(const User *a, const User *b){
    return a->register_time >= b->register_time;
}
// 比较等级 升
static bool cmp_lvl(const User *a, const User *b){
    return (int)a->lvl <= (int)b->lvl;
}
// 比较等级 降
static bool cmp_lvl_r(const User *a, const User *b){
    return (int)a->lvl >= (int)b->lvl;
}
// 比较累计充值 升
static bool cmp_tot_charge(const User *a, const User *b){
    return a->sum <= b->sum;
}
// 比较累计充值 降
static bool cmp_tot_charge_r(const User *a, const User *b){
    return a->sum >= b->sum;
}
#define NUMBER_OF_ALLOWED_SORTING_STANDARD 7

// 排序的函数指针数组
static bool (*f[15])(const User *, const User *) = {cmp_id, cmp_name_length, cmp_name_lex, cmp_op_count, cmp_reg_time, cmp_tot_charge, cmp_lvl,
                                             cmp_id_r, cmp_name_length_r, cmp_name_lex_r, cmp_op_count_r, cmp_reg_time_r, cmp_tot_charge_r, cmp_lvl_r};

/**
 * 一次归并排序
 * @param pu 实际存数据的user *数组
 * @param pt 辅助空间
 * @param l 排序左边界 （范围 0 <= x <= n-1)
 * @param r 排序右边界
 * @param f 指定的函数指针
 */
static void mergeSort(User *pu[], User *pt[],  int l, int r, bool (*f)(const User *, const User *)){
    if(l>=r){
        return;
    }
    int mid = (l + r) / 2;
    mergeSort(pu, pt, l, mid, f);
    mergeSort(pu, pt, mid + 1, r, f);

    int p1 = l, p2 = mid + 1, p3 = l;
    while(p1 <= mid && p2 <= r){
        if(f(pu[p1], pu[p2])){  
            pt[p3++] = pu[p1++];
        }else{
            pt[p3++] = pu[p2++];
        }
    }
    while(p1 <= mid)
        pt[p3++] = pu[p1++];
    while(p2 <= r)
        pt[p3++] = pu[p2++];

    for (int i = l; i <= r;++i)
        pu[i] = pt[i];
}


// 多条件排序
void combinedSort(void){
    printf("可供排序的关键字： 1、id 2、姓名长度 3、姓名字典序 4、总操作数 5、注册时间 6、总充值金额 7、等级\n");

    printf("请勿多次对同一关键字排序（仅以第一次出现的顺序为准）\n");
    printf("请输入选择的排序关键字的个数：（不超过7项）\n");
    int m;
    if(read_int(&m, NULL, "请输入1~7的整数！\n")){
        // 输入范围非法
        if(m<1 || m>7){
            printf("请输入1~7的整数！\n");
            return;
        } 
    }

    printf("以数字前+/-的形式表示升降序，条件间以空格分隔，主条件在前\n");
    printf("如 -2 +1 表示: 姓名长度长的在前，若长度相同则id小的在前\n");
    printf("请输入：\n");

    int order[10];
    for (int i = 0; i < m;++i){
        if(!read_int(&order[i], NULL, NULL) || order[i]<-7 || order[i]>7 || order[i]==0){
            // 输入范围非法
            printf("输入错误，排序失败！\n");
            return;
        }
    }

    // 反过来， 不然其实是主条件在后。。同一条件最后的有效。。。
    for (int i = 0; i < m / 2;++i){
        int t = order[i];
        order[i] = order[m - 1 - i];
        order[m - 1 - i] = t;
    }

    // 准备当前用户链表对应的user *数组
    User **pu, **pt;
    pu = (User **)malloc(sizeof(User *) * head->size); // 主要的存储位置
    pt = (User **)malloc(sizeof(User *) * head->size); // 辅助数组，额外空间
    
    Node *t = head->next;
    for (int i = 0; i < head->size;++i){
        pu[i] = &(t->user);
        t = t->next;
    }

    // 排序
    for (int i = 0; i < m; i++){
        mergeSort(pu, pt, 0, head->size - 1, f[(order[i] > 0 ? 
                                                    order[i] - 1
                                                    : NUMBER_OF_ALLOWED_SORTING_STANDARD - order[i] - 1)]);
    }

    // 输出排序结果
    for (int i = 0; i < head->size; ++i){
        user_iter(pu[i], '\t');
    }

    // 释放
    free(pu);
    free(pt);
}
/*********************************************** 排   序 ***********************************************/


/*********************************************** 统   计 ***********************************************/
bool count_summoney(int begin_day, int end_day,double *shouru,double *sum)
{
    int i;
    bool flag;
    //若最后的返回值为 0 ，则说明该时间段内没有购物的信息
    for (i = 0; i < record->capacity; i++) {
        if (record->rows[i].time >= begin_day) {
            for (int j = i; record->rows[j].time <= end_day; j++) {
                if (record->rows[j].op == Consumption) {
                    *shouru = *shouru - record->rows[j].change;
                    *sum = *sum - record->rows[j].change * 1.0 / discount(record->rows[j].user.lvl);
                }
            }
            break;
        }
    }
    if (*shouru) {
        flag = true;
    }
    else {
        flag = false;
    }
    return flag;
}
/*********************************************** 统   计 ***********************************************/


/*********************************************** 筛   选 ***********************************************/
static int screen_id(User** result, int len) {
    int a, b;
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选的ID范围(从小到大)：");
        scanf("%d%d", &a, &b);
        flag = true;
    } while (a > b);
    for (int i = 0; i < len; i++) {
        if (result[i]->id >= a && result[i]->id <= b)
            result[index++] = result[i];
    }
    return index;
}

static int screen_name_length(User** result, int len) {
    int a, b;
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选用户姓名长度的范围(从小到大)：");
        scanf("%d%d", &a, &b);
        flag = true;
    } while (a > b);
    for (int i = 0; i < len; i++) {
        if (strlen(result[i]->name) >= a && strlen(result[i]->name) <= b)
            result[index++] = result[i];
    }
    return index;
}

static int screen_op_counts(User** result, int len) {
    int a, b;
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选用户操作条目个数的范围(从小到大)：");
        scanf("%d%d", &a, &b);
        flag = true;
    } while (a > b);

    for (int i = 0; i < len; i++) {
        if (result[i]->op_count >= a && result[i]->op_count <= b)
            result[index++] = result[i];
    }
    return index;
}

static int screen_register_time(User** result, int len) {
    int a, b;
    //用来存储转换后的天数
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选用户注册时间的范围(从先到后)：");
        while (!initialize_register_time(&a)) {
        }
        while (!initialize_register_time(&b)) {
        }
        flag = true;
    } while (a > b);

    for (int i = 0; i < len; i++) {
        if (result[i]->register_time >= a && result[i]->register_time <= b)
            result[index++] = result[i];
    }
    return index;
}

static int screen_sum_of_charge(User** result, int len) {
    double a, b;
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选用户总充值金额的范围(从小到大)：");
        scanf("%lf%lf", &a, &b);
        flag = true;
    } while (a > b);
    for (int i = 0; i < len; i++) {
        if (result[i]->sum >= a && result[i]->sum <= b)
            result[index++] = result[i];
    }
    return index;
}

static int screen_level(User** result, int len) {
    int a, b;
    int index = 0;
    bool flag = false;
    do {
        if (flag) {
            printf("您的输入顺序有误！请重新输入！\n");
        }
        printf("请输入您要筛选用户等级的范围(从低到高)：");
        printf("等级为：0 -> Normal,1 -> Silver,2 -> Gold,3 -> Platinum,4 -> Diamond\n");
        scanf("%d%d", &a, &b);
        flag = true;
    } while (a > b);

    for (int i = 0; i < len; i++) {
        if (result[i]->lvl >= a && result[i]->lvl <= b)
            result[index++] = result[i];
    }
    return index;
}

void screen() {
    User* result[100];
    Node* p = head->next;
    int len = head->size;
    for (int i = 0; i < len; i++) {
        result[i] = &(p->user);
        p = p->next;
    }

    int select, ifcontinue = 1, cnt = 0;
    // select 表示用户的选择->进行哪一种筛选
    // ifcontinue 表示用户是否继续进行排序-> 1 为继续 0 为 停止
    // cnt 表示用户筛选的次数-> 用来判断输出语句用

    do {
        printf("请选择筛选条件：\n");
        printf("1、id 2、姓名长度 3、总操作数 4、注册时间 5、总充值金额 6、等级\n");
        if (ifcontinue) {
            cnt++;
            printf("请选择第%d个筛选条件：\n", cnt);
            if (read_int(&select, NULL, "请输入1~6的整数！\n")) {
                // 输入范围非法
                if (select < 1 || select >6) {
                    printf("请输入1~6的整数！\n");
                    return;
                }
            }
            else {
                // 读入非法
                return;
            }
        }

        switch (select) {
        case 1:
            len = screen_id(result, len);
            break;
        case 2:
            len = screen_name_length(result, len);
            break;
        case 3:
            len = screen_op_counts(result, len);
            break;
        case 4:
            len = screen_register_time(result, len);
            break;
        case 5:
            len = screen_sum_of_charge(result, len);
            break;
        default: 
            len = screen_level(result, len);
        }
        for (int i = 0; i < len; i++) {
            user_iter(result[i], '\t');
        }
        printf("是否继续筛选？ 1-是 0-否\n");
        printf("请输入：\n");
        scanf("%d", &ifcontinue);
    } while (ifcontinue);
}
/*********************************************** 筛   选 ***********************************************/

