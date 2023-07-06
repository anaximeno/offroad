#include <stdio.h>
#include <stdlib.h>

#include "../include/offroad.h"
#include "../include/common.h"
#include "../include/argparser.h"
#include "../include/pnode.h"
#include "../include/rnode.h"

offroad_func_result *execute_offroad(offroad_cli_args *args)
{
    if (args->error != NULL)
        return create_result(1, args->error, ERROR);

    switch (args->run_type)
    {
    case RNODE:
        return execute_rnode(&args->to.rnode);

    case PNODE:
        return execute_pnode(&args->to.pnode);

    default:
        return create_result(1, "Unknown run type", ERROR);
    }
}