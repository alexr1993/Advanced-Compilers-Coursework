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
  if (obj == NULL) {
    printf("\n");
    return;
  }
  if (e_type == INTERPRET) print_val(obj->val);
  if (e_type == IR) print_tac(obj->code);
}

bool should_eval_l(NODE *n, FRAME *f) {
  if (n->left == NULL) return false;
  // Fn declarations handled in first pass
  return n->type != 'D';
}

bool should_eval_r(NODE *n, FRAME *f) {
  if (n->right == NULL) return false;
  if (e_type == INTERPRET) {
    switch(n->type) {
     // Only eval function body if it belongs to the current frame
     case 'D':
      return n == f->root;
     case ';': case IF: case APPLY:
      return false;
     default:
      return true;
    }
  } else if (e_type == IR) {
    switch(n->type) {
     case 'D':
      return n == f->root;
     case IF: case ';':
      return false;
     default:
      return true;
    }
  }
  perror("Something's wrong");
  return false;
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
  if (should_eval_l(n, f)) l = evaluate(n->left,  f);
  if (should_eval_r(n, f)) r = evaluate(n->right, f);
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
   case 'D':
    return control(n, l, r, f);
   default:
    return r; // TODO this is a stab in the dark, check function AST structure
  }
}

