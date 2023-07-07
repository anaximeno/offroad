#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <getopt.h>

#include "../include/ax-c-common/include/ax-c-common.h"

#include "../include/argparser.h"
#include "../include/pnode.h"
#include "../include/rnode.h"

offroad_cli_parse_info *create_parse_info()
{
    offroad_cli_parse_info *info = axlocate(offroad_cli_parse_info);

    if (info != NULL)
    {
        info->is_pnode = false;
        info->is_rnode = false;
        info->filename = NULL;
        info->host = NULL;
        info->port = -1;
    }

    return info;
}

offroad_cli_args *create_args(offroad_run_type run_type)
{
    offroad_cli_args *args = axlocate(offroad_cli_args);

    if (args != NULL)
        args->run_type = run_type;

    return args;
}

ax_result_p validate_args_file(char *filename, offroad_cli_args *args)
{
    if (args != NULL)
    {
        struct stat fs;
        if (stat(filename, &fs) == -1)
            return ax_result_err(errno, "File error");
        else if (!S_ISREG(fs.st_mode))
            return ax_result_err(1, "File is not regular");
        else if (!(fs.st_mode & S_IXUSR) || !(fs.st_mode & S_IXOTH))
            return ax_result_err(1, "File is not executable");
        else
            return ax_result_ok(NULL);
    }

    return ax_result_err(1, "Internal Error, received null args to validate");
}

/** Returns: "ok(offroad_cli_parse_info*)" or "err" */
ax_result_p parse_commands_info(int argc, char **argv)
{
    offroad_cli_parse_info *info = NULL;

    if (argc == 1)
        return ax_result_err(1, "No arguments were provided");

    info = create_parse_info();

    if (info == NULL)
        return ax_result_err(1, "Insufficient memory");

    int c, option_index;

    static struct option long_options[] = {
        {"run", required_argument, 0, 'r'},
        {"process", no_argument, 0, 0},
        {"port", required_argument, 0, 0},
        {"host", required_argument, 0, 0}};

    while (true)
    {
        option_index = 0;

        c = getopt_long(argc, argv, "r:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            if (strcmp(long_options[option_index].name, "port") == 0)
                info->port = atoi(optarg);
            else if (strcmp(long_options[option_index].name, "host") == 0)
                info->host = optarg;
            else if (strcmp(long_options[option_index].name, "process") == 0)
                info->is_pnode = true;
            break;

        case 'r': /* R NODE*/
            info->is_rnode = true;
            info->filename = optarg;
            break;

        case '?':
            break;

        default:
            free(info);
            return ax_result_err(1, "Argument parsing error");
        }
    }

    if (optind < argc)
    {
        free(info);
        return ax_result_err(ERROR, "Received unknown arguments");
    }

    return ax_result_ok(info);
}

ax_result_p process_rnode_info(offroad_cli_parse_info *info)
{
    offroad_cli_args *args = create_args(RNODE);

    if (args == NULL)
        return ax_result_err(1, "Insufficient memory");
    else if (info->host == NULL || info->port == -1)
        return ax_result_err(1, "'--run/-r' requires '--host' and '--port' to be defined");
    else
    {
        args->to.rnode.filename = info->filename;
        args->to.rnode.host = info->host;
        args->to.rnode.port = info->port;
        args->to.rnode.file = NULL;

        ax_result_p result = validate_args_file(args->to.rnode.filename, args);

        if (result != NULL && result->type == OK)
        {
            ax_free_result(&result);
            args->to.rnode.file = fopen(args->to.rnode.filename, "r");

            if (args->to.rnode.file == NULL)
                result = ax_result_err(1, "File could not be loaded");
            else
                result = ax_result_ok(args);
        }

        return result;
    }
}

ax_result_p process_pnode_info(offroad_cli_parse_info *info)
{
    offroad_cli_args *args = create_args(PNODE);

    if (args == NULL)
        return ax_result_err(1, "Insufficient Memory");
    else if (info->port == -1)
        return ax_result_err(1, "'--process/-p' requires '--port' to be defined");
    else
        args->to.pnode.port = info->port;

    return ax_result_ok(args);
}

ax_result_p parse_args_from_info(offroad_cli_parse_info *info)
{
    if (info != NULL)
    {
        if (info->is_rnode && info->is_pnode)
            return ax_result_err(1, "'--run/-r' and '--process/-p' should be called separetely");
        else if (info->is_rnode)
            return process_rnode_info(info);
        else if (info->is_pnode)
            return process_pnode_info(info);
    }

    return ax_result_err(1, "Got invalid params during the parsing of the arguments info");
}

extern ax_result_p parse_args(int argc, char **argv)
{
    offroad_cli_parse_info *info = NULL;
    ax_result_p result = parse_commands_info(argc, argv);

    if (result != NULL && result->type == OK)
    {
        info = axunwrap_ok(result, offroad_cli_parse_info);
        ax_free_result(&result);
        return parse_args_from_info(info);
    }

    return result;
}

extern void free_args(offroad_cli_args **args)
{
    if (args != NULL && *args)
    {
        if ((*args)->run_type == RNODE && (*args)->to.rnode.file != NULL)
            fclose((*args)->to.rnode.file);

        free((*args));
    }
}