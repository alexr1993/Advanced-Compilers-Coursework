#include <stdio.h>
#include "C.tab.h"
#include <string.h>
#include <stdlib.h>

#include "token.h"

TOKEN *new_token(int type)
{
    TOKEN *ans = (TOKEN*)malloc(sizeof(TOKEN));
    ans->type = type;
    ans->newly_created = true;
    return ans;
}

TOKEN *make_string(char *s)
{
    TOKEN *ans = new_token(STRING_LITERAL);
    int len = strlen(s);
    ans->lexeme = (char*)calloc(1, len-1);
    strncpy(ans->lexeme, s+1, len-2);
    return ans;
}

TOKEN *make_int(char *s)
{
    int n = *s!='\'' ? atoi(s) : *(s+1);
    TOKEN *ans = new_token(CONSTANT);
    ans->value = n;
    return ans;
}

TOKEN *make_identifier(char *s)
{
    TOKEN *ans = new_token(IDENTIFIER);
    ans->lexeme = (char*)malloc(1+strlen(s));
    strcpy(ans->lexeme, s);
    return ans;
}

char *type_to_str(int type) {
  switch(type) {
    case STRING_LITERAL:
      return "STRING_LITERAL";
    case CONSTANT:
      return "CONSTANT";
    case INT:
      return "INT";
    case FUNCTION:
      return "FUNCTION";
    case VOID:
      return "VOID";
    case IDENTIFIER:
      return "IDENTIFIER";
    default:
      return "TYPE_NOT_RECOGNISED";
  }
}

void print_token(TOKEN *t) {
    printf("TOKEN type: %s, lexeme: \"%s\", new? %s\n",
            type_to_str(t->type),
            t->lexeme,
            t->newly_created ? "yes" : "no");
}
