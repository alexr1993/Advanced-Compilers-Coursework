#include <stdio.h>

#include "environment.h"
#include "../util.h"

// Environment maps names to locations
ENV_MAPPING *environment;

// State maps locations to values
STATE_MAPPING *state;

int STACK_POINTER = 0;

void init_environment(void)
{
    environment = new_env_mapping("testvar", 1);
    state = new_state_mapping(1, 42); // map testvar to = 42
}

STATE_MAPPING *envlookup(char *name, int type)
{
    ENV_MAPPING current_env = environment;
    STATE_MAPPING current_state = state;

    /* Find the env mapping with the given name */
    while (true)
    {
        if (str_eq(name, current_env->name))
        {
            break;
        }
        // Name has no location associated
        if (!current_env->next)
        {
            printf("Variable name lookup failed!\n";
            abort();
        }
        else
        {
            current_env = current_env->next;
        }
    }
    /* Find the state mapping with the location */
    while (true)
    {
        if (current_env->location == current_state->location)
        {
            break;
        }
        // Location has no state
        if (!current_state->next)
        {
            printf("Location of variable has no value!\n");
            abort();
        }
        else
        {
            current_state = current_state->next;
        }
    }
    return current_state;
}

void envstore_int(char *name, int value)
{
    ENV_MAPPING current_env = environment;
    STATE_MAPPING current_state = state;

    // Get last env mapping
    while (current_env->next)
    {
        current_env = current_env->next;
    }
    ENV_MAPPING *new_em = new_env_mapping(name, STACK_POINTER);

    // Store new env mapping
    current_env->next = new_em;
}

void statestore_int(int location, int value)
{

};

void envstore(char *name, int value)
{
    ENV_MAPPING current_env = environment;
    STATE_MAPPING current_state = int_state;

    while (true)
    {
        if (
}
