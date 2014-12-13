#include <stdio.h>
#include <stdlib.h>
#include "C.tab.h"
#include "token.h"

#include "nodes.h"
extern int counter;
NODE* make_node(int t, NODE* left, NODE* right)
{
    NODE *a = (NODE*)malloc(sizeof(NODE));
    if (a==NULL) {
      perror("Cannot make node");
      exit(1);
    }
    a->type = t;
    a->left = left;
    a->right = right;

    printf("(%d)", counter);
    counter++;
    return a;
}

NODE* make_leaf(TOKEN* l)
{
    NODE *a = (NODE*)malloc(sizeof(NODE));
/*     printf("make_leaf: %p -> %p\n", l, a); */
    if (a==NULL) {
      perror("Cannot make leaf");
      exit(1);
    }
    a->type = LEAF;
    a->left = (NODE*)l;
    a->right = NULL;
    printf("(%d)", counter);
    counter++;
    return a;
}

int is_leaf(NODE *node)
{
    return node->type == LEAF;
}

/* Leaf type nodes store a token as their left child */
TOKEN *get_token(NODE *node)
{
    if (is_leaf(node)) {
        return (TOKEN *)node->left;
    }
    else {
        return NULL;
    }
}
