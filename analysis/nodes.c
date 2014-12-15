#include <stdio.h>
#include <stdlib.h>
#include "C.tab.h"
#include "token.h"

#include "nodes.h"
extern int counter;
void append_next_D(NODE *n, NODE *next_D);

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

    /* Keep a track of D nodes in the subtree (marking start of frames) */

    // If either left or right are 'D's put them first
    if (left != NULL) {
        if ((char)left->type == 'D') {
            append_next_D(a, left);
        }
        append_next_D(a, left->next_D);
    }
    if (right != NULL) {
        if ((char)right->type == 'D') {
            append_next_D(a, right);
        }
        append_next_D(a, right->next_D);
    }

    //printf("(%d)", counter);
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
    a->next_D = NULL;
    //printf("(%d)", counter);
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

void append_next_D(NODE *n, NODE *next_D)
{
    NODE *end = n;
    while (end->next_D != NULL) end = end->next_D;
    end->next_D = next_D;
}

void print_node(NODE *node)
{
    printf("NODE %c Left: %p, Right: %p, next_D: %p\n",
           (char)node->type,
           node->left,
           node->right,
           node->next_D);
}
