#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

#include "analysis/C.tab.h"
#include "analysis/token.h"
#include "analysis/symbol_table.h"
#include "analysis/environment.h"

extern FRAME *gbl_frame;
extern struct token_stack *ts;

extern int yyparse();


/* Check token lookup works */
START_TEST(basic_frame) {
  init_environment();
  // Token should only by newly_created on it's first lookup
  TOKEN *t = lookup_token("variable", gbl_frame->symbols);
  ck_assert_int_eq(1, t->newly_created);

  t = lookup_token("variable", gbl_frame->symbols);
  ck_assert_int_eq(0, t->newly_created);
} END_TEST

/* Check frames nest correctly */
START_TEST(parent_frame) {
  init_environment();

  FRAME *current = new_frame(gbl_frame, "testframe");

  ck_assert_int_eq(
    (int)current->parent, (int)gbl_frame
  );
} END_TEST

/* Check basic stack operations */
START_TEST(token_stack) {
  init_token_stack();

  TOKEN *t = make_identifier("myvar");
  push(t);

  TOKEN *t2 = make_identifier("myvar2");
  push(t2);

  ck_assert_int_eq(ts->size, 2);
} END_TEST

/* Checks relationships of all frames in tree */
void check_frames(FRAME *parent) {
  FRAME *child = parent->child;
  while (child != NULL) {
    ck_assert_str_eq(parent->proc_id, child->parent->proc_id);
    check_frames(child);
    child = child->sibling;
  }
}

/* Check frame structure using C-- source code */
START_TEST(frames) {
  init_environment();
  set_input_file("t/src/closure.cmm");
  yyparse();

  check_frames(gbl_frame);
} END_TEST

/*
 * Test Suite Setup
 */
Suite *evaluate_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Frontend");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, basic_frame);
  tcase_add_test(tc_core, parent_frame);
  tcase_add_test(tc_core, token_stack);
  tcase_add_test(tc_core, frames);
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
