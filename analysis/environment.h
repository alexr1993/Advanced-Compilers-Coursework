#ifndef ANALYSIS_ENVIRONMENT_H_
#define ANALYSIS_ENVIRONMENT_H_

#include <stdbool.h>

#include "C.tab.h"
#include "symbol_table.h"
#include "token.h"
#include "nodes.h"

#include "common/util.h"

#define INT_TYPE  0
#define FN_TYPE   1

struct node;
struct function;
struct param;
struct frame;
struct value;

/* Vars */
struct token *int_token, *void_token, *function_token, *return_label;
struct frame *gbl_frame;

/* Types */

typedef struct addr_desc { // maps names with addresses
  int reg_addr;
  int mem_addr;
  struct value *val;
  bool live;
  char *str;
} ADDR_DESC;

typedef union STATE {
  int integer;
  struct function *function;
  struct addr_descriptor *addr;
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
  bool return_called;
} FRAME;

typedef struct param {
  struct token *token;
  struct param *next;
} PARAM;

typedef struct function {
  int    return_type; // INT_FN or INT_TYPE
  PARAM  *params;
  int    nparams;
  FRAME  *frame;
  char   *proc_id;
  struct node *body;
} function;

/* Runtime procedures */
VALUE *get_val(char *name, FRAME *frame);
void   set_val(char *name, STATE* state, FRAME *frame);
FRAME *get_frame(char *name, FRAME *parent);
bool   is_true(VALUE *boolean);

/* Environment construction */
void      init_environment();
VALUE    *new_val(int type, STATE *s);
STATE    *new_int_state(int value);
STATE    *new_fn_state(struct function* function);
STATE    *new_state(int type);
FRAME    *new_frame(char *proc_id);
function *new_function(int return_type, FRAME *frame, PARAM *params);
PARAM    *new_param(struct token *t);
ADDR_DESC *new_address_descriptor(int);
/* Diagnostics */
void  print_frame(FRAME *frame);
void  print_val(VALUE *val);
void  print_environment(FRAME *frame);
void  print_function(function *f);
char *data_type_to_str(int type);
void print_addr_descriptor(ADDR_DESC *d);
#endif
