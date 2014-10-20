#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "function.h"

#define INT_TYPE 0
#define FN_TYPE  1

/*
 * Types required for name -> storage -> state lookup useing environment
 * and state respectively
 */
typedef struct ENV_MAPPING
{
    char *name;
    int  *location;
    int type;
    INT_ENV_MAPPING *next;
} ENV_MAPPING;

typedef struct STATE_MAPPING
{
    int *location;
    int value;
    NODE *closure;
    INT_STATE_MAPPING *next;
} STATE_MAPPING;

STATE_MAPPING *envlookup(char *name);

void envstore_int(char *name, int value);
// TODO add arguments param
void envstore_fn(char *name, int return_type, NODE *closure);

extern ENV_MAPPING *new_env_mapping(char *name, int *location);
extern STATE_MAPPING *new_state_mapping(int *location, int value);
#endif
