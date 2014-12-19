#ifndef __UTIL_H
#define __UTIL_H

#include <string.h>
#include "analysis/nodes.h"
struct node;

static char *const LINE =
"**************************************************************************";


char *named(int t);
void print_banner(char *content);
void print_leaf(struct node *tree, int level);
void print_branch(struct node *tree);
void print_tree(struct node *tree);
void set_input_file(char *filename);
void close_intput_file();
#define str_eq(s1, s2)    (!strcmp ((s1),(s2)))

#endif
