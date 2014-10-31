#include <check.h>
#include <stdlib.h>

#include "../evaluation/environment.h"
#include "test_source/basic_arithmetic.h"

extern FRAME *gbl_frame;

START_TEST (interpret)
{
    /*
    ck_assert_str_eq(
        interpret(basic_addition()),
        "8");
    */
}
END_TEST

START_TEST (environment)
{
    init_environment();
    ck_assert_ptr_ne(gbl_frame, NULL);
}
END_TEST

Suite *evaluate_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Interpret");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, interpret);
    tcase_add_test(tc_core, environment);
    suite_add_tcase(s, tc_core);

    return s;
}

int main (void)
{
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
