#include <stdlib.h>

#include "../include/pnode.h"

extern int execute_pnode(struct pnode_args* args)
{
    return 0; // TODO
}

void free_pnode_args(struct pnode_args **args)
{
    free(*args);
    *args = NULL;
}
