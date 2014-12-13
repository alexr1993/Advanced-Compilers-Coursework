#include <check.h>
#include <stdlib.h>

#include "analysis/symbol_table.h"
#include "evaluation/environment.h"
#include "test_source/basic_arithmetic.h"

extern FRAME *gbl_frame;
extern TOKEN **symbtable;

//START_TEST (interpret) {
  /*
  ck_assert_str_eq(
    interpret(basic_addition()),
    "8");
  */
//} END_TEST


START_TEST (symboltable) {
  init_symbtable();

  // Token should only by newly_created on it's first lookup
  TOKEN *t = lookup_token("variable");
  ck_assert_int_eq(1, t->newly_created);

  t = lookup_token("variable");
  ck_assert_int_eq(0, t->newly_created);
} END_TEST

START_TEST (environment) {
  init_environment();
  char *testvar = "testvar";

  // int variable test
  init_var(testvar, INT_TYPE, gbl_frame);

  STATE *state = new_int_state(42);
  assign_var(testvar, INT_TYPE, state, gbl_frame);

  ck_assert_int_eq(
    42,
    lookup_var("testvar", INT_TYPE, gbl_frame)->state->value
  );

  // fn variable test
  init_var(testvar, FN_TYPE, gbl_frame);

  state = new_fn_state(new_function(
    INT_TYPE,
    gbl_frame,
    NULL,
    "testvar"));
} END_TEST

Suite *evaluate_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Frontend");

  /* Core test case */
  tc_core = tcase_create("Core");

  //tcase_add_test(tc_core, interpret);
  tcase_add_test(tc_core, symboltable);
  //tcase_add_test(tc_core, environment);
  suite_add_tcase(s, tc_core);

  return s;
}

int main (void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = evaluate_suite();
  sr = srunner_create(s);

  /* Run suite */
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
