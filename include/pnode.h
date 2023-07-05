#ifndef _OFFROAD_P_NODE
#define _OFFROAD_P_NODE

#define OFFROAD_P_NODE_TMP_FILE "runner"

struct pnode_args
{
    int port;
};

int execute_pnode(struct pnode_args* args);

void free_pnode_args(struct pnode_args** rnode_args);

#endif