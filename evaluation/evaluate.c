#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "function.h"
#include "param.h"

#include "../analysis/C.tab.h"
#include "../analysis/nodes.h"
#include "../analysis/token.h"
#include "../util.h"

extern TOKEN *void_token;
ENV *add(ENV *left_operand, ENV *right_operand, FRAME *frame);

STATE *processFnParamsandName(
    STATE *left_operand, STATE *right_operand, FRAME *frame );

ENV *declareVariables( NODE *parent,       ENV *left_operand,
                       ENV *right_operand, FRAME *frame       );

/*
 * Supported unary operators: -, return
 *
 */
ENV *evaluate_unary(NODE *operator, ENV *operand, FRAME *frame,
                      bool is_first_pass                           )
{
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case RETURN:
        printf("Processing return operator\n");
        if (is_first_pass)
        {
            return NULL;
        }
        if (operand->state->var_name)
        {
            // TODO infer return type from function sig
            ENV *var = lookup_var(operand->state->var_name, INT_TYPE, frame);
            return var;
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

ENV *evaluate_binary( NODE *operator,      ENV *left_operand,
                      ENV *right_operand,  FRAME *frame       )
{

    // TODO add type checking
    printf("operator: %s\n", named(operator->type));
    switch (operator->type)
    {
      case '+':
        printf("Processing add\n");
        return add(left_operand, right_operand, frame);

      case '=':
        /* Should return the lvalue, only for ints */
        printf("Processing assignment\n");

        printf( "Lexeme: %s, Value: %d\n",
                left_operand->state->var_name,
                right_operand->state->value       );

        // Assign int to variable - function assignment not supported
        STATE *int_state = new_int_state(right_operand->state->value);
        assign_var(left_operand->state->var_name,
                   INT_TYPE,
                   int_state,
                   frame );

        return new_env("", INT_TYPE, int_state);

      case ';':
        printf("Processing multiple statements\n");
        return right_operand;

      case APPLY:
        printf("Processing function application\n");
        printf("Calling function \"%s\"\n", left_operand->state->var_name);
        return call(left_operand->state->var_name, frame, right_operand);

      default:
        printf("Unactionable binary operator!\n");
        return NULL;
    }
}

/* Handle anything related to initialisation and variable names */
ENV *first_pass_evaluate_binary( NODE *parent,
                                 NODE *operator,
                                 ENV *left_operand,
                                 ENV *right_operand,
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
        function *fn = left_operand->state->function;
        fn->body = operator->right;

        // Store function in frame
        init_var(fn->name, FN_TYPE, frame);
        assign_var(fn->name, FN_TYPE, left_operand->state, frame);
        return NULL;

      case 'd':
        printf("Processing function signature\n");
        // Left operand: Return type
        // Right operand: Function state
        //
        // Returns: Function state

        right_operand->state->function->return_type = left_operand->state->value;
        return right_operand;


      case 'F':
        printf("Processing function name and params\n");
        return new_env("", FN_TYPE, processFnParamsandName(
                                          left_operand->state,
                                          right_operand->state,
                                          frame)
               );

      case '~':
        printf("Processing initialisation\n");
        return declareVariables( parent,
                                 left_operand,
                                 right_operand,
                                 frame          );

      case '=':
        // Left operand: var_name
        // Right operand: value
        // Return: var_name
        /* Should return the lvalue, only for ints */
        printf("Processing assignment\n");

        // This is the fast pass - so don't assign, just return the name in
        // case there is an initialisation which needs it
        // Assume only ints can be in assignments

        return new_env("",
                       PARAM_TYPE,
                       new_param_state(
                          new_param(left_operand->state->var_name, INT_TYPE)));

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
        left_operand->state->param->next = right_operand->state->param;
        return left_operand;

      case ';':
        printf("Processing multiple statements!\n");
        return new_env("", FN_TYPE, new_fn_body_state(operator));


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
ENV *evaluate (NODE *node, NODE *parent, FRAME *frame, bool is_first_pass)
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
            // Apply must accept env as in other cases it may be a list
            if (parent->type == APPLY)
            {
                // No name until bound to function frame
                ENV *env = new_env("temp",
                                    INT_TYPE,
                                    new_int_state(t->value));

                return new_env("", ENV_TYPE, new_env_state(env));
            }
            // Int state for both types and numbers
            //printf("Found leaf: %d\n", t->value);
            return new_env("", INT_TYPE, new_int_state(t->value));
        }
        // TODO merge these final two conditions unless a reason not to emerges
        else if (t->type == STRING_LITERAL)
        {
            //printf("Found leaf: %s\n", t->lexeme);
            return new_env("", STR_TYPE, new_var_name_state(t->lexeme));
        }
        else // variable?
        {
            //printf("Found leaf: %s\n", t->lexeme);
            return new_env("", STR_TYPE, new_var_name_state(t->lexeme));
        }
    }

    /* Recursive case: Evaluate unary operator    */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node,
                               evaluate(node->left, node, frame, is_first_pass),
                               frame,
                               is_first_pass );
    }
    /* Recursive case:
     *
     * Evaluate binary operator if both children are leaves
     */
    else
    {
        print_branch(node);
        ENV *left_operand  = evaluate(node->left, node, frame, is_first_pass);
        ENV *right_operand = evaluate(node->right, node, frame, is_first_pass);
        //print_tree(right_operand);

        if (is_first_pass)
        {
            first_pass_evaluate_binary( parent,
                                        node,
                                        left_operand,
                                        right_operand,
                                        frame          );
            return NULL;
        }
        else
        {
            return evaluate_binary( node,
                                    left_operand,
                                    right_operand,
                                    frame          );
        }
    }
    printf("Something's not been caught by an eval branch, aborting!\n");
    exit(1);
}

