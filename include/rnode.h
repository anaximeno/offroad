#ifndef _OFFROAD_R_NODE
#define _OFFROAD_R_NODE

#include <stdio.h>

#include "common.h"

struct rnode_args
{
    FILE *file;
    char *filename;
    char *host;
    int port;
};

offroad_func_result* execute_rnode(struct rnode_args *args);

void free_rnode_args(struct rnode_args **rnode_args);

#endif