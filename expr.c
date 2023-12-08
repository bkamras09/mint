#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "expr.h"
#include <errno.h>
#include "math.h"


AstNode *expr_AstNode(node **n);
AstNode *term_AstNode(node **n);
AstNode *factor_AstNode(node **n);
AstNode *exponent_AstNode(node **n);

AstNode *new_AstNode_Operand(float value) {
    AstNode *n = malloc(sizeof(AstNode));
    if (!n) return NULL;
    n->left = n->right = NULL;
    n->type = DIGIT;
    n->value = value;
    snprintf(n->token, sizeof(n->token), "%f", value);
    return n;
}

AstNode *new_AstNode_Operator(ETokenType type, AstNode *left, AstNode *right) {
    AstNode *n = malloc(sizeof(AstNode));
    if (!n) return NULL;
    n->left = left;
    n->right = right;
    n->type = type;
    // Token representation for operators is typically a single character
    snprintf(n->token, sizeof(n->token), "%c", type); // Assuming type is a char representing the operator
    return n;
}

AstNode *factor_AstNode(node **n) {
    if ((*n)->token_type == DIGIT) {
        float value = atof((*n)->token);
        AstNode *operand = new_AstNode_Operand(value);
        *n = (*n)->next;  // Move to the next token
        return operand;
    } else if ((*n)->token_type == LP) {
        *n = (*n)->next;  // Move past the '(' token
        AstNode *subExpr = expr_AstNode(n);  // Recursively build the AST for the sub-expression
        if ((*n)->token_type != RP) {
            fprintf(stderr, "Error: Expected right parenthesis\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next;  // Move past the ')' token
        return subExpr;
    } else {
        // ... Handle variables and other types of factors
    }
}

AstNode *exponent_AstNode(node **n) {
    AstNode *base = factor_AstNode(n);
    while (*n != NULL && (*n)->token_type == EXP) {
        *n = (*n)->next;  // Move to the exponent
        AstNode *exp = exponent_AstNode(n);  // Recursively build the AST for the exponent
        base = new_AstNode_Operator(EXP, base, exp);
    }
    return base;
}

AstNode *term_AstNode(node **n) {
    AstNode *result = exponent_AstNode(n);
    while (*n != NULL && ((*n)->token_type == MUL || (*n)->token_type == DIV)) {
        ETokenType opType = (*n)->token_type;
        *n = (*n)->next;  // Move to the next token
        AstNode *rhs = exponent_AstNode(n);  // Build the AST for the right-hand side
        result = new_AstNode_Operator(opType, result, rhs);
    }
    return result;
}

AstNode *expr_AstNode(node **n) {
    AstNode *result = term_AstNode(n);
    while (*n != NULL && ((*n)->token_type == PLUS || (*n)->token_type == MINUS)) {
        ETokenType opType = (*n)->token_type;
        *n = (*n)->next;  // Move to the next token
        AstNode *rhs = term_AstNode(n);  // Build the AST for the right-hand side
        result = new_AstNode_Operator(opType, result, rhs);
    }
    return result;
}

































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
    }
    return result;
}

float exponent(node **n) {
    float base = factor(n);

    while (*n != NULL && (*n)->token_type == EXP) {
        *n = (*n)->next;
        float exp = factor(n);
        base = powf(base, exp);
    }

    return base;
}


/* code some weirdo on 4chan suggested
op = lex_operator();
end = 0;
while (!end) {
    while (op && is_prefix(op)) {
        push(opstack, op);
        op = lex_operator();
    }

    if (term = parse_term()) {
        push(termstack, term);
    }
    else { return NULL; }

    op = lex_operator();
    while (op && is_postfix(op)) {
        reduce();
        push(opstack, op);
        op = lex_operator();
    }

    op = lex_operator();
    if(op && is_infix(op)) {
        reduce();
        push(opstack, op);
        op = lex_operator();
    }
    else {
        end = 1;
    }
}
while (opstack) {
    reduce();
}

return pop(termstack);


op = lex_operator();
while (op && is_postfix(op)) {
    reduce_while_we_can(); # reduce() previously seen prefix and infix operators that have a higher precedence or same precedence and left
    push(opstack, op);
    reduce(op); # pop operator from opstack, pop operands (depending on arity of op) from termstack, make an AST node and push it on termstack
    op = lex_operator();
}

if (op && is_infix(op)) {
    reduce_while_we_can(); # calls reduce() inside
    push(opstack, op);
}
else {
    end = 1;
}
# + put the lex_operator() inside the while (!end) loop, at the beginning
*/




