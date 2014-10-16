#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "analysis/nodes.h"
#include "analysis/token.h"
#include "evaluate.h"
#include "util.h"

#define str_eq(s1, s2)    (!strcmp ((s1),(s2)))

extern int yydebug;
extern NODE *yyparse (void);
extern NODE *ans;
extern void init_symbtable (void);

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
    evaluate(tree);
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
        printf("Entering evaluate\n");
        NODE *output = evaluate(tree);
        printf("--------------------------------------------\n");
        print_leaf(output->left, 0);
    }
    // start interactive session
    while (false) // TODO change to true when implemented
    {
        char *command = prompt(); // accept input one expression at a time
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

    yyin = fopen("tests/test_source/add.c", "r");

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

