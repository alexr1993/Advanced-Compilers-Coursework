#include "environment.h"
#include "../util.h"

#define MAX_ENTRIES 100
// Environment maps names to locations
INT_ENV_MAPPING *[MAX_ENTRIES] INT_BINDINGS;
FN_ENV_MAPPING  *[MAX_ENTRIES] FN_BINDINGS;

// State maps locations to values
INT_STATE_MAPPING *[MAX_ENTRIES] INT_LOCATIONS;
FN_STATE_MAPPING *[MAX_ENTRIES] FN_LOCATIONS;

void init_environment(void)
{
    // Set everything to 0 (probs unecessary)
}

int get_lowest_free_index(mapping)
{
    for (int i=0; i<MAX_ENTRIES; ++i)
    {
        if (!mapping[i])
        {
            printf("Lowest free index in mapping: %d\n", i);
            return i;
        }
    }
    return -1;
}

envlookup_int(char *name)
{
    int location;

    /* Lookup binding location */
    for (int i=0; i<MAX_ENTRIES; ++i)
    {
        if (str_eq(name, INT_BINDINGS[i]->name))
        {
            location = INT_BINDINGS[i]->location;
        }
    }

    int value;

    /* Lookup value at location */
    for (int i=0; i<MAX_ENTRIES; ++i)
    {
        if (location == INT_LOCATIONS[i]->location)
        {
            value = INT_LOCATIONS[i]->value;
        }
    }
    return value;
}

void envstore_int(char *name, int value)
{
    int bind_index = get_lowest_free_index(INT_BINDINGS);
    INT_BINDINGS[bind_index] = { .name = name, .location = location };

    int loc_index = get_lowest_free_index(INT_LOCATIONS);
    INT_LOCATIONS[loc_index] = { .location = location, .value = value };
}
