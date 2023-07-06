#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
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

offroad_func_result *validate_args_file(char *filename, offroad_cli_args *args)
{
    if (args != NULL)
    {
        struct stat fs;
        if (stat(filename, &fs) == -1)
            return err_result(errno, "File error", ERROR);
        else if (!S_ISREG(fs.st_mode))
            return err_result(1, "File is not regular", ERROR);
        else if (!(fs.st_mode & S_IXUSR) || !(fs.st_mode & S_IXOTH))
            return err_result(1, "File is not executable", ERROR);
        else
            return ok_result(NULL);
    }

    return err_result(1, "Internal Error, received null args to validate", WARNING);
}

/** Returns: "ok(offroad_cli_parse_info*)" or "err" */
offroad_func_result *parse_commands(int argc, char **argv)
{
    offroad_func_result *result = NULL;
    offroad_cli_parse_info *info = NULL;

    if (argc == 1)
    {
        result = err_result(1, "No arguments were provided", ERROR);
        goto return_block;
    }

    info = create_parse_info();

    if (info == NULL)
    {
        result = err_result(1, "Insufficient memory", CRITICAL);
        goto return_block;
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
            info->is_rnode = true;
            info->filename = optarg;
            break;

        case 'p': /* P NODE*/
            info->is_pnode = true;
            break;

        case '?':
            break;

        default:
            free_result(&result);
            result = err_result(1, "Argument parsing error", ERROR);
            goto return_block;
        }
    }

    if (optind < argc)
        result = err_result(1, "Received unknown arguments", ERROR);
    else
        result = ok_result(info);

return_block:
    if (result != NULL && result->type == ERR)
    {
        free(info);
        info = NULL;
    }

    return result;
}

offroad_func_result *process_rnode_parse_info(offroad_cli_parse_info *info, offroad_cli_args *args)
{
    offroad_func_result *result = NULL;

    args->run_type = RNODE;

    if (info->host == NULL || info->port == -1)
    {
        result = err_result(1, "'--run/-r' requires '--host' and '--port' to be defined", ERROR);
    }
    else
    {
        args->to.rnode.filename = info->filename;
        args->to.rnode.host = info->host;
        args->to.rnode.port = info->port;
        args->to.rnode.file = NULL;

        result = validate_args_file(args->to.rnode.filename, args);

        if (result != NULL && result->type == OK)
        {
            free_result(&result);

            args->to.rnode.file = fopen(args->to.rnode.filename, "r");

            if (args->to.rnode.file == NULL)
                result = err_result(1, "File could not be loaded", ERROR);
            else
                result = ok_result(NULL);
        }
    }

    return result;
}

offroad_func_result *process_pnode_parse_info(offroad_cli_parse_info *info, offroad_cli_args *args)
{
    args->run_type = PNODE;

    if (info->port == -1)
        return err_result(1, "'--process/-p' requires '--port' to be defined", ERROR);

    args->to.pnode.port = info->port;
    return ok_result(NULL);
}

offroad_func_result *process_parsed_info(offroad_cli_parse_info *info, offroad_cli_args *args)
{
    if (info != NULL && args != NULL)
    {
        if (info->is_rnode && info->is_pnode)
            return err_result(1, "'--run/-r' and '--process/-p' should be called separetely", ERROR);
        else if (info->is_rnode)
            return process_rnode_parse_info(info, args);
        else if (info->is_pnode)
            return process_pnode_parse_info(info, args);
    }

    return err_result(1, "Got invalid params during the parsing of the arguments info", WARNING);
}

offroad_cli_args *create_args()
{
    offroad_cli_args *args = (offroad_cli_args *)malloc(sizeof(offroad_cli_args));

    if (args != NULL)
        args->run_type = UNKNOWN;

    return args;
}

extern offroad_func_result *parse_args(int argc, char **argv)
{
    offroad_func_result *result = parse_commands(argc, argv);
    offroad_cli_args *args = create_args();

    if (args != NULL && result != NULL)
    {
        if (result->type == OK)
        {
            offroad_cli_parse_info *info = create_parse_info();
            UNWRAP_OK_RESULT(result, info, offroad_cli_parse_info);

            result = process_parsed_info(info, args);

            free(info);
            return ok_result(args);
        }

        return result;
    }
    else
    {
        return err_result(1, "Insufficient memory", CRITICAL);
    }
}

extern void free_args(offroad_cli_args **args)
{
    if ((*args)->run_type == RNODE && (*args)->to.rnode.file != NULL)
        fclose((*args)->to.rnode.file);

    free(*args);
    *args = NULL;
}