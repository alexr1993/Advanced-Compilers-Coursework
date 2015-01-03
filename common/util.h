#ifndef COMMON_UTIL_H_
#define COMMON_UTIL_H_

#include <string.h>
#include "analysis/nodes.h"
#include "analysis/environment.h"
struct node;

static char *const LINE =
"**************************************************************************";


char *named(int t);
void print_banner(char *content);
void print_leaf(struct node *tree, int level);
void print_branch(struct node *tree);
void print_tree(struct node *tree);
char *int_to_str(int a);
void set_input_file(char *filename);
void close_input_file();
void parse(char *filename);
struct node *get_true_root(struct node *);
struct node *get_false_root(struct node *);
#define str_eq(s1, s2)    (!strcmp ((s1),(s2)))

#endif
