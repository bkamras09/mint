#include <stdio.h>
#include "shared.h"

AstNode *expr_AstNode(node **n, VarMap *map) {
    printf("parsing expr\n");
    if (!n || !*n) return NULL;

    if ((*n)->token_type != LP) {
        fprintf(stderr, "Error: Expected '(' at the beginning of expression\n");
        exit(EXIT_FAILURE);
    }
    *n = (*n)->next; // Move past the '(' token

    AstNode *opNode = NULL;
    if ((*n)->token_type == LET) {
        // Handle LET
        *n = (*n)->next; // Move past the 'LET' token

        if ((*n)->token_type != LP) {
            fprintf(stderr, "Error: Expected '(' after 'let'\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the '(' for variable binding

        // Assuming the next token is the variable name
        char varName[64]; // Assuming variable names are no longer than 63 characters
        if ((*n)->token_type == ALPHA) {
            strncpy(varName, (*n)->token, sizeof(varName) - 1);
            varName[sizeof(varName) - 1] = '\0'; // Ensure null-termination
            *n = (*n)->next; // Move past the variable name
        } else {
            fprintf(stderr, "Error: Expected variable name in 'let' expression\n");
            exit(EXIT_FAILURE);
        }

        // The next part is the value expression
        AstNode *valueExpr = expr_AstNode(n, map); // Recursively parse the value expression

        if ((*n)->token_type != RP) {
            fprintf(stderr, "Error: Expected ')' after 'let' value expression\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the ')'

        // Now, parse the body of the let expression
        AstNode *bodyExpr = expr_AstNode(n, map);

        // Create a 'LET' node
        opNode = malloc(sizeof(AstNode));
        if (!opNode) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        opNode->type = LET;
        strncpy(opNode->token, "let", sizeof(opNode->token) - 1);
        opNode->left = malloc(sizeof(AstNode)); // Create a node for variable binding
        if (!opNode->left) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        opNode->left->type = ALPHA;
        strncpy(opNode->left->token, varName, sizeof(opNode->left->token) - 1);
        opNode->left->left = valueExpr; // Attach the value expression to the variable node
        opNode->left->right = NULL;
        opNode->right = bodyExpr; // The body of the let expression
    } else {
        // Handle other expressions as before
    }

    // Ensure to close the expression with a ')' check as before

    return opNode;
}