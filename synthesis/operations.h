#ifndef SYNTHESIS_OPERATIONS_H_
#define SYNTHESIS_OPERATIONS_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

#include "evaluate.h"
#include "interpret.h"
#include "tac.h"

EVAL *evaluate_leaf(NODE *n, FRAME *f, EVAL_TYPE e_type);

EVAL *arithmetic(NODE *n, EVAL *l, EVAL *r, FRAME *f, EVAL_TYPE e_type);
EVAL *logic(NODE *n, EVAL *l, EVAL *r, FRAME *f, EVAL_TYPE e_type);
EVAL *control(NODE *n, EVAL *l, EVAL *r, FRAME *f, EVAL_TYPE e_type);
#endif
