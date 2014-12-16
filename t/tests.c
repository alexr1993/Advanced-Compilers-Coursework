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
extern void register_frame_pointers(FRAME *, FRAME *);
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

  FRAME *current = new_frame("testframe");
  register_frame_pointers(gbl_frame, current);

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
  printf("\nclosure.cmm\n");
  printf("===========\n");
  set_input_file("t/src/closure.cmm");
  init_environment();
  yyparse();
  print_environment(gbl_frame);
  check_frames(gbl_frame);

  ck_assert_str_eq(gbl_frame->child->proc_id, "cplus");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "cplusa");

  printf("\nawkward_declarations.cmm\n");
  printf("===========\n");
  set_input_file("t/src/awkward_declarations.cmm");
  init_environment();
  yyparse();
  print_environment(gbl_frame);
  check_frames(gbl_frame);

  // It's possible that these will be the other way roudn
  ck_assert_str_eq(gbl_frame->child->proc_id, "main");
  ck_assert_str_eq(gbl_frame->child->sibling->proc_id, "g");

  printf("\nnested_subroutine.cmm\n");
  printf("===========\n");
  set_input_file("t/src/nested_subroutine.cmm");
  init_environment();
  yyparse();
  print_environment(gbl_frame);
  check_frames(gbl_frame);

  ck_assert_str_eq(gbl_frame->child->proc_id, "fact");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "inner_fact");

  printf("\nfirst_class_function.cmm\n");
  printf("===========\n");
  set_input_file("t/src/first_class_function.cmm");
  init_environment();
  yyparse();
  print_environment(gbl_frame);
  check_frames(gbl_frame);

  ck_assert_str_eq(gbl_frame->child->proc_id, "twice");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "g");

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
