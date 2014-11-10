#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "param.h"

#include "../analysis/C.tab.h"
#include "../analysis/nodes.h"
#include "../analysis/token.h"
#include "../util.h"

extern TOKEN *void_token;

/*
 * Supported unary operators: -, return
 *
 */
STATE *evaluate_unary(NODE *operator, STATE *operand, FRAME *frame)
{
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case RETURN:
        printf("Processing return operator\n");

        if (operand->var_name)
        {
            // TODO infer return type from function sig
            ENV *var = lookup_var(operand->var_name, INT_TYPE, frame);
            return var->state;
        }
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

      case APPLY:
        // lookup_var(left_operand->var_name);
        // return call(fn, right_operand)
        break;

      default:
        printf("Unactionable binary operator!\n");
        return NULL;
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
        printf("Processing fn definition\n");
        // Left operand: function struct
        // Right operand: body of function
        // Returns: Nothing

        // Set body of function
        function *fn = left_operand->function;
        fn->body = operator->right;

        // Store function in frame
        init_var(fn->name, FN_TYPE, frame);
        assign_var(fn->name, FN_TYPE, left_operand, frame);
        return NULL;

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
        printf("Frame created for function \"%s\"\n", left_operand->var_name);

        PARAM *params = NULL;
        if (right_operand && right_operand->param)
        {
            params = right_operand->param;
        }

        // Create function struct
        FRAME *func_frame = new_frame(frame, params, NULL);

        STATE *fn_state = new_fn_state( new_function( 0,
                                           func_frame,
                                           NULL,
                                           left_operand->var_name ));

        // TODO remove if not necessary
        // if (str_eq(left_operand->var_name, "main"))
        // {
        //     if (frame != gbl_frame)
        //     {
        //         printf("Main found not in global frame, something's wrong!\n");
        //         printf("Exiting!\n");
        //         exit(1);
        //     }
        //     main = fn_state;
        // }
        // Function still has no retrun type or body (hence 0 and NULL)
        return fn_state;

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
            while (temp != NULL)
            {
                // FN_TYPE or INT_TYPE
                temp->type = left_operand->value;
                temp = temp->next;
            }
            return right_operand;
        }
        else
        {
            printf("Processing variable declaration..."); // Return nothing
            if (str_eq(left_operand->var_name, "int"))
            {
                printf("Of type int...");
                printf("Called \"%s\"\n", temp->name);

                while (temp)
                {
                    init_var(temp->name, INT_TYPE, frame);
                    temp = temp->next;
                }
            }
            else if (str_eq(left_operand->var_name, "function"))
            {
                printf("Of type function!\n");
                // init function variable - not yet supported
                return NULL;
            }
            else
            {
                // Multifunction initialisation, just return
                printf("Multiple function initialisation!\n");
                return NULL;
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
        // Assume only ints can be in assignments

        return new_param_state(new_param(left_operand->var_name, INT_TYPE));

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

      case ';':
        printf("Processing multiple statements!\n");
        return new_fn_body_state(operator);


      default:
        printf("Unknown binary operator!\n");
        return NULL;
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
        if (t->type == CONSTANT)
        {
            // Int state for both types and numbers
            //printf("Found leaf: %d\n", t->value);
            return new_int_state(t->value);
        }
        // TODO merge these final two conditions unless a reason not to emerges
        else if (t->type == STRING_LITERAL)
        {
            //printf("Found leaf: %s\n", t->lexeme);
            return new_var_name_state(t->lexeme);
        }
        else // variable?
        {
            //printf("Found leaf: %s\n", t->lexeme);
            return new_var_name_state(t->lexeme);
        }
    }

    /* Recursive case: Evaluate unary operator    */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node,
                               evaluate(node->left, node, frame, is_first_pass),
                               frame );
    }

    /* Recursive case:
     *
     * Evaluate binary operator if both children are leaves
     */
    else
    {
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
    return NULL;
}


