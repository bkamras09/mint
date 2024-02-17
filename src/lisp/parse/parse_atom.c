#include <errno.h>
#include "parse.h"
#include "math.h"
#include "new_AstNode.c"
#include "new_AstNode_operand.c"
#include "../../shared/shared.c"

AstNode *parse_atom(node **n, VarMap **map) {
        AstNode *operand = NULL;

        switch((*n)->token_type) {
        case LP:
                return parse_sexpr(n, map);
        case DIGIT:
                operand = new_AstNode_operand(atof((*n)->token));
                *n = (*n)->next; // Move to the next token
                return operand;
        case IDENTIFIER:
                operand = new_AstNode((*n)->token);
                operand->type = (*n)->token_type;
                operand->value =  lookup_var(map, operand->token);
                *n = (*n)->next; // Move to the next token
                return operand;
        default:
                fprintf(stderr, "Error: Unexpected token type in factor\n");
                exit(EXIT_FAILURE);
        }
}
