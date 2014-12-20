%{
#define YYSTYPE struct node*
#include "nodes.h"
#include <stdio.h>
#include <stdbool.h>

#include "common/util.h"
#include "environment.h"
#include "token.h"

#define YYDEBUG 1

extern struct token *int_token, *void_token, *function_token, *lasttok;
extern char yytext[];
extern int column;
extern FRAME *gbl_frame;

NODE *ans;

int V, v, counter = 1, current_type, current_return_type;

int yyerror(char *s);
int yylex();


/* Functions for construction of environment */
void create_frame(NODE *);
void populate_gbl_frame(NODE *);
char *name_from_fn_def(NODE *);
void bond_with_children(NODE *n, bool is_gbl);
void register_frame_pointers(FRAME *parent, FRAME *child);
void init_val(TOKEN *t);
void init_fn(TOKEN *t, FRAME *frame);
FRAME *parent_frame;
%}

%token IDENTIFIER CONSTANT STRING_LITERAL
%token LE_OP GE_OP EQ_OP NE_OP

%token EXTERN AUTO
%token INT VOID FUNCTION
%token APPLY LEAF

%token  IF ELSE WHILE CONTINUE BREAK RETURN

%start goal
%%

goal    :  translation_unit { ans = $$ = $1; populate_gbl_frame($1); }
        ;

primary_expression
	: IDENTIFIER { $$ = make_leaf(lasttok); }
	| CONSTANT 			{ $$ = make_leaf(lasttok); }
	| STRING_LITERAL 		{ $$ = make_leaf(lasttok); }
	| '(' expression ')' 		{ $$ = $2; }
	;

postfix_expression
	: primary_expression		{ $$ = $1; }
	| postfix_expression '(' ')'    { $$ = make_node(APPLY, $1, NULL); }
	| postfix_expression '(' argument_expression_list ')' {
				          $$ = make_node(APPLY, $1, $3); }
	;

argument_expression_list
	: assignment_expression		{ $$ = $1; }
	| argument_expression_list ',' assignment_expression {
          $$ = make_node(',', $1, $3); }
	;

unary_expression
	: postfix_expression		{ $$ = $1; }
	;

multiplicative_expression
	: unary_expression		{ $$ = $1; }
	| multiplicative_expression '*' unary_expression {
                                          $$ = make_node('*', $1, $3); }
	| multiplicative_expression '/' unary_expression {
                                          $$ = make_node('/', $1, $3); }
	| multiplicative_expression '%' unary_expression {
                                          $$ = make_node('%', $1, $3); }
	;

additive_expression
	: multiplicative_expression		{ $$ = $1; }
	| additive_expression '+' multiplicative_expression {
                                          $$ = make_node('+', $1, $3); }
	| additive_expression '-' multiplicative_expression {
                                          $$ = make_node('-', $1, $3); }
	;

relational_expression
	: additive_expression		{ $$ = $1; }
	| relational_expression '<' additive_expression	{
                                          $$ = make_node('<', $1, $3); }
	| relational_expression '>' additive_expression {
                                          $$ = make_node('>', $1, $3); }
	| relational_expression LE_OP additive_expression {
                                          $$ = make_node(LE_OP, $1, $3); }
	| relational_expression GE_OP additive_expression {
                                          $$ = make_node(GE_OP, $1, $3); }
	;

equality_expression
	: relational_expression		{ $$ = $1; }
	| equality_expression EQ_OP relational_expression {
                                          $$ = make_node(EQ_OP, $1, $3); }
	| equality_expression NE_OP relational_expression {
                                          $$ = make_node(NE_OP, $1, $3); }
	;

assignment_expression
	: equality_expression		{ $$ = $1; }
	| unary_expression '=' assignment_expression {
        $$ = make_node('=', $1, $3);
      }
	;

expression
	: assignment_expression		{ $$ = $1; }
	| expression ',' assignment_expression { $$ = make_node(',', $1, $3); }
	;

