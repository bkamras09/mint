#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "../infix_parse_lex/lex.h"
#include "../shared/shared.c"

node *lex_infix (char *in) {
        remove_spaces(in);
        unsigned int i = 0;
        node *cur = make_node();
        node *head = make_node();
        int inside_unary_op = 1;

        if (!cur || !head) return NULL;

        head = cur;
        head->prev = cur->prev = NULL;

        while(*in){
                node *n = make_node();

                char Variable_id[TOKEN_SIZE_LIMIT];
                unsigned int j = 0;

                if (!n) return NULL;

                ETokenType t = get_ETokenType((*in));

                switch(t){
                case PLUS:
                case MINUS:
                        if (inside_unary_op) {
                                if (t == PLUS) { update_node(cur, n, in, POS); }
                                else { update_node(cur, n, in, NEG); }
                        } else {
                                inside_unary_op = 1;
                                if (t == PLUS) { update_node(cur, n, in, t);
                                } else { update_node(cur, n, in, t); }
                        }
                        in++;
                        break;
                case DIV:
                case MUL:
                case EXP:
                case EQ:
                        inside_unary_op = 1;
                        update_node(cur, n, in, t);
                        in++;
                        break;
                case RP:
                case LP:
                case LB:
                case RB:
                        update_node(cur, n, in, t);
                        in++;
                        break;
                case DIGIT:
                        inside_unary_op = 0;
                        for (i = 0; isdigit(*in) || *in == '.'; i++) cur->token[i] = *in++;
                        cur->token[i] = '\0';
                        cur->token_type = DIGIT;
                        break;
                case ALPHA:
                        while ((isalnum(*in) || *in == '_') && j < TOKEN_SIZE_LIMIT) {  // Variable names can include underscores
                                Variable_id[j++] = *in++;
                        }
                        Variable_id[j] = '\0';
                        strncpy(cur->token, Variable_id, TOKEN_SIZE_LIMIT);
                        cur->token_type = IDENTIFIER;
                break;
                default:
                        printf("Error lexing.  Got: %d\n", *in);
                        in++;
                        break;
                }
                cur = add_node(cur);
        }
        cur->next = NULL;

        return head;
}