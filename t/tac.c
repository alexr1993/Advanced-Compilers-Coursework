#include "tac.h"

START_TEST(t_tac_control) {
} END_TEST

START_TEST(t_tac_logic) {

} END_TEST

START_TEST(t_tac_arithmetic) {

} END_TEST

Suite *tac_suite() {
  Suite *s = suite_create("tac");

  TCase *control = tcase_create("control");
  tcase_add_test(control, t_tac_control);

  TCase *logic = tcase_create("logic");
  tcase_add_test(logic, t_tac_logic);

  TCase *arithmetic = tcase_create("arithmetic");
  tcase_add_test(arithmetic, t_tac_arithmetic);

  suite_add_tcase(s, control);
  suite_add_tcase(s, logic);
  suite_add_tcase(s, arithmetic);
  return s;
}

