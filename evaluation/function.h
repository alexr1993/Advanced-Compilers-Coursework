#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "environment.h"
#include "../analysis/nodes.h"
#include "param.h"

struct ENV;
struct FRAME;
struct PARAM;

typedef struct function
{
    int    return_type; // INT_FN or INT_TYPE as defined in environment.h
    struct FRAME *scope;
    NODE   *body;
    char   *name;
} function;

function *new_function( int return_type,
                        struct FRAME *scope,
                        NODE *body,
                        char *name   );
#endif
