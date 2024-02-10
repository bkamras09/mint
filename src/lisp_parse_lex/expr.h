#ifndef _EXPRESSION_HEADER
#define _EXPRESSION_HEADER
#include <stdbool.h>
#include "../shared/shared.c"

// DECLARATIONS
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
AstNode *new_AstNode_Operand(float value);
AstNode *get_next_AstNode(char *p);
AstNode *expr_AstNode(node **n, VarMap *map);
AstNode *term_AstNode(node **n);
AstNode *factor_AstNode(node **n, VarMap *map);
AstNode *exponent_AstNode(node **n);
AstNode *parse_atom(node **n, VarMap **map);
AstNode *parse_sexpr(node **n, VarMap **map);

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
//float visit(AstNode *n);
float visit (AstNode *n, VarMap **map);
int isOperator(ETokenType t);

// DEFINITIONS

int isOperator(ETokenType type) {
        return (type == PLUS || type == MINUS || type == MUL || type == DIV || type == EXP);
}

AstNode *new_AstNode(char p[]) { 
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = n->right = NULL;
        ETokenType t = get_ETokenType(*p);

        switch (t) {
        case DIGIT:
                strcpy(n->token, get_digits(p));
                p += strlen(n->token);
                n->value = atof(n->token);
                break;
        case POS: case NEG: case MINUS: case PLUS: case DIV: case MUL:   case EXP: case ALPHA:
        case RP:  case LP:  case LB:    case RB:   case EQ:  case ERROR: case END_OF_FILE: case IDENTIFIER:
                strncpy(n->token, p, strlen(p));
                break;
        case LET:
            strncpy(n->token, "let", 3);
            break;
        default:
                printf("Error while building node.  \
                    Expected a valid token type but got: \
                    %s\n",
                       type_as_string(t));
                break;
        }

        n->type = t;
        n->current_token = p;

        return n;
}

AstNode *new_AstNode_Operand(float value) {
        AstNode *n = malloc(sizeof(AstNode));
        if (!n) return NULL;
        n->left = n->right = NULL;
        n->type = DIGIT;
        n->value = value;
        snprintf(n->token, sizeof(n->token), "%f", value);
        return n;
}

void free_AstNode(AstNode *n) {
        if (!n) return;
        free_AstNode(n->left);
        free_AstNode(n->right);
        free(n);
}

AstNode *expr_AstNode(node **n, VarMap *map) {
    printf("parsing expr\n");
    if (!n || !*n) return NULL;

    if ((*n)->token_type != LP) {
        fprintf(stderr, "Error: Expected '(' at the beginning of expression\n");
        exit(EXIT_FAILURE);
    }
    *n = (*n)->next; // Move past the '(' token

    AstNode *opNode = NULL;
    if ((*n)->token_type == LET) {
        // Handle LET
        *n = (*n)->next; // Move past the 'LET' token

        if ((*n)->token_type != LP) {
            fprintf(stderr, "Error: Expected '(' after 'let'\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the '(' for variable binding

        // Assuming the next token is the variable name
        char varName[64]; // Assuming variable names are no longer than 63 characters
        if ((*n)->token_type == ALPHA) {
            strncpy(varName, (*n)->token, sizeof(varName) - 1);
            varName[sizeof(varName) - 1] = '\0'; // Ensure null-termination
            *n = (*n)->next; // Move past the variable name
        } else {
            fprintf(stderr, "Error: Expected variable name in 'let' expression\n");
            exit(EXIT_FAILURE);
        }

        // The next part is the value expression
        AstNode *valueExpr = expr_AstNode(n, map); // Recursively parse the value expression

        if ((*n)->token_type != RP) {
            fprintf(stderr, "Error: Expected ')' after 'let' value expression\n");
            exit(EXIT_FAILURE);
        }
        *n = (*n)->next; // Move past the ')'

        // Now, parse the body of the let expression
        AstNode *bodyExpr = expr_AstNode(n, map);

        // Create a 'LET' node
        opNode = malloc(sizeof(AstNode));
        if (!opNode) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        opNode->type = LET;
        strncpy(opNode->token, "let", sizeof(opNode->token) - 1);
        opNode->left = malloc(sizeof(AstNode)); // Create a node for variable binding
        if (!opNode->left) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        opNode->left->type = ALPHA;
        strncpy(opNode->left->token, varName, sizeof(opNode->left->token) - 1);
        opNode->left->left = valueExpr; // Attach the value expression to the variable node
        opNode->left->right = NULL;
        opNode->right = bodyExpr; // The body of the let expression
    } else {
        // Handle other expressions as before
    }

    // Ensure to close the expression with a ')' check as before

    return opNode;
}

// Function to evaluate an AST node.
/*
float visit(AstNode* n, VarMap* map) {
    if (!n) { return 0.0; }

    switch (n->type) {
        case DIGIT:
            return n->value;
        case VAR:
            return lookupVar(map, n->token);
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case EXP: {
            float left = visit(n->left, map);
            float right = visit(n->right, map);
            switch (n->type) {
                case PLUS: return left + right;
                case MINUS: return left - right;
                case MUL: return left * right;
                case DIV:
                    if (right == 0.0) {
                        fprintf(stderr, "Error: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    return left / right;
                case EXP:
                    // Assuming powf is available or use pow for double
                    return powf(left, right);
                default:
                    return 0.0; // Unreachable but keeps compiler happy
            }
        }
        case LET: {
            // Assuming LET syntax is (let (varName valueExpr) bodyExpr)
            // You'll need to adjust parsing to accommodate this.
            // For simplicity, this example assumes the AST for let is already constructed accordingly.
            char varName[64];
            strcpy(varName, n->left->token); // Variable name
            float value = visit(n->left->right, map); // Variable value expression
            updateVarMap(&map, varName, value);
            return visit(n->right, map); // Evaluate the body expression with the new variable binding
        }
        default:
            fprintf(stderr, "Unknown node type\n");
            exit(EXIT_FAILURE);
    }
}
*/
#endif
