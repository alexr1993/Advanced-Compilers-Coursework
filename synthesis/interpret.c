#include "interpret.h"

#include <stdio.h>

#include "evaluate.h"
extern int V,v;

VALUE *interpret_leaf(NODE *n, FRAME *f) {
  TOKEN *t = get_token(n);

  switch(t->type) {
   case IDENTIFIER:
    return get_val(t->lexeme, f);
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
  if (v) {
    char msg[80];
    sprintf(msg, "INTERPRET Calling %s", name);
    print_banner(msg);
  }
  return evaluate(get_frame(name, caller)->root, caller, INTERPRET);
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r, FRAME *f) {
  switch(n->type) {
   case APPLY:
    // return call(name, frame);
    break;
   case IF:
    break;
   case ELSE:
    break;
   case RETURN:
    f->return_called = true;
    return l;
   case BREAK:
   default:
    return NULL;
  }
}
