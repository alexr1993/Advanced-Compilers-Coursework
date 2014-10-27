#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

// Environment maps names to locations
ENV *gbl_environment;

// State maps locations to values
STATE *state;

void print_env(ENV *current)
{
        char *type;
        if (current->type == INT_TYPE) type = "INT";
        else type = "FN";

        printf("Name: %s, Type: %s", current->name, type);

        if  (current->type == INT_TYPE)
        {
            printf(", Value: %d", current->state->value);
        }
        printf("\n");
}

 /* Adds env mapping to the environment */
ENV *_add_env(ENV *env, FRAME *frame)
{
    ENV *current = gbl_environment;

    /* Lazily initialise */
    if (!current)
    {
        // env is the first variable
        printf("Setting gbl_environment\n");
        gbl_environment = env;
        printf("gbl_environment->name: %s\n", gbl_environment->name);
    }
    else
    {
        // Add new env to the end
        while (current->next)
        {
            current = current->next;
        }
        current->next = env;
    }
    // Return whole gbl_environment
    return gbl_environment;
}

/*
 * Finds and returns the env mapping with the given name and type
 *
 * Checks given env first, then recursive looks at parent until var
 * is found
 *
 */
ENV *lookup_var(char *name, int type, FRAME *frame)
{
    printf("Name: %s\n", name);
    printf("Checking there is any env\n");
    // Check there are any variables
    if (!gbl_environment || !gbl_environment->name)
    {
        // Lookup variable in next stack frame
        return lookup_var(name, type, frame->next);
    }
    // FIXME reading gbl_environment->name causes a segfault
    ENV *current_env = gbl_environment;

    /* Find the env mapping with the given name */
    while (true)
    {
        //print_env(current_env);

        // Mapping found
        if (str_eq(name, current_env->name))
        {
            break;
        }
        // Lookup failed
        if (!current_env->next)
        {
            printf("Variable name lookup failed!\n");
            return NULL;
        }
        // Still looking
        else
        {
            current_env = current_env->next;
        }
    }
    return current_env;
}

ENV *new_env(char *name, int type, STATE *state)
{
    // Init env mapping
    ENV *new_env = malloc(sizeof(ENV));
    int name_length = strlen(name);

    new_env->name = malloc(name_length * sizeof(char));
    strncpy(new_env->name, name, name_length);
    printf("Name: %s, var->name: %s\n", name, new_env->name);
    new_env->type = type;
    new_env->state = state;

    return new_env;
}

STATE *new_int_state(int value)
{
    STATE *state = malloc(sizeof(STATE));
    state->value = value;
    return state;
}

STATE *new_fn_state(function *closure)
{
    STATE *state = malloc(sizeof(STATE));
    state->closure = closure;
    return state;
}

/* Attempts to store new variable, initialised to default values */
ENV *init_var(char *name, int type, FRAME *frame)
{
    // Check for collision
    if (lookup_var(name, type, frame))
    {
        printf("Attempted to initialise variable twice!\n");
        abort();
    }

    // Init union type (the state)
    STATE *new_state;

    if (type == INT_TYPE)
    {
        new_state = new_int_state(0);
    }
    else if (type == FN_TYPE)
    {
        new_state = new_fn_state( new_function(0, NULL, NULL, NULL) );
    }

    // Init env mapping
    ENV *new_var = new_env(name, type, new_state);
    _add_env(new_var, frame);

    return new_var;
}

/* Sets the state of the variable */
ENV *assign_var(char *name, int type, STATE* value, FRAME *frame)
{
    ENV *var = lookup_var(name, type, frame);

    if (var && var->type == type)
    {
        var->state = value;
    }
    else
    {
        printf("Error: Assigning to uninitialised variable, check type!");
        abort();
    }
    return var;
}

/* Init some globals, or whatever else */
void init_environment(FRAME *frame)
{
    ENV *var1 = init_var("testvar", INT_TYPE, frame);

    STATE *state = new_int_state(42);
    assign_var("testvar", INT_TYPE, state, frame);
}

void print_environment(void)
{
    ENV *current = gbl_environment;
    printf("VARIABLES CURRENTLY IN ENVIRONMENT\n");
    while (current)
    {
        print_env(current);
        current = current->next;
    }
}