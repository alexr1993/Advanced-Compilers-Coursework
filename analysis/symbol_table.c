#include "symbol_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C.tab.h"

/*
 * Adapted from CM20029 Coursework Assignment 1 (Tom Crick - cs1tc@bath.ac.uk)
 * 30 Apr 2003
 */
extern int V, v;

TOKEN **new_symbtable() {
  TOKEN **symbtable = (TOKEN**)calloc(HASH_SIZE, sizeof(TOKEN*));
  return symbtable;
}

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

/* Runtime == false (i.e. parse-time)
 * Looks up the input string, returning a token if it's in the symbol table
 * Otherwise it creates a token and insert it.
 *
 * Runtime == true
 * Looks up token, returning NULL if it is absent
 */
TOKEN* lookup_token(char *s, TOKEN **symbtable, bool runtime) {
  int	h = hash(s);
  TOKEN *a = symbtable[h];
  TOKEN *ans;

  /* Return token for s if it exists */
  if (V) printf("\nSYMBOL TABLE Lookup: \"%s\"...", s);
  while (a!=NULL) {
    if (strcmp(a->lexeme, s)==0) {
      a->newly_created = false;
      if (V) printf("found!\n");
      if (V) print_token(a);
      return a;
    }
    a = a->next;
  }
  if (runtime) {
    if (V) printf("not found!\n");
    return NULL;
  }
  /* If not create and insert a token for s */
  ans = make_identifier(s);
  enter_token(ans, symbtable);
  if (V) printf("not found: \"%s\" stored at %p\n", s, ans);
  if (V) print_token(ans);
  return ans;
}

void print_symbtable(TOKEN **symbtable, bool should_print_val) {
  int i;
  if (!should_print_val) printf("SYMBOL TABLE\n");
  printf("%s\n", LINE);
  TOKEN *current;

  // Scan through hashtable's array
  for (i = 0; i < HASH_SIZE; i++) {
    if (symbtable[i] != NULL) {
      // Print all of chain
      current = symbtable[i];
      while(current) {
        print_token(current);
        if (should_print_val) {
          printf("  ");
          print_val(current->val);
        }
        current = current->next;
      }
    }
  }
  printf("%s\n", LINE);
}
