#include "interpret.h"

#include "evaluate.h"
int interpret_arithmetic(int op, int l, int r) {
  switch(op) {
   case '+':
    return l + r;
   default:
    return 0;
  }
}

int interpret_logic(int op, int l, int r) {
  switch(op) {
   case EQ_OP:
    return l == r;
   default:
    return 0;
  }
}

/* Control Flow */
void bind_arg(FRAME *caller, FRAME *callee) {

}

/* Calls the function by name, given its parent */
VALUE *call(char *name, FRAME *caller) {
  return evaluate(get_frame(name, caller)->root, caller, INTERPRET); 
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r) {
  return NULL; // TODO
}
