#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/common.h"

static char *__program__name__ = "offroad";

void __set_program_name(char *name)
{
    __program__name__ = name;
}

char *__get_program_name()
{
    return __program__name__;
}
