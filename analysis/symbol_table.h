#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include "token.h"
#include "C.tab.h"

static TOKEN** symbtable;

void init_symbtable(void);
TOKEN* lookup_token(char *s);

#endif
