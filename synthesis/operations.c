#include "operations.h"

#include "stdio.h"
#include "stdlib.h"
/*
 * Shim between evaluate.c and interpret.c | tac.c
 */

VALUE *evaluate_leaf(NODE *n, FRAME *f, EVAL_TYPE e_type) {
  switch(e_type) {
   case INTERPRET:
    return interpret_leaf(n, f);
   case TAC:
    return tac_leaf(n, f);
   default:
    return NULL;
  }
}

VALUE *arithmetic(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type) {
  VALUE *output = new_val(INT_TYPE, NULL);

  switch(e_type) {
   case INTERPRET:
    output->state = new_int_state(
      interpret_arithmetic(n->type, l->state->integer, r->state->integer));
   break;

   case TAC:
    tac_arithmetic(n, l, r);
    break;

   default:
    perror("Unknown EVAL_TYPE\n");
    exit(-1);
  }
  return output;
}

VALUE *logic(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type) {
  VALUE *output = new_val(INT_TYPE, NULL);

  switch(e_type) {
   case INTERPRET:
    output->state = new_int_state(
      interpret_logic(n->type, l->state->integer, r->state->integer));
    break;

   case TAC:
    tac_logic(n, l, r);
    break;

   default:
    perror("Unknown EVAL_TYPE\n");
    exit(-1);
  }
  return output;
}

VALUE *control(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type) {
  VALUE *output = new_val(INT_TYPE, NULL);
  switch(e_type) {
   case INTERPRET:
    return interpret_control(n, l, r, f);

   case TAC:
    tac_control(n, l, r);
    return NULL;

   default:
    perror("Unknown EVAL_TYPE\n");
    exit(-1);
  }
}
