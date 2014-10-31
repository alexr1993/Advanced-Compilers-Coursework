#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "environment.h"

#include "../analysis/C.tab.h"
#include "../analysis/nodes.h"
#include "../analysis/token.h"
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
        return operand;
    }
}

STATE *evaluate_binary( NODE *operator,        STATE *left_operand,
                        STATE *right_operand,  FRAME *frame         )
{
    // TODO add type checking
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case '+':
        printf("Processing add\n");
        printf("Output: %d\n", left_operand->value + right_operand->value);
        return new_int_state(left_operand->value + right_operand->value);

      case '=':
        /* Should return the lvalue, only for ints */
        printf("Processing assignment\n");

        /* lvalue type should match rvalue type, this can be confirmed using
           a var lookup, or from a parent initialisation operator */
        printf( "Lexeme: %s, Value: %d\n",
                left_operand->var_name,
                right_operand->value       );

        STATE *int_state = new_int_state(right_operand->value);
        assign_var(left_operand->var_name,
                   INT_TYPE,
                   int_state,
                   frame );

        return int_state;

      case ';':
        printf("Processing multiple statements\n");
        return right_operand;

      default:
        printf("Unknown binary operator!\n");
        abort();
    }
}

/* Handle anything related to initialisation and variable names */
STATE *first_pass_evaluate_binary( NODE *parent,
                                   NODE *operator,
                                   STATE *left_operand,
                                   STATE *right_operand,
                                   FRAME *frame          )
{
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case 'D':
        printf("Processing fn definition");
        // Left operand: function struct
        // Right operand: body of function
        // Returns: Nothing

        // Set body of function
        function *fn = left_operand->function;
        fn->body = right_operand->fn_body;

        // Store function in frame
        init_var(fn->name, FN_TYPE, frame);
        assign_var(fn->name, FN_TYPE, left_operand, frame);
        return;

      case 'd':
        printf("Processing function signature\n");
        // Left operand: Return type
        // Right operand: Function state
        //
        // Returns: Function state

        right_operand->function->return_type = left_operand->value;
        return right_operand;


      case 'F':
        printf("Processing function name and params\n");
        // Left operand: function name
        // Right operand: Param
        // Returns: Function state

        // Create new frame for function and populate it with params
        FRAME *func_frame = new_frame(frame, right_operand->param, NULL);

        // Function still has no retrun type or body (hence 0 and NULL)
        return new_fn_state( new_function( 0,
                                           func_frame,
                                           NULL,
                                           left_operand->var_name ));

      case '~':
        printf("Processing initialisation\n");
        // This is either used for declaring variables, paramaters or
        // *multiple* functions
        //
        // It's use in registering function can be ignored, although it can be
        // used for declaring function type params
        //
        // When used for declaring vars/params, the left operand will always be
        // a type
        // The right operand will either be a var_name, a comma, (multiple
        // assignments), or an equals sign (assignment)
        //
        // Left operand: type
        // Right operand: param (with no type)
        // Returns: Param (return type never used in var inits)
        PARAM *param = right_operand->param;
        PARAM *temp  = param;

        if (parent->type == 'F')
        {
            printf("Processing param declaration\n"); // Return param

            // Set the type of the param(s) declared
            while (temp)
            {
                // FN_TYPE or INT_TYPE
                temp->type = left_operand->value;
                temp = temp->next;
            }
            return right_operand;
        }
        else
        {
            printf("Processing variable declaration\n"); // Return nothing
            if (left_operand->value == INT_TYPE)
            {
                while (temp)
                {
                    init_var(temp->name, left_operand->value, frame);
                    temp = temp->next;
                }
            }
            else if (left_operand->value == FN_TYPE)
            {
                // init function variable - not yet supported
                return;
            }
            else
            {
                // Multifunction initialisation, just return
                return;
            }
            return NULL;
        }

      case '=':
        // Left operand: var_name
        // Right operand: value
        // Return: var_name
        /* Should return the lvalue, only for ints */
        printf("Processing assignment\n");

        // This is the fast pass - so don't assign, just return the name in
        // case there is an initialisation which needs it
        return left_operand;

      case ',':
        printf("Processing multiple initialisations\n");
        // Can either be used to declare multiple variables, or multiple
        // parameters.
        //
        // Variable Usage:
        // Left operand is always an assignment, right operand is either an
        // assignment or another comma
        //
        // Parameter Usage:
        // Left operand is always an initialisation, right operand is either an
        // assignment or another initialisation
        //
        // Left operand: param
        // right operand: param
        left_operand->param->next = right_operand->param;
        return left_operand;

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
STATE *evaluate (NODE *node, NODE *parent, FRAME *frame, bool is_first_pass)
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
        TOKEN *t = (TOKEN *)node->left;
        if (node->type == CONSTANT)
        {
            // Int state for both types and numbers
            return new_int_state(t->value);
        }
        else if (node->type == STRING_LITERAL)
        {
            return new_var_name_state(t->lexeme);
        }
        else
        {
            return;
        }
    }

    /* Recursive case: Evaluate unary operator    */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node,
                               evaluate(node->left, node, frame, is_first_pass) );
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
        STATE *left_operand  = evaluate(node->left, node, frame, is_first_pass);
        STATE *right_operand = evaluate(node->right, node, frame, is_first_pass);
        //print_tree(right_operand);

        if (is_first_pass)
        {
            return first_pass_evaluate_binary( parent,
                                               node,
                                               left_operand,
                                               right_operand,
                                               frame         );
        }
        if (!is_first_pass)
        {
            return evaluate_binary( node,
                                    left_operand,
                                    right_operand,
                                    frame          );
        }
    }
}


