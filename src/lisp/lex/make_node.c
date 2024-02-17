#include <stdio.h>
#include "../../shared/shared.h"

node
*make_node(void)
{
        node *n = calloc(1, (sizeof(node)));
        if (!n) return NULL;
        n->prev = n->next = NULL;
        return n;
}
