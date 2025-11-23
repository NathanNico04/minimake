#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cli.h"

static char *makefile_name = NULL;
static int flag_print = 0;
static int flag_help = 0;

int handle_arguments(int argc, char **argv) {
    if(argc < 2) return 0;

    for(int i = 1; i < argc; i++) {
        if(argv[i] == NULL || argv[i][0] == '\0') {
            fprintf(stderr, "minimake: *** empty string invalid as argument. Stop.\n");
            return 2;
        }

        if(strcmp(argv[i], "-h") == 0) {
            flag_help = 1;
        } else if(strcmp(argv[i], "-p") == 0) {
            flag_print = 1;
        } else if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 < argc) {
                makefile_name = argv[i + 1];
                i++;
            } else {
                fprintf(stderr, "minimake: -f option requires a filename\n");
                return 2;
            }
        }
    }

    if(flag_help) {
        print_help();
        return 0;
    }

    if(makefile_name == NULL) makefile_name = "Makefile";

    FILE *f = fopen(makefile_name, "r");
    if(!f) {
        fprintf(stderr, "minimake: *** %s: No such file. Stop.\n", makefile_name);
        return 2;
    }
    fclose(f);

    if(parse_minimakefile(makefile_name) != 0) return 2;

    if(flag_print) {
        pretty_print(variables, rules); 
        return 0;
    }

    return 0;
}

void print_help(void) {
    printf("Usage: minimake [target] [-f makefile] [-p] [-h]\n");
    printf("-f file : specify Makefile to use\n");
    printf("-p      : pretty-print the Makefile and exit\n");
    printf("-h      : display this help message\n");
}

void pretty_print(struct variable *variables, struct rule *rules) {
    printf("# variables\n");
    struct variable *v = variables;
    while(v != NULL) {
        printf("'%s' = '%s'\n", v->name, v->value);
        v = v->next;
    }

    printf("# rules\n");
    struct rule *r = rules;
    while(r != NULL) {
        printf("(%s):", r->target);
        for(int i = 0; i < r->nb_deps; i++) {
            printf(" [%s]", r->dependencies[i]);
        }
        printf("\n");
        for(int i = 0; i < r->nb_cmds; i++) {
            printf("'%s'\n", r->recipe[i]);
        }
        r = r->next;
    }
}
