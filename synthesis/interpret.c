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
   case '<':
    return l < r;
   case '>':
    return l > r;
   case LE_OP:
    return l <= r;
   case GE_OP:
    return l >= r;
   case EQ_OP:
    return l == r;
   case NE_OP:
    return l != r;
   default:
    return 0;
  }
}

void bind_arg(FRAME *caller, FRAME *callee) {

}

VALUE *call(function *func) {
  if (v) {
    char msg[80];
    sprintf(msg, "INTERPRET Calling %s", func->proc_id);
    print_banner(msg);
  }
  print_function(func);
  print_frame(func->frame);
  return evaluate(func->frame->root, func->frame, INTERPRET);
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r, FRAME *f) {
  bool else_exists;
  NODE *true_eval, *false_eval;

  switch(n->type) {
   case APPLY:
    // todo bind args
    if (V) printf("Function to be called:\n");
    if (V) print_function(l->state->function);
    return call(l->state->function);

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
    return NULL;

   case ';':
    if (V) printf("INTERPRET ; Return has been called? %s\n",
                  f->return_called ? "yes" : "no");
    return f->return_called ? l : evaluate(n->right, f, INTERPRET);

   // Doesn't really belong here but whatever
   case '=':
    if (V) printf("INTERPRET = Assigning ");
    if (V) print_val(r);
    if (V) printf("to ");
    if (V) print_val(l);
    l->state = r->state;
    return l;
   case 'D':
    return r;
   default:
    return NULL;
  }
}
