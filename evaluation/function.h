#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "environment.h"
#include "../analysis/nodes.h"

struct ENV;

typedef struct function
{
    int  *return_type; // INT_FN or INT_TYPE as defined in environment.h
    char *name;
    struct ENV *arguments;
    NODE *fn_body;
} function;

#endif
