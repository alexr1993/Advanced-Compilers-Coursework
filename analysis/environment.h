#ifndef ANALYSIS_ENVIRONMENT_H_
#define ANALYSIS_ENVIRONMENT_H_

typedef struct node NODE;

#include <stdbool.h>

#include "util.h"

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

typedef struct VALUE {
  int type;
  STATE *state;
} VALUE;

typedef struct FRAME {
  struct FRAME *parent;
  struct FRAME *child;
  struct FRAME *sibling;
  int nchildren;
  TOKEN **symbols;
  char *proc_id; // name of enclosing procedure
  NODE *root;
} FRAME;

typedef struct PARAM {
  TOKEN *token;
  struct PARAM *next;
} PARAM;

typedef struct function {
  int    return_type; // INT_FN or INT_TYPE
  PARAM  *params;
  FRAME  *frame;
  char   *proc_id;
  NODE   *body;
} function;


FRAME *gbl_frame;

/* Environment construction */
void init_environment();
VALUE *new_val(int type, TOKEN *t, FRAME *frame);
STATE *new_int_state(int value);
STATE *new_fn_state(struct function* function);
FRAME *new_frame(char *proc_id);

/* Diagnostics */
void print_frame(FRAME *frame);
void print_val(VALUE *val);
void print_environment(FRAME *frame);
char *data_type_to_str(int type);
#endif
