#ifndef _EXPRESSION_HEADER
#define _EXPRESSION_HEADER
#include <stdbool.h>
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

AstNode *new_AstNode(char p[]);
AstNode *new_AstNode_Operator(ETokenType type, AstNode *left, AstNode *right);
AstNode *new_AstNode_Operand(float value);
AstNode *get_next_AstNode(char *p);
AstNode *expr_AstNode(node **n);
AstNode *term_AstNode(node **n);
AstNode *factor_AstNode(node **n);
AstNode *exponent_AstNode(node **n);

float expr(node **n);
float term(node **n);
float factor(node **n);
float exponent(node **n);
float do_binop(AstNode **n);

char *get_digits(char *p);
char *get_alphas(char *p);
char *get_next_token(char p[]);
char *type_as_string(ETokenType t);
char *ETokenType_as_string(char *p);

bool eat_token(char p, ETokenType t);
float visit(AstNode *n);

#endif
