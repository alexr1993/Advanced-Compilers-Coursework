#include "function.h"
#include <stdlib.h>
#include <stdio.h>

function *new_function( int return_type, ENV *params,
                        FRAME *scope,      NODE *body   )
{
    function *fn = malloc(sizeof(function));

    fn->return_type = return_type;
    fn->params      = params;
    fn->scope       = scope;
    fn->body        = body;

    return fn;
}

/*
 * Adds function to environment
 *
 * Functions are immutable, and are declared in a scope, not assigned
 * (for now anyway)
 *
 * This is why there is no assign operation
 */
void register_function( int return_type, char *name, ENV *params,
                        FRAME *frame,      NODE *body               )
{
    ENV *env = init_var(name, FN_TYPE, frame);
    STATE *state = new_fn_state(new_function(return_type,
                                             params,
                                             frame,
                                             body        ));
    assign_var(name, FN_TYPE, state, frame);
    return;
}

/*
 * Assigns params to given args list (for function call)
 *
 */
int *bind_args(function *fn, ENV *args)
{
    ENV *current_param = fn->params;
    ENV *current_arg   = args;

    while (current_param && current_arg)
    {
        if (current_param->type == current_arg->type)
        {
            // TODO check this is mem safe
            current_param->state = current_arg->state;
        }
        else
        {
            perror("Argument passing error!\n");
            abort();
        }
        current_param = current_param->next;
        current_arg   = current_arg->next;
    }
    if (current_param || current_arg)
    {
        perror("Arguments and Params lists do not match!\n");
        abort();
    }
    return 0;
}