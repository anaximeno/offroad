#include <stdio.h>
#include <stdlib.h>

#include "include/argparser.h"
#include "include/common.h"

int main(int argc, char** argv)
{
    offroad_cli_args* args = parse_args(argc, argv);

    if (args != NULL)
    {
        if (args->error != NULL)
        {
            debug_msg(args->error, ERROR);
        }
        else
        {
            debug_msg(args->filename, INFO);
        }

        free_args(&args);
    }
    else
    {
        debug_msg("Insuficient memory!", CRITICAL);
    }

    return 0;
}
