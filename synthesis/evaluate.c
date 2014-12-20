#include "evaluate.h"

#include "interpret.h"
#include "tac.h"
#include "mips.h"
#include "operations.h"

extern int V,v;

/* Post order traversal of abstract syntax tree */
VALUE *evaluate(NODE *n, FRAME *f, EVAL_TYPE e_type) {
  if (n->type == LEAF) {
    VALUE *val = evaluate_leaf(n, f, e_type);
    if (V) {
      printf("EVALUATE Leaf: \n  ");
      print_token(get_token(n));
      printf("  ");
      print_val(val);
    }
    return val;
  }

  /* Eval children */
  VALUE *l, *r;
  if (str_eq(named(n->type), "d")) return NULL;
  if (n->left)  l = evaluate(n->left, f, e_type);
  if (n->right) r = evaluate(n->right, f, e_type);

  if (V) {
    printf("EVALUATE %s\n", named(n->type));
    printf("  L:");
    print_val(l);
    printf("  R:");
    print_val(r);
  }
  /* Eval node */
  switch(n->type) {
   /* Arithmetic */
   case '+': case '-': case '*': case '/':
    return arithmetic(n, l, r, f, e_type);
   /* Logic */
   case '<': case '>': case LE_OP: case GE_OP: case EQ_OP: case NE_OP:
    return logic(n, l, r, f, e_type);
   /* Control Flow */
   case APPLY: case IF: case ELSE: case RETURN: case BREAK:
    return control(n, l, r, f, e_type);
   default:
    return r; // TODO this is a stab in the dark, check function AST structure
  }
}
