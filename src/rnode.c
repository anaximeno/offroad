#include "../include/rnode.h"

void free_rnode_args(struct rnode_args** rnode_args)
{
    if ((*rnode_args)->file != NULL)
    {
        fclose((*rnode_args)->file);
    }

    free(*rnode_args);

    *rnode_args = NULL;
}
