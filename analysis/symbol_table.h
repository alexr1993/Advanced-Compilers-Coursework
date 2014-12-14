#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include <stdbool.h>

#include "token.h"

TOKEN **new_symbtable();
TOKEN* lookup_token(char *s, TOKEN **symbtable);
void enter_token(TOKEN *t, TOKEN **symbtable);
void print_symbtable(TOKEN **symbtable, bool should_print_var);
#endif
