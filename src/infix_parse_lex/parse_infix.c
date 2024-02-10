#include "expr.h"
#include "math.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Arbitrarily decided to put the factor in the right-hand subtree.
AstNode *new_AstNode_unary(ETokenType type, AstNode *right) {
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = NULL;
        n->right = right;
        n->type = type;
        snprintf(n->token, sizeof(n->token), "%c", type);

        return n;
}

AstNode *new_AstNode_Operator(ETokenType type, AstNode *left, AstNode *right) {
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = left;
        n->right = right;
        n->type = type;
        snprintf(n->token, sizeof(n->token), "%c",
                 type); // Assuming type is a char representing the operator
        return n;
}

AstNode *new_AstNode_Operand(float value) {
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = n->right = NULL;
        n->type = DIGIT;
        n->value = value;
        snprintf(n->token, sizeof(n->token), "%f", value);
        return n;
}

AstNode *unary_op_AstNode(node **n) {
        AstNode *result = factor_AstNode(n);

        while (*n != NULL && ((*n)->token_type == NEG || (*n)->token_type == POS)) {
                ETokenType t = (*n)->token_type; 
                *n = (*n)->next; // move to the operand
                AstNode *unary_operand = unary_op_AstNode(n);
                result = new_AstNode_unary(t, unary_operand);
        }

        return result;
}

AstNode *exponent_AstNode(node **n) {
        AstNode *base = unary_op_AstNode(n);

        while (*n != NULL && (*n)->token_type == EXP) {
                *n = (*n)->next; // Move to the exponent
                AstNode *exp = unary_op_AstNode(n); // Recursively build the AST for the exponent
                base = new_AstNode_Operator(EXP, base, exp);
        }

        return base;
}

AstNode *term_AstNode(node **n) {
        AstNode *result = exponent_AstNode(n);

        while (*n != NULL && ((*n)->token_type == MUL || (*n)->token_type == DIV)) {
                ETokenType opType = (*n)->token_type;
                *n = (*n)->next;                    // Move to the next token
                AstNode *rhs = exponent_AstNode(n); // Build the AST for the right-hand side
                result = new_AstNode_Operator(opType, result, rhs);
        }

        return result;
}

AstNode *factor_AstNode(node **n) {
        if ((*n)->token_type == DIGIT) {
                float value = atof((*n)->token);
                AstNode *operand = new_AstNode_Operand(value);
                *n = (*n)->next; // Move to the next token
                return operand;
        } else if ((*n)->token_type == IDENTIFIER) {
                AstNode *operand = new_AstNode("0");
                strcpy(operand->token, (*n)->token);
                return operand;
        } else if ((*n)->token_type == LP) {
                *n = (*n)->next; // Move past the '(' token
                AstNode *subExpr = expr_AstNode(n); // Recursively build the AST for the sub-expression
                if ((*n)->token_type != RP) {
                fprintf(stderr, "Error: Expected right parenthesis\n");
                exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the ')' token
        return subExpr;
        }
        /* else {
        ... Handle variables and other types of factors
        }*/
        return NULL;
}

AstNode *expr_AstNode(node **n) {
        AstNode *result = term_AstNode(n);
        while (*n != NULL && ((*n)->token_type == PLUS || (*n)->token_type == MINUS)) {
                ETokenType opType = (*n)->token_type;
                *n = (*n)->next;                // Move to the next token
                AstNode *rhs = term_AstNode(n); // Build the AST for the right-hand side
                result = new_AstNode_Operator(opType, result, rhs);
        }
        return result;
}

float visit(AstNode *n) {
        if (n != NULL) {
                if (n->left == NULL && n->right == NULL) {
                        return n->value;
                } else {
                        float left_result = visit(n->left);
                        float right_result = visit(n->right);

                        switch (n->type) {
                        case PLUS:
                                return left_result + right_result;
                        case MINUS:
                                return left_result - right_result;
                        case MUL:
                                return left_result * right_result;
                        case DIV:
                                if (right_result == 0.0) {
                                        fprintf(stderr, "Error: Division by zero\n");
                                        exit(EXIT_FAILURE);
                                }

                                return left_result / right_result;
                        case EXP:
                                return powf(left_result, right_result);
                        case NEG:
                                return right_result * -1;
                        case POS:
                                return right_result;
                        case EQ:
                                left_result = right_result;
                                return left_result;
                        default:
                                fprintf(stderr, "Error: Unexpected operator.  Got: %d, which is %s\n", n->type, type_as_string(n->type));
                                exit(EXIT_FAILURE);
                        }
                }
        }
        return 0.0; // Handle empty tree
}

// Expression parsing function for LISP style expressions.  Didn't work, but keeping it here just in case.
/*
AstNode *expr_AstNode(node **n) {
    if (!n || !*n) return NULL;

    // Expecting an open parenthesis or an operand at the beginning of an expression
    if ((*n)->token_type != LP) {
        fprintf(stderr, "Error: Expected '(' at the beginning of expression\n");
        exit(EXIT_FAILURE);
    }

    *n = (*n)->next; // Move past the '(' token

    // Expecting an operator after '('
    if (!(*n) || ((*n)->token_type != PLUS && (*n)->token_type != MINUS &&
                  (*n)->token_type != MUL && (*n)->token_type != DIV && 
                  (*n)->token_type != EXP)) {
        fprintf(stderr, "Error: Expected operator after '('\n");
        exit(EXIT_FAILURE);
    }

    AstNode *opNode = new_AstNode((*n)->token);
    *n = (*n)->next; // Move past the operator

    // Parse operands
    while (*n && (*n)->token_type != RP) {
        AstNode *operand = factor_AstNode(n);
        if (!opNode->left) opNode->left = operand;
        else if (!opNode->right) opNode->right = operand;
        else {
            // Chain additional operands to the right
            AstNode *currentRight = opNode->right;
            while (currentRight->right) currentRight = currentRight->right;
            currentRight->right = operand;
        }
    }

    if (*n && (*n)->token_type == RP) {
        *n = (*n)->next; // Move past the closing ')'
    } else {
        fprintf(stderr, "Error: Expected ')' at the end of expression\n");
        exit(EXIT_FAILURE);
    }

    return opNode;
}
*/