#include "tac.h"

#include <stdio.h>
#include <stdlib.h>

/****************************************************************************
 * INSTANCE MANIPULATION
 ***************************************************************************/

TAC *new_tac(TOKEN *arg1, TOKEN *arg2, TOKEN *op, TOKEN *result) {
  TAC *code = malloc(sizeof(TAC));
  code->arg1   = arg1;
  code->arg2   = arg2;
  code->op     = op;
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

TAC *tac_leaf(NODE *n, FRAME *f) {
  TOKEN *t = get_token(n);
  switch(t->type) {
   case CONSTANT:
    return new_tac(t, NULL, NULL, NULL);
   case IDENTIFIER:
    // generate "load variable"
    return t;
  }
  return NULL;
}

TAC *tac_arithmetic(NODE *n, TAC *l, TAC* r) {
  return new_tac(NULL,NULL,NULL,NULL);
}

TAC *tac_logic(NODE *n, TAC *l, TAC *r) {
  return new_tac(NULL,NULL,NULL,NULL);
}

TAC *tac_control(NODE *n, TAC *l, TAC *r) {
  switch(n->type) {
   case APPLY:
    break;
   case IF:
    break;
   case RETURN:
    break;
   case ';':
    break;
   case '=':
    break;
   default:
    break;
  }
  return NULL;
}


void generate_tac() {

}

/****************************************************************************
 * DIAGNOSTICS
 ***************************************************************************/

void print_tac(TAC *t) {
  printf("%s := %s %s %s\n", t->result, t->arg1, t->op, t->arg2);
}
