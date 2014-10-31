#ifndef __PARAM_H
#define __PARAM_H

#include "environment.h"

/* params exist in singly linked lists, their order must match that of args */
typedef struct PARAM
{
    char *name;
    int  type;
    struct PARAM *next;
} PARAM;

#endif
