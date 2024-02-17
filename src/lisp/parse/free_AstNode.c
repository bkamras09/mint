#include <stdio.h>
#include "shared.h"

void
free_AstNode(AstNode *n)
{
        if (!n) return;
        free_AstNode(n->left);
        free_AstNode(n->right);
        free(n);
}