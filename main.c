#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "translate.h"

/* Translate source to three address code */
void translate_to_TAC()
{
    return;
}

/* Translate three address code to MIPS */
void translate_to_MIPS()
{
    return;
}

/* Read node, evaluate, continue */
void interpret_source()
{
    translate();
    return;

}

/* Interpret --C program */
int main ( int argc, char *argv[] )
{
    int c         = 0;
    int max_chars = 100; //for strncmp
    char *action  = "";

    // Determine translation requested
    while ((c = getopt(argc, argv, "a:")) != -1)
    {
        switch (c)
        {
            case 'a':
                action = optarg;
                printf("Action Selected: %s\n", action);
                break;
            default:
                printf("Invalid argument.\n");
                abort();
        }
    }

    /* Translate */
    if ( strncmp(action, "", max_chars) == 0 )
    {
        printf("No action selected\n");
        abort();
    }
    else if ( strncmp(action, "interpret", max_chars) == 0 )
    {
        interpret_source();
    }
    else if ( strncmp(action, "tac", max_chars) == 0 )
    {
        translate_to_TAC();
    }
    else if ( strncmp(action, "mips", max_chars) == 0 )
    {
        translate_to_MIPS(); // expects TAC input
    }
    else
    {
        printf("Action is invalid!\n");
        abort();
    }
    return 0;
}


