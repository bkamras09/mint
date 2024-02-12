/*
 * OVERVIEW: the logic for the parser.
 * It's just recursive descent of the sequence:
 *
 *    expr -> term -> exponent -> factor -> expr (for infix expressions)
 * 
 *    expr -> factor -> expr(for lisp style expressions)
 */

#include "expr.h"
#include "math.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared/shared.c"

float visit(AstNode *n, VarMap **map) {
        if (!n) { return 0.0; }

        if (n->type == DIGIT) { return n->value; }
        if (n->type == IDENTIFIER) {return lookupVar(map, n->token); }

        float result = visit(n->left, map);

        if (n->type == DEFINE) { return 0.0; } // DEFINE doesn't evaluate to a value

        AstNode *rightOperand = n->left->right;

        while (rightOperand) {
                float rightValue = visit(rightOperand, map);
                switch (n->type) {
                case PLUS:  result += rightValue; break;
                case MINUS: result -= rightValue; break;
                case MUL:   result *= rightValue; break;
                case DIV:
                        if (rightValue == 0.0) {
                                fprintf(stderr, "Error: Division by zero\n");
                                exit(EXIT_FAILURE);
                        }
                        result /= rightValue;
                        break;
                case EXP:   result = powf(result, rightValue); break;
                default:    fprintf(stderr, "Unknown operator: %s\n", n->token); exit(EXIT_FAILURE);
                }
                rightOperand = rightOperand->right;
        }

        return result;
}

AstNode *parse_define_expr(node **cursor, VarMap **map) {
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

    char varName[64];
    strncpy(varName, (*cursor)->token, sizeof(varName) - 1);
    varName[sizeof(varName) - 1] = '\0';
    *cursor = (*cursor)->next;

    AstNode *valueExpr = NULL;

    if ((*cursor)->token_type == LP) {
        valueExpr = parse_sexpr(cursor, map);
    } else if ((*cursor)->token_type == DIGIT) {
        valueExpr = new_AstNode_Operand(atof((*cursor)->token));
        *cursor = (*cursor)->next;
    } else if ((*cursor)->token_type == IDENTIFIER) {
        valueExpr = new_AstNode((*cursor)->token);
        valueExpr->type = IDENTIFIER;
        *cursor = (*cursor)->next;
    } else {
        fprintf(stderr, "Error: Expected value expression in 'define' expression\n");
        exit(EXIT_FAILURE);
    }

    if (!valueExpr) {
        fprintf(stderr, "Error: Failed to parse value expression in 'define'\n");
        exit(EXIT_FAILURE);
    }

    // Update the variable in the map with the new value after evaluation
    // This should happen after valueExpr is evaluated to get the actual value
    float value = visit(valueExpr, map); // Ensure this function returns the evaluated value
    updateVarMap(map, varName, value);

    // Since 'define' does not evaluate to a value in traditional isp,
    // I just return the node here and don't give it any children.
    // When this node is visited, it simply returns 0.
    // In the future, the visit function will return nodes instead of floats,
    // and another function will decide how to interpret the node's payload
    // e.g., a define will return an empty string since it's purpose is to
    // alter memory rather than evaluate soemthing.
    AstNode *defineNode = (AstNode *)malloc(sizeof(AstNode));
    if (!defineNode) {
        fprintf(stderr, "Memory allocation failed for 'define' node\n");
        exit(EXIT_FAILURE);
    }
    defineNode->type = DEFINE;
    strncpy(defineNode->token, "def", sizeof(defineNode->token) - 1);
    defineNode->left = NULL; // No left child
    defineNode->right = NULL; // No right child

    return defineNode;
}

AstNode *parse_sexpr(node **n, VarMap **map) {
        if (!n || !*n) return NULL;

        if ((*n)->token_type != LP) {
                fprintf(stderr, "Error: Expected '(' at the beginning of expression\n");
                exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the '(' token
        
        if ((*n)->token_type == DEFINE) { return parse_define_expr(n, map); }
        else if (!(*n) || !isOperator((*n)->token_type)) {
                fprintf(stderr, "Error: Expected operator after '('\n");
                exit(EXIT_FAILURE);
        }

        AstNode *opNode = new_AstNode((*n)->token);
        *n = (*n)->next; // Move past the operator
        opNode->left = parse_atom(n, map); // Parse the first operand

        // Chain additional operands
        AstNode *currentOperand = opNode->left;
        while (*n && (*n)->token_type != RP) {
                AstNode *newOperand = parse_atom(n, map);
                currentOperand->right = newOperand;
                currentOperand = newOperand;
        }

        if (*n && (*n)->token_type == RP) {
                *n = (*n)->next; // Move past the closing ')'
        } else {
                fprintf(stderr, "Error: Expected ')' at the end of expression\n");
                exit(EXIT_FAILURE);
        }

        return opNode;
}

AstNode *parse_atom(node **n, VarMap **map) {
        if ((*n)->token_type == LP) {
                return parse_sexpr(n, map);
        } else if ((*n)->token_type == DIGIT) {
                AstNode *operand = new_AstNode_Operand(atof((*n)->token));
                *n = (*n)->next; // Move to the next token
                return operand;
        } else if ((*n)->token_type == IDENTIFIER) {
                AstNode *operand = new_AstNode((*n)->token);
                operand->type = (*n)->token_type;
                operand->value =  lookupVar(map, operand->token);
                *n = (*n)->next; // Move to the next token
                return operand;
        } else {
                fprintf(stderr, "Error: Unexpected token type in factor\n");
                exit(EXIT_FAILURE);
        }
}
