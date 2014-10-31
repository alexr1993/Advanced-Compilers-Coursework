#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "function.h"
#include "param.h"
#include "../util.h"

#define INT_TYPE 0
#define FN_TYPE  1

struct function;
struct PARAM;

/*
 * Types required for name -> storage -> state lookup using environment
 * and state respectively
 */
typedef union STATE
{
    int value;
    struct function *function;
    struct PARAM *param;
    char *var_name; // only used for variable/function names
    NODE *fn_body;
} STATE;

// Stores name -> state mappings, AKA a binding
typedef struct ENV
{
    char *name;
    int type;
    STATE *state;
    struct ENV *next;
} ENV;

// Stack frames, provides lookup (lexical scope)
typedef struct FRAME
{
    struct ENV *variable;
    struct PARAM *param;
    struct FRAME *parent;
    struct FRAME *child;
    struct FRAME *sibling;
} FRAME;

struct FRAME *gbl_frame;

ENV *lookup_var(char *name, int type, FRAME *frame);
ENV *init_var(char *name, int type, FRAME *frame);
ENV *assign_var(char *name, int type, STATE *value, FRAME *frame);

ENV *new_env_mapping(char *name, int *location);

STATE *new_int_state(int value);
STATE *new_fn_state(struct function* function);
STATE *new_var_name_state(char *name);

FRAME *new_frame(FRAME *parent, struct PARAM *params, ENV *variables);

void init_environment();
void print_environment(void);
#endif
