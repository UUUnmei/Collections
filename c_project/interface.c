
#include "defines.h"
#include <stdbool.h>
#include "tools.h"
#include <stdio.h>
#include "structure.h"
#include "lib.h"

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
 * 用户功能菜单
 */
void show_menu2(void){

    SetColor(240);
    SetCursorPosition(1, 1);
    printf("请选择功能：");
    SetCursorPosition(1, 2);
    printf("(上下键选择,回车确认)");
    SetCursorPosition(15, 3);
    printf("-----[用户]-----");

    SetCursorPosition(15, 5);
    SetBackColor();//第一个选项设置背景色以表示当前选中
    printf("查看个人信息");
    SetCursorPosition(15, 7);
    SetColor(240);
    printf("消费");
    SetCursorPosition(15, 9);
    printf("续费");
    SetCursorPosition(15, 11);
    printf("改密码");
    SetCursorPosition(15, 13);
    printf("挂失");
    SetCursorPosition(15, 15);
    printf("退出登录状态");
    SetCursorPosition(15, 17);
    printf("----------------");

    SetCursorPosition(0, 31);
}

/**
 * 处理用户操作
 */
bool execute_menu2(Node *u){
    int ch;//记录键入值
    // int key = 1;//记录选中项，初始选择第一个
    bool flag = false;//记录是否键入Enter键标记，初始置为否
    while ((ch = getch())){
        switch (ch)//检测输入键
        {
        case 72://UP上方向键
            if (key > 1)//当此时选中项为第一项时，UP上方向键无效
            {
                switch (key){
                case 2:
                    update_text_after_move("查看个人信息", MenuPosition[0], "消费", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("消费", MenuPosition[1], "续费", MenuPosition[2]);
                    break;
                case 4:
                    update_text_after_move("续费", MenuPosition[2], "改密码", MenuPosition[3]);
                    break;
                case 5:
                    update_text_after_move("改密码", MenuPosition[3], "挂失", MenuPosition[4]);
                    break;
                case 6:
                    update_text_after_move("挂失", MenuPosition[4], "退出登录状态", MenuPosition[5]);
                    break;
                }
                --key;
            }
            break;

        case 80://DOWN下方向键
            if (key < 6){
                switch (key){
                case 1:
                    update_text_after_move("消费", MenuPosition[1], "查看个人信息", MenuPosition[0]);
                    break;
                case 2:
                    update_text_after_move("续费", MenuPosition[2], "消费", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("改密码", MenuPosition[3], "续费", MenuPosition[2]);
                    break;
                case 4:
                    update_text_after_move("挂失", MenuPosition[4], "改密码", MenuPosition[3]);
                    break;
                case 5:
                    update_text_after_move("退出登录状态", MenuPosition[5], "挂失", MenuPosition[4]);
                    break;
                }
                ++key;
            }
            break;

        case 13://Enter回车键
            flag = true;
            break;
        default://无效按键
            break;
        }
        if (flag) break;//输入Enter回车键确认，退出检查输入循环

        SetCursorPosition(0, 31);//将光标置于左下角，避免关标闪烁影响游戏体验
    }


    
    flag = true;
    SetCursorPosition(0, 18);

    double some;
    switch (key){
        case 1:{
            printf("------USER_INFO_BEGIN------\n");
            user_iter(&(u->user), '\n');
            printf("------USER_INFO_END------\n");
            break;
        }
        case 2:{
            if(read_double(&some, "请输入消费金额：\n", "金额不合法：\n")){
                consume(u, some);
            }
            break;
        }
        case 3:{
            if(read_double(&some, "请输入续费金额：\n", "金额不合法：\n")){
                recharge(u, some);
            }
            break;
        }
        case 4:{
            if(loginday)
                change_password(u);
            add_user_op_index(u);
            update_record(record, u->user, ChangePassword, loginday, 0);
            break;
        }
        case 5:{
            if(client_report_loss(u)){
                printf("挂失成功！\n");
                goto MYOUT;
            }else
                printf("挂失失败！\n");
            break;
        }
        default:{
            
            //case 6
            // 退出登录

MYOUT:
            flag = false;
            add_user_op_index(u);
            // 此时date中保存的应该是上面操作2、3、4后的时间，认为与他们中最晚的相同
            update_record(record, u->user, LogOut, loginday, 0); 
        }
            
    }
    return flag;
}


/**
 * 管理员功能菜单
 */
void show_menu3(void){
    SetColor(240);
    SetCursorPosition(1, 1);
    printf("请选择功能：");
    SetCursorPosition(1, 2);
    printf("(上下键选择,回车确认)");
    SetCursorPosition(15, 3);
    printf("-----[管理员]-----");

    SetCursorPosition(15, 5);
    SetBackColor();//第一个选项设置背景色以表示当前选中
    printf("用户退卡");
    SetCursorPosition(15, 7);
    SetColor(240);
    printf("用户挂失");
    SetCursorPosition(15, 9);
    printf("挂失恢复");
    SetCursorPosition(15, 11);  
    printf("统计");
    SetCursorPosition(15, 13);
    printf("排序");
    SetCursorPosition(15, 15);
    printf("筛选");
    SetCursorPosition(15, 17);
    printf("浏览用户列表");
    SetCursorPosition(15, 19);
    printf("浏览操作列表");
    SetCursorPosition(15, 21);
    printf("退出登录状态");
    SetCursorPosition(15, 23);
    printf("----------------");

    SetCursorPosition(0, 31);
}
/**
 * 处理管理员操作
 */
bool execute_menu3(void){
    int ch;//记录键入值
    // int key = 1;//记录选中项，初始选择第一个
    bool flag = false;//记录是否键入Enter键标记，初始置为否
    while ((ch = getch())){
        switch (ch)//检测输入键
        {
        case 72://UP上方向键
            if (key > 1)//当此时选中项为第一项时，UP上方向键无效
            {
                switch (key){
                case 2:
                    update_text_after_move("用户退卡", MenuPosition[0], "用户挂失", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("用户挂失", MenuPosition[1], "挂失恢复", MenuPosition[2]);
                    break;
                case 4:
                    update_text_after_move("挂失恢复", MenuPosition[2], "统计", MenuPosition[3]);
                    break;
                case 5:
                    update_text_after_move("统计", MenuPosition[3], "排序", MenuPosition[4]);
                    break;
                case 6:
                    update_text_after_move("排序", MenuPosition[4], "筛选", MenuPosition[5]);
                    break;
                case 7:
                    update_text_after_move("筛选", MenuPosition[5], "浏览用户列表", MenuPosition[6]);
                    break;
                case 8:
                    update_text_after_move("浏览用户列表", MenuPosition[6], "浏览操作列表", MenuPosition[7]);
                    break;
                case 9:
                    update_text_after_move("浏览操作列表", MenuPosition[7], "退出登录状态", MenuPosition[8]);
                    break;
                }
                --key;
            }
            break;

        case 80://DOWN下方向键
            if (key < 9){
                switch (key){
                case 1:
                    update_text_after_move("用户挂失", MenuPosition[1], "用户退卡", MenuPosition[0]);
                    break;
                case 2:
                    update_text_after_move("挂失恢复", MenuPosition[2], "用户挂失", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("统计", MenuPosition[3], "挂失恢复", MenuPosition[2]);
                    break;
                case 4:
                    update_text_after_move("排序", MenuPosition[4], "统计", MenuPosition[3]);
                    break;
                case 5:
                    update_text_after_move("筛选", MenuPosition[5], "排序", MenuPosition[4]);
                    break;
                case 6:
                    update_text_after_move("浏览用户列表", MenuPosition[6], "筛选", MenuPosition[5]);
                    break;
                case 7:
                    update_text_after_move("浏览操作列表", MenuPosition[7], "浏览用户列表", MenuPosition[6]);
                    break;
                case 8:
                    update_text_after_move("退出登录状态", MenuPosition[8], "浏览操作列表", MenuPosition[7]);
                    break;
                }
                ++key;
            }
            break;

        case 13://Enter回车键
            flag = true;
            break;
        default://无效按键
            break;
        }
        if (flag) break;//输入Enter回车键确认，退出检查输入循环

        SetCursorPosition(0, 31);//将光标置于左下角，避免关标闪烁影响游戏体验
    }


    flag = true;
    SetCursorPosition(0, 24);
    switch (key){
        case 1:{
            if(refund_card()){
                printf("退卡成功\n");
            }else{
                printf("退卡失败\n");
            }
            break;
        }
        case 2:{
            if(admin_report_loss()){
                printf("挂失成功\n");
            }else{
                printf("挂失失败\n");
            }
            break;
        }
        case 3:{
            int id; 
            if(read_int(&id, "请输入要恢复用户的id\n", "输入非法\n")){
                Node *t = find_user_by_id(head, id);
                if(t){
                    if(t->user.validity == Valid){
                        printf("用户信息有效，无需恢复\n");
                    }else{
                        recover(t);
                        printf("恢复成功\n");
                    }
                }else{
                    printf("用户不存在\n");
                }
                
            }
            break;
        }
        case 4:{
            // 统计
            //函数接口：（case 里面的语句）
            int begin_time, end_time;
            double shouru = 0;
            double sum = 0;
            printf("请输入您要统计的开始时间：\n");
            if (!initialize_register_time(&begin_time)) {
                break;
            }
            printf("请输入您要统计的结束时间：\n");
            if (!initialize_register_time(&end_time)) {
                break;
            }
            if (begin_time > end_time) {
                printf("输入时间顺序有误！");
                break;
            }
            if (!count_summoney(begin_time, end_time, &shouru, &sum)){
                printf("该段时间内没有客户的购物记录！\n");
            }
            else {
                printf("该段时间内客户共消费%f元。\n", sum);
                printf("用户共优惠%f元。\n", sum - shouru);
                printf("该段时间总收入共%f元。\n", shouru);
            }
            break;
        }
        case 5:{
            // 排序
            combinedSort();
            break;
        }
        case 6:{
            // 筛选
            screen();
            break;
        }
        case 7:{
            printf("------USER_INFO_BEGIN------\n");
            Node *t = head->next;
            while (t){
                system("color F0");//调整下背景颜色
                user_iter(&(t->user), '\t');
                t = t->next;
            }
            printf("------USER_INFO_END------\n");
            break;
        }
        case 8:{
            // 浏览大表
            //printf("%d\n", record->size);

            printf("------OPERATION_INFO_BEGIN------\n");
            for (int i = 0; i < record->size;++i){
                system("color F0");//调整下背景颜色
                row_iter(&(record->rows[i]));
            }
                
            printf("------OPERATION_INFO_END------\n");
            break;
        }
        default:
            //case 9
            flag = false;
    }
    return flag;
}




/**
 * 用户模式
 */
void client_control(Node *u){
    printf("您在以用户身份登录\n");
    bool running = true;
    do{
        key = 1;
        show_menu2();
        running = execute_menu2(u);
        system("pause");
        system("cls");
    } while (running);
    printf("您已退出登录\n");
}

/**
 * 管理员模式
 */
void admin_control(void){
    printf("您在以管理员身份登录\n");
    bool running = true;
    do{
        key = 1;
        show_menu3();
        running = execute_menu3();
        system("pause");
        system("cls");
    } while (running);
    printf("您已退出登录\n");
}


void show_menu1(void){
    SetColor(240);
    SetCursorPosition(1, 1);
    printf("请选择功能：");
    SetCursorPosition(1, 2);
    printf("(上下键选择,回车确认)");
    SetCursorPosition(15, 5);
    SetBackColor();//第一个选项设置背景色以表示当前选中
    printf("退出系统");
    SetCursorPosition(15, 7);
    SetColor(240);
    printf("用户注册");
    SetCursorPosition(15, 9);
    printf("用户登录");
    SetCursorPosition(15, 11);
    printf("管理员登录");
    SetCursorPosition(0, 31);
}

void execute_menu1(){
    int ch;//记录键入值
    // int key = 1;//记录选中项，初始选择第一个
    bool flag = false;//记录是否键入Enter键标记，初始置为否
    while ((ch = getch())){
        switch (ch)//检测输入键
        {
        case 72://UP上方向键
            if (key > 1)//当此时选中项为第一项时，UP上方向键无效
            {
                switch (key){
                case 2:
                    update_text_after_move("退出系统", MenuPosition[0], "用户注册", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("用户注册", MenuPosition[1], "用户登录", MenuPosition[2]);
                    break;
                case 4:
                    update_text_after_move("用户登录", MenuPosition[2], "管理员登录", MenuPosition[3]);
                    break;
                }
                --key;
            }
            break;

        case 80://DOWN下方向键
            if (key < 4){
                switch (key){
                case 1:
                    update_text_after_move("用户注册", MenuPosition[1], "退出系统", MenuPosition[0]);
                    break;
                case 2:
                    update_text_after_move("用户登录", MenuPosition[2], "用户注册", MenuPosition[1]);
                    break;
                case 3:
                    update_text_after_move("管理员登录", MenuPosition[3], "用户登录", MenuPosition[2]);
                    break;
                }
                ++key;
            }
            break;

        case 13://Enter回车键
            flag = true;
            break;
        default://无效按键
            break;
        }
        if (flag) break;//输入Enter回车键确认，退出检查输入循环

        SetCursorPosition(0, 31);//将光标置于左下角，避免关标闪烁影响游戏体验
    }

    system("cls");
    SetCursorPosition(0, 0);
    switch(key){
        case 1:{
            printf("想要保存本次操作吗？保存请选1，其他视为不保存\n");
            int op;
            if(read_int(&op, NULL, NULL) && op==1)
                write_info();
            FreeConsole();
            exit(EXIT_SUCCESS);
        }     
        case 2:{
            if(user_register()){
                printf("用户注册成功!\n");
            }else{
                printf("用户注册失败!\n");
            }
            break;
        }
        case 3:{
            int ret = log_in(COMMON_NODE);
            if(ret){
                system("cls");
                client_control(find_user_by_id(head, ret));
            }else{
                // ret == 0
                printf("登录失败\n");
            }
            break; 
        }
        case 4:{
            int ret = log_in(ADMIN);
            if(ret){       
                system("cls");
                admin_control();      
            }else{
                // ret == 0
                printf("登录失败\n");
            }
            break;
        }
    }
}