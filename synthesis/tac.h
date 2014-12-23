#ifndef SYNTHESIS_TAC_H_
#define SYNTHESIS_TAC_H_

#include "evaluate.h"

#include "analysis/token.h"
#include "analysis/environment.h"
#include "analysis/nodes.h"

typedef struct tac {
  TOKEN *arg1;
  TOKEN *arg2;
  TOKEN *result;
  TOKEN *op;
} TAC;

TAC *tac_leaf(NODE *, FRAME *);
TAC *tac_arithmetic(NODE *, TAC *, TAC *);
TAC *tac_logic(NODE *, TAC *, TAC *);
TAC *tac_control(NODE *, TAC *, TAC *);

TAC *new_tac(TOKEN *, TOKEN *, TOKEN *, TOKEN *);

void generate_tac();

void print_tac(TAC *);
#endif
