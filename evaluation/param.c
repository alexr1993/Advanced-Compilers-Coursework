#include "param.h"
#include <stdlib.h>

PARAM *new_param(char *name, int type)
{
    PARAM *new_param = malloc(sizeof(PARAM));
    int name_length = strlen(name);

    new_param->name = malloc(name_length * sizeof(char));
    strncpy(new_param->name, name, name_length);

    new_param->type = type;
    return new_param;
}