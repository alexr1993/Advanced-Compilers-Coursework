#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "environment.h"
#include "../analysis/nodes.h"

struct ENV;

typedef struct function
{
    int    return_type; // INT_FN or INT_TYPE as defined in environment.h
    struct ENV *params;
    struct FRAME *scope;
    NODE   *body;
} function;

function *new_function( int return_type, struct ENV *params,
                        struct FRAME *scope,      NODE *body   );
#endif
