#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <getopt.h>

#include "../include/argparser.h"
#include "../include/pnode.h"
#include "../include/rnode.h"
#include "../include/common.h"

offroad_cli_parse_info *create_parse_info()
{
    offroad_cli_parse_info *info = (offroad_cli_parse_info *)malloc(sizeof(offroad_cli_parse_info));

    if (info != NULL)
    {
        info->is_pnode = false;
        info->is_rnode = false;
        info->host = NULL;
        info->filename = NULL;
        info->port = -1;
    }

    return info;
}

bool validate_args_file(char *filename, offroad_cli_args **args)
{
    if (args != NULL && *args != NULL)
    {
        struct stat fs;

        if (stat(filename, &fs) == -1)
            (*args)->error = "File error";
        else if (!S_ISREG(fs.st_mode))
            (*args)->error = "File is not regular";
        else if (!(fs.st_mode & S_IXUSR) || !(fs.st_mode & S_IXOTH))
            (*args)->error = "File is not executable";
        else
            return true;
    }

    return false;
}

offroad_cli_parse_info *parse_commands(int argc, char **argv, offroad_cli_args **args)
{
    if (argc == 1)
    {
        (*args)->error = "No arguments were provided";
        return NULL;
    }

    offroad_cli_parse_info *info = create_parse_info();

    if (info == NULL)
    {
        (*args)->error = "Insufficient memory";
        return NULL;
    }

    int c, option_index;

    static struct option long_options[] = {
        {"run", required_argument, 0, 'r'},
        {"process", no_argument, 0, 'p'},
        {"port", required_argument, 0, 0},
        {"host", required_argument, 0, 0}};

    while (true)
    {
        option_index = 0;

        c = getopt_long(argc, argv, "p:r:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            if (strcmp(long_options[option_index].name, "port") == 0)
                info->port = atoi(optarg);
            else if (strcmp(long_options[option_index].name, "host") == 0)
                info->host = optarg;
            break;

        case 'r': /* R NODE*/
            info->filename = optarg;
            info->is_rnode = true;
            break;

        case 'p': /* P NODE*/
            info->is_pnode = true;
            break;

        case '?':
        default:
            (*args)->error = "Argument parsing error";
            break;
        }
    }

    if (optind < argc)
    {
        // XXX: should improve here, the unknown arguments can be accessed
        // using the optind variable.
        (*args)->error = "Unknow argmuent";
    }

    return info;
}

void process_rnode_parse_info(offroad_cli_parse_info *info, offroad_cli_args **args)
{
    (*args)->run_type = RNODE;

    if (info->host == NULL || info->port == -1)
    {
        (*args)->error = "'--run/-r' requires '--host' and '--port' to be defined";
    }
    else
    {
        (*args)->to.rnode.filename = info->filename;
        (*args)->to.rnode.host = info->host;
        (*args)->to.rnode.port = info->port;
        (*args)->to.rnode.file = NULL;

        if (validate_args_file((*args)->to.rnode.filename, args))
        {
            (*args)->to.rnode.file = fopen((*args)->to.rnode.filename, "r");

            if ((*args)->to.rnode.file == NULL)
                (*args)->error = "File could not be loaded";
        }
    }
}

void process_pnode_parse_info(offroad_cli_parse_info *info, offroad_cli_args **args)
{
    (*args)->run_type = PNODE;

    if (info->port == -1)
        (*args)->error = "'--process/-p' requires '--port' to be defined";
    else
        (*args)->to.pnode.port = info->port;
}

void process_parsed_info(offroad_cli_parse_info *info, offroad_cli_args **args)
{
    if (info != NULL && args != NULL && *args != NULL)
    {
        if (info->is_rnode && info->is_pnode)
            (*args)->error = "'--run/-r' and '--process/-p' should be called separetely";
        else if (info->is_rnode)
            process_rnode_parse_info(info, args);
        else if (info->is_pnode)
            process_pnode_parse_info(info, args);
    }
}

extern offroad_cli_args *parse_args(int argc, char **argv)
{
    offroad_cli_args *args = (offroad_cli_args *)malloc(sizeof(offroad_cli_args));

    if (args != NULL)
    {
        args->error = NULL;
        args->run_type = NONE;

        offroad_cli_parse_info *info = NULL;
        info = parse_commands(argc, argv, &args);

        if (args->error == NULL && info != NULL)
            process_parsed_info(info, &args);

        return args;
    }

    return NULL;
}

extern void free_args(offroad_cli_args **args)
{
    if ((*args)->run_type == RNODE && (*args)->to.rnode.file != NULL)
        fclose((*args)->to.rnode.file);

    free(*args);
    *args = NULL;
}