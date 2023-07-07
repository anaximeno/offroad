#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/offroad.h"
#include "include/argparser.h"

#include "include/ax-c-common/include/ax-c-common.h"

int main(int argc, char **argv)
{
    int outcode = 0;

    ax_result_p result = parse_args(argc, argv);

    if (result == NULL)
    {
        ax_log(WARN, "Insuficient memory!");
    }
    else if (result->type == OK)
    {
        offroad_cli_args *args = axunwrap_ok(result, offroad_cli_args);
        ax_free_result(&result);

        result = execute_offroad(args);

        free_args(&args);
    }

    if (result != NULL && result->type == ERR)
    {
        outcode = result->to.err.errnum;
        ax_log(ERROR, result->to.err.message);
    }

    ax_free_result(&result);
    return outcode;
}
