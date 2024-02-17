#include <stdio.h>
#include "../../shared/shared.h"

void
print_all_tokens(node *n)
{
        while (n->next != NULL) {
                printf("token: %s\n", n->token);
                n = n->next;
        }
}