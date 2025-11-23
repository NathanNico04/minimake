#ifndef PARSER_H
#define PARSER_H

struct variable {
    char *name;
    char *value;
    struct variable *next;
};

struct rule {
    char *target;
    char **dependencies;
    int nb_deps;
    char **recipe;
    int nb_cmds;
    struct rule *next;
};

extern struct variable *variables;
extern struct rule *rules;


int parse_minimakefile(const char *filename);
char *expand_variable(const char *str);

#endif /*! PARSER_H */
