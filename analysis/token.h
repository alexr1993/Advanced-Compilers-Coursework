#ifndef __TOKEN_H
#define __TOKEN_H

typedef union STATE STATE;
typedef struct FRAME FRAME;
#include "evaluation/environment.h"

#define TRUE 1
#define FALSE 0
#define TEST_MODE 0

typedef struct TOKEN
{
  int           type; // Use tokens to store type info when walking down the tree
  char          *lexeme;
  int           value;
  STATE         *state;
  struct TOKEN  *next;
  int           newly_created;
  FRAME        *frame;
} TOKEN;

struct token_stack {
  int size;
  TOKEN *top;
};
struct token_stack *ts;

TOKEN* new_token(int);
TOKEN *make_string(char *s);
TOKEN *make_int(char *s);
TOKEN *make_identifier(char *s);

void init_token_stack();
void print_token(TOKEN *t);
TOKEN *pop();
void push(TOKEN *);
#endif
