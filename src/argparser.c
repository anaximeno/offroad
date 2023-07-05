#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <getopt.h>

#include "../include/argparser.h"
#include "../include/common.h"

bool validate_args_file(offroad_cli_args **args)
{
    if (args != NULL && *args != NULL)
    {
        struct stat fs;

        if (stat((*args)->to.rnode.filename, &fs) == -1)
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

bool load_file(offroad_cli_args **args)
{
    if (args != NULL && *args != NULL)
    {
        (*args)->to.rnode.file = fopen((*args)->to.rnode.filename, "r");

        if ((*args)->to.rnode.file != NULL)
        {
            return true;
        }
        else
        {
            (*args)->error = "File could not be loaded";
        }
    }

    return false;
}

void parse_commands(int argc, char **argv, offroad_cli_args **args)
{
    if (argc == 1)
    {
        (*args)->error = "No arguments were provided";
        return;
    }

    int c, option_index;

    static struct option long_options[] = {
        {"rnode", required_argument, 0, 'r'},
        {"pnode", required_argument, 0, 'p'}};

    while (true)
    {
        option_index = 0;

        c = getopt_long(argc, argv, "p:r:", long_options, &option_index);

        if (c == -1)
        {
            break;
        }

        switch (c)
        {
        case 'r': /* R NODE*/
            (*args)->run_type = RNODE;
            (*args)->to.rnode.filename = optarg;
            break;

        case 'p': /* P NODE*/
            (*args)->run_type = PNODE;
            (*args)->to.pnode.port = atoi(optarg);
            break;

        case '?':
            break;

        default:
            (*args)->error = "Argument parsing error";
            break;
        }
    }
}

void prepare_pnode_args(offroad_cli_args **args)
{
    // TODO
}

void prepare_rnode_args(offroad_cli_args **args)
{
    (*args)->to.rnode.file = NULL;

    if (validate_args_file(&args))
    {
        load_file(&args);
    }
}

extern offroad_cli_args *parse_args(int argc, char **argv)
{
    offroad_cli_args *args = (offroad_cli_args *)malloc(sizeof(offroad_cli_args));

    if (args != NULL)
    {
        parse_commands(argc, argv, &args);

        if (args->error == NULL)
        {
            if (args->run_type == RNODE)
            {
                prepare_rnode_args(&args);
            }
            else if (args->run_type == PNODE)
            {
                prepare_pnode_args(&args);
            }
            else
            {
                args->error = "Unknown parse run type state";
            }
        }

        return args;
    }

    return NULL;
}

extern void free_args(offroad_cli_args **args)
{
    if ((*args)->run_type == RNODE && (*args)->to.rnode.file != NULL)
    {
        fclose((*args)->to.rnode.file);
    }

    free(*args);

    *args = NULL;
}