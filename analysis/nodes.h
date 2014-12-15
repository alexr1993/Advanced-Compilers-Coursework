#ifndef __NODES_H
#define __NODES_H

typedef struct TOKEN TOKEN;
#include "token.h"
#include "environment.h"

typedef struct node {
  int          type;
  struct node *left;
  struct node *right;
  struct node *next_D;
  FRAME *frame;
} NODE;

NODE* make_leaf(TOKEN*);
NODE* make_node(int, NODE*, NODE*);
int is_leaf(NODE *node);
TOKEN *get_token(NODE *node);
void print_node(NODE *node);
#endif
