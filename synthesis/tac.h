#ifndef SYNTHESIS_TAC_H_
#define SYNTHESIS_TAC_H_

#include "analysis/token.h"
#include "analysis/environment.h"
#include "analysis/nodes.h"

typedef struct tac {
  TOKEN *arg1;
  TOKEN *arg2;
  TOKEN *result;
  TOKEN *op;
} TAC;

VALUE *tac_leaf(NODE *, FRAME *);
void tac_arithmetic(NODE *, VALUE *, VALUE *);
void tac_logic(NODE *, VALUE *, VALUE *);
void tac_control(NODE *, VALUE *, VALUE *);

TAC *new_tac(TOKEN *, TOKEN *, TOKEN *, TOKEN *);
void print_tac(TAC *);

void generate_tac();
#endif
