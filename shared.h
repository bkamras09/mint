#ifndef SHARED_HEADER
#define SHARED_HEADER
#define TOKEN_SIZE_LIMIT 128
#define SYMBOL_TABLE_SIZE 256
#define INPUT_SIZE_LIMIT 32768

typedef enum {
	INT,
	DIGIT,
	ALPHA,
	PLUS,
	MINUS,
	MUL,
	DIV,
	EXP,
	LP,
	RP,
	LB,
	RB,
	END_OF_FILE,
	ERROR,
	TERM_OPERATOR,
	FACTOR_OPERATOR,
	UNINITIALIZED,
	EXIT,
	NUMBER,
	IDENTIFIER} ETokenType;

typedef struct node {
	char token[TOKEN_SIZE_LIMIT];
    float value;
	ETokenType token_type;
	struct node *next, *prev;
} node;

typedef struct {
    char name[TOKEN_SIZE_LIMIT];
    float value;
} Variable;

char *type_as_string(ETokenType t);
ETokenType get_ETokenType(char p);
char *ETokenType_as_string(char *p);

#endif