/****************************************************************************
 *                                                                          *
 *                      BINARY OPERATION IMPLEMENTATIONS                    *
 *                                                                          *
 ****************************************************************************/

ENV *add(ENV *left_operand, ENV *right_operand, FRAME *frame)
{
    int a, b;

    if (left_operand->type == INT_TYPE)
    {
        printf("LEFT OPERAND: %d\n", left_operand->state->value);
        a = left_operand->state->value;
    }
    else
    {
        a = lookup_var(left_operand->state->var_name, INT_TYPE, frame)
                ->state->value;
    }
    if (right_operand->type == INT_TYPE)
    {
        b = right_operand->state->value;
    }
    else
    {
        b = lookup_var(right_operand->state->var_name, INT_TYPE, frame)
                ->state->value;
    }
    printf("Output: %d\n", a + b);
    return new_env("", INT_TYPE, new_int_state(a + b));

}

STATE *processFnParamsandName( STATE *left_operand,
                               STATE *right_operand,
                               FRAME *frame          )
{
    // Left operand: function name
    // Right operand: Param
    // Returns: Function state

    // Create new frame for function and populate it with params
    printf("Frame created for function \"%s\"\n", left_operand->var_name);

    PARAM *params = NULL;
    if (right_operand && str_eq(right_operand->var_name, "void"))
    {
    }
    else if (right_operand && right_operand->param)
    {
        printf("Param(s) detected for function \"%s\"\n",
               left_operand->var_name                     );

        params = right_operand->param;
    }

    // Create function struct
    FRAME *func_frame = new_frame(frame, params, NULL);

    STATE *fn_state = new_fn_state( new_function(
                                       0,
                                       func_frame,
                                       NULL,
                                       left_operand->var_name ));
    return fn_state;
}

ENV *declareVariables( NODE *parent,
                         ENV *left_operand,
                         ENV *right_operand,
                         FRAME *frame          )
{
    // No operands mean ~ is holding two fn definitions together
    if (!left_operand && !right_operand)
    {
        printf("Declare Vars has been called with no operands");
        return NULL;
    }
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

    // Single param declarations will have a var name passed in
    if (right_operand->type == STR_TYPE)
    {
        char *name = right_operand->state->var_name;
        // Potentially bad mem management, create a new var instead?
        right_operand->state->param = new_param(name, 0);
    }

    PARAM *temp = right_operand->state->param;

    // Expect a chain of params with no type
    if (parent->type == 'F')
    {
        printf("Processing param declaration\n"); // Return param

        // Set the type of the param(s) declared
        while (temp)
        {
            // Use left op to get the type
            if (str_eq(left_operand->state->var_name, "int"))
            {
                printf("Of type int...");
                temp->type = INT_TYPE;
            }
            else
            {
                printf("Of type fn...");
                temp->type = FN_TYPE;
            }

            printf("Called \"%s\"\n", temp->name);
            temp = temp->next;
        }
        // Params have now had their type set
        return right_operand;
    }
    // Expect chain of params to be used for initing vars
    // This section does not need to return anything
    else
    {
        printf("Processing variable declaration..."); // Return nothing
        if (str_eq(left_operand->state->var_name, "int"))
        {
            printf("Of type int...");
            printf("Called \"%s\"\n", temp->name);

            while (temp)
            {
                init_var(temp->name, INT_TYPE, frame);
                temp = temp->next;
            }
        }
        else
        {
            printf("Of type function!\n");
            // init function variable - not yet supported
        }
        return NULL;
    }
}
