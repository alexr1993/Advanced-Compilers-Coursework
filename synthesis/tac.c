#include "tac.h"

#include <stdio.h>
#include <stdlib.h>

extern int V, v;
extern NODE *ans;
extern FRAME *gbl_frame;

static bool unresolved_if = false;
static TOKEN *next_label = NULL;;

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
   case RETURN: case PUSH: case LOAD: case GOTO:
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
  if (unresolved_if) {
    code->label = next_label;
  }
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

void push_args(NODE *argstree, FRAME *f) {
  if (argstree->type == ',') {
    push_args(argstree->left, f);
    push_args(argstree->right, f);
  } else {
    new_tac(evaluate(argstree, f)->code->result, NULL, NULL, PUSH);
  }
}

TAC *gen_fn(NODE *D) {
  printf("Begin function \"%s\"\n", D->frame->proc_id);
  // pop params
  TAC *code = evaluate(D->right, D->frame)->code;
  printf("End function \"%s\"\n", D->frame->proc_id);
  return code;
}

TAC *gen_cond(NODE *n, TAC *cond, FRAME *f) {
  //Create true f'nality
  TOKEN *true_label = new_label();
  new_tac(cond->result, true_label, NULL, IF); // if true jump to true

  NODE *false_root = get_false_root(n);

  // False branch, jumps to label with currently unknown destination
  evaluate(false_root, f);
  TAC *goto_exit = new_tac(new_label(), NULL, NULL, GOTO); // Exit cond

  // True branch
  NODE *true_root = get_true_root(n);
  TAC *true_code = evaluate(true_root,f)->code;
  true_code->label = true_label; // Mark as destination for true jump
  printf("(2) "); print_tac(true_code);

  // TODO this is an open issue - the best resolution is to set the label to 
  // 'return' once the function exits with not statements to jump to after the
  // if, this would be after gen_fn or something.
  // next created TAC must carry next_label or change the label to a 'return'
  unresolved_if = true;
  next_label = goto_exit->arg1;
}

TAC *tac_control(NODE *n, TAC *l, TAC *r, FRAME *f) {
  TAC *code;
  TAC *tmp;
  //return new_tac(l->result, r->result, new_temp(), n->type);
  switch(n->type) {
   case 'D':
    code = gen_fn(n);
    break;
   case APPLY:
    push_args(n->right, f);
    code = new_tac(l->result, NULL, new_temp(), APPLY);
    // TODO jump to label
    // goto(fn->label)
    break;
   case IF:
    // TODO evaluate boolean and jump
    code = gen_cond(n, l, f);
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
    //perror("Error: TAC control problem\n");
    code = NULL;
    break;
  }
  return code;
}

void gen(function *func) {
  evaluate(func->frame->root, func->frame);
}

void generate_tac() {
  //function *main_fn = get_val("main", gbl_frame)->state->function;
 evaluate(ans, gbl_frame);
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
  if (t->label != NULL) {
    printf("%s: ", t->label->lexeme);
    printf("%s\n", t->str);
  } else {
    printf("    %s\n", t->str);
  }
}

