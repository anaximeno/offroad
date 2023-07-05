#include <stdio.h>
#include <stdlib.h>

#include "../include/offroad.h"
#include "../include/common.h"
#include "../include/argparser.h"
#include "../include/pnode.h"
#include "../include/rnode.h"

int execute_offroad(offroad_cli_args *args)
{
    if (args->error != NULL)
    {
        debug_msg(args->error, ERROR);
        return 1;
    }

    switch (args->run_type)
    {
    case RNODE:
        execute_rnode(&args->to.rnode);
        break;

    case PNODE:
        execute_pnode(&args->to.pnode);
        break;

    default:
        args->error = "Unknown run type";
        debug_msg(args->error, ERROR);
        return 1;
    }

    return 0;
}