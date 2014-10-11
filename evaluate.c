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

NODE *evaluate_node (NODE *node)
{
        /* return if self evaluating node, leaves *should* be */
        if (node->type == LEAF)
        {
            return node;
        }
        /* otherwise recurse */
        else
        {
            evaluate_tree(node);
        }
}

/* Handles the crunching of parse trees */
NODE *evaluate_tree (NODE *parse_tree)
{
    /* recursively turn branches into leaves */
    if (parse_tree->type != LEAF)
    {
        NODE *left  = parse_tree->left;
        NODE *right = parse_tree->right;
        evaluate_node(left);
        evaluate_node(right);
    }
    /* tree is just a leaf, so evaluate */
    else
    {
        evaluate_node(parse_tree);
    }
}

