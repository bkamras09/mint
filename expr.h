#ifndef _EXPRESSION_HEADER
#define _EXPRESSION_HEADER
#include "shared.h"

struct AstNode {
	char token[TOKEN_SIZE_LIMIT];
	ETokenType type;
	float value;
    char *current_token;
	struct AstNode *left;
	struct AstNode *right;
};
typedef struct AstNode AstNode;

ETokenType get_ETokenType(char p);
char *ETokenType_as_string(char *p);
AstNode *new_AstNode(char p[]);
float expr(node **n);
float term(node **n);
float factor(node **n);
float exponent(node **n);
char *get_digits(char *p);
char *get_alphas(char *p);
bool eat_token(char p, ETokenType t);
char *get_next_token(char *p);
char *type_as_string(ETokenType t);
AstNode *get_next_AstNode(char *p);

#endif
