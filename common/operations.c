#include "operations.h"

// TODO for each token type, create a function with params:
/*
 * NODE *parent
 * NODE *operator
 * ENV  *left_operand
 * ENV  *right_operand
 * FRAME *frame
 * mode ::= EVAL | FIRST_PASS | TAC | MIPS
 */

/*
 * Left operand: function struct
 * Right operand: body of function
 * Returns: Nothing
 */
void D_first_pass(NODE *operator, ENV *left_operand, FRAME *frame) {
  // Set body of function
  function *fn = left_operand->state->function;
  fn->body = operator->right;
  // Store function in frame
  //init_var(fn->name, FN_TYPE, frame);
  assign_var(fn->name, FN_TYPE, left_operand->state, frame);
}

/* Function definition symbol implementation */
ENV *D(NODE *parent, NODE *operator, ENV *left_operand,
       ENV *right_operand, FRAME *frame, int mode) {

  switch (mode) {
    case EVAL:
      break;
    case FIRST_PASS:
      D_first_pass(operator, left_operand, frame);
      return NULL;
    case TAC:
      break;
    case MIPS:
      break;
  }
  return NULL;
}
