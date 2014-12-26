#include "evaluate.h"

#include <stdio.h>
#include <stdlib.h>

#include "interpret.h"
#include "tac.h"
#include "mips.h"
#include "operations.h"

extern int V,v;

EVAL *new_eval(void *obj) {
  EVAL *eval = malloc(sizeof(EVAL));
  switch(e_type) {
   case INTERPRET:
    eval->val = (VALUE *)obj;
    break;
   case IR:
    eval->code = (TAC *)obj;
    break;
   default:
    perror("Error: Invalid e_type\n");
    return NULL;
  }
  return eval;
}

void print_eval(EVAL *obj) {
  if (obj == NULL) return;
  if (e_type == INTERPRET) print_val(obj->val);
  if (e_type == IR) print_tac(obj->code);
}

/* Post order traversal of abstract syntax tree */
EVAL *evaluate(NODE *n, FRAME *f) {
  if (n->type == LEAF) {
    EVAL *eval = evaluate_leaf(n, f);
    if (V) {
      printf("EVALUATE Leaf: \n  ");
      print_token(get_token(n));
      printf("  ");
      print_eval(eval);
    }
    return eval;
  }

  /* Eval children */
  EVAL *l = NULL, *r = NULL;
  // TODO encapsulate this "should_evaluate()"
  if (n->type == 'd' || (n->type == 'D' && n != f->root)) return NULL;
  if (n->left) l = evaluate(n->left, f);

  // Certain control statement will evaluate the right child themselves if
  // necessary
  if (n->right && n->type != ';' && n->type != IF && n->type != APPLY) {
    r = evaluate(n->right, f); // don't want to do this for IF and ;
  }

  if (V) {
    printf("EVALUATE (\"%s\") %s\n", f->proc_id, named(n->type));
    printf("  L:");
    print_eval(l);
    printf("  R:");
    print_eval(r);
  }
  /* Eval node */
  switch(n->type) {
   /* Arithmetic */
   case '+': case '-': case '*': case '/':
    return arithmetic(n, l, r, f);
   /* Logic */
   case '<': case '>': case LE_OP: case GE_OP: case EQ_OP: case NE_OP:
    return logic(n, l, r, f);
   /* Control Flow */
   case APPLY: case IF: case ELSE: case RETURN: case BREAK: case ';': case '=':
    return control(n, l, r, f);
   default:
    return r; // TODO this is a stab in the dark, check function AST structure
  }
}

