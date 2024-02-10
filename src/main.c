/* OVERVIEW: a baby programming language. */

#include "lisp_parse_lex/expr.c"
#include "io/io.c"
#include "shared/shared.c"
#include "lisp_parse_lex/lex.c"
#include <math.h>

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
        char *current_token;
        print_greeting();
        VarMap *global_symbol_table = NULL; // Global symbol table for variable storage
        add_default_vars(&global_symbol_table);

        for (;;) {
                printf("> ");
                current_token = get_line(input);
                node *head = lex(input, &global_symbol_table);
                AstNode *result = parse_sexpr(&head, &global_symbol_table);
                float computed_result = visit(result, &global_symbol_table);
                printf("%.6f\n", computed_result);
                free(result);
                free_all_tokens(head);
        }

        printf("bye\n");

        return 0;
}