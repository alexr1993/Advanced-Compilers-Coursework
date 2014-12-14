#ifndef EVALUATION_EVALUATE_
#define EVALUATION_EVALUATE_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "function.h"
#include "param.h"

#include "analysis/C.tab.h"
#include "analysis/nodes.h"
#include "analysis/token.h"
#include "analysis/environment.h"

#define EVAL 1
#define FIRST_PASS 2
#define TAC 3
#define MIPS 4

ENV *evaluate (NODE *node, NODE *parent, FRAME *frame, int eval_mode);

#endif
