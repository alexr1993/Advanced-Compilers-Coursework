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
  int op;
  struct tac *next;
  struct tac *prev;
  char *str;
  TOKEN *label;
} TAC;

typedef struct label {
  char *name;
  TAC *code;
} LABEL;

TAC *tac_leaf(NODE *, FRAME *);
TAC *tac_arith_and_logic(NODE *, TAC *, TAC *);
TAC *tac_control(NODE *, TAC *, TAC *, FRAME *);
void link_tac(TAC *, TAC *);
TAC *new_tac(TOKEN *, TOKEN *, TOKEN *, int, TAC *);

TAC *generate_tac();

void print_tac(TAC *);
#endif
