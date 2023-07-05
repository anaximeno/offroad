#include <stdlib.h>

#include "common.h"
#include "rnode.h"
#include "pnode.h"

#ifndef _OFFROAD_ARGPARSER
#define _OFFROAD_ARGPARSER

/** Defines the spected arguments for the program. */
typedef struct offroad_cli_args
{
    offroad_run_type run_type;

    union
    {
        struct rnode_args rnode;
        struct pnode_args pnode;
    } to;

    char *error;
} offroad_cli_args;

/** Returns the command line arguments for the program. */
offroad_cli_args *parse_args(int argc, char **argv);

/** Securely frees the args variable. */
void free_args(offroad_cli_args **args);

#endif