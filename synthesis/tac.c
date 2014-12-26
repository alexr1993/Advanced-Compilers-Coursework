#include "tac.h"

#include <stdio.h>
#include <stdlib.h>

extern int V, v;

/****************************************************************************
 * INSTANCE MANIPULATION
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

void create_str_rep(TAC *code) {
  char *str = malloc(50);

  switch(code->op) {
   case APPLY:
    sprintf(str, "%s := %s %s", op_to_str(code->result), named(code->op),
            op_to_str(code->arg1));
    break;
   case IF:
    sprintf(str, "if %s goto %s", op_to_str(code->arg1), op_to_str(code->arg2));
    break;
   case RETURN: case PUSH: case LOAD:
    sprintf(str, "%s %s", named(code->op), op_to_str(code->arg1));
    break;
   case '=':
    sprintf(str, "%s := %s", op_to_str(code->result), op_to_str(code->arg1));
    break;
   default:
    sprintf(str,                  "%s := %s %s %s", op_to_str(code->result),
            op_to_str(code->arg1), named(code->op), op_to_str(code->arg2));
  }
  code->str = str;
}

TAC *new_tac(TOKEN *arg1, TOKEN *arg2, TOKEN *result, int op) {
  TAC *code = malloc(sizeof(TAC));
  code->arg1   = arg1;
  code->arg2   = arg2;
  code->op     = op;
  code->result = result;
  if (op == 0) {
    code->str = NULL;
  } else {
    create_str_rep(code);
    print_tac(code);
  }
  return code;
}

TOKEN *new_temp() {
  static int id = 0;
  id++;
  char name[4];
  sprintf(name, "t%d", id);
  if (V) printf("TAC \"%s\" created\n", name);
  return make_identifier(name);
}

TOKEN *new_label() {
  static int id = 0;
  id++;
  char name[4];
  sprintf(name, "L%d", id);
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
    code = new_tac(t, NULL, t, LOAD);
    break;
   case CONSTANT:
    code = new_tac(t, NULL, t, 0);
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
    // TODO jump to label
    break;
   case IF:
    // TODO evaluate boolean and jump
    code = new_tac(l->result, new_label(), NULL, IF);
    break;
   case RETURN:
    code = new_tac(l->result, NULL, NULL, RETURN);
    break;
   case ';':
    //tmp = evaluate(n->left, f)->code;
    code = evaluate(n->right, f)->code;
    //code->prev = tmp;
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

void print_tac(TAC *t) {
  if (t == NULL) {
    printf("NULL TAC (print_tac)\n");
    return;
  } else if (t->str == NULL) {
    printf("TAC has no string representation\n");
    return;
  }
  printf("%s\n", t->str);
}

