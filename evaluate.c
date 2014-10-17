#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

/*
 * Supported unary operators: -, return
 *
 */
NODE *evaluate_unary(NODE *operator, NODE *operand)
{
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case RETURN:
        printf("Processing return operator\n");
        return operand;
      /* The only unary operator that makes it through the parses is RETURN
       * This seems to be a bug with the prewritten code (in C.y)
       */
      //case '-':
      //  printf("Processing unary minus\n");
      //  TOKEN *t = new_token(CONSTANT); // new token for result
      //  TOKEN *left_token = (TOKEN *) operand->left;

      //  t->value = 0 - left_token->value;
      //  return make_leaf(t);
      default:
        printf("Unknown unary operator\n");
        return;
    }
}

NODE *evaluate_binary(NODE *operator, NODE *left_operand, NODE *right_operand)
{
    TOKEN *left_token;
    TOKEN *right_token;
    /* Leaf data is stored in the left child of the leaf node */
    if (left_operand->left && right_operand->left)
    { // FIXME this fails because no node is returned from the child operation
        left_token  = (TOKEN *) left_operand->left;
        right_token = (TOKEN *) right_operand->left;
    }

    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case 'D':
        printf("Processing function definition\n");
        // Currently return without checking signature
        // Program is assumed to be correct
        return right_operand;
      case 'd':
        printf("Processing function signature\n");
        // Currently does nothing as we do not type check
        return operator;
      case 'F':
        printf("Processing function return type\n");
        return operator;
      case '+':
        printf("Processing add\n");
        printf("Output: %d\n", left_token->value + right_token->value);
        TOKEN *t = new_token(CONSTANT);
        t->value = left_token->value + right_token->value;
        return make_leaf(t);
      default:
        printf("Unknown binary operator\n");
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
    /* Base case:
     *
     * return if self evaluating node, leaves *should* be
     */
    if (node->type == LEAF)
    {
        printf("Evaluate output: ");

        // TODO make print_leaf less confusing and painful
        print_leaf(node->left, 0);
        return node;
    }
    /* Recursive case: Evaluate unary operator    */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node, evaluate(node->left) );
    }

    /* Recursive case:
     *
     * Evaluate binary operator if both children are leaves
     */
    else
    {
        print_branch(node);
        NODE *left_operand  = evaluate(node->left);
        NODE *right_operand = evaluate(node->right);
        print_tree(right_operand);

        return evaluate_binary( node,
                                left_operand,
                                right_operand );
    }
}

