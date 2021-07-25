
#include "defines.h"
#include "tools.h"
#include "lib.h"
#include "structure.h"
#include "interface.h"

// 菜单选项的位置
const COORD MenuPosition[] = {{15, 5}, {15, 7}, {15, 9}, {15, 11}, {15, 13}, {15, 15}, {15, 17}, 
                                {15, 19}, {15, 21}, {15, 23}};
const int DAYS_EACH_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const char *level_name[] = {"Normal", "Silver", "Gold", "Platinum", "Diamond"};
const char *operation_name[] = {"Register", "LogIn", "LogOut", "Consumption", "Charge", "ChangePassword", "ChangeLevel",
                                "Refund", "ReportLoss", "Recover"};
const int admin_id = 10086; //管理员id
const char admin_pwd[] = "admin"; // 管理员密码
Record *record;
Node *head;
int loginday;
int day_lasttime = -99999999;
int key = 1;
int ID = 1;
int now_rows = 1;


int main(){
    record = create_record();
    head = create_head();
    load_info();
    SetWindowSize(80, 30);//设置窗口大小
    
    while (1){
        key = 1;
        show_menu1();
        execute_menu1();
        system("pause");
        system("cls");
    }

    return 0;
}