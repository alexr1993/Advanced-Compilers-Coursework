#include "evaluate.h"

#include <stdio.h>

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
  VALUE *l = NULL, *r = NULL;
  // TODO encapsulate this "should_evaluate()"
  if (n->type == 'd' || n->type == 'D' && n != f->root) return NULL;
  if (n->left) l = evaluate(n->left, f, e_type);

  // Certain control statement will evaluate the right child themselves if
  // necessary
  if (n->right && n->type != ';' && n->type != IF) {
    r = evaluate(n->right, f, e_type); // don't want to do this for IF and ;
  }

  if (V) {
    printf("EVALUATE (\"%s\") %s\n", f->proc_id, named(n->type));
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
   case APPLY: case IF: case ELSE: case RETURN: case BREAK: case ';': case '=':
    return control(n, l, r, f, e_type);
   default:
    return r; // TODO this is a stab in the dark, check function AST structure
  }
}
