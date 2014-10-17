#include <string.h>

char *named(int t);
void print_leaf(NODE *tree, int level);
void print_branch(NODE *tree);
void print_tree(NODE *tree);

#define str_eq(s1, s2)    (!strcmp ((s1),(s2)))
