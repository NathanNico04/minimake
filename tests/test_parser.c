#include <criterion/criterion.h>
#include "../src/parser.h"

Test(parser, parse_variables) {
    parse_minimakefile("Minimakefile");
    cr_assert_not_null(variables);
    cr_assert_str_eq(variables->name, "BAR");
    cr_assert_str_eq(variables->value, "world");
}

Test(parser, parse_rules) {
    parse_minimakefile("Minimakefile");
    cr_assert_not_null(rules);
    cr_assert_str_eq(rules->target, "all");
    cr_assert_eq(rules->nb_deps, 2);
}
