#include <stdlib.h>

#include "../include/pnode.h"

void free_pnode_args(struct pnode_args **pnode_args)
{
    free(*pnode_args);
    *pnode_args = NULL;
}
