#ifndef ANALYSIS_TOKEN_H_
#define ANALYSIS_TOKEN_H_

struct frame;
struct value;
#include "environment.h"

#define TRUE 1
#define FALSE 0
#define TEST_MODE 0

/* Data */
typedef enum DECLARATION_TYPE { VARIABLE, PARAMETER } DECLARATION_TYPE;

typedef struct token {
  int           type; // Type of lexical token (IDENTIFIER | CONSTANT ... )
  char          *lexeme;
  int           value;
  struct value  *val;
  struct token  *next;
  int           newly_created;
  // For IDENTIFIERS only
  int data_type;
  DECLARATION_TYPE declaration_type;
} TOKEN;

struct token_stack {
  int size;
  TOKEN *top;
};
struct token_stack *ts;

TOKEN *int_token, *void_token, *function_token;

/* Functions */
TOKEN* new_token(int);
TOKEN *make_string(char *s);
TOKEN *make_int(char *s);
TOKEN *make_identifier(char *s);

void init_token_stack();
void print_token(TOKEN *t);

TOKEN *pop();
void push(TOKEN *);
void print_token_stack();

void mark_params();

char *type_to_str(int);
#endif
