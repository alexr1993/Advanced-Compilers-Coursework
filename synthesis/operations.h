#ifndef SYNTHESIS_OPERATIONS_H_
#define SYNTHESIS_OPERATIONS_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

#include "evaluate.h"
#include "interpret.h"
#include "tac.h"

EVAL *evaluate_leaf(NODE *n, FRAME *f);

EVAL *arithmetic(NODE *n, EVAL *l, EVAL *r, FRAME *f);
EVAL *logic(NODE *n, EVAL *l, EVAL *r, FRAME *f);
EVAL *control(NODE *n, EVAL *l, EVAL *r, FRAME *f);
#endif
