#ifndef SYNTHESIS_OPERATIONS_H_
#define SYNTHESIS_OPERATIONS_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

#include "evaluate.h"
#include "interpret.h"
#include "tac.h"

enum eval_type;

VALUE *evaluate_leaf(NODE *n, enum eval_type e_type);

VALUE *arithmetic(NODE *n, VALUE *l, VALUE *r, enum eval_type e_type);
VALUE *logic(NODE *n, VALUE *l, VALUE *r, enum eval_type e_type);
VALUE *control(NODE *n, VALUE *l, VALUE *r, enum eval_type e_type);
#endif
