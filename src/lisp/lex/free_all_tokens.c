#include <stdio.h>
#include "../../shared/shared.h"

void
free_all_tokens(node *n)
{
        node *current = n;
        while (current != NULL) {
                current = n->next;
                free(n);
                n = current;
        }
}
