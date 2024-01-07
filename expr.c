/*
 * OVERVIEW: the logic for the parser.
 * It's just recursive descent of the sequence:
 *
 *    expr -> term -> exponent -> factor -> expr 
 */

#include "expr.h"
#include "math.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstNode *new_AstNode(char p[]) { 
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = n->right = NULL;
        ETokenType t = get_ETokenType(*p);

        switch (t) {
        case DIGIT:
                strcpy(n->token, get_digits(p));
                p += strlen(n->token);
                n->value = atof(n->token);
                break;
        case ALPHA:
                strcpy(n->token, get_alphas(p));
                break;
        case POS:
        case NEG:
        case MINUS:
        case PLUS:
        case DIV:
        case MUL:
        case EXP:
        case RP:
        case LP:
        case LB:
        case RB:
        case ERROR:
        case END_OF_FILE:
                strncpy(n->token, p, 1);
                break;
        default:
                printf("Error while building node.  \
                    Expected a valid token type but got: \
                    %s\n",
                       type_as_string(t));
                break;
        }

        n->type = t;
        n->current_token = p;

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

AstNode *factor_AstNode(node **n) {
				if ((*n)->token_type == DIGIT) {
								float value = atof((*n)->token);
								AstNode *operand = new_AstNode_Operand(value);
								*n = (*n)->next; // Move to the next token
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
			// This is a leaf node (operand)
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
			default:
				fprintf(stderr, "Error: Unexpected operator.  Got: %d, which is %s\n", n->type, type_as_string(n->type));
				exit(EXIT_FAILURE);
			}
		}
	}
	return 0.0; // Handle empty tree
}

AstNode *infix_to_lisp(AstNode *n) {
	if (n != NULL) {
		if (n->left == NULL && n->right == NULL) {
			return n;
		} else {
			printf(" (");
			char op;

			switch (n->type) {
			case PLUS:
			case POS:
				op = '+';
				break;
			case MINUS:
			case NEG:
				op = '-';
				break;
			case MUL:
				op = '*';
				break;
			case DIV:
				op = '/';
				break;
			case EXP:
				op = '^';
				break;
			default:
				op = ' ';
				printf("Error: unknown operator: %s\n", type_as_string(n->type));
				break;
			}

			printf(" %c", op);

			AstNode *left_result = infix_to_lisp(n->left);
			if (left_result->left == NULL && left_result->right == NULL) { printf(" %f", left_result->value); }
			AstNode *right_result = infix_to_lisp(n->right);
			if (right_result->left == NULL && right_result->right == NULL) { printf(" %f", right_result->value); }
			printf(" )");

			return n;
		}
	}
	return new_AstNode("0");
}