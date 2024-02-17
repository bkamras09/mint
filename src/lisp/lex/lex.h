#ifndef LEXER_HEADER
#define LEXER_HEADER
#include "../../shared/shared.c"

node *make_node(void);
node *add_node(node *n);
node *lex(char *in);

void update_node(node *c, node *n, char *i, ETokenType t);
void remove_spaces(char* s);
void print_list(node *n);
void get_token(node *c, node *n, char *i, ETokenType t, unsigned int length);
void free_all_tokens(node *n);

char *get_digits(char *p);
char *get_alphas(char *p);

bool str_cmp(char a[], char b[]);

#endif
