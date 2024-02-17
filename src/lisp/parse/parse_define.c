#include "parse.h"
#include <errno.h>

AstNode *parse_define(node **cursor, VarMap **map) {
    if ((*cursor)->token_type != DEFINE) {
        fprintf(stderr, "Error: Expected 'define' keyword\n");
        exit(EXIT_FAILURE);
    }
    *cursor = (*cursor)->next; // Move past the 'define' token

    // Expecting variable name
    if ((*cursor)->token_type != IDENTIFIER) {
        fprintf(stderr, "Error: Expected variable name in 'define' expression\n");
        exit(EXIT_FAILURE);
    }

    char var_name[64];
    strncpy(var_name, (*cursor)->token, sizeof(var_name) - 1);
    var_name[sizeof(var_name) - 1] = '\0';
    *cursor = (*cursor)->next;

    AstNode *value_expr = NULL;

    if ((*cursor)->token_type == LP) {
        value_expr = parse_sexpr(cursor, map);
    } else if ((*cursor)->token_type == DIGIT) {
        value_expr = new_AstNode_operand(atof((*cursor)->token));
        *cursor = (*cursor)->next;
    } else if ((*cursor)->token_type == IDENTIFIER) {
        value_expr = new_AstNode((*cursor)->token);
        value_expr->type = IDENTIFIER;
        *cursor = (*cursor)->next;
    } else {
        fprintf(stderr, "Error: Expected value expression in 'define' expression\n");
        exit(EXIT_FAILURE);
    }

    if (!value_expr) {
        fprintf(stderr, "Error: Failed to parse value expression in 'define'\n");
        exit(EXIT_FAILURE);
    }

    // Update the variable in the map with the new value after evaluation
    // This should happen after valueExpr is evaluated to get the actual value
    float value = visit(value_expr, map); // Ensure this function returns the evaluated value
    update_var_map(map, var_name, value);

    // Since 'define' does not evaluate to a value in traditional lisp,
    // I just return the node here and don't give it any children.
    // When this node is visited, it simply returns 0.
    // In the future, the visit function will return nodes instead of floats,
    // and another function will decide how to interpret the node's payload
    // e.g., a define will return an empty string since it's purpose is to
    // alter memory rather than evaluate soemthing.
    AstNode *define_node = (AstNode *)malloc(sizeof(AstNode));
    if (!define_node) {
        fprintf(stderr, "Memory allocation failed for 'define' node\n");
        exit(EXIT_FAILURE);
    }
    define_node->type = DEFINE;
    strncpy(define_node->token, "def", sizeof(define_node->token) - 1);
    define_node->left = NULL; // No left child
    define_node->right = NULL; // No right child

    return define_node;
}
