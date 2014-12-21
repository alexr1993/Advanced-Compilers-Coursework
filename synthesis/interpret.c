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
   case '-':
    return l - r;
   case '*':
    return l * r;
   case '/':
    return l / r;
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
  FRAME *callee = get_frame(name, caller);
  return evaluate(callee->root, callee, INTERPRET);
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r, FRAME *f) {
  bool else_exists;
  NODE *true_eval, *false_eval;

  switch(n->type) {
   case APPLY:
    // todo bind args
    return call(l->state->function->proc_id, f);
   case IF:
    else_exists = str_eq("else", named(n->right->type));
    true_eval  = else_exists ? n->right->left  : n->right->right;
    false_eval = else_exists ? n->right->right : NULL;

    return evaluate( is_true(l) ? true_eval : false_eval, f, INTERPRET);
   case RETURN:
    if (V) printf("INTERPRET return called for function \"%s\"\n", f->proc_id);
    f->return_called = true;
    return l;
   case BREAK:
    break;
   case ';':
    if (V) printf("INTERPRET ; Return has been called? %s\n",
                  f->return_called ? "yes" : "no");
    return f->return_called ? l : evaluate(n->right, f, INTERPRET);
   default:
    return NULL;
  }
}
