/* OVERVIEW: a baby programming language. */

#include "lisp_parse_lex/expr.c"
#include "io/io.c"
#include "shared/shared.c"
#include "lisp_parse_lex/lex.c"
#include <math.h>

// This is an X macro for the token types.
// Eventually it will be used to dynamically generate the token type enum and the string representation of the token types.
// However, X macros are a pain in the a$$ to get to work, so I'm leaving it for now.
/*
#define X(name) name,
typedef enum {
    #include "shared/ETokenType_def.h"
} ETokenType;
#undef X


char *TYPE_AS_STRING(ETokenType t) {
    switch (t) {
        #define X(name) case name: return #name;
        #include "ETokenType_def.h"
        #undef X
        default:
            printf("Error getting token type. Got: %d\n", t);
            return "ERROR";
    }
}
*/

int main(void) {
        char input[INPUT_SIZE_LIMIT];
        print_greeting();
        VarMap *global_symbol_table = NULL;
        add_default_vars(&global_symbol_table);

        for (;;) {
                printf("> ");
                node *head = lex(get_line(input));
                AstNode *result = parse_sexpr(&head, &global_symbol_table);
                float computed_result = visit(result, &global_symbol_table);
                printf("%.6f\n", computed_result);
                free(result);
                free_all_tokens(head);
        }

        printf("bye\n");

        return 0;
}
