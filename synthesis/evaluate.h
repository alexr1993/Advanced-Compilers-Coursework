#ifndef SYNTHESIS_EVALUATE_
#define SYNTHESIS_EVALUATE_

#include "analysis/nodes.h"

typedef enum { INTERPRET, TAC } eval_type;
VALUE *evaluate(NODE *, eval_type);

#endif
