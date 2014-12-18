#include "operations.h"

VALUE *evaluate_leaf(NODE *n, EVAL_TYPE e_type) {
  return NULL; // TODO
}

VALUE *arithmetic(NODE *n, VALUE *l, VALUE *r, EVAL_TYPE e_type) {
  switch(e_type) {
   case INTERPRET:
    return interpret_arithmetic(n->type, l->state->integer, r->state->integer);
   case TAC:
    return tac_arithmetic(n, l, r);
  }
}

VALUE *logic(NODE *n, VALUE *l, VALUE *r, EVAL_TYPE e_type) {
  switch(e_type) {
   case INTERPRET:
    return interpret_logic(n->type, l->state->integer, r->state->integer);
   case TAC:
    return tac_logic(n, l, r);
  }
}

VALUE *control(NODE *n, VALUE *l, VALUE *r, EVAL_TYPE e_type) {
  switch(e_type) {
   case INTERPRET:
    return interpret_control(n, l, r);
   case TAC:
    return tac_control(n, l, r);
  }
}
