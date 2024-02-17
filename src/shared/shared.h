#ifndef _SHARED_HEADER
#define _SHARED_HEADER

#define TOKEN_SIZE_LIMIT 256
#define SYMBOL_TABLE_SIZE 256
#define INPUT_SIZE_LIMIT 32768

typedef enum {
	AND, OR, COND, LOOP, LEQ, EQ, GEQ,
	MOD, INT, DIGIT, ALPHA,
	PLUS, MINUS, MUL, DIV, EXP, // binary ops
	NEG, POS, // unary ops
	LP, RP, LB, RB,
	FN, MACRO,
	END_OF_FILE, ERROR, UNINITIALIZED, EXIT, NUMBER, IDENTIFIER,
	DEFINE, LET, SET, SETBANG,
	VAR,
	SPACE
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
	int type;
} Variable;

// Simplified version of a map for variable bindings.
typedef struct VarMap {
    char varName[64];
    float value;
    struct VarMap* next;
} VarMap;

char *type_as_string(ETokenType t);
char *ETokenType_as_string(char *p);
char *ETokenType_to_string(ETokenType t);

ETokenType get_ETokenType(char p);

float lookup_var(VarMap **map, const char* varName);

void print_greeting(void);
void print_all_tokens(node *head);
void update_var_map(VarMap** map, const char* varName, float value);
void print_var_map(VarMap **map);
void free_var_vap(VarMap **map);
#endif
