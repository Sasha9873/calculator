#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstring>


static const char *error_names[] = {
    "All is ok",
    "Not enough memory",
    "NULL pointer",
    "There is no such elem in hash table now"
};

typedef enum errors{ALL_OK                  = 0,
                    NOT_MEMORY              = -1,
                	NULL_POINTER			= -2,
                    NO_NEED_ELEM            = -3,}
errors_t;