#include <stdio.h>
#include "lex.h"

void update_node(node *c, node *n, char *i, ETokenType t) {
        int j = 0;
        while (isalpha(*i)) { j++; i++; }
        i++; j++;
        i -= j; // walk back to the beginning of the token
        strncpy(c->token, i, j);
        c->token[j] = '\0';
        c->token_type = t;
        c->next = n;
        n->prev = c;
        c = n;
}
