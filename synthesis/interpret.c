#include "interpret.h"

#include "evaluate.h"

VALUE *interpret_leaf(NODE *n, FRAME *f) {
  TOKEN *t = get_token(n);

  switch(t->type) {
   case IDENTIFIER:
    return t->val; // Not sure if this will always reference the correct var
    // may have to use get_val(t->lexeme, f);
   case CONSTANT:
    return new_val(INT_TYPE, new_int_state(t->value));
   default:
    return NULL;
  }
}

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

void bind_arg(FRAME *caller, FRAME *callee) {

}

VALUE *call(char *name, FRAME *caller) {
  return evaluate(get_frame(name, caller)->root, caller, INTERPRET); 
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r) {
  return NULL; // TODO
}
