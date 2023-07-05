#include <stdio.h>
#include <stdlib.h>

#include "../include/offroad.h"
#include "../include/common.h"
#include "../include/argparser.h"

int execute_offroad(offroad_cli_args* args)
{
    if (args->error != NULL)
    {
        debug_msg(args->error, ERROR);
        return 1;
    }

    //TODO

    return 0;
}