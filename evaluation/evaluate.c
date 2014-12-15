#include "evaluate.h"

#include "util.h"
#include "common/operations.h"

ENV *add(ENV *left_operand, ENV *right_operand, FRAME *frame);

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
        printf("First arg: name \"%s\", type \"%d\", value \"%d\"\n",
               right_operand->state->env->name, // FIXME this is nonsense
               right_operand->state->env->type,
               right_operand->state->env->state->value);
        return call(left_operand->state->var_name,
                    frame,
                    right_operand->state->env);

      case IF:
        // Only gets called to check condition before preceding
        // return the truthiness of the condition
        printf("Processing if statement\n");
        printf("boolean value: %d\n", left_operand->state->value);
        return left_operand;

      default:
        printf("Unactionable binary operator!\n");
        return NULL;
    }
}
/**
 * Parses operator nodes in the case of a branch
 *
 * Simply returns leaves
 *
 */
ENV *evaluate (NODE *node, NODE *parent, FRAME *frame, int eval_mode)
{
    /* Base case: HANDLE LEAVES
     *
     * return if self evaluating node, leaves *should* be
     */
    if (node->type == LEAF)
    {
        printf("Processing Leaf: ");
        // TODO make print_leaf less confusing and painful
        print_leaf(node->left, 0);

        TOKEN *t = (TOKEN *)node->left;
        if (t->type == CONSTANT)
        {
            // Apply must accept env as in other cases it may be a list
            if (parent->type == APPLY)
            {
                // No name until bound to function frame
                ENV *env = new_env("",
                                    INT_TYPE,
                                    new_int_state(t->value));

                return new_env("", INT_TYPE, new_env_state(env));
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

    /* Recursive case: Evaluate unary */
    else if (!node->right)
    {
        print_branch(node);
        // evaluate unary operator with operant node->left
        return evaluate_unary( node,
                               evaluate(node->left, node, frame, eval_mode),
                               frame,
                               eval_mode );
    }
    /* Recursive case: Evaluate binary */
    else
    {
        print_branch(node);
        ENV *left_operand  = evaluate(node->left, node, frame, eval_mode);
        ENV *right_operand = NULL;

        if (eval_mode == FIRST_PASS)
        {
            right_operand = evaluate(node->right, node, frame, eval_mode);
        }
        /* At runtime we don't always want to execute the children */
        else
        {
            switch (node->type)
            {
              case 'D':
                right_operand = NULL;
                break;

              case IF:
                if (left_operand->state->value != 0)
                {
                    right_operand
                        = evaluate(node->right, node, frame, eval_mode);
                }
              break;

              default:
                right_operand
                    = evaluate(node->right, node, frame, eval_mode);
                break;
            }
        }
        if (eval_mode == FIRST_PASS)
        {
            return first_pass_evaluate_binary( parent,
                                        node,
                                        left_operand,
                                        right_operand,
                                        frame          );
        }
        else
        {
            /* Handle return statement which can appear anywhere */
            if (left_operand && left_operand->is_return)
            {
                return left_operand;
            }
            if (right_operand && right_operand->is_return)
            {
                return right_operand;
            }
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
