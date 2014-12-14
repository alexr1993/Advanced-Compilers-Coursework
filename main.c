#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "analysis/nodes.h"
#include "analysis/token.h"
#include "analysis/C.tab.h"
#include "analysis/environment.h"
#include "evaluation/evaluate.h"
#include "generation/tac.h"
#include "util.h"

extern int yydebug;
extern int yyparse (void);
extern NODE *ans;
extern FRAME *gbl_frame;
extern int V;
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
    yyparse();
    NODE *tree = ans;
    print_tree(tree);
    evaluate(tree, NULL, gbl_frame, TAC);
    return;
}

/* Translate three address code to MIPS */
void translate_to_MIPS() {
    return;
}

/* Basic evaluation of parse tree */
void interpret_source(void)
{
    yyparse();
    NODE *tree = ans;
    print_tree(tree);
/*
    printf("\n\nStarting first pass evaluation\n");
    printf("==============================\n\n");
    evaluate(tree, NULL, gbl_frame, FIRST_PASS); // Option 1

    printf("\n\nExecuting\n");
    printf("=========\n\n");

    STATE *return_status = call("main", gbl_frame, NULL)->state;
    printf("\n\n----------Interpretation Complete----------\n\n");
    printf("%d\n\n", return_status->value);
*/
}

/* Interpret --C program */
int main ( int argc, char *argv[] )
{
    int c         = 0;
    int len;
    char *action  = "";
    V = 1; // Verbose
    init_environment();
    init_token_stack();

    // Determine translation requested
    while ((c = getopt(argc, argv, "a:df:")) != -1)
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

    if (!yyin) yyin = fopen("tests/test_source/if.cmm", "r");

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

