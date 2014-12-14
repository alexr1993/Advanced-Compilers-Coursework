#include "function.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "evaluate.h"

function *new_function( int return_type,
                        FRAME *scope,
                        NODE *body,
                        char *name   )
{
    function *fn = malloc(sizeof(function));

    fn->return_type = return_type;
    fn->scope       = scope;
    fn->body        = body;
    fn->name        = name; // The original name

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
void register_function( int return_type,
                        FRAME *frame,
                        NODE *body,
                        char *name        )
{
    init_var(name, FN_TYPE, frame);
    STATE *state = new_fn_state(new_function(return_type,
                                             frame,
                                             body,
                                             name         ));
    assign_var(name, FN_TYPE, state, frame);
    return;
}

/*
 * Assigns params to given args list (for function call)
 *
 */
void *bind_args(function *fn, ENV *args)
{
    ENV *current_param = fn->scope->param;
    ENV *current_arg = args;

    while (current_param && current_arg)
    {
        printf("param type: %d, arg type: %d, arg value: %d\n",
                current_param->state->param->type,
                current_arg->type,
                current_arg->state->value                       );

        if (current_param->state->param->type != current_arg->type)
        {
            printf("Argument passing type error!\n");
            exit(1);
        }
        current_arg->name = current_param->state->param->name;

        current_param = current_param->next;
        current_arg   = current_arg->next;
    }
    if (current_param || current_arg)
    {
        perror("Arguments and Params lists do not match!\n");
        exit(1);
    }

    // All good, bind the args
    ENV *vars = fn->scope->variable;
    args->next = vars;
    fn->scope->variable = args;
    print_frame(fn->scope);
    return 0;
}

ENV *call(char *name, FRAME *frame, ENV *args)
{
    function *function = lookup_var(name, FN_TYPE, frame)->state->function;
    if (args)
    {
        bind_args(function, args);
    }
    return evaluate(function->body, NULL, function->scope, EVAL);
}

