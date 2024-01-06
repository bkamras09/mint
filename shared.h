#ifndef _SHARED_HEADER
#define _SHARED_HEADER
#define TOKEN_SIZE_LIMIT 32
#define SYMBOL_TABLE_SIZE 256
#define INPUT_SIZE_LIMIT 32768

/* TODO: consider refactoring these into different enums  */
typedef enum {
  AND, OR, COND, LOOP, LEQ, EQ, GEQ,

	MOD, INT, DIGIT, ALPHA,

	PLUS, MINUS, MUL, DIV, EXP, // binary ops

  NEG, POS, // unary ops

	LP, RP, LB, RB,

	FN, MACRO,

	END_OF_FILE, ERROR, UNINITIALIZED, EXIT, NUMBER, IDENTIFIER
} ETokenType;

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
char *ETokenType_as_string(char *p);
char *ETokenType_to_string(ETokenType t);

ETokenType get_ETokenType(char p);

#endif
