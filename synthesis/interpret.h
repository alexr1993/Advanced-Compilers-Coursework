#ifndef SYNTHESIS_INTERPRET_H_
#define SYNTHESIS_INTERPRET_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

VALUE *interpret_leaf(NODE *n, FRAME *f);

int interpret_arithmetic(int op, int l, int r);
int interpret_logic(int op, int l, int r);

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r, FRAME *f);
VALUE *interpret_program();
VALUE *call(function *func);
#endif
