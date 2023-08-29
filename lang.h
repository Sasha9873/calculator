#pragma once

#include "all.h"
#include "hash_table.h"

/*#include <stdio.h>
#include <stdlib.h>
#include <cstring>

static const char *error_names[] = {
    "All is ok",
    "Not enough memory",
    "NULL pointer",
};

typedef enum errors{ALL_OK                  = 0,
                    NOT_MEMORY              = -1,
                	NULL_POINTER			= -2,}
errors_t;

static const int POISON = -666;
*/


#define SYNTAX_ERROR()\
        printf("Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        syntax_error(str, pos);\


double get_num(char* str, int* pos, hash_table* tab);
double get_var(char* str, int* pos, hash_table* tab);
void syntax_error(char* str, int* pos, hash_table* tab);
double get_expression(char* str, int* pos, hash_table* tab);
double get_general(char* str, int* pos, hash_table* tab);
double get_mul_div(char* str, int* pos, hash_table* tab);
double get_parenthesis(char* str, int* pos, hash_table* tab);
void clear_tabs(char* str, int* pos);