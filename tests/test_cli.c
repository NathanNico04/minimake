#include <criterion/criterion.h>
#include "../src/parser.h"
#include "../src/cli.h"

Test(cli, help_option) {
    char *argv[] = {"minimake", "-h", NULL};
    int ret = handle_arguments(2, argv);
    cr_assert_eq(ret, 0);
}

Test(cli, print_option) {
    char *argv[] = {"minimake", "-p", "-f", "Makefile", NULL};
    int ret = handle_arguments(4, argv);
    cr_assert_eq(ret, 0);
}