declaration
	: declaration_specifiers ';'		{ $$ = $1; }
	| function_definition {
        if (V) printf("PARSER Found function definition\n");
        current_type = FN_TYPE;
        create_frame($1);
        $$ = $1;
      }
	| declaration_specifiers init_declarator_list ';' {
      $$ = make_node('~', $1, $2);
    }
	;

declaration_specifiers
	: storage_class_specifier		{ $$ = $1; }
	| storage_class_specifier declaration_specifiers {
                                                  $$ = make_node('~', $1, $2); }
	| type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers { $$ = make_node('~', $1, $2); }
	;

init_declarator_list
	: init_declarator		{ $$ = $1; }
	| init_declarator_list ',' init_declarator { $$ = make_node(',', $1, $3); }
	;

init_declarator
	: declarator		{ $$ = $1; }
	| declarator '=' assignment_expression { $$ = make_node('=', $1, $3); }
	;

storage_class_specifier
	: EXTERN	{ $$ = $1; }
	| AUTO		{ $$ = $1; }
	;

type_specifier
	: VOID		{ $$ = make_leaf(void_token); }
	| INT		{ $$ = make_leaf(int_token); current_type = INT_TYPE; }
	| FUNCTION	{ $$ = make_leaf(function_token); current_type = FN_TYPE; }
	;

declarator
	: direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER		{
      $$ = make_leaf(lasttok);
      lasttok->data_type = current_type;
      push(lasttok);
    }
	| '(' declarator ')'	{ $$ = $2; }
    | direct_declarator '(' parameter_list ')' {
        $$ = make_node('F', $1, $3);
        mark_params();
      }
	| direct_declarator '(' identifier_list ')'{
        $$ = make_node('F', $1, $3);
        mark_params();
      }
	| direct_declarator '(' ')' {
        $$ = make_node('F', $1, NULL);
      }
	;

