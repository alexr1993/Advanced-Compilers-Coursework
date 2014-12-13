#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include "token.h"

TOKEN **new_symbtable();
TOKEN* lookup_token(char *s, TOKEN **symbtable);

#endif
