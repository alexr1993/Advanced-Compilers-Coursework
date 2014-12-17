#ifndef ANALYSIS_SYMBOL_TABLE_H_
#define ANALYSIS_SYMBOL_TABLE_H_

#include <stdbool.h>
#include "token.h"

struct token **new_symbtable();
struct token* lookup_token(char *s, struct token **symbtable);
void enter_token(struct token *t, struct token **symbtable);
void print_symbtable(struct token **symbtable, bool should_print_var);
#endif
