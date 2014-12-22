#include "interpret.h"

#include <stdio.h>
#include <stdlib.h>

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

/* In-order traversal of argstree, evaluates the args and stores the resulting
 * values in args
 */
void arg_traversal(NODE *argstree, FRAME *caller, VALUE **args,
                   int *current_arg_ptr,     int nargs) {

  if (*current_arg_ptr == nargs) return; // ignore excess args

  if (argstree->type == ',') {
    arg_traversal(argstree->left, caller, args, current_arg_ptr, nargs);
    arg_traversal(argstree->right, caller, args, current_arg_ptr, nargs);
  } else {
    args[*current_arg_ptr] = evaluate(argstree, caller, INTERPRET);
    (*current_arg_ptr)++;
    if (V) printf("INTERPRET Evaluated arg %d\n", *current_arg_ptr);
  }
}

/* Creates array of arg values from an args subtree (the right child of an
 * APPLY node
 */
VALUE **eval_args(NODE *argstree, int nargs, FRAME *caller) {
  /* Linearise args tree */
  VALUE **args = malloc(nargs * sizeof(VALUE));
  int current_arg = 0;
  arg_traversal(argstree, caller, args, &current_arg, nargs);
  return args;
}

/* Eval and assign arguments to parameters */
void bind_args(function *func, NODE *argstree, FRAME *caller) {
  if (V) printf("INTERPRET Evaluating arguments for \"%s\" call\n",
                func->proc_id);
  VALUE **args = eval_args(argstree, func->nparams, caller);

  int i;
  PARAM *p = func->params;
  for(i = 0; i < func->nparams; i++) {
    // FIXME get_val may be overcomplicating the code, token->val may do?
    get_val(p->token->lexeme, func->frame)->state = args[i]->state;
    if (V) printf("INTERPRET Bound param \"%s\" with value:\n",
                  p->token->lexeme);

    if (V) print_val(args[i]);
    p = p->next;
  }
}

VALUE *call(function *func) {
  // TODO create activation record
  if (v) {
    char msg[80];
    sprintf(msg, "INTERPRET Calling %s", func->proc_id);
    print_banner(msg);
  }
  if (V) print_function(func);
  if (V) print_frame(func->frame);
  return evaluate(func->frame->root, func->frame, INTERPRET);
}

VALUE *interpret_program() {
  function *main_fn = get_val("main", gbl_frame)->state->function;
  return call(main_fn);
}

VALUE *interpret_control(NODE *n, VALUE *l, VALUE *r, FRAME *f) {
  bool else_exists;
  NODE *true_eval, *false_eval;

  switch(n->type) {
   case APPLY:
    // todo bind args
    if (V) printf("INTERPRET Function to be called:\n");
    if (V) print_function(l->state->function);
    if (n->right) bind_args(l->state->function, n->right, f);
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
