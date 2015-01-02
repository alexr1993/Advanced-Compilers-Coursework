#ifndef SYNTHESIS_MIPS_H_
#define SYNTHESIS_MIPS_H_

#define NREGISTERS 32

#include "tac.h"
#include "evaluate.h"

#include "analysis/environment.h"
#include "analysis/token.h"

#include "common/util.h"

typedef struct mips {
  TAC *corresp_tac;
  TOKEN *arg;
  TOKEN *result;
  int op;
  char *str;
  struct mips *next;
  struct mips *prev;
} MIPS;

ADDR_DESC *reg_descriptors[NREGISTERS];

MIPS *generate_mips(TAC *first);


#endif
