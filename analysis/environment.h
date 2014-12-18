#ifndef ANALYSIS_ENVIRONMENT_H_
#define ANALYSIS_ENVIRONMENT_H_

struct node;

#include <stdbool.h>

#include "common/util.h"

#include "C.tab.h"
#include "symbol_table.h"
#include "token.h"

#define INT_TYPE  0
#define FN_TYPE   1

struct function;
struct PARAM;

/* Types */
typedef union STATE {
  int integer;
  struct function *function;
} STATE;

typedef struct value {
  int type;
  STATE *state;
} VALUE;

typedef struct frame {
  struct frame *parent;
  struct frame *child;
  struct frame *sibling;
  int nchildren;
  struct token **symbols;
  char *proc_id; // name of enclosing procedure
  struct node *root;
} FRAME;

typedef struct PARAM {
  struct token *token;
  struct PARAM *next;
} PARAM;

typedef struct function {
  int    return_type; // INT_FN or INT_TYPE
  PARAM  *params;
  FRAME  *frame;
  char   *proc_id;
  struct node   *body;
} function;


FRAME *gbl_frame;

/* Modifiers */
VALUE *get_val(char *name, FRAME *frame);
void set_val(char *name, STATE* state, FRAME *frame);

FRAME *get_frame(char *name, FRAME *parent);

/* Environment construction */
void init_environment();
VALUE *new_val(int type, FRAME *frame);
STATE *new_int_state(int value);
STATE *new_fn_state(struct function* function);
FRAME *new_frame(char *proc_id);

/* Diagnostics */
void print_frame(FRAME *frame);
void print_val(VALUE *val);
void print_environment(FRAME *frame);
char *data_type_to_str(int type);
#endif
