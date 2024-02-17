#include <stdlib.h>
#include "../../shared/shared.h"
#include "parse.h"

AstNode
*create_AstNode(
        float value,
        AstNode *left,
        AstNode *right,
        ETokenType token_type,
        char *token)
{
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = left;
        n->right = right;
        n->type = token_type;
        n->value = value;
        snprintf(n->token, sizeof(n->token), "%s", token);

        return n;
}