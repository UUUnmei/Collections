

#pragma once

#include "defines.h"
#include <stdbool.h>

Record *create_record(void);
Node *create_head(void);
Node *create_new_empty_user(void);
bool initialize_user(Node *node);
bool initialize_register_time(int *time);
void add_new_user(Node **head, Node *user);
Node *find_user_by_id(Node *head, int id);
void resize_user_indices(User *u);
void resize_record(Record *rec);
void add_user_op_index(Node *u);
void update_record(Record *rec, User u, Operation op, int time, double change);
void user_iter(User *u, const char c);
void row_iter(Row *row);