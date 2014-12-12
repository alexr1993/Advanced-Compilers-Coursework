#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

typedef struct node NODE;
#include <stdbool.h>
#include "function.h"
#include "param.h"
#include "util.h"

#define INT_TYPE  0
#define FN_TYPE   1
#define STR_TYPE  2
#define NODE_TYPE 3
#define ENV_TYPE  4
#define PARAM_TYPE 5
#define NO_TYPE 999

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
    struct ENV *env;
} STATE;

// Stores name -> state mappings, AKA a binding
typedef struct ENV
{
    char *name;
    int type;
    STATE *state;
    bool is_return;
    struct ENV *next;
} ENV;

// Stack frames, provides lookup (lexical scope)
typedef struct FRAME
{
    struct ENV *variable;
    struct ENV *param;
    struct FRAME *parent;
    struct FRAME *child;
    struct FRAME *sibling;
} FRAME;

struct FRAME *gbl_frame;

ENV *lookup_var(char *name, int type, FRAME *frame);
ENV *init_var(char *name, int type, FRAME *frame);
ENV *assign_var(char *name, int type, STATE *value, FRAME *frame);

ENV *new_env(char *name, int type, union STATE *state);

STATE *new_int_state(int value);
STATE *new_fn_state(struct function* function);
STATE *new_var_name_state(char *name);
STATE *new_fn_body_state(NODE *body);
STATE *new_param_state(struct PARAM *param);
STATE *new_env_state(struct ENV *env);

FRAME *new_frame(FRAME *parent, struct ENV *params, ENV *variables);

void init_environment();
void print_frame(FRAME *frame);
#endif
