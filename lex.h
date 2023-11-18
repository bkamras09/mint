#ifndef LEXER_HEADER
#define LEXER_HEADER
#include "shared.h"

node *make_node();
node *add_node(node *n);
void update_node(node *c, node *n, char *i, ETokenType t);
void remove_spaces(char* s);
node * lex (char *in);
char *get_digits(char *p);
char *get_alphas(char *p);
bool str_cmp(char a[], char b[]);

#endif