parameter_list
	: parameter_declaration		{ $$ = $1; }
	| parameter_list ',' parameter_declaration { $$ = make_node(',', $1, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator {
        $$ = make_node('~', $1, $2);
      }
	| declaration_specifiers abstract_declarator {
        $$ = make_node('~', $1, $2);
      }
	| declaration_specifiers	{ $$ = $1; }
	;

identifier_list
	: IDENTIFIER                    { $$ = make_leaf(lasttok); }
	| identifier_list ',' IDENTIFIER {
         $$ = make_node(',', $1, make_leaf(lasttok)); }
	;

abstract_declarator
	:  direct_abstract_declarator    { $$ = $1; }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'    { $$ = $2; }
	| '(' ')'    { $$ = NULL; }
	| '(' parameter_list ')'    { $$ = $2; }
	| direct_abstract_declarator '(' ')'    { $$ = make_node(APPLY, $1, NULL); }
	| direct_abstract_declarator '(' parameter_list ')' {
        $$ = make_node(APPLY, $1, $3); }
	;

statement
	: compound_statement		{ $$ = $1; }
	| expression_statement		{ $$ = $1; }
	| selection_statement		{ $$ = $1; }
	| iteration_statement		{ $$ = $1; }
	| jump_statement		{ $$ = $1; }
	;

compound_statement
	: '{' '}'                       { $$ = NULL; }
	| '{' statement_list '}'	{ $$ = $2; }
	| '{' declaration_list '}'	{ $$ = $2; }
	| '{' declaration_list statement_list '}' { $$ = make_node(';', $2, $3); }
	;

declaration_list
	: declaration			{ $$ = $1; }
	| declaration_list declaration { $$ = make_node(';', $1, $2); }
	;

statement_list
	: statement			{ $$ = $1; }
	| statement_list statement 	{ $$ = make_node(';', $1, $2); }
	;

expression_statement
	: ';'				{ $$ = NULL; }
	| expression ';'		{ $$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement { $$ = make_node(IF, $3, $5); }
	| IF '(' expression ')' statement ELSE statement {
        $$ = make_node(IF, $3, make_node(ELSE, $5, $7)); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = make_node(WHILE, $3, $5); }
	;

jump_statement
	: CONTINUE ';'                  { $$ = make_node(CONTINUE, NULL, NULL); }
	| BREAK ';'                     { $$ = make_node(BREAK, NULL, NULL); }
	| RETURN ';'	                { $$ = make_node(RETURN, NULL, NULL); }
	| RETURN expression ';'		    { $$ = make_node(RETURN, $2, NULL); }
	;

translation_unit
	: external_declaration		{ $$ = $1; }
	| translation_unit external_declaration { $$ = make_node('~', $1, $2);}
	;

external_declaration
	:  declaration                    { $$ = $1; }
	;

function_definition
	:  declaration_specifiers declarator compound_statement  {
        current_return_type = current_type;
        $$ = make_node('D', make_node('d', $1, $2), $3);
      }
	;
%%

int yyerror(char *s) {
  fflush(stdout);
  printf("\n%*s\n%*s\n", column, "^", column, s);
  return 0;
}

/****************************************************************************
 * SEMANTIC ACTIONS
 ***************************************************************************/

/*
 * Creates a frame using a 'D' node
 */
void create_frame(NODE *n) {

  FRAME *frame = new_frame(name_from_fn_def(n));
  n->frame = frame;
  frame->root = n;

  bond_with_children(n, false);

  /* Process every identifier that was found on the subtree */
  TOKEN *t = pop();

  // Register all members of the frames symboltable
  while (!str_eq(t->lexeme, frame->proc_id)) {
    if (t->val == NULL) init_val(t);
    // Parameter Identifiers have already been marked
    if (t->declaration_type != PARAMETER) {
      t->declaration_type = VARIABLE;
    }
    enter_token(t, frame->symbols);

    t = pop();
  }
  // the current token is the function which owns this frame
  init_fn(t, frame);
  push(t); // let enclosing frames find this fn
}

/*
 * Registers the remaining tokens on the token stack with the global frame
 */
void populate_gbl_frame(NODE *n) {
  if (V) printf("Populating gbl frame!\n");
  TOKEN *t = pop();
  while (t != NULL) {
    if (t->val == NULL) init_val(t);
    enter_token(t, gbl_frame->symbols);
    t = pop();
  }
  bond_with_children(n, true);
}

/*
 * Fetches the name of the function given the 'D' node (the fns definition)
 */
char *name_from_fn_def(NODE *D) {
  TOKEN *t = get_token(D->left->right->left);
  return t->lexeme;
}

/*
 * Find nodes indicating frame starts and establish pointers between them
 */
void bond_with_children(NODE *n, bool is_gbl) {
  NODE *D;
  // Binding a tree with root 'D' with the global frame is an edge case
  if (is_gbl && (char)n->type == 'D') {
    D = n;
  } else {
    D = n->next_D;
  }
  while (D != NULL) {
    if (V) printf("Next 'D' child in subtree:\n");
    FRAME *child = D->frame;
    register_frame_pointers(is_gbl ? gbl_frame : n->frame, child);
    D = D->next_D;
  }
  // These frames have now been bonded
  n->next_D = NULL;
}

/*
 * Adds references from the child frame to the parent and vice versa
 * Child is insert as the first child of it's parent
 */
void register_frame_pointers(FRAME *parent, FRAME *child) {
  child->parent = parent;
  child->sibling = parent->child;
  parent->child = child;
  parent->nchildren++;
  if (V) printf("Registered \"%s\" as the parent of child \"%s\"\n",
  parent->proc_id, child->proc_id);
  if (V) print_frame(parent);
}

/* Some tokens e.g. function definition roots will have their value
 * initialised explicitly, other declarations will need this
 *
 * Essentially the tokens passed to this are functions or ints which have been
 * declared with normal declaration syntax
 */
void init_val(TOKEN *t) {
    t->val = new_val(t->data_type, new_state(t->data_type));
}

/* Initialise function with the most recently parsed return type  */
void init_fn(TOKEN *t, FRAME *frame) {
  t->data_type = FN_TYPE;
  t->declaration_type = VARIABLE;
  t->val = new_val(FN_TYPE,
           new_fn_state(new_function(current_return_type, frame)));
}
