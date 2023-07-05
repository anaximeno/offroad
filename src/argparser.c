#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "../include/argparser.h"
#include "../include/common.h"

bool validate_args_file(offroad_cli_args** args)
{
    if (args != NULL && *args != NULL)
    {
        struct stat fs;

        if (stat((*args)->filename, &fs) == -1)
        {
            (*args)->error = "File error";
        }
        else if (!S_ISREG(fs.st_mode))
        {
            (*args)->error = "File is not regular";
        }
        else if (!(fs.st_mode & S_IXUSR) || !(fs.st_mode & S_IXOTH))
        {
            (*args)->error = "File is not executable";
        }
        else
        {
            return true;
        }
    }

    return false;
}

extern offroad_cli_args* parse_args(int argc, char** argv)
{
    offroad_cli_args* args = (offroad_cli_args*)malloc(sizeof(offroad_cli_args));

    if (args != NULL)
    {
        args->filename = NULL;
        args->error = NULL;

        if (argc > 1)
        {
            args->filename = argv[1];
            validate_args_file(&args);
        }
        else
        {
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