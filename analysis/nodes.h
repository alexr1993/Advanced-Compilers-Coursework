#ifndef ANALYSIS_NODES_H_
#define ANALYSIS_NODES_H_

struct token;

#include "token.h"
#include "environment.h"

typedef struct node {
  int          type;
  struct node *left;
  struct node *right;
  struct node *next_D;
  struct frame *frame;
} NODE;

NODE *make_leaf(struct token *);
NODE *make_node(int, NODE*, NODE*);
int is_leaf(NODE *node);
struct token *get_token(NODE *node);
void print_node(NODE *node);
#endif
