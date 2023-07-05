#include <stdlib.h>

#ifndef _OFFROAD_R_NODE
#define _OFFROAD_R_NODE

struct rnode_args
{
    char *filename;
    FILE *file;
};

void free_rnode_args(struct rnode_args** rnode_args);

#endif