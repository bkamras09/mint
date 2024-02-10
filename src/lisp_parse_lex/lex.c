#ifndef LEXER
#define LEXER

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex.h"

/* The lexer is a fairly simple finite discrete automata.
   Whitespace is insignificant in infix notation, but significant in lisp style notation.

   Possible states:
   OPERAND:
   Can be DIGIT or ALPHA.
   DIGIT:
   Accepts any string of digits and a period.  Internal alphas are forbidden and result in error.
   ALPHA:
   Accepts any string of alphabetical characters.  External trailing digits (e.g. num2) are allowed.
   OPERATOR:
   PLUS:
   MINUS:
   DIV:
   MUL:
   EXP*/
node *lex (char *in, VarMap **map) {
        unsigned int i = 0;
        node *cur = make_node();
        node *head = make_node();

        if (!cur || !head) return NULL;

        head = cur;
        head->prev = cur->prev = NULL;

        while(*in){
                while(isspace(*in)) in++;
                node *n = make_node();
                char Variable_id[TOKEN_SIZE_LIMIT];
                unsigned int j = 0;

                if (!n) return NULL;

                ETokenType t = get_ETokenType((*in));

                switch(t){
                case PLUS:
                case MINUS:
                case DIV:
                case MUL:
                case EXP:
                case EQ:
                case RP:
                case LP:
                case LB:
                case RB:
                        update_node(cur, n, in++, t); break;
                case DIGIT:
                        for (i = 0; isdigit(*in) || *in == '.'; i++) cur->token[i] = *in++;
                        cur->token[i] = '\0';
                        cur->token_type = DIGIT;
                        break;
                case ALPHA:
                        if (str_cmp(in, "let")) {
                                get_token(cur, n, in, LET, 3);
                                in += 3;
                        } else if (str_cmp(in, "define")) {
                                get_token(cur, n, in, DEFINE, 6);
                                in += 6;
                        } else {
                                while ((isalnum(*in) || *in == '_' || *in == '-') && j < TOKEN_SIZE_LIMIT) {
                                        Variable_id[j++] = *in++;
                                }
                                Variable_id[j] = '\0';
                                strncpy(
                                        cur->token,
                                        Variable_id,
                                        TOKEN_SIZE_LIMIT);
                                cur->token_type = IDENTIFIER;
                        }
                break;
                default:
                        printf("Error lexing.  Got: %d\n", *in++); break;
                }
                cur = add_node(cur);
        }
        cur->next = NULL;

        return head;
}
#endif