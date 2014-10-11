#include <stdbool.h>
#include <stdio.h>
#include "analysis/nodes.h"
#include "analysis/token.h"

extern TOKEN *void_token;

bool is_self_evaluating (NODE *node)
{
    return true;
}

bool has_child (NODE *node)
{
    return false;
}

NODE *get_next_child(NODE *tree)
{
    // return the empty node
    NODE *empty_node = make_leaf(void_token);
    return empty_node;
}/* Handles the crunching of parse trees */

NODE *evaluate (NODE *parse_tree)
{
    while (has_child(parse_tree))
    {
        NODE *current = get_next_child(parse_tree);

        /* return if self evaluating node */
        if (is_self_evaluating(current))
        {
            return current;
        }
        /* otherwise recurse */
        else
        {
            evaluate(current);
        }
    }
}

