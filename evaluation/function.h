#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "environment.h"
#include "../analysis/nodes.h"
#include "param.h"

struct ENV;
struct FRAME;
struct PARAM;
union STATE;

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

void *bind_args(struct function *fn, struct ENV *args);
struct ENV *call(char *name, struct FRAME *frame, struct ENV *args);

#endif
