#ifndef __NODES_H
#define __NODES_H

typedef struct TOKEN TOKEN;
#include "token.h"

typedef struct node {
  int          type;
  struct node *left;
  struct node *right;
} NODE;

NODE* make_leaf(TOKEN*);
NODE* make_node(int, NODE*, NODE*);

#endif
