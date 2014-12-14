#ifndef COMMON_OPERATIONS_
#define COMMON_OPERATIONS_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "evaluation/evaluate.h"
#include "evaluation/function.h"
#include "evaluation/param.h"

#include "analysis/environment.h"
#include "analysis/C.tab.h"
#include "analysis/nodes.h"
#include "analysis/token.h"
ENV *D(NODE *parent, NODE *operator, ENV *left_operand,
       ENV *right_operand, FRAME *frame, int mode);
#endif
