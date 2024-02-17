#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parse.h"
#include "../../shared/shared.h"

float
visit(AstNode *n, VarMap **map)
{
        if (!n) { return 0.0; }

        if (n->type == DIGIT) return n->value;
        if (n->type == IDENTIFIER) return lookup_var(map, n->token);

        float result = visit(n->left, map);

        // DEFINE doesn't evaluate to a value.  Return 0.
        // Look into returning an empty node or NULL union when I eventually support types.
        if (n->type == DEFINE) { return 0.0; }

        AstNode *right_operand= n->left->right;

        while (right_operand) {
                float right_value = visit(right_operand, map);
                switch (n->type) {
                case PLUS:  result += right_value; break;
                case MINUS: result -= right_value; break;
                case MUL:   result *= right_value; break;
                case DIV:
                        if (right_value == 0.0) {
                                fprintf(stderr, "Error: Division by zero\n");
                                exit(EXIT_FAILURE);
                        }
                        result /= right_value;
                        break;
                case EXP:   result = powf(result, right_value); break;
                default:    fprintf(stderr, "Unknown operator: %s\n", n->token); exit(EXIT_FAILURE);
                }
                right_operand = right_operand->right;
        }

        return result;
}
