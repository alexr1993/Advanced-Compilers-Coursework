#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

extern int v,V;

/****************************************************************************
 * RUNTIME OPERATIONS
 ****************************************************************************/

VALUE *get_val(char *name, FRAME *frame) {
  if (V) printf("ENVIRONMENT Looking up variable \"%s\" in frame \"%s\"\n",
                 name,
                 frame->proc_id);
  TOKEN *t = lookup_token(name, frame->symbols, true);

  // If token is not found in symbtable, check parent's
  if (t != NULL) {
    return t->val;
  } else if (frame->parent == NULL) {
    /* Symbol lookup failed in entire tree lineage */
    if (v) printf("ENVIRONMENT Lookup of variable \"%s\" returned nothing!\n",
                  name);
    if (str_eq("main", name)) {
      printf("Make sure your program has a main()!\n");
      exit(0);
    }
    return NULL;
  } else {
    return get_val(name, frame->parent);
  }
}

/* Sets the state of the variable */
void set_val(char *name, STATE* state, FRAME *frame) {
  get_val(name, frame)->state = state;
}

FRAME *get_frame(char *name, FRAME *parent) {
  if (V) printf("ENVIRONMENT Looking up frame \"%s\"\n", name);
  return get_val(name, parent)->state->function->frame;
}

bool is_true(VALUE *boolean) {
  return boolean->state->integer == 1;
}

/****************************************************************************
 * CONSTRUCTORS
 ***************************************************************************/

STATE *new_int_state(int integer) {
  STATE *state = malloc(sizeof(STATE));
  state->integer = integer;
  return state;
}

STATE *new_fn_state(function *function) {
  STATE *state = malloc(sizeof(STATE));
  state->function = function;
  return state;
}

STATE *new_state(int type) {
  switch(type) {
   case INT_TYPE:
    return new_int_state(0);
   case FN_TYPE:
    return new_fn_state(NULL);
   default:
    return NULL;
  }
}

FRAME *new_frame(char *proc_id) {
  if (V) printf("\nCreating new frame \"%s\"\n", proc_id);
  FRAME *frame = malloc(sizeof(FRAME));

  frame->symbols = new_symbtable();
  frame->proc_id = proc_id; // Should be the original token's lexeme
  frame->child   = NULL;
  frame->sibling = NULL;
  frame->parent  = NULL;
  frame->return_called = false;
  frame->nchildren = 0;
  return frame;
}

function *new_function(int return_type, FRAME *frame, PARAM *params) {
  function *fn = malloc(sizeof(function));
  fn->return_type = return_type;
  fn->frame       = frame;
  fn->proc_id     = frame->proc_id;
  fn->body        = frame->root;
  fn->params      = params;
  fn->nparams     = 0;
  PARAM *tmp = params;
  while (tmp != NULL) {
    fn->nparams++;
    tmp = tmp->next;
  }

  if (V) {
    printf("New function created:\n");
    print_function(fn);
  }
  return fn;
}

PARAM *new_param(TOKEN *t) {
  PARAM *p = malloc(sizeof(PARAM));
  p->token = t;
  p->next = NULL;
  return p;
}

VALUE *new_val(int type, STATE *s) {
  VALUE *val = malloc(sizeof(VALUE));
  val->type  = type;
  val->state = s;
  val->addr  = NULL;
  return val;
}

ADDR_DESC *new_address_descriptor(int reg_addr) {
  ADDR_DESC *desc = malloc(sizeof(ADDR_DESC));
  desc->mem_addr = -1;
  desc->contents = NULL;
  desc->live = false;
  desc->str = malloc(10); // e.g. $t2
  return desc;
}

/* Define commonly used variables for parsing */
void init_environment() {

  char *name = malloc(10 * sizeof(char)); // gbl_frame\0
  name = "gbl_frame";
  gbl_frame = new_frame(name);

  int_token = new_token(INT);
  int_token->lexeme = "int";
  function_token = new_token(FUNCTION);
  function_token->lexeme = "function";
  void_token = new_token(VOID);
  void_token->lexeme = "void";
  return_label = make_identifier("Lreturn");
  init_token_stack();
}

/****************************************************************************
 * DIAGNOSTICS
 ***************************************************************************/
void print_state(STATE *s, int type) {
  switch(type) {
   case INT_TYPE:
    printf("state: %d\n", s->integer);
    break;
   case FN_TYPE:
    printf("state: function_state\n");
    break;
   default:
    printf("\n");
  }
}

void print_val(VALUE *val) {
  if (!val) {
    printf("VALUE not initialised\n");
    return;
  }
  printf("VALUE type: %s, ", data_type_to_str(val->type));
  print_state(val->state, val->type);
}

void print_frame(FRAME *frame) {
  printf("\nFRAME \"%s\", parent: \"%s\", children: %d\n",
         frame->proc_id,
         frame->parent ? frame->parent->proc_id : NULL,
         frame->nchildren  );
  print_symbtable(frame->symbols, true);
}

/* Prints entire subtree of frame */
void print_environment(FRAME *f) {
  print_frame(f);
  FRAME *child = f->child;

  while (child != NULL) {
    print_environment(child);
    child = child->sibling;
  }
}

void print_params(PARAM *p) {
  printf("PARAM LIST CONTENTS\n");
  while (p != NULL) {
    print_token(p->token);
    p = p->next;
  }
}

char *data_type_to_str(int type) {
  switch(type) {
   case INT_TYPE:
    return "INT_TYPE";
   case FN_TYPE:
    return "FN_TYPE";
   default:
    return "TYPE_NOT_RECOGNISED";
  }
}

void print_function(function *f) {
  printf("Returns: %s, has body? %s, has frame? %s, param(s)? %s\n",
         data_type_to_str(f->return_type),
         f->body == NULL ? "no" : "yes",
         f->frame == NULL ? "no" : "yes",
         f->params == NULL ? "no" : "yes");
}

void print_addr_descriptor(ADDR_DESC *d) {
  printf("%s - Live: %s, Contents: %s\n",
         d->str,
         d->live ? "true" : "false",
           d->contents == NULL                ? "Empty"
         : d->contents->data_type == INT_TYPE ? d->contents->val->state->integer
         :                                      "function value"
  );
}
