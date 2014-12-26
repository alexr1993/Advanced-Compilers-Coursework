#include "tac.h"

#include <stdio.h>
#include <stdlib.h>

extern int V, v;

/****************************************************************************
 * INSTANCE MANIPULATION
 ***************************************************************************/

TAC *new_tac(TOKEN *arg1, TOKEN *arg2, TOKEN *result, int op) {
  TAC *code = malloc(sizeof(TAC));
  code->arg1   = arg1;
  code->arg2   = arg2;
  code->op     = op;
  code->result = result;
  // Print TAC if it is not simply hosting a constant integer
  if (result == NULL || result->type != CONSTANT) print_tac(code);
  return code;
}

TOKEN *new_temp() {
  static int id = 0;
  id++;
  char name[2];
  sprintf(name, "t%d", id);
  if (V) printf("TAC \"%s\" created\n", name);
  return make_identifier(name);
}

/****************************************************************************
 * TRANSLATION
 ***************************************************************************/

TAC *tac_leaf(NODE *n, FRAME *f) {
  TOKEN *t = get_token(n);
  TAC *code;
  switch(t->type) {
   case IDENTIFIER:
    code = new_tac(NULL, NULL, t, LOAD);
    break;
   case CONSTANT:
    code = new_tac(NULL, NULL, t, 0);
    break;
   default:
    return NULL;
  }
  return code;
}

TAC *tac_arithmetic(NODE *n, TAC *l, TAC* r) {
  return new_tac(l->result , r->result, new_temp(), n->type);
}

TAC *tac_logic(NODE *n, TAC *l, TAC *r) {
  return new_tac(l->result , r->result, new_temp(), n->type);
}

void push_arg(TAC *code) {
  switch(code->result->type) {
   case IDENTIFIER:
    printf("PUSH %s\n", code->result->lexeme);
    break;
   case CONSTANT:
    printf("PUSH %d\n", code->result->value);
    break;
  }
}

void push_args(NODE *argstree, FRAME *f) {
  if (argstree->type == ',') {
    push_args(argstree->left, f);
    push_args(argstree->right, f);
  } else {
    push_arg(evaluate(argstree, f)->code);
  }
}

TAC *tac_control(NODE *n, TAC *l, TAC *r, FRAME *f) {
  TAC *code;
  TAC *tmp;
  //return new_tac(l->result, r->result, new_temp(), n->type);
  switch(n->type) {
   case APPLY:
    push_args(n->right, f);
    code = new_tac(l->result, NULL, new_temp(), APPLY);
    break;
   case IF:
    break;
   case RETURN:
    code = new_tac(l->result, NULL, NULL, RETURN);
    break;
   case ';':
    tmp = evaluate(n->left, f)->code;
    code = evaluate(n->right, f)->code;
    code->prev = tmp;
    break;
   case '=':
    code = new_tac(r->result, NULL, l->result, '=');
    break;
   default:
    perror("Error: TAC control problem\n");
    code = NULL;
    break;
  }
  return code;
}

void gen(function *func) {
  evaluate(func->frame->root, func->frame);
}

void generate_tac() {
  function *main_fn = get_val("main", gbl_frame)->state->function;
  gen(main_fn);
}

/****************************************************************************
 * DIAGNOSTICS
 ***************************************************************************/

char *op_to_str(TOKEN *t) {
  if (t == NULL) return "";
  static char b[100];
  switch(t->type) {
   case IDENTIFIER:
    return t->lexeme;
   case CONSTANT:
    sprintf(b, "%d", t->value);
    return b;
  }
  return NULL;
}

char *int_to_str(int x) {
  static char str[15];
  sprintf(str, "%d", x);
  return str;
}

void print_tac(TAC *t) {
  if (t == NULL) {
    printf("NULL TAC (print_tac)\n");
    return;
  }
  /* Arithmetic/Logic */
  if (t->result != NULL && t->arg1 != NULL)
    printf("%s := %s %s %s\n", op_to_str(t->result),
                               op_to_str(t->arg1),
                               named(t->op),
                               op_to_str(t->arg2));
  /* APPLY, PUSH */
  else if (t->result != NULL)
    printf("%s %s\n", named(t->op),
                         op_to_str(t->result));
  /* =, RETURN (stuff with no result temp) */
  else
    printf("%s %s %s\n", named(t->op),
                         op_to_str(t->arg1),
                         op_to_str(t->arg2));
}
