/* OVERVIEW: a baby programming language. */

#include "expr.c"
#include "io.c"
#include "shared.c"
#include "lex.c"
#include <math.h>

int main(void) {
        char input[INPUT_SIZE_LIMIT];
        char *current_token = input;
        print_greeting();

        for (;;) {
                printf("> ");
                current_token = get_line(input);
                node *head = lex(input);
                if (head->token_type == EXIT) {
                        printf("exit\n");
                        break;
                }


                // Used to test the validity of the list passed to the AST builder.
                /*node *n = head;

                while (n->next) {
                        printf("token: %s\top: %s\n",
                               n->token,
                               type_as_string(n->token_type));
                        n = n->next;
                }
                */


                AstNode *result = expr_AstNode(&head);
                float computed_result = visit(result);

                printf("%.6f\n", computed_result);

                //AstNode *r = to_prefix(result);
                //printf("\n");
                while (head != NULL) {
                        head = head->next;
                        free(head);
                }
        }

        printf("bye\n");

        return 0;
}

