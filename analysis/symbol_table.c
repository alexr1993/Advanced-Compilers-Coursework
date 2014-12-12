/*
 * Adapted from
 * CM20029 Coursework Assignment 1
 * Tom Crick
 * cs1tc@bath.ac.uk
 * 30 Apr 2003
 *
 * symbol_table.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "C.tab.h"

static TOKEN** symbtable;
#define HASH_SIZE (1000)
TOKEN *int_token, *void_token, *function_token;

void init_symbtable(void)
{
    symbtable = (TOKEN**)calloc(HASH_SIZE, sizeof(TOKEN*));
    int_token = new_token(INT);
    int_token->lexeme = "int";
    function_token = new_token(FUNCTION);
    function_token->lexeme = "function";
    void_token = new_token(VOID);
    void_token->lexeme = "void";
}

// Return index of Identifier in symbol table
int hash(char *s)
{
    int h = 0;
    while (*s != '\0') {
      h = (h<<4) ^ *s++;
    }
    return (0x7fffffff&h) % HASH_SIZE;
}

/*
 * Looks up the input string, returning a token if it's in the symbol table
 *
 * Otherwise it creates a token and insert it
 */
TOKEN* lookup_token(char *s)
{
    int	h = hash(s);
    TOKEN *a = symbtable[h];
    TOKEN *ans;

    /* Return token for s if it exists */
    printf("\nSYMBOL TABLE: Lookup: \"%s\"\n", s);
    while (a!=NULL) {
      if (strcmp(a->lexeme, s)==0) return a;
      a = a->next;
    }

    /* If not create and insert a token for s */
    ans = new_token(IDENTIFIER);
    ans->lexeme = (char*)malloc(1+strlen(s));
    strcpy(ans->lexeme, s);
    ans->next = symbtable[h];
    symbtable[h] = ans;
    printf("SYMBOL TABLE: \"%s\" stored at %p\n", s, ans);
    return ans;
}
