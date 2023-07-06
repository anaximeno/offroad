#include <stdio.h>
#include <stdlib.h>

#include "../include/offroad.h"
#include "../include/common.h"
#include "../include/argparser.h"
#include "../include/pnode.h"
#include "../include/rnode.h"

#include "../include/ax-c-common/include/ax-c-common.h"

ax_result_p execute_offroad(offroad_cli_args *args)
{
    if (args != NULL)
    {
        switch (args->run_type)
        {
        case RNODE:
            return execute_rnode(&args->to.rnode);

        case PNODE:
            return execute_pnode(&args->to.pnode);

        default:
            return ax_result_err(1, "Internal error, run type unknown");
        }
    }

    return ax_result_err(1, "Internal error, got null 'args'");
}