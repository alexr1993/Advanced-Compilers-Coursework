#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

extern int V;

/****************************************************************************
 * MODIFIERS
 ****************************************************************************/

VALUE *get_val(char *name, FRAME *frame) {
  printf("Looking up variable \"%s\"...", name);
  return lookup_token(name, frame->symbols)->val;
}

/* Sets the state of the variable */
void set_val(char *name, STATE* state, FRAME *frame) {
  get_val(name, frame)->state = state;
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

FRAME *new_frame(char *proc_id) {
  if (V) printf("\nCreating new frame \"%s\"\n", proc_id);
  FRAME *frame = malloc(sizeof(FRAME));

  frame->symbols = new_symbtable();
  frame->proc_id = proc_id; // Should be the original token's lexeme
  frame->child   = NULL;
  frame->sibling = NULL;
  return frame;
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
  p->next = NULL;
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

/* Define commonly used variables for parsing */
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
