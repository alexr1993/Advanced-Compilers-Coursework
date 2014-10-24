#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../analysis/C.tab.h"
#include "../analysis/nodes.h"
#include "../analysis/token.h"
#include "environment.h"
#include "../util.h"

extern TOKEN *void_token;

/*
 * Supported unary operators: -, return
 *
 */
STATE *evaluate_unary(NODE *operator, STATE *operand)
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

STATE *evaluate_binary(NODE *operator, STATE *left_operand, STATE *right_operand)
{
    TOKEN *left_token;
    TOKEN *right_token;
    /* Leaf data is stored in the left child of the leaf node */
    if ( left_operand && left_operand->left
         && right_operand && right_operand->left)
    {
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
        return new_int_state(right_operand ? right_operand : NULL);


      case 'd':
        printf("Processing function signature\n");
        // Currently does nothing as we do not type check
        return new_fn_state(operator);

      case 'F':
        printf("Processing function return type\n");
        // Return null values to represent the return type
        if (left_token->type == INT)
        {
            return new_int_state(0);
        }
        else
        {
            return new_fn_state(NULL);
        }

      case '+':
        printf("Processing add\n");
        printf("Output: %d\n", left_token->value + right_token->value);
        return new_int_state(left_token->value + right_token->value);

      case '~':
        // TODO make STATE the return type of everything, or this section
        // won't really work for closures
        print_environment();
        if (left_token->type == INT)
        {
            printf("Processing int initialisation\n");
            // TODO get name and value from evaluated = node
            //envstore_int(name, value);
            // If there is no initialisation, then the two children will be
            // the type, and then the function
            init_var(right_token->lexeme, INT_TYPE);
            if (right_token->value != 0)
            {
                STATE *var_state = new_int_state(right_token->value);

                assign_var(right_token->lexeme,
                           INT_TYPE,
                           var_state);

            }
            return;
        }
        else
        {
            init_function(ret_type,  name,    args,
                                   lex_scope, right_operand);

            printf("Processing fn initialisation\n");
        }

     case '=':
        /* Should return the lvalue, only for ints */
        printf("Processing assignment\n");

        /* lvalue type should match rvalue type, this can be confirmed using
           a var lookup, or from a parent initialisation operator */
        printf("Lexeme: %s, Value: %d\n", left_token->lexeme, right_token->value);

        STATE *int_state = new_int_state(right_token->value);
        assign_var(left_token->lexeme,
                   INT_TYPE,
                   int_state);

        return int_state;

      case ';':
        printf("Processing multiple statements\n");
        return right_operand;

      default:
        printf("Unknown binary operator!\n");
        abort();
    }
}

/**
 * Parses operator nodes in the case of a branch
 *
 * Simply returns leaves
 *
 */
STATE *evaluate (NODE *node)
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
        // TODO Initialisation must be done before it's leaves
        // which may involve assignment
        if (node->type == '~');
        print_branch(node);
        STATE *left_operand  = evaluate(node->left);
        STATE *right_operand = evaluate(node->right);
        //print_tree(right_operand);

        return evaluate_binary( node,
                                left_operand,
                                right_operand );
    }
}

