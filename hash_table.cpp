#include "hash_table.h"
#include "all.h"

hash_table* create_tab(errors_t* error)
{
    hash_table* tab = (hash_table*)calloc(1, sizeof(hash_table));
    if(tab == NULL)
        return tab;

    tab->length = HASH_LEN;

    for(int index = 0; index < tab->length; ++index){
        tab->table[index] = NULL;
    }

    return tab;
}

int str_hash(char* str, int table_size)
{
    int pos = 0, val = 0, coeff = 17, power = 17;

    while(str[pos] != '\0')
    {
        val += (str[pos] * power) % table_size;

        power *= coeff;
        power %= table_size;
        val %= table_size;

        ++pos;
    }

    return val;
}

errors_t add_elem(hash_table* tab, char* key, int value)
{
    if(!tab || !key)
        return NULL_POINTER;

    int index = str_hash(key,tab->length);

    labels* curr = tab->table[index];

    if(curr)
    {
        while(curr->next != NULL && !strcmp(curr->key, key))
        {
            curr = curr->next;
        }


        if(!strcmp(curr->key, key))
            curr->value = value;

        else
        {
            errors_t error;
            curr->next = create_new_label(&error, key, value);
        } 

        return ALL_OK;
    }

    errors_t error;

    tab->table[index] = create_new_label(&error, key, value); 

    return error;
}

labels* create_new_label(errors_t* error, char* key, int value)
{
    //printf("0");
    if(!error)
        return NULL;

    if(!key)
    {
        *error = NULL_POINTER;
        return NULL;
    }
    //printf("1");

    labels* new_label = (labels*)calloc(1, sizeof(labels));
    if(!new_label)
    {
        *error = NOT_MEMORY;
        return NULL;
    }

    new_label->next = NULL;
    //printf("2");

    new_label->key = (char*)calloc(strlen(key) + 1, sizeof(char));
    if(!new_label->key)
    {
        free(new_label);
        *error = NOT_MEMORY;
        return NULL;
    }
    strcpy(new_label->key, key);

    new_label->value = value;
    printf("new val = %d\n", value);

    *error = ALL_OK;
    return new_label;
}

errors_t delete_hash_table(hash_table** tab)
{
    if(!(*tab) || !tab)
        return NULL_POINTER;

    for(int i = 0; i < (*tab)->length; i++)
    {
        labels* ptr = (*tab)->table[i];
        while(ptr != NULL)
        {
            labels* old_ptr = ptr;
            ptr = ptr->next;
            free(old_ptr->key);
            free(old_ptr);
        }
    }

    free(*tab);

    return ALL_OK;
}


/**
 * if elem do not exist returns 0
 * if somthing was wrong returns const POISON = -666
 * else returns need elem value
 */
int get_value(hash_table* tab, char* key, errors_t* error)
{
    if(!error)
        return POISON;

    if(!tab || !key)
    {
        *error = NULL_POINTER;
        return POISON;
    }


    int hash = str_hash(key, tab->length);

    labels* curr = tab->table[hash];
    //printf("curr = %p\n", curr);

    while(curr && strcmp(curr->key, key))
        curr = curr->next;

    if(!curr)
    {
        *error = NO_NEED_ELEM;
        return 0;
    }


    *error = ALL_OK;

    return curr->value;
}