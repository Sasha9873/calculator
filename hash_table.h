#pragma once

#include "all.h"


static const int HASH_LEN = 100003;
static const int POISON = -666;

typedef struct labels{
    char* key;
    int value;
    struct labels* next;
}labels;

typedef struct{
    int length = HASH_LEN;
    labels* table[HASH_LEN];
}hash_table;


hash_table* create_tab(errors_t* error);
errors_t add_elem(hash_table* tab, char* key, int value);
errors_t delete_hash_table(hash_table** tab);
int str_hash(char* str, int table_size);
int get_value(hash_table* tab, char* key, errors_t* error);
labels* create_new_label(errors_t* error, char* key, int value);
