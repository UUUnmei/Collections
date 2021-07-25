
#pragma once
#include "defines.h"
#include "structure.h"
#include <stdbool.h>

bool user_register(void);
int log_in(NodeType type);

bool refund_card(void);
bool admin_report_loss(void);
bool count_summoney(int begin_day, int end_day, double *shouru, double *sum);
void screen();
void combinedSort(void);

void consume(Node *u, double money);
void recharge(Node *u, double money);
bool client_report_loss(Node *u);


bool change_password(Node *p);
bool change_Level(Node *p);
void recover(Node *p);
