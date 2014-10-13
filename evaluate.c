#include <stdbool.h>
#include <stdio.h>
#include "analysis/C.tab.h"
#include "analysis/nodes.h"
#include "analysis/token.h"

extern TOKEN *void_token;

/*
 * maybe use this as an error checker, to catch when leaves cannot
 * evaluate
 */
bool is_self_evaluating (NODE *node)
{
    return true;
}

NODE *evaluate_tree (NODE *parse_tree);

/**
 * Parses operator nodes in the case of a branch
 *
 * Simply returns leaves
 *
 */
NODE *evaluate_node (NODE *node)
{
    TOKEN *t = (TOKEN *)node;
    printf("Lexeme: %s\n", t->lexeme);
    /* return if self evaluating node, leaves *should* be */
    if (node->type == LEAF)
    {
        return node;
    }

    /* TODO else if node has 1 child execute unary operator
    */

    /* Evaluate binary operator if both children are leaves */
    else if (    node->left  && node->left->type  == LEAF
              && node->right && node->right->type == LEAF )
    {
    }

    /* TODO else if node has 2 children, one being a leaf
    */

    /* otherwise recurse */
    else
    {
        evaluate_tree(node);
    }
}

/**
 * Handles the crunching of parse trees
 *
 * Invokes evaluate_node on children
 */
NODE *evaluate_tree (NODE *parse_tree)
{
    /* recursively turn branches into leaves */
    if (parse_tree->type != LEAF)
    {
        NODE *left  = parse_tree->left;
        NODE *right = parse_tree->right;
        if (left)  evaluate_node(left);
        if (right) evaluate_node(right);
    }
    /* tree is just a leaf, so evaluate */
    else
    {
        evaluate_node(parse_tree);
    }
}
