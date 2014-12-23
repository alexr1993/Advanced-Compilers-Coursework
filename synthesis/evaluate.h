#ifndef SYNTHESIS_EVALUATE_
#define SYNTHESIS_EVALUATE_

typedef enum eval_type { INTERPRET, IR } EVAL_TYPE;

#include "analysis/nodes.h"
#include "analysis/environment.h"
#include "analysis/token.h"

VALUE *evaluate(NODE *, FRAME *, EVAL_TYPE);

#endif
