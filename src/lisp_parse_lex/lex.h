#ifndef LEXER_HEADER
#define LEXER_HEADER
#include "../shared/shared.c"

// DECLARATIONS
node *make_node();
node *add_node(node *n);
node *lex (char *in, VarMap **map);

void update_node(node *c, node *n, char *i, ETokenType t);
void remove_spaces(char* s);
void print_list(node *n);
void get_token(node *c, node *n, char *i, ETokenType t, unsigned int length);
void free_all_tokens(node *n);

char *get_digits(char *p);
char *get_alphas(char *p);

bool str_cmp(char a[], char b[]);

// DEFINITIONS
char *get_digits(char *p) {
        unsigned int i;
        char result[TOKEN_SIZE_LIMIT];
        char *r = result;
        for (i = 0; (isdigit(*p) || *p == '.') && i<TOKEN_SIZE_LIMIT; i++) {
                result[i] = *p++;
        }
        result[i] = '\0';

        return r;
}
char *get_alphas(char *p) {
        unsigned int i = 0;
        char result[TOKEN_SIZE_LIMIT];
        char *r = result;
        while (isalpha(*p)) {
                result[i] = *p++;
        }
        result[i] = '\0';

        return r;
}
node *make_node() {
        node *n = calloc(1, (sizeof(node)));
        if (!n) return NULL;
        n->prev = n->next = NULL;
        return n;
}
node *add_node(node *n) {
        node *new_node = make_node();
        n->next = new_node;
        new_node->prev = n;
        return new_node;
}
void update_node(node *c, node *n, char *i, ETokenType t) {
        int j = 0;
        while (isalpha(*i)) { j++; i++; }
        i++; j++;
        i -= j; // walk back to the beginning of the token
        strncpy(c->token, i, j);
        c->token[j] = '\0';
        c->token_type = t;
        c->next = n;
        n->prev = c;
        c = n;
}
void get_token(node *c, node *n, char *i, ETokenType t, unsigned int length) {
        strncpy(c->token, i, length);
        c->token_type = t;
        c->next = n;
        n->prev = c;
        c = n;
}
void lex_keyword(char *keyword) {

}
void remove_spaces(char* s) {
        char *d = s;
        do { while (*d == ' ') { ++d;}
        } while ((*s++ = *d++));
}
bool str_cmp(char a[], char b[]) {
        while (*a != '\0' && *b != '\0') {
                if (*a++ != *b++) { return false; }
        }

        return true;
}
void print_list(node *n) {
        while (n->next != NULL) {
                printf("token: %s\n", n->token);
                n = n->next;
        }
}
void free_all_tokens(node *n) {
        node *current = n;
        while (current != NULL) {
                current = n->next;
                free(n);
                n = current;
        }
}
#endif
