#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "function.h"
#include "../util.h"

#define INT_TYPE 0
#define FN_TYPE  1

/*
 * Types required for name -> storage -> state lookup using environment
 * and state respectively
 */
typedef union STATE
{
    int value;
    NODE *closure;
} STATE;

typedef struct ENV
{
    char *name;
    int type;
    STATE *state;
    struct ENV *next;
} ENV;

ENV *lookup_var(char *name, int type);
ENV *init_var(char *name, int type);
ENV *assign_var(char *name, int type, STATE *value);

ENV *new_env_mapping(char *name, int *location);
STATE *new_int_state(int value);
STATE *new_fn_state(NODE *closure);
#endif
