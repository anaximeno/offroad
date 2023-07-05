#include <stdio.h>
#include <stdlib.h>

#include "../include/rnode.h"

extern void free_rnode_args(struct rnode_args** args)
{
    if ((*args)->file != NULL)
    {
        fclose((*args)->file);
    }

    free(*args);

    *args = NULL;
}

extern int execute_rnode(struct rnode_args* args)
{
    return 0;
}