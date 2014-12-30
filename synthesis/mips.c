#include "mips.h"

#include <stdio.h>
#include <stdlib.h>

/* Handles memory management, register allocation, instruction selection and
 * evaluation order
 *
 * Labels must be converted to addresses of instructions
 */

MIPS *gen_arith_and_log(TAC *code) {
  return NULL;
}

/* Selects and returns instruction for given operation */
MIPS *gen_instruction(TAC *code) {
  print_tac(code);
  switch(code->op) {
   case APPLY:
    break;
   case IF:
    break;
   case RETURN:
    break;
   case PUSH:
    break;
   case LOAD:
    break;
   case GOTO:
    break;
   case POP: break;
   case START: break;
   case END: break;
   case EQ_OP: break;
   default:
    return gen_arith_and_log(code);
  }
  return NULL;
}

MIPS *generate_mips(TAC *first) {
  while (first != NULL) {
    gen_instruction(first);
    first = first->next;
  }
  return NULL;
}
