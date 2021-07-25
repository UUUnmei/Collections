#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structure.h"
#include <string.h>


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
 * 设置窗口 
 * @param cols 列（宽）
 * @param lines 行（高）
 */
void SetWindowSize(int cols, int lines)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      
    COORD winBufferSize = {cols, lines * 10};
    SetConsoleScreenBufferSize(Handle, winBufferSize);           // 设置窗口缓冲区大小（大于窗口，会有滚动条）

    SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = lines; 
    Rect.Right = cols; 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);           // 设置窗口大小
    system("title 会员卡管理系统");
    system("color F0");//调整下背景颜色
    system("cls");
}
/**
 * //设置光标位置
 * @param x 横 
 * @param y 纵
 */
void SetCursorPosition(const int x, const int y)
{
    COORD position;
    position.X = x * 2;
    position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

//设置文本颜色
void SetColor(int colorID)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                            colorID);
}
//设置文本背景色
void SetBackColor(void)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            0xE0
                            );
                        // 黄色
}


/**
 * 菜单中上下移动的更新
 * @param current_text 移动后位置上的内容
 * @param current_pos 移动后的位置
 * @param before_text 当前所处位置上的内容
 * @param before_pos 当前位置
 */
void update_text_after_move(const char *current_text, COORD current_pos,
                            const char *before_text, COORD before_pos){

    SetCursorPosition(current_pos.X, current_pos.Y);//给待选中项设置背景色
    SetBackColor();
    printf("%s", current_text);

    SetCursorPosition(before_pos.X, before_pos.Y);//将已选中项取消我背景色
    SetColor(240); // 默认白底黑字
    printf("%s", before_text);

}


/**
 * 读入一个double（非零）
 * @param ret 目标存储位置
 * @param input_msg 输入的提示语句(需要换行的话要自己加上)没有就NULL
 * @param error_msg 报错的提示语句(需要换行的话要自己加上)没有就NULL
 */
bool read_double(double *ret, const char *input_msg, const char *error_msg){
    if(input_msg)
        printf("%s", input_msg);
    char buffer[100];
    scanf("%s", buffer);
    double t;
    if (!(t=atof(buffer))){
        // 未能将输入转为double
        // 输入非法
        if(error_msg)
            printf("%s", error_msg);
        return false;
    }
    *ret = t;
    return true;
}

/**
 * 读入一个整数（非零）
 * @param ret 目标存储位置
 * @param input_msg 输入的提示语句(需要换行的话要自己加上)没有就NULL
 * @param error_msg 报错的提示语句(需要换行的话要自己加上)没有就NULL
 */
bool read_int(int *ret, const char *input_msg, const char *error_msg){
    double t;
    read_double(&t, input_msg, error_msg);
    if(fabs(t - (int)t) > 1e-5){  //如果足够接近就认为输入的是整数
        if(error_msg)
            printf("%s", error_msg);
        return false;
    }
    *ret = (int)t;
    return true;
}


/**
 * 返回从1970.01.01开始的天数（若日期在这之前则为负数）
 */
