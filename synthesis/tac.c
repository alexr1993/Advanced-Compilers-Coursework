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
  print_tac(code);
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
    printf("LOAD %s\n", t->lexeme);
    code = new_tac(NULL, NULL, t, NULL);
    break;
   case CONSTANT:
    code = new_tac(NULL, NULL, t, NULL);
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
  //return new_tac(l->result, r->result, new_temp(), n->type);
  switch(n->type) {
   case APPLY:
    push_args(n->right, f);
    new_tac(l->result, NULL, new_temp(), APPLY);
    break;
   case IF:
    break;
   case RETURN:
    code = new_tac(NULL, NULL, NULL, RETURN);
    break;
   case ';':
    break;
   case '=':
    code = new_tac(r->result, NULL, l->result, '=');
    break;
   default:
    break;
  }
  return NULL;
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
  if (t == NULL) return NULL;
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
  printf("%s := %s %s %s\n", t->result->lexeme,
                             op_to_str(t->arg1),
                             named(t->op),
                             op_to_str(t->arg2));
}
