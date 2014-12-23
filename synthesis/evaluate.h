#ifndef SYNTHESIS_EVALUATE_H_
#define SYNTHESIS_EVALUATE_H_

typedef enum eval_type { INTERPRET, IR } EVAL_TYPE;
EVAL_TYPE e_type;

#include "tac.h"

#include "analysis/nodes.h"
#include "analysis/environment.h"
#include "analysis/token.h"

struct tac;

typedef union eval {
  VALUE *val;
  struct tac *code;
} EVAL;

EVAL *evaluate(NODE *, FRAME *, EVAL_TYPE);
EVAL *new_eval(void *);
#endif