int days_from_civil(int year, int month, int day){
    year -= (month <= 2);
    int era = (year >= 0 ? year : year - 399) / 400;
    unsigned year_of_era = (unsigned)(year - era * 400);
    unsigned day_of_year = (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;
    unsigned day_of_era = year_of_era * 365 + year_of_era/4 - year_of_era/100 + day_of_year;
    return era * 146097 + (int)day_of_era - 719468;
}

/**
 * 根据天数返回年月日
 */
Date civil_from_days(int days){
    days += 719468;
    int era = (days >= 0 ? days : days - 146096) / 146097;
    unsigned day_of_era = (unsigned)(days - era * 146097);
    unsigned year_of_era = (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146096) / 365;
    int y = (int)year_of_era + era * 400;
    unsigned day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    unsigned mp = (5 * day_of_year + 2) / 153;
    unsigned d = day_of_year - (153 * mp + 2) / 5 + 1;
    unsigned m = mp + (mp < 10 ? 3 : -9);
    Date ret = {y + (m <= 2), m, d};
    return ret;
}


// 对于days_from_civil的简单包装
int date2time(Date date){
    return days_from_civil(date.year, date.month, date.day);
}


/**
 * 判断日期合法性
 * @param year (0, inf)
 * @param month [1, 12]
 * @param day depends
 */  
bool is_date_legal(int year, int month, int day){
    if(month>=1 && month<=12){
        int d = DAYS_EACH_MONTH[month - 1];
        // 闰年
        if(year>0 && ((year % 4 == 0 && year % 100) || (year % 400 == 0))) 
            d = 29;
        if(day>=1 && day<=d)
            return true;
    }
    return false;
}

/**
 * 读入日期
 * 以Date指针返回
 * 目前无需像read_int/double那样附加提示信息
 * 毕竟这里消息相对固定
 * 有需再改
 */
bool read_date(Date *date){
    int year, month, day;
    printf("请输入年月日，以空格分隔 如: 2021 3 10\n");
    //scanf("%d%d%d", &year, &month, &day);

    if(!(read_int(&year, NULL, NULL) && read_int(&month, NULL, NULL) && read_int(&day, NULL, NULL))){
        printf("输入日期有误！\n");
        return false;
    }

    if(!is_date_legal(year, month, day)){
        printf("输入日期有误！\n");
        return false;
    }
    date->year = year;
    date->month = month;
    date->day = day;
    return true;
}

extern int now_rows;
//默认存储位置"C:\\Users\\Administor\\Desktop\\用户信息.csv"

// 从当前文件夹下的 用户信息.csv 中读取
void load_info(void) {


    //FILE* fp = fopen("C:\\Users\\Administor\\Desktop\\用户信息.csv", "r+");//默认本地已存储用户信息
    FILE* fp = fopen(".\\用户信息.csv", "r+");//默认本地已存储用户信息
    
    if (!fp) {
        return;
    }//读取未成功
    char buffer[300];//暂存读入数据
    char* line;
    char* p[13];//存储指向各列数据的指针
    char* buf;//存储待分割字符串
    int user_id, column = 0;
    Node* pre;
    fseek(fp, 93L, SEEK_SET);//文件指针定位至文件头(跳过第一行)
    while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)//读至最后一行
    {
        //根据文件信息重建 record 和 会员链表

        char* TokenPtr = strtok_s(line, ",", &buf);
        p[0] = TokenPtr;
        user_id = atoi(TokenPtr);
        if(user_id > ID)
            ID = user_id + 1;
        while (p[column]) {
            p[++column] = strtok_s(NULL, ",", &buf);
        }
        p[11][strlen(p[11]) - 1] = '\0'; // 这个是密保答案，在大表每行的最后，实测会把最后的\n带上。这里把它去掉（换成\0）
        if (record->size == record->capacity) resize_record(record);//扩容
        record->rows[record->size].user.id = user_id;
        strcpy(record->rows[record->size].user.name, p[1]);
        strcpy(record->rows[record->size].user.password, p[2]);
        record->rows[record->size].user.lvl = atoi(p[3]);
        record->rows[record->size].user.money = atof(p[4]);
        record->rows[record->size].user.sum = atof(p[5]);
        record->rows[record->size].user.register_time = atoi(p[6]);
        record->rows[record->size].op = atoi(p[7]);
        record->rows[record->size].user.op_capacity = INDICES_INITIAL_LENGTH;
        record->rows[record->size].user.indices = (int *)malloc(sizeof(int) * INDICES_INITIAL_LENGTH);
        record->rows[record->size].user.op_count = 1;
        record->rows[record->size].user.validity = Valid;
        Node *t = find_user_by_id(head, user_id);
        if(t){
            record->rows[record->size].user.op_count = t->user.op_count;
            while(record->rows[record->size].user.op_count >= record->rows[record->size].user.op_capacity)
                resize_user_indices(&(record->rows[record->size].user));
            for (int i = 0; i < t->user.op_count;++i)
                record->rows[record->size].user.indices[i] = t->user.indices[i];
        }
       
        record->rows[record->size].change = atof(p[8]);
        record->rows[record->size].time = atoi(p[9]);
        if(record->rows[record->size].time > day_lasttime)
            day_lasttime = record->rows[record->size].time;
        record->rows[record->size].validity = Valid;

        record->rows[record->size].user.question_choose = atoi(p[10]);
        strcpy(record->rows[record->size].user.secretguard, p[11]);
        record->size++;
        for (pre = head->next;pre && pre->user.id != user_id;pre = pre->next);
        if (pre)//已读用户，修改结点信息
        {
            if (pre->user.op_count == pre->user.op_capacity) {
                resize_user_indices(&(pre->user));//扩容
            }
            pre->user.indices[pre->user.op_count++] = now_rows;
            if (atoi(p[7]) == 4) {
                pre->user.sum += atof(p[8]);
                pre->user.money += atof(p[8]);
            }
            else if (atoi(p[7]) == 3) pre->user.money += atof(p[8]);
            
            if(atoi(p[7]) == 5){
                strcpy(pre->user.password, p[2]);
            }
            if(atoi(p[7]) == 8)
                pre->user.validity = Invalid;
            if(atoi(p[7]) == 9)
                pre->user.validity = Valid;
            if(atoi(p[3]) > (int)pre->user.lvl)
                pre->user.lvl = atoi(p[3]);
        }
        else {//未读取用户，登记信息
            Node* t = (Node*)malloc(sizeof(Node));
            t->type = COMMON_NODE;
            t->next = head->next;
            head->next = t;
            head->size++;
            t->user.op_count = 0;
            t->user.validity = Valid;
            t->user.indices = (int*)malloc(sizeof(int) * INDICES_INITIAL_LENGTH);
            t->user.id = user_id;
            strcpy(t->user.name, p[1]);
            strcpy(t->user.password, p[2]);
            t->user.lvl = atoi(p[3]);
            t->user.money = atof(p[4]);
            t->user.sum = atof(p[5]);
            t->user.register_time = atoi(p[6]);
            // !!
            t->user.op_capacity = INDICES_INITIAL_LENGTH;
            t->user.indices[t->user.op_count++] = now_rows;
            t->user.question_choose = atoi(p[10]);
            strcpy(t->user.secretguard, p[11]);
        }
        column = 0;
        now_rows++;
    };
    fclose(fp);
}

// 写入到当前文件夹下的 用户信息.csv
void write_info(void) {
    //FILE* fp = fopen("C:\\Users\\Administor\\Desktop\\用户信息.csv", "w+");
    FILE* fp = fopen(".\\用户信息.csv", "w+");
    
    if (!fp) return;
    fprintf(fp, "ID,用户名,密码,等级,余额,累计充值金额,注册时间,操作,操作金额,操作时间,密码问题序号,密报答案\n");//表头
    for (int i = 0;i < record->size;i++) {
        if (record->rows[i].validity)
        fprintf(fp, "%d,%s,%s,%d,%lf,%lf,%d,%d,%lf,%d,%d,%s\n", record->rows[i].user.id, record->rows[i].user.name, record->rows[i].user.password,
            record->rows[i].user.lvl, record->rows[i].user.money, record->rows[i].user.sum, record->rows[i].user.register_time,
            record->rows[i].op, record->rows[i].change, record->rows[i].time, 
            record->rows[i].user.question_choose, record->rows[i].user.secretguard);
    }
    fclose(fp);
}
