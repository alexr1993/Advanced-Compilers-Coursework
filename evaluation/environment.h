#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "function.h"
#include "param.h"
#include "../util.h"

#define INT_TYPE 0
#define FN_TYPE  1

struct function;
/*
 * Types required for name -> storage -> state lookup using environment
 * and state respectively
 */
typedef union STATE
{
    int value;
    struct function *closure;
    struct param *paramlist;
} STATE;

// Stores name -> state mappings, AKA a binding
typedef struct ENV
{
    char *name;
    int type;
    STATE *state;
    struct ENV *next;
    // Singly linked list will cause with mutually recursive functions
    // A frame structure is necessary, as described in Abelson book
    // A list of frames (for each function), where each frame has a list of bindings
    // 2 structs - a frame list and a binding list
} ENV;

// Stack frames, provides lookup (lexical scope)
typedef struct FRAME
{
    struct ENV *variables;
    struct FRAME *next;
} FRAME;

ENV *lookup_var(char *name, int type, FRAME *frame);
ENV *init_var(char *name, int type, FRAME *frame);
ENV *assign_var(char *name, int type, STATE *value, FRAME *frame);

ENV *new_env_mapping(char *name, int *location);
STATE *new_int_state(int value);
STATE *new_fn_state(struct function* closure);

void init_environment(FRAME *frame);
void print_environment(void);
#endif
