#ifndef SYNTHESIS_INTERPRET_
#define SYNTHESIS_INTERPRET_

#include "analysis/environment.h"
#include "analysis/nodes.h"

/* Structs needed for CMM runtime */

VALUE *arithmetic(NODE *);
VALUE *boolean(NODE *);
VALUE *assignment(NODE *);
#endif
