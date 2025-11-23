#ifndef CLI_H
#define CLI_H

#include "parser.h"

int handle_arguments(int argc, char **argv);
void print_help(void);
void pretty_print(struct variable *variables, struct rule *rules);

#endif /* !CLI_H */
