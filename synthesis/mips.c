#include "mips.h"

#include <stdio.h>
#include <stdlib.h>

ADDR_DESC *r_zero, *r_at, **r_v, **r_a, **r_t, **r_s, **r_k,
          *r_gp, *r_gp, *r_sp, *r_fp, *r_ra;

/* Handles memory management, register allocation, instruction selection and
 * evaluation order
 *
 * Labels must be converted to addresses of instructions
 */


/* Implementation is guided by Aho/Sethi/Ullman P535+ */

// TODO implement register descriptors to keep track of register -> name
// mappings and address descriptors to keep track of name -> address mappings

/* MIPS REGISTERS
 *
 * $0                the constant zero
 * $1      ($at)     assembler temporary
 * $2-$3   ($v0-$v1) return value(s)
 * $4-$7   ($a0-$a3) arguments
 * $8-$15  ($t0-$t7) temps
 * $16-$23 ($s0-$s7) saved
 * $24-$25 ($t8-$t9) more temps
 * $26-27  ($k0-$k1) reserved for kernel
 * $28     ($gp)
 * $29     ($sp)
 * $30     ($fp)
 * $31     ($ra)
 */

/****************************************************************************
 * Instance Manipulation
 ***************************************************************************/

MIPS *new_mips(ADDR_DESC *arg, ADDR_DESC *res, int op, TAC *corresp_tac) {
  MIPS *mcode = malloc(sizeof(MIPS));
  mcode->corresp_tac = corresp_tac;
  mcode->arg = NULL;
  mcode->res = NULL;
  mcode->op  = NULL;
  mcode->str = malloc(20);
  mcode->next = NULL;
  mcode->prev = NULL;
  return mcode;
}

/* Returns a register or possibly just a mem address */
ADDR_DESC *get_reg() {
  // TODO check reg_descriptor for free registers and return one
  return reg_descriptors[1];
}

MIPS *gen_arith_and_log(TAC *code) {
  return NULL;
}

/* Selects and returns instruction for given operation */
MIPS *gen_instruction(TAC *code) {
  ADDR_DESC *r = get_reg(); // output loc
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
void init_reg_descriptors();

MIPS *generate_mips(TAC *first) {
  init_reg_descriptors();
  while (first != NULL) {
    gen_instruction(first);
    first = first->next;
  }
  return NULL;
}

/****************************************************************************
 * Diagnostics
 ***************************************************************************/

void print_mips(MIPS *mcode) {
  if (mcode->str == NULL) printf("String rep for mips is NULL\n");
  else printf("%s\n", mcode->str);
}

void print_mips_prog(MIPS *mcode) {
  while (mcode != NULL) {
    print_mips(mcode);
    mcode = mcode->next;
  }
}

/****************************************************************************
 * Init
 ***************************************************************************/
void print_reg_descriptors() {
  // TODO, also make a print_addr_descriptor()
}

/* Creates address descriptors for each register and links aliases of the
 * conventional blocks to the registers in the reg_descriptors array
 */
void init_reg_descriptors() {
  // The temporaries block of regs is not contiguous so encapsulate them in
  // this pointer array
  r_t = malloc(8 * sizeof(ADDR_DESC *));
  ADDR_DESC *reg;
  int i;
  for (i = 0; i < NREGISTERS; i++) {
    reg = new_address_descriptor(i);

    // Setup strings and aliases
    switch(i) {
     case 0:
      sprintf(reg->str, "$zero");
      r_zero = reg;
      break;

     case 1:
      sprintf(reg->str, "$at");
      r_at = reg;
      break;

     case 2: case 3:
      sprintf(reg->str, "$v%d", i - 2);
      r_v = &reg;
      break;

     case 4: case 5: case 6: case 7:
      sprintf(reg->str, "$a%d", i - 4);
      r_a = &reg;
      break;

     case 8:  case 9:  case 10: case 11:
     case 12: case 13: case 14: case 15:
      sprintf(reg->str, "$t%d", i - 8);
      r_t[i-8] = reg;
      break;

     case 16: case 17: case 18: case 19:
     case 20: case 21: case 22: case 23:
      sprintf(reg->str, "$s%d", i - 16);
      r_s = &reg;
      break;

     case 24: case 25:
      sprintf(reg->str, "$t%d", i - 16);
      r_t[i-16] = reg;
      break;

     case 26: case 27:
      sprintf(reg->str, "$k%d", i - 26);
      r_k = &reg;

     case 28:
      sprintf(reg->str, "$gp");
      r_gp = reg;

     case 29:
      sprintf(reg->str, "$sp");
      r_sp = reg;

     case 30:
      sprintf(reg->str, "$fp");
      r_fp = reg;

     case 31:
      sprintf(reg->str, "$ra");
      r_ra = reg;
    }
    reg_descriptors[i] = reg;
  }
}
