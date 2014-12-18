#ifndef SYNTHESIS_TAC_H_
#define SYNTHESIS_TAC_H_

#include "analysis/environment.h"
#include "analysis/nodes.h"

void tac_arithmetic(NODE *, VALUE *, VALUE *);
void tac_logic(NODE *, VALUE *, VALUE *);
void tac_control(NODE *, VALUE *, VALUE *);

#endif
