#include <stdio.h>
#include <stdlib.h>

#include "../include/argparser.h"

extern offroad_cli_args* parse_args(int argc, char** argv)
{
    offroad_cli_args* args = (offroad_cli_args*)malloc(sizeof(offroad_cli_args));

    if (args != NULL)
    {
        if (argc > 1)
        {
            args->filename = argv[1];
            args->error = NULL;
        }
        else
        {
            args->filename = NULL;
            args->error = "File not provided";
        }

        return args;
    }

    return NULL;
}

extern void free_args(offroad_cli_args** args)
{
    free(*args);
    *args = NULL;
}