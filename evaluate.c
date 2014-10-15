#include <stdbool.h>
#include <stdio.h>
#include "analysis/C.tab.h"
#include "analysis/nodes.h"
#include "analysis/token.h"
#include "util.h"

extern TOKEN *void_token;

/*
 * maybe use this as an error checker, to catch when leaves cannot
 * evaluate
 */
bool is_self_evaluating (NODE *node)
{
    return true;
}

/**
 * Supported unary operators: -, return
 *
 */
NODE *evaluate_unary(NODE *operator, NODE *operand)
{
    switch (operator->type)
    {
      case RETURN:
        printf("Processing return operator\n");
        return; // operand->left;
      default:
        return;
    }
}

NODE *evaluate_binary(NODE *operator, NODE *left_operand, NODE *right_operand)
{
    switch (operator->type)
    {
      case 'F':
        printf("Processing function definition\n");
        return;
      case 'f':
        printf("Processing function signature\n");
        return;
      case '+':
        printf("Processing add\n");
        return;
    }
}

/**
 * Parses operator nodes in the case of a branch
 *
 * Simply returns leaves
 *
 */
NODE *evaluate (NODE *node)
{
    /* return if self evaluating node, leaves *should* be */
    if (node->type == LEAF)
    {
        print_leaf(node->left, 0);
        return node;
    }
    /* TODO else if node has 1 child execute unary operator
    */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node, evaluate(node->left) );
    }

    /* Evaluate binary operator if both children are leaves */
    else
    {
        print_branch(node);
        NODE *left_operand  = evaluate(node->left);
        NODE *right_operand = evaluate(node->right);

        // operate(evaluate(node->left), evaluate(node->right))
        return evaluate_binary( node,
                                left_operand,
                                right_operand );
    }
}

