#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "function.h"

/*
 * Types required for name -> storage -> state lookup useing environment
 * and state respectively
 */
typedef struct INT_ENV_MAPPING
{
    char *name;
    int  *location;
} INT_ENV_MAPPING;

typedef struct FN_ENV_MAPPING
{
    char      *name;
    function  *location;
} FN_ENV_MAPPING;

typedef struct INT_STATE_MAPPING
{
    int *location;
    int  value;
} INT_STATE_MAPPING;

typedef struct FN_STATE_MAPPING
{
    function *location;
    function  fn;
} FN_STATE_MAPPING;

INT_STATE_MAPPING   *envlookup_int(char *name);
FN_STATE_MAPPING    *envlookup_fn(char *name);

void envstore_int(char *name, int value);
// TODO void envstore_fn();
#endif
