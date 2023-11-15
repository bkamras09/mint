#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "expr.h"
#include <errno.h>
#include "math.h"

AstNode *new_AstNode(char p[]) {
	AstNode *n = malloc(sizeof(AstNode));
	if (!n) return NULL;
	n->left = n->right = NULL;
	ETokenType t = get_ETokenType(*p);

    switch(t) {
        case DIGIT:
            strcpy(n->token, get_digits(p));
            p += strlen(n->token);
            n->value = atof(n->token);
            break;
        case ALPHA:
            strcpy(n->token, get_alphas(p));
            break;
        case MINUS: case PLUS: case DIV: case MUL: case EXP: case RP:
        case LP: case LB: case RB: case ERROR: case END_OF_FILE:
            strncpy(n->token, p, 1);
            break;
        default:
            printf("Error while building node.  Expected a valid token type but got: %s\n", type_as_string(t));
            break;
    }

	n->type = t;
    n->current_token = p;

	return n;
}

float factor(node **n) {
    if ((*n)->token_type == DIGIT) {
        float result = atof((*n)->token);
        *n = (*n)->next;
        return result;
    } else if ((*n)->token_type == LP) {
        *n = (*n)->next;
        float result = expr(n);
        if ((*n)->token_type != RP) {
            fprintf(stderr, "Error: Expected right parenthesis\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next;
        return result;
    } else {
        fprintf(stderr, "Error: Expected number or left parenthesis\n");
        exit(EXIT_FAILURE);
    }
}

float term(node **n) {
    float result = exponent(n);  // Handles exponentiation

    while (*n != NULL && ((*n)->token_type == MUL || (*n)->token_type == DIV)) {
        ETokenType op = (*n)->token_type;
        *n = (*n)->next;
        float rhs = exponent(n);  // Should call exponent, not factor
        if (op == MUL) { result *= rhs; }
        else { result /= rhs; }
    }
    return result;
}

float expr(node **n) {
    float result = term(n);  // Handles multiplication and division

    while (*n != NULL && ((*n)->token_type == PLUS || (*n)->token_type == MINUS)) {
        ETokenType op = (*n)->token_type;
        *n = (*n)->next;
        float rhs = term(n);  // Should call term, not exponent
        if (op == PLUS) { result += rhs; }
        else { result -= rhs; }
    }
    return result;
}

float exponent(node **n) {
    float base = factor(n);  // Handles the base of the exponentiation

    while (*n != NULL && (*n)->token_type == EXP) {
        *n = (*n)->next;
        float exp = factor(n);  // Handles the exponent, not exponent(n)
        base = powf(base, exp);
    }

    return base;
}

