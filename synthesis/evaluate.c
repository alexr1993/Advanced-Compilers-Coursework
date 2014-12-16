#include "interpret.h"
#include "tac.h"
#include "mips.h"

/* Things to evaluate
 *
 *
 * Arithmetic * /  + -
 * Logic      == < > != || && !
 * Control flow APPLY IF ELSE WHILE CONTINUE BREAK RETURN
 */

VALUE *evaluate(NODE *n) {
  switch(n->type) {
   case '+': case '-': case '*': case '/':
    break;
   case '<': case '>': case LE_OP: case GE_OP: case EQ_OP: case NE_OP:
    break;
   case LEAF:
    return evaluate_leaf(n);
   case APPLY:
    break;
   case IF:
    break;
   case ELSE:
    break;
   case RETURN:
    break;
   case BREAK:
    break;
   default:
    return NULL;
  }
}


