#ifndef HOSHICREDITS_H
#define HOSHICREDITS_H

#include "datatypes.h"

typedef struct ModCredit
{
    char *role;     // the name of the role
    char **names;   // null terminated array containing the names of the people who performed the role
} ModCredit;

#endif