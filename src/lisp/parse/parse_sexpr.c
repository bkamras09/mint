#include <stdio.h>
#include <stdlib.h>
#include "../../shared/shared.h"
#include "parse.h"
#include "parse_define.c"
#include "parse_atom.c"
#include "is_operator.c"

AstNode
*parse_sexpr(node **n, VarMap **map)
{
        if (!n || !*n) return NULL;

        if ((*n)->token_type != LP) {
                fprintf(stderr, "Error: Expected '(' at the beginning of expression\n");
                exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the '(' token
        
        if ((*n)->token_type == DEFINE) { return parse_define(n, map); }
        else if (!(*n) || !is_operator((*n)->token_type)) {
                fprintf(stderr, "Error: Expected operator after '('\n");
                exit(EXIT_FAILURE);
        }

        AstNode *op_node = new_AstNode((*n)->token);
        *n = (*n)->next; // Move past the operator
        op_node->left = parse_atom(n, map); // Parse the first operand

        // Chain additional operands
        AstNode *current_operand = op_node->left;
        while (*n && (*n)->token_type != RP) {
                AstNode *new_operand = parse_atom(n, map);
                current_operand->right = new_operand;
                current_operand = new_operand;
        }

        if (*n && (*n)->token_type == RP) {
                *n = (*n)->next; // Move past the closing ')'
        } else {
                fprintf(stderr, "Error: Expected ')' at the end of expression\n");
                exit(EXIT_FAILURE);
        }

        return op_node;
}
