#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "frontend.h"
#include "interpret.h"
#include "tac.h"

int V,v;
void print_test_help() {
  printf("Enter one of the following:\n");
  printf(" -f (frontend)\n -i (interpret)\n -t (tac)\n -m (mips)\n");
}

Suite *mips_suite() {
  Suite *s = NULL;
  return s;
}

int main (int argc, char *argv[]) {
  if (argc == 1) {
    print_test_help();
    exit(0);
  }
  V = 1;
  v = 1;

  Suite *s;
  int c = 0;
  while ((c = getopt(argc, argv, "fitm")) != -1) {
    switch(c) {
     case 'f':
      s = frontend_suite();
      break;
     case 'i':
      s = interpret_suite();
      break;
     case 't':
      s = tac_suite();
      break;
     case 'm':
      s = mips_suite();
      break;
     default:
      printf("Invalid test option\n");
      exit(0);
    }
  }

  int number_failed;
  SRunner *sr;

  sr = srunner_create(s);

  /* Run suite */
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
