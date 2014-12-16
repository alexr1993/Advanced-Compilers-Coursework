#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "environment.h"

extern int V;

char *data_type_to_str(int type) {
  switch(type) {
   case INT_TYPE:
    return "INT_TYPE";
   case FN_TYPE:
    return "FN_TYPE";
   default:
    return "TYPE_NOT_RECOGNISED";
  }
}

void print_state(STATE *s, int type) {
  switch(type) {
   case INT_TYPE:
    printf("state: %d\n", s->value);
    break;
   case FN_TYPE:
    printf("state: function_state\n");
    break;
  }
}

void print_var(VARIABLE *var) {
  if (!var) {
    printf("VARIABLE not initialised\n");
    return;
  }
  printf("VARIABLE type: %s, ", data_type_to_str(var->type));
  print_state(var->state, var->type);
}

/*
 * Prints frame including all of it's children
 */
void print_frame(FRAME *frame)
{
  if (frame->parent == NULL) {
    printf("\nFRAME \"%s\"\n", frame->proc_id);
  } else {
    printf("\nFRAME \"%s\", child of \"%s\"\n",
           frame->proc_id,
           frame->parent->proc_id);
  }
  print_symbtable(frame->symbols, true);
}

/* Prints subtree of frame */
void print_environment(FRAME *f) {
  print_frame(f);
  FRAME *child = f->child;

  while (child != NULL) {
    print_environment(child);
    child = child->sibling;
  }
}

/*
 * Finds and returns the env mapping with the given name and type
 *
 * Checks given env first, then recursive looks at parent until var
 * is found
 *
 */
ENV *lookup_var(char *name, int type, FRAME *frame)
{
  printf("Looking up variable \"%s\"...", name);

  while (frame)
  {
    ENV *frame_var = frame->variable;

    /* Find the env mapping with the given name */
    while (frame_var)
    {
      // Mapping found
      if (str_eq(name, frame_var->name) && type == frame_var->type)
      {
        printf("Found!\n");
        return frame_var;
      }
      // Still looking
      frame_var = frame_var->next;
    }
    printf("Not found, attempting to look in parent frame!\n");
    printf("Current frame: \n");
    print_frame(frame);

    frame = frame->parent;
  }

  printf("Variable lookup failed!\n");

  return NULL;
}

STATE *new_int_state(int value)
{
  STATE *state = malloc(sizeof(STATE));
  state->value = value;
  return state;
}

STATE *new_fn_state(function *function)
{
  STATE *state = malloc(sizeof(STATE));
  state->function = function;
  return state;
}

/* Sets the state of the variable */
ENV *assign_var(char *name, int type, STATE* value, FRAME *frame)
{
  ENV *var = lookup_var(name, type, frame);

  if (var && var->type == type)
  {
    var->state = value;
    if (type == INT_TYPE)
    {
      printf("Assigned %d to variable \"%s\"!\n", value->value, name);
    }
  }
  else
  {
    printf("Error: Assigning to uninitialised variable, check type!\n");
    abort();
  }
  return var;
}

/*
 * Create new frame and attack it correctly to its parent
 *
 */
FRAME *new_frame(FRAME *parent, char *proc_id)
{
  if (V) printf("\nCreating new frame \"%s\"\n", proc_id);
  FRAME *new_frame = malloc(sizeof(FRAME));
  new_frame->parent = parent;

  // Add frame as child of parent
  if (parent)
  {
    FRAME *child = parent->child;
    parent->child = new_frame;
    new_frame->sibling = child;
  }
  new_frame->symbols = new_symbtable();
  new_frame->proc_id = proc_id; // Should be token's lexeme
  return new_frame;
}

VARIABLE *new_var(int type, TOKEN *t, FRAME *frame) {
  STATE *s;
  switch(type) {
   case INT_TYPE:
    s = new_int_state(0);
    break;
   case FN_TYPE:
    s = new_fn_state(new_function(INT_TYPE, frame, NULL, t->lexeme));
    break;
   default:
    printf("Error: type for new variable not recognised\n");
    exit(-1);
  }

  VARIABLE *var = malloc(sizeof(VARIABLE));
  var->type = type;
  var->state = s;
  return var;
}

/*
 * Define commonly used variables for parsing
 */
void init_environment() {
  gbl_frame = new_frame(NULL, "gbl_frame");

  int_token = new_token(INT);
  int_token->lexeme = "int";
  function_token = new_token(FUNCTION);
  function_token->lexeme = "function";
  void_token = new_token(VOID);
  void_token->lexeme = "void";

  init_token_stack();
}

