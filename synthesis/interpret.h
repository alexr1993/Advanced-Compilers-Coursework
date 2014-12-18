#ifndef SYNTHESIS_INTERPRET_H_
#define SYNTHESIS_INTERPRET_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

int interpret_arithmetic(int op, int l, int r);
int interpret_logic(int op, int l, int r);

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r);
VALUE *call(char *name, FRAME *caller);
#endif
