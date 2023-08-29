#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

//#include "stack.h"


#define DUMP(reason)\
    fprintf(new_tree->logs, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
    tree_dump(new_tree, reason);

typedef struct node
{
	size_t level = 0;
	char* data = 0;
	struct node* parent = NULL;
	struct node* left = NULL;
	struct node* right = NULL;

} Node;

typedef struct tree{
	Node* root = NULL;
	size_t size = 0;
	FILE* file_with_code = NULL;
	FILE* logs = NULL;
	FILE* graph = NULL;

} Tree;


static const int MAX_STR_LEN = 100;

static const char *error_names[] = {
    "All is ok",
    "Not enough memory",
    "Push after an empty element",
    "Pop zero element",
    "Pointer to file is NULL",
    "You have no tree",
    "You send bad pointer",
    "You want to add NULL element",
    "Adding after NULL element"
};

typedef enum errors{
    ALL_OK           =  0,
    NO_MEMORY        = -1,
    AFTER_EMPTY_ELEM = -2,
    DELETE_ZERO_ELEM = -3,
    NULL_FILE        = -4,
    NO_TREE			 = -5,
    BAD_PTR			 = -6,
    NULL_ELEM 		 = -7,
    AFTER_NULL_ELEM	 = -8,
}errors_t;


Node* find(Node* root, char* value, errors_t* error);
void read_base(Tree* save_tree, FILE* base, int level, Node* after, errors_t* error);
Tree* create_default_tree(errors_t* error);