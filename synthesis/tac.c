#include "tac.h"

#include <stdlib.h>

/****************************************************************************
 * INSTANCE MANIPULATION
 ***************************************************************************/

TAC *new_tac(TOKEN *arg1, TOKEN *arg2, TOKEN *op, TOKEN *result) {
  TAC *code = malloc(sizeof(TAC));
  code->arg1 = arg1;
  code->arg2 = arg2;
  code->op = op;
  code->result = result;
  return code;
}

int new_temp() {
  static int id = 0;
  return id++;
}

/****************************************************************************
 * TRANSLATION
 ***************************************************************************/

VALUE *tac_leaf(NODE *n, FRAME *f) {
  TOKEN *t = get_token(n);
  switch(t->type) {
   case CONSTANT:
    // generate TAC
    break;
   case IDENTIFIER:
    // generate TAC
    break;
  }
  return NULL;
}

void tac_arithmetic(NODE *n, VALUE *l, VALUE *r) {
}

void tac_logic(NODE *n, VALUE *l, VALUE *r) {
}

void tac_control(NODE *n, VALUE *l, VALUE *r) {
}

void generate_tac() {

}
