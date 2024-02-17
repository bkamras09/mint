#include <stdio.h>
#include "../lex/get_digits.c"
#include "../../shared/type_as_string.c"
#include "../../shared/shared.h"
#include "parse.h"

AstNode
*new_AstNode(char p[])
{ 
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
        case POS: case NEG: case MINUS: case PLUS: case DIV: case MUL:   case EXP: case ALPHA:
        case RP:  case LP:  case LB:    case RB:   case EQ:  case ERROR: case END_OF_FILE: case IDENTIFIER:
                strncpy(n->token, p, strlen(p));
                break;
        case LET:
            strncpy(n->token, "let", 3);
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

