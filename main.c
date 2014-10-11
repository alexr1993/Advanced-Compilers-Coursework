#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "analysis/nodes.h"
#include "translate.h"

extern int yydebug;
extern NODE *yyparse (void);
extern NODE *ans;
extern void init_symbtable (void);

/* Read input from user to evaluate */
char *prompt (void)
{
    // TODO implement as input mechanism which will accept single lines
    // or multi line constructs, e.g. while, function declarations
    return "";
}

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

/* Basic evaluation of parse tree */
void interpret_source(char *input)
{
    if (input != "")
    {
        yyparse();
        NODE *tree = ans;
        print_tree(tree);
        evaluate(tree);
    }
    // start interactive session
    while (false) // TODO change to true when implemented
    {
        char *command = prompt(); // accept input one expression at a time
        evaluate(command);
    }
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
        printf("%s\n", optarg);
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
        char *interpret_input = "";
        interpret_source(interpret_input);
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

