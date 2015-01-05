#ifndef T_MIPS_H_
#define T_MIPS_H_

#include <check.h>

#include "analysis/C.tab.h"
#include "analysis/environment.h"
#include "analysis/nodes.h"
#include "analysis/token.h"

#include "synthesis/tac.h"
#include "synthesis/mips.h"

Suite *mips_suite();
#endif
