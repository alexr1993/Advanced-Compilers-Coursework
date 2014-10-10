#include <check.h>
#include <stdlib.h>

#include "test_source/basic_arithmetic.h"

START_TEST (interpret)
{
    ck_assert_str_eq(
        interpret(basic_addition()),
        "8");
}
END_TEST

Suite *interpret_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Interpret");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, interpret);
    suite_add_tcase(s, tc_core);

    return s;
}

int main (void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = interpret_suite();
    sr = srunner_create(s);

    /* Run suite */
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
