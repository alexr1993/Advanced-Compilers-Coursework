#include "tac.h"
TAC *output;

START_TEST(t_tac_control) {
  parse("t/src/control/1.cmm");
  output = generate_tac();

  parse("t/src/control/2.cmm");
  output = generate_tac();

  /* Aint nobody got time for factorial...
     parse("t/src/control/3.cmm");
    */
  parse("t/src/control/4.cmm");
  output = generate_tac();
} END_TEST

START_TEST(t_tac_logic) {
  parse("t/src/logic/1.cmm");
  output = generate_tac();

  parse("t/src/logic/2.cmm");
  output = generate_tac();

  parse("t/src/logic/3.cmm");
  output = generate_tac();
} END_TEST

START_TEST(t_tac_arithmetic) {
  parse("t/src/arithmetic/1.cmm");
  output = generate_tac();

  parse("t/src/arithmetic/2.cmm");
  output = generate_tac();
} END_TEST

START_TEST(t_tac_integration) {
  parse("t/src/integration_tests/closure.cmm");
  output = generate_tac();
/* Another factorial one, can't test until implementing a call stack
  parse("t/src/integration_tests/nested_subroutine.cmm");
   output = generate_tac();
 */
  parse("t/src/integration_tests/first_class_function.cmm");
  output = generate_tac();

  parse("t/src/integration_tests/awkward_declarations.cmm");
  output = generate_tac();
} END_TEST

Suite *tac_suite() {
  Suite *s = suite_create("tac");

  TCase *control = tcase_create("control");
  tcase_add_test(control, t_tac_control);

  TCase *logic = tcase_create("logic");
  tcase_add_test(logic, t_tac_logic);

  TCase *arithmetic = tcase_create("arithmetic");
  tcase_add_test(arithmetic, t_tac_arithmetic);

  TCase *integration = tcase_create("integration");
  tcase_add_test(integration, t_tac_integration);

  suite_add_tcase(s, control);
  suite_add_tcase(s, logic);
  suite_add_tcase(s, arithmetic);
  suite_add_tcase(s, integration);
  return s;
}

