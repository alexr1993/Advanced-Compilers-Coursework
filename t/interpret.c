#include "interpret.h"

#include "analysis/C.tab.h"
#include "analysis/environment.h"
#include "analysis/nodes.h"
#include "analysis/token.h"

#include "synthesis/interpret.h"

extern FRAME *gbl_frame;
extern NODE ans;
extern int *yyparse();

START_TEST(t_interpret_control) {
  set_input_file("t/src/control/1.cmm");
  init_environment();
  yyparse();
  close_input_file();

  ck_assert_int_eq(42, call("main", gbl_frame)->state->integer);
} END_TEST

START_TEST(t_interpret_logic) {

} END_TEST

START_TEST(t_interpret_arithmetic) {

} END_TEST

Suite *interpret_suite() {
  Suite *s = suite_create("interpret");

  TCase *control = tcase_create("control");
  tcase_add_test(control, t_interpret_control);

  TCase *logic = tcase_create("logic");
  tcase_add_test(logic, t_interpret_logic);

  TCase *arithmetic = tcase_create("arithmetic");
  tcase_add_test(arithmetic, t_interpret_arithmetic);

  suite_add_tcase(s, control);
  suite_add_tcase(s, logic);
  suite_add_tcase(s, arithmetic);
  return s;
}
