#pragma once
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "defines.h"

// extern const COORD MenuPosition[];
// extern const int DAYS_EACH_MONTH[12];


//设置窗口及其缓冲区大小
void SetWindowSize(int cols, int lines);
//设置光标位置
void SetCursorPosition(const int x, const int y); 
//设置文本颜色
void SetColor(int colorID);
//设置文本背景色
void SetBackColor();

//移动光标后更新文本
void update_text_after_move(const char *current_text, COORD current_pos,
                            const char *before_text, COORD before_pos);


/**
 * 读入一个double（非零）
 * @param ret 目标存储位置
 * @param input_msg 输入的提示语句(需要换行的话要自己加上)没有就NULL
 * @param error_msg 报错的提示语句(需要换行的话要自己加上)没有就NULL
 */
bool read_double(double *ret, const char *input_msg, const char *error_msg);

/**
 * 读入一个整数（非零）
 * @param ret 目标存储位置
 * @param input_msg 输入的提示语句(需要换行的话要自己加上)没有就NULL
 * @param error_msg 报错的提示语句(需要换行的话要自己加上)没有就NULL
 */
bool read_int(int *ret, const char *input_msg, const char *error_msg);



/**
 * 返回从1970.01.01开始的天数（若日期在这之前则为负数）
 */
int days_from_civil(int year, int month, int day);

typedef struct{
    int year, month, day;
} Date;
/**
 * 根据天数返回年月日
 */
Date civil_from_days(int days);


// 对于days_from_civil的简单包装
int date2time(Date date);

/**
 * 判断日期合法性
 * @param year (0, inf)
 * @param month [1, 12]
 * @param day depends
 */
bool is_date_legal(int year, int month, int day);

/**
 * 读入日期
 * 以Date指针返回
 * 目前无需像read_int/double那样附加提示信息
 * 毕竟这里消息相对固定
 * 有需再改
 */
bool read_date(Date *date);

void load_info(void);
void write_info(void);