#include <stdio.h>
#include "../../shared/shared.h"

AstNode
*new_AstNode_operand(float value)
{
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = n->right = NULL;
        n->type = DIGIT;
        n->value = value;
        snprintf(n->token, sizeof(n->token), "%f", value);
        return n;
}
