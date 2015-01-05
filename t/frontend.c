#include "frontend.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include "common/util.h"

#include "analysis/C.tab.h"
#include "analysis/token.h"
#include "analysis/symbol_table.h"
#include "analysis/environment.h"

extern FRAME *gbl_frame;
extern struct token_stack *ts;
extern void register_frame_pointers(FRAME *, FRAME *);
extern int yyparse();
extern FILE *yyin;
extern int V,v;

char *filename;
/* Check token lookup works */
START_TEST(basic_frame) {
  init_environment();
  // Token should only by newly_created on it's first lookup
  TOKEN *t = lookup_token("variable", gbl_frame->symbols, false);
  ck_assert_int_eq(1, t->newly_created);

  t = lookup_token("variable", gbl_frame->symbols, false);
  ck_assert_int_eq(0, t->newly_created);
} END_TEST

/* Check frames nest correctly */
START_TEST(parent_frame) {
  init_environment();

  FRAME *current = new_frame("testframe");
  register_frame_pointers(gbl_frame, current);

  ck_assert_int_eq(
    (uintptr_t)current->parent, (uintptr_t)gbl_frame
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
  int nchildren = 0;
  FRAME *child = parent->child;
  while (child != NULL) {
    nchildren++;
    ck_assert_str_eq(parent->proc_id, child->parent->proc_id);
    check_frames(child);
    child = child->sibling;
  }
  ck_assert_int_eq(nchildren, parent->nchildren);
}

START_TEST(frames) {
  filename = "t/src/integration_tests/closure.cmm";
  if (v) print_banner(filename);
  parse(filename);
  if (V) print_environment(gbl_frame);
  check_frames(gbl_frame);

  // FIXME this test is fragile
  ck_assert_str_eq(gbl_frame->child->proc_id, "cplus");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "cplusa");

  filename = "t/src/integration_tests/awkward_declarations.cmm";
  if (v) print_banner(filename);
  parse(filename);

  if (V) print_environment(gbl_frame);
  check_frames(gbl_frame);

  // FIXME so is this: It's possible that these will be the other way roudn
  ck_assert_str_eq(gbl_frame->child->proc_id, "main");
  ck_assert_str_eq(gbl_frame->child->sibling->proc_id, "g");

  filename = "t/src/integration_tests/nested_subroutine.cmm";
  if (v) print_banner(filename);
  parse(filename);

  if (V) print_environment(gbl_frame);
  check_frames(gbl_frame);

  ck_assert_str_eq(gbl_frame->child->proc_id, "fact");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "inner_fact");

  filename = "t/src/integration_tests/first_class_function.cmm";
  if (v) print_banner(filename);
  parse(filename);

  if (V) print_environment(gbl_frame);
  check_frames(gbl_frame);

  ck_assert_str_eq(gbl_frame->child->proc_id, "twice");
  ck_assert_str_eq(gbl_frame->child->child->proc_id, "g");

} END_TEST

FRAME *find_child(FRAME *parent, char *name) {
  FRAME *child = parent->child;
  while (!str_eq(name, child->proc_id)) {
    child = child->sibling;
  }
  return child;
}

START_TEST(parameter_recognition) {
  filename = "t/src/integration_tests/awkward_declarations.cmm";
  parse(filename);

  if (V) print_environment(gbl_frame);
  TOKEN *t = lookup_token("x", find_child(gbl_frame, "f")->symbols, true);
  ck_assert(t->declaration_type == PARAMETER);

} END_TEST

/****************************************************************************
 * SUITE SETUP
 ***************************************************************************/

Suite *frontend_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("frontend");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, basic_frame);
  tcase_add_test(tc_core, parent_frame);
  tcase_add_test(tc_core, token_stack);
  tcase_add_test(tc_core, frames);
  //FIXME this fails because the prev test case uses the same file
  tcase_add_test(tc_core, parameter_recognition);
  suite_add_tcase(s, tc_core);
  return s;
}


