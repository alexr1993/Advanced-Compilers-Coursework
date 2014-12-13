#include <stdio.h>
#include "C.tab.h"
#include <string.h>
#include <stdlib.h>

#include "token.h"
extern int V;

TOKEN *new_token(int type)
{
    TOKEN *ans = (TOKEN*)malloc(sizeof(TOKEN));
    ans->type = type;
    ans->newly_created = true;
    return ans;
}

TOKEN *make_string(char *s)
{
    TOKEN *ans = new_token(STRING_LITERAL);
    int len = strlen(s);
    ans->lexeme = (char*)calloc(1, len-1);
    strncpy(ans->lexeme, s+1, len-2);
    return ans;
}

TOKEN *make_int(char *s)
{
    int n = *s!='\'' ? atoi(s) : *(s+1);
    TOKEN *ans = new_token(CONSTANT);
    ans->value = n;
    return ans;
}

TOKEN *make_identifier(char *s)
{
    TOKEN *ans = new_token(IDENTIFIER);
    ans->lexeme = (char*)malloc(1+strlen(s));
    strcpy(ans->lexeme, s);
    return ans;
}

/* The type of the token is not the type of the value which identifiers
 * point to */
char *type_to_str(int type) {
  switch(type) {
    case STRING_LITERAL:
      return "STRING_LITERAL";
    case CONSTANT:
      return "CONSTANT";
    case IDENTIFIER:
      return "IDENTIFIER";
    default:
      return "TYPE_NOT_RECOGNISED";
  }
}

void print_token(TOKEN *t) {
    printf("TOKEN type: %s, lexeme: \"%s\", new? %s\n",
            type_to_str(t->type),
            t->lexeme,
            t->newly_created ? "yes" : "no");
}

/* The token stack is useful for populating frames during parsing */

void init_token_stack() {
    ts = malloc(sizeof(struct token_stack));
}

/* Push down the token stack */
void push(TOKEN *t) {
  if (ts->size == 0) {
    ts->top = t;
  } else {
    t->next = ts->top;
    ts->top = t;
  }
  ts->size++;
  if (V) printf("\nTOKEN push\n");
}

/* Return the top of the token stack */
TOKEN *pop() {
  if (ts->size == 0) return NULL;
  TOKEN *rtn = ts->top;
  ts->top = ts->top->next;
  rtn->next = NULL;
  ts->size--;
  if (V) printf("\nTOKEN pop\n");
  return rtn;
}

void print_token_stack() {
  TOKEN *t = ts->top;

  while (t != NULL) {
    print_token(t);
    t = t->next;
  }
}
