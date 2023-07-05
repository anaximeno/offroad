#include <stdio.h>
#include <stdlib.h>

#include "include/offroad.h"
#include "include/argparser.h"
#include "include/common.h"

int main(int argc, char **argv)
{
    offroad_cli_args *args = parse_args(argc, argv);
    int out = 0;

    if (args != NULL)
    {
        if (args->error != NULL)
        {
            debug_msg(args->error, ERROR);
            out = 1;
        }
        else
        {
            out = execute_offroad(args);
        }

        free_args(&args);
    }
    else
    {
        debug_msg("Insuficient memory!", CRITICAL);
    }

    return out;
}
