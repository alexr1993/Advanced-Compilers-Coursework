#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "analysis/nodes.h"
#include "analysis/token.h"
#include "analysis/C.tab.h"
#include "analysis/environment.h"

#include "synthesis/evaluate.h"
#include "synthesis/interpret.h"
#include "synthesis/tac.h"
#include "synthesis/mips.h"

#include "common/util.h"

extern int V, v;
extern EVAL_TYPE e_type;
char *filename;

void translate_to_TAC() {
  parse(NULL);
  generate_tac();
}

void translate_to_MIPS() {
  parse(NULL);
  generate_mips(generate_tac());
}

void interpret(void) {
  parse(NULL);
  VALUE *output = interpret_program();
  printf("Output: %d\n", output->state->integer);
}

int main(int argc, char *argv[]) {
  int c = 0;
  int len;
  char *action = NULL;
  V = 0; // Verbose
  v = 0;

  // Determine translation requested
  while ((c = getopt(argc, argv, "vVa:f:")) != -1) {
    switch (c) {
     case 'a':
      // copy action type to var
      len = strlen(optarg);
      action = (char *) malloc(len * sizeof(char));
      strncpy(action, optarg, len);
      printf("Action Selected: %s\n", action);
      break;
     case 'f':
      printf("Input source file: %s\n", optarg);
      set_input_file(optarg);
      break;
     case 'V':
      v = 1;
      V = 1;
      break;
     case 'v':
      v = 1;
      break;
     default:
      printf("Invalid argument.\n");
      abort();
    }
  }
  if (action == NULL) {
    action = "interpret";
    printf("Setting mode to \"%s\" by default.\n", action);
  }

  /* Translate */
  if      (str_eq(action, "interpret")) interpret();
  else if (str_eq(action, "tac"))       translate_to_TAC();
  else if (str_eq(action, "mips"))      translate_to_MIPS();
  else printf("Action is invalid!\n");
  return 0;
}
