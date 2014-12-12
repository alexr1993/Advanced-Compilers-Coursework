#ifndef __TOKEN_H
#define __TOKEN_H

typedef union STATE STATE;
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
} TOKEN;

extern TOKEN* new_token(int);

#endif
