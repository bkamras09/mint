#include <stdio.h>
#include "lex.h"

void
get_token(node *c, node *n, char *i, ETokenType t, unsigned int length)
{
        strncpy(c->token, i, length);
        c->token_type = t;
        c->next = n;
        n->prev = c;
        c = n;
}
