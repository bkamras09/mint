// OVERVIEW: this code evaluates an expression directly without building an AST.

#include "shared.h"
#include "expr.h"
#include "math.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
				float result = exponent(n);

				while (*n != NULL && ((*n)->token_type == MUL || (*n)->token_type == DIV)) {
								ETokenType op = (*n)->token_type;
								*n = (*n)->next;
								float rhs = exponent(n);
								if (op == MUL) { result *= rhs; }
								else { result /= rhs; }
				}

				return result;
}

float expr(node **n) {
				float result = term(n);

				while (*n != NULL && ((*n)->token_type == PLUS || (*n)->token_type == MINUS)) {
								ETokenType op = (*n)->token_type;
								*n = (*n)->next;
								float rhs = term(n);
								if (op == PLUS) { result += rhs; }
								else { result -= rhs; }
								//return result;
				}

				return result;
}

float exponent(node **n) {
				float base = factor(n);

				while (*n != NULL && (*n)->token_type == EXP){
								*n = (*n)->next;
								float exp = factor(n);
								base = powf(base, exp);
				}

				return base;
}