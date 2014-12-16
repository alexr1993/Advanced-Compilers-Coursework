#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

extern int V;

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

void print_state(STATE *s, int type) {
  switch(type) {
   case INT_TYPE:
    printf("state: %d\n", s->integer);
    break;
   case FN_TYPE:
    printf("state: function_state\n");
    break;
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

/*
 * Prints frame including all of it's children
 */
void print_frame(FRAME *frame) {
  printf("\nFRAME \"%s\", parent: \"%s\", children: %d\n",
         frame->proc_id,
         frame->parent ? frame->parent->proc_id : NULL,
         frame->nchildren  );
  print_symbtable(frame->symbols, true);
}

/* Prints subtree of frame */
void print_environment(FRAME *f) {
  print_frame(f);
  FRAME *child = f->child;

  while (child != NULL) {
    print_environment(child);
    child = child->sibling;
  }
}

/*
 * Finds and returns the env mapping with the given name and type
 *
 * Checks given env first, then recursive looks at parent until val
 * is found
 *
 */
VALUE *lookup_val(char *name, int type, FRAME *frame) {
  printf("Looking up variable \"%s\"...", name);
  return NULL;
}

/* Sets the state of the variable */
VALUE *assign_val(char *name, int type, STATE* state, FRAME *frame) {
  VALUE *val = lookup_val(name, type, frame);
  return val;
}

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

/*
 * Create new frame and attack it correctly to its parent
 *
 */
FRAME *new_frame(char *proc_id)
{
  if (V) printf("\nCreating new frame \"%s\"\n", proc_id);
  FRAME *new_frame = malloc(sizeof(FRAME));

  new_frame->symbols = new_symbtable();
  new_frame->proc_id = proc_id; // Should be token's lexeme
  new_frame->child   = NULL;
  new_frame->sibling = NULL;
  return new_frame;
}

function *new_function(int return_type, FRAME *frame) {
  function *fn = malloc(sizeof(function));
  fn->return_type = return_type;
  fn->frame       = frame;
  fn->proc_id     = frame->proc_id;
  fn->body        = frame->root;
  return fn;
}

PARAM *new_param(TOKEN *t) {
  PARAM *p = malloc(sizeof(PARAM));
  p->token = t;
  return p;
}

VALUE *new_val(int type, TOKEN *t, FRAME *frame) {
  STATE *s;
  switch(type) {
   case INT_TYPE:
    s = new_int_state(0);
    break;
   case FN_TYPE:
    s = new_fn_state(new_function(INT_TYPE, frame));
    break;
   default:
    printf("Error: type for new variable not recognised\n");
    exit(-1);
  }

  VALUE *val = malloc(sizeof(VALUE));
  val->type = type;
  val->state = s;
  return val;
}

/*
 * Define commonly used variables for parsing
 */
void init_environment() {
  gbl_frame = new_frame("gbl_frame");

  int_token = new_token(INT);
  int_token->lexeme = "int";
  function_token = new_token(FUNCTION);
  function_token->lexeme = "function";
  void_token = new_token(VOID);
  void_token->lexeme = "void";

  init_token_stack();
}

