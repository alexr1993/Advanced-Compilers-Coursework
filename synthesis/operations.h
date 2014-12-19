#ifndef SYNTHESIS_OPERATIONS_H_
#define SYNTHESIS_OPERATIONS_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

#include "evaluate.h"
#include "interpret.h"
#include "tac.h"

VALUE *evaluate_leaf(NODE *n, FRAME *f, EVAL_TYPE e_type);

VALUE *arithmetic(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type);
VALUE *logic(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type);
VALUE *control(NODE *n, VALUE *l, VALUE *r, FRAME *f, EVAL_TYPE e_type);
#endif
