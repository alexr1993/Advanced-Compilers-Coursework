#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

typedef struct node NODE;

#include <stdbool.h>

#include "util.h"

#include "token.h"
#include "symbol_table.h"
#include "C.tab.h"

#include "evaluation/function.h"
#include "evaluation/param.h"

#define INT_TYPE  0
#define FN_TYPE   1

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

typedef struct VALUE {
  int type;
  STATE *state;
} VALUE;

// Stack frames, provides lookup (lexical scope)
typedef struct FRAME
{
    struct ENV *variable;
    struct ENV *param;
    struct FRAME *parent;
    struct FRAME *child;
    struct FRAME *sibling;
    TOKEN **symbols;
    char *proc_id; // name of enclosing procedure
} FRAME;

struct FRAME *gbl_frame;

ENV *lookup_val(char *name, int type, FRAME *frame);
ENV *init_val(char *name, int type, FRAME *frame);
ENV *assign_val(char *name, int type, STATE *value, FRAME *frame);

ENV *new_env(char *name, int type, union STATE *state);

STATE *new_int_state(int value);
STATE *new_fn_state(struct function* function);
STATE *new_var_name_state(char *name);
STATE *new_fn_body_state(NODE *body);
STATE *new_param_state(struct PARAM *param);
STATE *new_env_state(struct ENV *env);

FRAME *new_frame(FRAME *parent, char *proc_id);

VALUE *new_val(int type, TOKEN *t, FRAME *frame);
void print_frame(FRAME *frame);
void print_val(VALUE *val);
void print_environment(FRAME *frame);
char *data_type_to_str(int type);
void init_environment();
#endif
