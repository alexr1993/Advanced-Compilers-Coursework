#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

// State maps locations to values
STATE *state; // TODO remove?

void print_env(ENV *env)
{
    ENV *current = env;
    printf("VARIABLES CURRENTLY IN ENVIRONMENT\n");
    while (current)
    {
        char * type;
        if (current->type == INT_TYPE) type = "INT";
        else type = "FN";

        printf("Name: %s, Type: %s", current->name, type);

        if  (current->type == INT_TYPE)
        {
            printf(", Value: %d", current->state->value);
        }
        current = current->next;
        printf("\n");
    }
}

/*
 * Prints frame including all of it's children
 */
void print_frame(FRAME *frame)
{
    print_env(frame->variable);

    FRAME *child = frame->child;
    while (child)
    {
        print_frame(child);
        child = child->sibling;
    }
}

 /* Adds env mapping to the environment */
void _add_env(ENV *env, FRAME *frame)
{
    // Insert env at the front of the frames variable list
    ENV *frame_var = frame->variable;
    env->next = frame_var;
    frame->variable = env;
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

    ENV *frame_var = frame->variable;

    /* Find the env mapping with the given name */
    while (frame_var)
    {
        // Mapping found
        if (str_eq(name, frame_var->name))
        {
            return frame_var;
        }
        // Still looking
        frame_var = frame_var->next;
    }
    printf("Variable name lookup failed!\n");
    return NULL;
}

/*
 * Returns new env sruct
 *
 */
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

STATE *new_fn_state(function *function)
{
    STATE *state = malloc(sizeof(STATE));
    state->function = function;
    return state;
}

STATE *new_var_name_state(char *name)
{
    STATE *state = malloc(sizeof(STATE));
    int name_length = strlen(name);

    state->var_name = malloc(name_length * sizeof(char));
    strncpy(state->var_name, name, name_length);
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

/*
 * Create new frame and attack it correctly to its parent
 *
 */
FRAME *new_frame(FRAME *parent, PARAM *params, ENV *variables)
{
    FRAME *new_frame = malloc(sizeof(FRAME));
    new_frame->parent = parent;

    // Add frame as child of parent
    FRAME *child = parent->child;
    parent->child = new_frame;
    new_frame->sibling = child;

    new_frame->param = params;
    new_frame->variable = variables;
    return new_frame;
}

/* Init some globals, or whatever else */
void init_environment(void)
{
    gbl_frame = malloc(sizeof(FRAME));

    // Test code
    ENV *var1 = init_var("testvar", INT_TYPE, gbl_frame);

    STATE *state = new_int_state(42);
    assign_var("testvar", INT_TYPE, state, gbl_frame);
}

