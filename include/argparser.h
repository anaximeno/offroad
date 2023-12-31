#ifndef _OFFROAD_ARGPARSER
#define _OFFROAD_ARGPARSER

#include <stdbool.h>

#include "rnode.h"
#include "pnode.h"

#include "../ax-c-common/ax-c-common.h"

typedef enum offroad_run_type
{
    UNKNOWN,
    PNODE,
    RNODE
} offroad_run_type;

/** Defines the spected arguments for the program. */
typedef struct offroad_cli_args
{
    offroad_run_type run_type;

    union
    {
        struct rnode_args rnode;
        struct pnode_args pnode;
    } to;
} offroad_cli_args;

typedef struct offroad_cli_parse_info
{
    bool is_rnode;
    bool is_pnode;
    char *filename;
    char *host;
    int port;
} offroad_cli_parse_info;

/** Returns the command line arguments for the program. */
ax_result_p parse_args(int argc, char **argv);

/** Used to create args. */
offroad_cli_args *create_args();

/** Securely frees the args variable. */
void free_args(offroad_cli_args **args);

#endif