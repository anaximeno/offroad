#include <stdio.h>
#include <stdlib.h>

#include "include/offroad.h"
#include "include/argparser.h"
#include "include/common.h"

int main(int argc, char **argv)
{
    int output = 0;

    offroad_func_result *res = NULL;

    offroad_cli_args *args = parse_args(argc, argv);

    if (args != NULL)
    {
        if (args->error != NULL)
            debug_msg(args->error, ERROR);
        else
            res = execute_offroad(args);

        if (res != NULL && res->status != 0)
        {
            output = res->status;
            debug_msg(res->error, res->dbg_level);

            free(res);
            res = NULL;
        }

        free_args(&args);
    }
    else
    {
        debug_msg("Insuficient memory!", CRITICAL);
    }

    return output;
}
