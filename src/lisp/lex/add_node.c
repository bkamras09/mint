#include <stdio.h>
#include "lex.h"

node *add_node(node *n) {
        node *new_node = make_node();
        n->next = new_node;
        new_node->prev = n;
        return new_node;
}
