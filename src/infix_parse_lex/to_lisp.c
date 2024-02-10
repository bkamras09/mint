#include "expr.h"
#include "math.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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