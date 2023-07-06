#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/offroad.h"
#include "include/argparser.h"
#include "include/common.h"

int main(int argc, char **argv)
{
    SET_PROG_NAME(argv[0]);

    int outcode = 0;

    offroad_func_result *result = parse_args(argc, argv);

    if (result == NULL)
    {
        debug_msg("Insuficient memory!", CRITICAL);
    }
    else if (result->type == OK)
    {
        offroad_cli_args *args = create_args();
        UNWRAP_OK_RESULT(result, args, offroad_cli_args);

        result = execute_offroad(args);

        if (result != NULL && result->type == ERR)
        {
            outcode = result->to.err.status;
            debug_msg(result->to.err.message, result->to.err.dbg_level);
        }

        free_args(&args);
    }
    else if (result->type == ERR)
    {
        outcode = result->to.err.status;
        debug_msg(result->to.err.message, result->to.err.dbg_level);
    }

    if (result != NULL)
        free_result(&result);

    return outcode;
}
