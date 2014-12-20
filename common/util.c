#include "util.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "analysis/nodes.h"
#include "analysis/C.tab.h"

extern FILE *yyin;
extern NODE *ans;
extern int V,v;

/* General Print Utils */
void print_banner(char *contents) {
  printf("%s\n%s\n%s\n", LINE, contents, LINE);
}

/* Prints string representation of input node type */
char *named(int t) {
  static char b[100];
  if (isgraph(t) || t==' ') {
    // Write the symbol to the string at identifier b
    sprintf(b, "%c", t);
    return b;
  }
  switch (t) {
   default: return "???";
   case IDENTIFIER:
    return "id";
   case CONSTANT:
    return "constant";
   case STRING_LITERAL:
    return "string";
   case LE_OP:
    return "<=";
   case GE_OP:
    return ">=";
   case EQ_OP:
    return "==";
   case NE_OP:
    return "!=";
   case EXTERN:
    return "extern";
   case AUTO:
    return "auto";
   case INT:
    return "int";
   case VOID:
    return "void";
   case APPLY:
    return "apply";
   case LEAF:
    return "leaf";
   case IF:
    return "if";
   case ELSE:
    return "else";
   case WHILE:
    return "while";
   case CONTINUE:
    return "continue";
   case BREAK:
    return "break";
   case RETURN:
    return "return";
  }
}

/* Prints string representation of node's token's type */
void print_leaf(NODE *tree, int level) {
  TOKEN *t = (TOKEN *)tree;
  int i;
  for (i=0; i<level; i++) putchar(' ');
  if (t->type == CONSTANT) {
    printf("%d (%s)\n", t->value, type_to_str(t->type));
  } else {
    printf("%s (%s)\n", t->lexeme, type_to_str(t->type));
  }
}

void print_branch(NODE *tree) {
    printf("%s\n", named(tree->type));
}

void print_tree0(NODE *tree, int level) {
  int i;
  if (tree==NULL) return;
  if (tree->type==LEAF) {
    print_leaf(tree->left, level);
  }
  else {
    for(i=0; i<level; i++) putchar(' ');
    print_branch(tree);
    print_tree0(tree->left, level+2);
    print_tree0(tree->right, level+2);
  }
}

void print_tree(NODE *tree) {
  print_tree0(tree, 0);
}

/* Input File Utils */
void set_input_file(char *filename) {
  printf("Setting input file to %s\n", filename);
  yyin = fopen(filename, "r");
  if (yyin == NULL && filename != NULL) perror("Input file error!");
}

void close_input_file() {
  if (yyin) fclose(yyin);
}

void parse(char *filename) {
  printf("Parse called with filename %s\n", filename);
  if (filename != NULL) {
    set_input_file(filename);
  }
  if (v) print_banner(filename);
  init_environment();
  if (v) printf("yyparsing\n");
  yyparse();
  if (V) print_tree(ans);
  close_input_file();
}
