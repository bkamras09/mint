#ifndef _EXPRESSION_HEADER
#define _EXPRESSION_HEADER
#include <stdbool.h>
#include "../../shared/shared.c"

struct AstNode {
	char token[TOKEN_SIZE_LIMIT];
	ETokenType type;
	float value;
	char *current_token;
	struct AstNode *left;
	struct AstNode *right;
	struct AstNode *last_statement;
};
typedef struct AstNode AstNode;

ETokenType get_ETokenType(char p);

AstNode *new_AstNode(char p[]);
AstNode *new_AstNode_Operator(ETokenType type, AstNode *left, AstNode *right);
AstNode *new_AstNode_operand(float value);
AstNode *get_next_AstNode(char *p);
AstNode *expr_AstNode(node **n, VarMap *map);
AstNode *term_AstNode(node **n);
AstNode *factor_AstNode(node **n, VarMap *map);
AstNode *exponent_AstNode(node **n);
AstNode *parse_atom(node **n, VarMap **map);
AstNode *parse_define(node **n, VarMap **map);
AstNode *parse_sexpr(node **n, VarMap **map);

float expr(node **n);
float term(node **n);
float factor(node **n);
float exponent(node **n);
float do_binop(AstNode **n);
float visit (AstNode *n, VarMap **map);

char *get_digits(char *p);
char *get_alphas(char *p);
char *get_next_token(char p[]);
char *type_as_string(ETokenType t);
char *ETokenType_as_string(char *p);

bool eat_token(char p, ETokenType t);

int is_operator(ETokenType t);

void free_AstNode(AstNode *n);

#endif
