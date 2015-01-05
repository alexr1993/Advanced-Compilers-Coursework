#include "interpret.h"

#include "common/util.h"

#include "analysis/C.tab.h"
#include "analysis/environment.h"
#include "analysis/nodes.h"
#include "analysis/token.h"

#include "synthesis/interpret.h"

extern FRAME *gbl_frame;
VALUE *output;

START_TEST(t_interpret_control) {
  parse("t/src/control/1.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);

  parse("t/src/control/2.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);

/* This test is a version of factorial that will fail until an env
 * bug is fixed - the argument is corrupted by procedures it is used in
  parse("t/src/control/3.cmm");
  output = interpret_program();
  ck_assert_int_eq(6, output->state->integer);
 */
  parse("t/src/control/4.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);
} END_TEST

START_TEST(t_interpret_logic) {
  parse("t/src/logic/1.cmm");
  output = interpret_program();
  ck_assert_int_eq(1, output->state->integer);

  parse("t/src/logic/2.cmm");
  output = interpret_program();
  ck_assert_int_eq(0, output->state->integer);

  parse("t/src/logic/3.cmm");
  output = interpret_program();
  ck_assert_int_eq(1, output->state->integer);


} END_TEST

START_TEST(t_interpret_arithmetic) {
  parse("t/src/arithmetic/1.cmm");
  output = interpret_program();
  ck_assert_int_eq(1, output->state->integer);

  parse("t/src/arithmetic/2.cmm");
  output = interpret_program();
  ck_assert_int_eq(1, output->state->integer);

} END_TEST

START_TEST(t_interpret_integration) {
  parse("t/src/integration_tests/closure.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);

  parse("t/src/integration_tests/nested_subroutine.cmm");
  output = interpret_program();
  ck_assert_int_eq(6, output->state->integer);

  parse("t/src/integration_tests/first_class_function.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);

  parse("t/src/integration_tests/awkward_declarations.cmm");
  output = interpret_program();
  ck_assert_int_eq(42, output->state->integer);
} END_TEST

Suite *interpret_suite() {
  Suite *s = suite_create("interpret");

  TCase *control = tcase_create("control");
  tcase_add_test(control, t_interpret_control);

  TCase *logic = tcase_create("logic");
  tcase_add_test(logic, t_interpret_logic);

  TCase *arithmetic = tcase_create("arithmetic");
  tcase_add_test(arithmetic, t_interpret_arithmetic);

  TCase *integration = tcase_create("integration");
  tcase_add_test(integration, t_interpret_integration);

  suite_add_tcase(s, control);
  suite_add_tcase(s, logic);
  suite_add_tcase(s, arithmetic);
  suite_add_tcase(s, integration);
  return s;
}
