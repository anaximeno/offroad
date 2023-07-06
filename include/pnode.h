#ifndef _OFFROAD_P_NODE
#define _OFFROAD_P_NODE

#include "common.h"

#define OFFROAD_P_NODE_TMP_FILE "runner"
#define OFFROAD_P_NODE_DEFAULT_HOST "127.0.0.1"

struct pnode_args
{
    int port;
};

offroad_func_result* execute_pnode(struct pnode_args *args);

void free_pnode_args(struct pnode_args **rnode_args);

#endif