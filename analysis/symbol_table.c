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
#include "C.tab.h"

#include "symbol_table.h"

#define HASH_SIZE (1000)
TOKEN *int_token, *void_token, *function_token;

TOKEN **new_symbtable() {
  TOKEN **symbtable = (TOKEN**)calloc(HASH_SIZE, sizeof(TOKEN*));
  int_token = new_token(INT);
  int_token->lexeme = "int";
  function_token = new_token(FUNCTION);
  function_token->lexeme = "function";
  void_token = new_token(VOID);
  void_token->lexeme = "void";
  return symbtable;
}

// Return index of Identifier in symbol table
int hash(char *s) {
  int h = 0;
  while (*s != '\0') {
    h = (h<<4) ^ *s++;
  }
  return (0x7fffffff&h) % HASH_SIZE;
}

/* Enter token at the start of the chain */
void enter_token(TOKEN *t, TOKEN **symbtable) {
  int h = hash(t->lexeme);
  t->next = symbtable[h];
  symbtable[h] = t;
}

/*
 * Looks up the input string, returning a token if it's in the symbol table
 *
 * Otherwise it creates a token and insert it
 */
TOKEN* lookup_token(char *s, TOKEN **symbtable) {
  int	h = hash(s);
  TOKEN *a = symbtable[h];
  TOKEN *ans;

  /* Return token for s if it exists */
  printf("\nSYMBOL TABLE: Lookup: \"%s\"\n", s);
  while (a!=NULL) {
    if (strcmp(a->lexeme, s)==0) {
      a->newly_created = false;
      print_token(a);
      return a;
    }
    a = a->next;
  }

  /* If not create and insert a token for s */
  ans = make_identifier(s);
  enter_token(ans, symbtable);
  printf("SYMBOL TABLE: \"%s\" stored at %p\n", s, ans);
  print_token(ans);
  return ans;
}

void print_symbtable(TOKEN **symbtable, bool should_print_var) {
  int i;
  if (!should_print_var) printf("SYMBOL TABLE\n");
  printf("===============================================================\n");
  TOKEN *current;
  for (i = 0; i < HASH_SIZE; i++) {
    if (symbtable[i] != NULL) {
      // Print all of chain
      current = symbtable[i];
      while(current) {
        print_token(current);
        if (should_print_var) {
          printf("  ");
          print_var(current->var);
        }
        current = current->next;
      }
    }
  }
  printf("===============================================================\n");
}
