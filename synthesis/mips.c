#include "mips.h"

#include <stdio.h>
#include <stdlib.h>

extern int V,v;
ADDR_DESC  *r_zero, *r_at,
          **r_v, **r_a, **r_t, **r_s, **r_k,
           *r_gp, *r_gp, *r_sp, *r_fp, *r_ra;

/* Handles memory management, register allocation, instruction selection and
 * evaluation order
 *
 * Labels must be converted to addresses of instructions
 */

/* Implementation is guided by Aho/Sethi/Ullman P535+ */

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

void create_str_rep(MIPS *mcode) {
  mcode->str = malloc(20);
  switch(mcode->op) {
   case APPLY:
    // TODO god save us all...
    break;
   case PUSH: case POP: case LOAD: case GOTO: case START: case END:
    if (mcode->result->type == IDENTIFIER) {
      sprintf(mcode->str, "%s %s",
             named(mcode->op),
             mcode->result->val->addr->str);
    } else {
      sprintf(mcode->str, "%s %s",
             named(mcode->op),
             mcode->result->val->state->integer);
    }
    break;
   // Arithmetic and logic
   default:
    if (mcode->arg->type == IDENTIFIER) {
      sprintf(mcode->str,
              "%s %s, %s",
              named(mcode->op),
              mcode->arg->val->addr->str,
              mcode->result->val->addr->str);
    } else {
      sprintf(mcode->str,
              "%s %d, %s",
              named(mcode->op),
              mcode->arg->val->state->integer,
              mcode->result->val->addr->str);
    }
  }
}
void print_mips(MIPS *mcode);

MIPS *new_mips(int op, TOKEN *arg, TOKEN *res, TAC *corresp_tac) {
  MIPS *mcode = malloc(sizeof(MIPS));
  mcode->corresp_tac = corresp_tac;
  mcode->arg = arg;
  mcode->result = res;
  mcode->op  = op;
  create_str_rep(mcode);
  mcode->next = NULL;
  mcode->prev = NULL;
  print_mips(mcode);
  return mcode;
}

/* Returns the first free reg in the reglist */
ADDR_DESC *get_reg(ADDR_DESC **reglist, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (reglist[i]->live == false) {
      return reglist[i];
    }
  }
  return NULL;
}

// Assume at least one arg is an identifier which needs loading
MIPS *gen_arith_and_log(TAC *code) {
  TOKEN *arg, *result;

  /* Pick an identifier with a register to store the result */
  if (code->arg1->type == IDENTIFIER) {
    result = code->arg1;
    arg = code->arg2;
  } else if (code->arg2 != NULL && code->arg2->type == IDENTIFIER) {
    result = code->arg2;
    arg = code->arg1;
  } else {
    perror("Error! Unhandled Condition");
    abort();
  }
 // Set result's address
  code->result->val->addr = result->val->addr;
  return new_mips(code->op, arg, result, code);
}

/* Generates MIPS statement to load a value into a register */
MIPS *gen_load(TAC *code) {
  // Assign the arg a register
  ADDR_DESC *reg = get_reg(r_t, 10);
  code->arg1->val->addr = reg;
  // Generate code
  return new_mips(LOAD, NULL, code->arg1, code);
}

/* Selects and returns instruction for given operation */
MIPS *gen_instruction(TAC *code) {
  if (code->op == 0) return NULL; // Weed out tac statements which store constants... these should be eliminated before here TODO
  print_tac(code);
  if (V) printf("MIPS Generating for op \"%s\"\n", named(code->op));
  switch(code->op) {
   case APPLY: break;
   case IF:  break;
   case RETURN: break;
   case LOAD: return gen_load(code);
   case GOTO: break;
   case PUSH: break;
   case POP: break;
   case START: break;
   case END: break;
   case '=': break;
   default:
    return gen_arith_and_log(code);
  }
  return NULL;
}
void init_reg_descriptors();
void print_reg_descriptors(ADDR_DESC **descriptors, int n);

MIPS *generate_mips(TAC *first) {
  init_reg_descriptors();
  print_reg_descriptors(reg_descriptors, NREGISTERS);

  /* Print registers by type */
  /*print_reg_descriptors(&r_zero, 1);
  print_reg_descriptors(&r_at, 1);
  print_reg_descriptors(r_v, 2);
  print_reg_descriptors(r_a, 4);
  print_reg_descriptors(r_t, 10);
  print_reg_descriptors(r_s, 8);
  print_reg_descriptors(r_k, 2);
  print_reg_descriptors(&r_gp, 1);
  print_reg_descriptors(&r_sp, 1);
  print_reg_descriptors(&r_fp, 1);
  print_reg_descriptors(&r_ra, 1);*/
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
  if (V) {
    printf("MIPS arg ");
    print_token(mcode->arg);
    printf("MIPS result ");
    if (mcode->result != NULL) print_token(mcode->result);
    else printf("none\n");
  }
}

void print_mips_prog(MIPS *mcode) {
  while (mcode != NULL) {
    print_mips(mcode);
    mcode = mcode->next;
  }
}

void print_reg_descriptors(ADDR_DESC **descriptors, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", i);
    print_addr_descriptor(descriptors[i]);
  }
}

/****************************************************************************
 * Init
 ***************************************************************************/
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
    reg_descriptors[i] = reg;

    // Setup strings and aliases
    switch(i) {
     case 0:
      sprintf(reg->str, "$zero");
      r_zero = reg;
      r_zero->live = true;
      // r_zero implicitly contains 0
      break;

     case 1:
      sprintf(reg->str, "$at");
      r_at = reg;
      break;

     case 2: r_v = &reg_descriptors[i];
     case 3:
      sprintf(reg->str, "$v%d", i - 2);
      break;

     case 4: r_a = &reg_descriptors[i];
     case 5: case 6: case 7:
      sprintf(reg->str, "$a%d", i - 4);
      break;

     case 8:  case 9:  case 10: case 11:
     case 12: case 13: case 14: case 15:
      sprintf(reg->str, "$t%d", i - 8);
      r_t[i-8] = reg;
      break;

     case 16: r_s = &reg_descriptors[i];
     case 17: case 18: case 19:
     case 20: case 21: case 22: case 23:
      sprintf(reg->str, "$s%d", i - 16);
      break;

     case 24: case 25:
      sprintf(reg->str, "$t%d", i - 16);
      r_t[i-16] = reg;
      break;

     case 26: r_k = &reg_descriptors[i];
     case 27:
      sprintf(reg->str, "$k%d", i - 26);
      break;

     case 28:
      sprintf(reg->str, "$gp");
      r_gp = reg;
      break;

     case 29:
      sprintf(reg->str, "$sp");
      r_sp = reg;
      break;

     case 30:
      sprintf(reg->str, "$fp");
      r_fp = reg;
      break;

     case 31:
      sprintf(reg->str, "$ra");
      r_ra = reg;
      break;
    }
  }
}
