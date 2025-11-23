#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

struct variable *variables = NULL;
struct rule *rules = NULL;

static char *my_strdup(const char *s) {
    if (!s) 
        return NULL;
    size_t len = 0;
    while (s[len] != '\0') len++;
        char *dup = malloc(len + 1);
    if (!dup) 
        return NULL;
    for (size_t i = 0; i <= len; i++) 
        dup[i] = s[i];
    
    return dup;
}

static char *trim(char *s) {
    while (*s && isspace(*s)) s++;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace(*end)) {
        *end = '\0';
        end--;
    }
    return s;
}

static struct variable *find_variable(const char *name) {
    struct variable *v = variables;
    while (v != NULL) {
        if (strcmp(v->name, name) == 0) return v;
        v = v->next;
    }
    return NULL;
}

static void append_value(char **res, size_t *ri, const char *val) {
    size_t vl = strlen(val);
    *res = realloc(*res, *ri + vl + 1);
    memcpy(*res + *ri, val, vl);
    *ri += vl;
    (*res)[*ri] = '\0';
}

static const char *get_variable_value(const char *name) {
    struct variable *v = find_variable(name);
    if (v != NULL) return v->value;
    return "";
}

char *expand_variable(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    result[0] = '\0';
    size_t ri = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '$' && str[i+1] == '(') {
            i += 2;
            size_t start = i;
            while (i < len && str[i] != ')') i++;
            char varname[256];
            memcpy(varname, str + start, i - start);
            varname[i - start] = '\0';
            const char *val = get_variable_value(varname);
            append_value(&result, &ri, val);
        } else {
            char c[2];
            c[0] = str[i];
            c[1] = '\0';
            append_value(&result, &ri, c);
        }
    }
    return result;
}

static void add_variable(const char *name, const char *value) {
    struct variable *v = malloc(sizeof(struct variable));
    v->name = my_strdup(name);
    v->value = my_strdup(value);
    v->next = variables;
    variables = v;
}

static void add_rule(struct rule *r) {
    r->next = rules;
    rules = r;
}

static void add_recipe_command(struct rule *r, char *line) {
    r->recipe = realloc(r->recipe, sizeof(char*) * (r->nb_cmds + 1));
    r->recipe[r->nb_cmds] = my_strdup(line + 1);
    r->nb_cmds++;
}

static void handle_variable_line(char *line) {
    char *eq = strchr(line, '=');
    if (eq != NULL) {
        *eq = '\0';
        char *name = trim(line);
        char *value = trim(eq + 1);
        add_variable(name, value);
    }
}

static void handle_rule_line(char *line, struct rule **current_rule) {
    char *colon = strchr(line, ':');
    if (colon != NULL) {
        *colon = '\0';
        char *target = trim(line);
        if (*target != '\0') {
            char *deps_str = trim(colon + 1);
            struct rule *r = malloc(sizeof(struct rule));
            r->target = expand_variable(target);
            r->dependencies = NULL;
            r->nb_deps = 0;
            r->recipe = NULL;
            r->nb_cmds = 0;
            r->next = NULL;
            char *dep = strtok(deps_str, " \t");
            while (dep != NULL) {
                r->dependencies = realloc(r->dependencies, sizeof(char*) * (r->nb_deps + 1));
                r->dependencies[r->nb_deps] = expand_variable(dep);
                r->nb_deps++;
                dep = strtok(NULL, " \t");
            }
            add_rule(r);
            *current_rule = r;
        }
    }
}

int parse_minimakefile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return 2;
    char line[1024];
    struct rule *current_rule = NULL;
    while (fgets(line, sizeof(line), f) != NULL) {
        char *l = trim(line);
        if (*l == '\0' || *l == '#') continue;
        if (l[0] == '\t') {
            if (current_rule != NULL) 
                add_recipe_command(current_rule, l);
        } else {
            current_rule = NULL;
            char *eq = strchr(l, '=');
            char *colon = strchr(l, ':');
            if (eq != NULL) {
                if (colon == NULL || eq < colon) handle_variable_line(l);
                else handle_rule_line(l, &current_rule);
            } else if (colon != NULL) {
                handle_rule_line(l, &current_rule);
            }
        }
    }
    fclose(f);
    return 0;
}
