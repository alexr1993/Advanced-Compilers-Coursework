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
  char *b = malloc(100);
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
   case RETURN: case PUSH: case LOAD: case GOTO: case POP: case START:
   case END:
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

TAC *traverse_to_end(TAC *code) {
  if (code == NULL) return NULL;
  while (code->next != NULL) {
    code = code->next;
  }
  return code;
}

TAC *traverse_to_start(TAC *code) {
  if (code == NULL) return NULL;
  while (code->prev != NULL) {
    code = code->prev;
  }
  return code;
}

// Links next to the end of prev
void link_tac(TAC *prev, TAC *next) {
  if (prev == NULL || next == NULL) perror("Error! Linking NULL");
  prev = traverse_to_end(prev);
  next = traverse_to_start(next);
  prev->next = next;
  next->prev = prev;
}

TAC *new_tac(TOKEN *arg1, TOKEN *arg2, TOKEN *result, int op, TAC *prev) {
  TAC *code = malloc(sizeof(TAC));
  code->arg1   = arg1;
  code->arg2   = arg2;
  code->op     = op;
  code->result = result;
  code->next   = NULL;
  if (prev != NULL) link_tac(prev, code);

  if (unresolved_if) {
    code->label = next_label;
    unresolved_if = false;
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
  switch(t->type) {
   case IDENTIFIER:
    return new_tac(t, NULL, t, LOAD, NULL);
    break;
   case CONSTANT:
    return new_tac(t, NULL, t, 0, NULL);
    break;
   default: // e.g. type labels "int" and "function"
    return NULL;
  }
}

TAC *tac_arith_and_logic(NODE *n, TAC *l, TAC* r) {
  l = traverse_to_end(l);
  r = traverse_to_end(r);
  link_tac(l, r);
  new_tac(l->result, r->result, new_temp(), n->type, r);
  return l;
}

TAC *push_args(NODE *argstree, FRAME *f, TAC *prev) {
  if (argstree->type == ',') {
    return push_args(argstree->right, f, push_args(argstree->left, f, prev));
  } else {
    new_tac(evaluate(argstree, f)->code->result, NULL, NULL, PUSH, prev);
    return prev;
  }
}

/* Generate function code and return the first instruction */
TAC *gen_fn(NODE *D) {
  // Begin
  TOKEN *fn_label = make_identifier(D->frame->proc_id);
  TAC *begin      = new_tac(fn_label, NULL, NULL, START, NULL);
  begin->label    = fn_label;
  TAC *prev       = begin;

  // Pop params
  function *fn = get_val(D->frame->proc_id, D->frame)->state->function;
  PARAM *tmp = fn->params;

  while (tmp != NULL) {
    prev = new_tac(tmp->token, NULL, tmp->token, POP, prev);
    tmp = tmp->next;
  }

  // Gen body
  TAC *code = evaluate(D->right, D->frame)->code;
  link_tac(prev, code);

  // End
  new_tac(fn_label, NULL, NULL, END, code);
  return begin;
}

/* Generate conditional code and return the first instruction */
TAC *gen_cond(NODE *n, TAC *cond, FRAME *f) {
  //Create true f'nality
  TOKEN *true_label = new_label();

  // if true jump to true
  TAC *if_start = new_tac(cond->result, true_label, NULL, IF, cond);
  NODE *false_root = get_false_root(n);
  // Jumps to label with currently unknown destination
  TAC *goto_exit = NULL;

  if (false_root != NULL) {
    // There is a false branch
    TAC *false_cond = evaluate(false_root, f)->code;
    link_tac(if_start, false_cond);
    goto_exit = new_tac(new_label(), NULL, NULL, GOTO, false_cond);
  } else {
    // There is no execution for the false outcome
    goto_exit = new_tac(new_label(), NULL, NULL, GOTO, if_start);
  }

  // True branch
  NODE *true_root = get_true_root(n);
  TAC *true_code = evaluate(true_root,f)->code;
  link_tac(goto_exit, true_code);
  true_code->label = true_label; // Mark as destination for true jump
  printf("(2) "); print_tac(true_code);

  // TODO this is an open issue - the best resolution is to set the label to 
  // 'return' once the function exits with not statements to jump to after the
  // if, this would be after gen_fn or something.
  // next created TAC must carry next_label or change the label to a 'return'
  unresolved_if = true;
  next_label = goto_exit->arg1;
  return cond;
}

/* Returns the first code out of the series it creates */
TAC *tac_control(NODE *n, TAC *l, TAC *r, FRAME *f) {
  l = traverse_to_end(l);
  r = traverse_to_end(r);

  switch(n->type) {
   case 'D':
    return gen_fn(n);
   case APPLY: // Eval args if any
    if (n->right != NULL) {
      new_tac(l->result, NULL, new_temp(), APPLY, push_args(n->right, f, l));
    } else {
      new_tac(l->result, NULL, new_temp(), APPLY, l);
    }
    return l;
   case IF: // in-order
    return gen_cond(n, l, f);
   case RETURN: // Unary
    new_tac(l->result, NULL, NULL, RETURN, l);
    return l;
   case ';': // in-order
    link_tac(l, r);
    return l;
    break;
   case '=': // post-order
    link_tac(l, r);
    new_tac(r->result, NULL, l->result, '=', r);
    return l;
   default:
    perror("Error: TAC control problem\n");
    abort();
  }
}
void print_program(TAC *t);

TAC *generate_tac() {
  e_type = IR;
  TAC *code = evaluate(ans, gbl_frame)->code;
  print_program(code);
  return code;
}

/****************************************************************************
 * DIAGNOSTICS
 ***************************************************************************/

void print_tac(TAC *t) {
  if (t == NULL) {
    printf("NULL TAC (print_tac)\n");
    return;
  } else if (t->str == NULL) {
    if (V) printf("TAC has no string representation\n");
    if (V) printf("TAC contains constant: %d\n", t->result->value);
    return;
  }
  if (t->label != NULL) {
    printf("%s: ", t->label->lexeme);
    printf("%s\n", t->str);
  } else {
    printf("    %s\n", t->str);
  }
}

void print_program(TAC *t) {
 printf("\n%s\nGenerated TAC\n%s\n", LINE, LINE);
  while (t != NULL) {
    print_tac(t);
    t = t->next;
  }
 printf("\n%s\nEND TAC\n%s\n", LINE, LINE);
}
