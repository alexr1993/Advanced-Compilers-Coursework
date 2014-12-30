#ifndef SYNTHESIS_MIPS_H_
#define SYNTHESIS_MIPS_H_

#include "tac.h"
#include "evaluate.h"

#include "analysis/environment.h"

#include "common/util.h"

typedef struct mips {
  TAC *corresp_tac;
} MIPS;

MIPS *gen_mips(TAC *first);


#endif
