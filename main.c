#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "analysis/nodes.h"
#include "analysis/token.h"
#include "evaluation/environment.h"
#include "evaluation/evaluate.h"
#include "util.h"

extern int yydebug;
extern NODE *yyparse (void);
extern NODE *ans;
extern void init_symbtable (void);
extern FRAME *gbl_frame;

FILE *yyin;

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
    init_symbtable();
    yyparse();
    NODE *tree = ans;
    print_tree(tree);
    //evaluate(tree);
    return;
}

/* Translate three address code to MIPS */
void translate_to_MIPS()
{
    return;
}

/* Basic evaluation of parse tree */
void interpret_source(void)
{
    if (yyin)
    {
        init_symbtable();
        yyparse();
        NODE *tree = ans;
        print_tree(tree);

        // TODO process contents of tree to populate global env
        //
        // This can be done by:
        //  1. Backpatching (but no idea how to implement)
        //
        //  2. First pass to get vars
        //      Issue is if you init env here then there can be weird
        //      situations where
        //      variables can be used before being initialised as initialisations are
        //      done on the first pass
        //
        //  3. Don't do it
        //      You must use forward declarations, and can't init at the same time
        //      as assign.
        //      Also not sure if this will work for functions
        //
        printf("\n\nStarting first pass evaluation\n");
        printf("==============================\n\n");
        evaluate(tree, NULL, gbl_frame, true); // Option 1

        printf("\n\nExecuting\n");
        printf("=========\n\n");

        STATE *return_status = call("main", gbl_frame, NULL)->state;
        printf("\n\n----------Interpretation Complete----------\n\n");
        printf("%d\n\n", return_status->value);
    }
    // start interactive session
    while (false) // TODO change to true when implemented
    {
        //char *command = prompt(); // accept input one expression at a time
        //evaluate(command);
    }
    return;
}

/* Interpret --C program */
int main ( int argc, char *argv[] )
{
    int c         = 0;
    int len;
    char *action  = "";

    init_environment();

    // Determine translation requested
    while ((c = getopt(argc, argv, "a:df")) != -1)
    {
        switch (c)
        {
            case 'a':
                // copy action type to var
                len = strlen(optarg);
                action = (char *) malloc(len * sizeof(char));
                strncpy(action, optarg, len);

                printf("Action Selected: %s\n", action);
                break;

            case 'd':
                yydebug = 1;
                break;

            case 'f':
                yyin = fopen(optarg, "r");
                if (!yyin)
                {
                    printf("Invalid input file path.\n");
                    abort();
                }
                printf("Input source file: %s\n", optarg);
                break;

            default:
                printf("Invalid argument.\n");
                abort();
        }
    }

    yyin = fopen("tests/test_source/if.cmm", "r");

    /* Translate */
    if ( str_eq(action, "") )
    {
        printf("No action selected\n");
        abort();
    }
    else if ( str_eq(action, "interpret") )
    {
        interpret_source();
    }
    else if ( str_eq(action, "tac") )
    {
        translate_to_TAC();
    }
    else if ( str_eq(action, "mips") )
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

