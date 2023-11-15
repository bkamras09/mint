#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "shared.h"

char *type_as_string(ETokenType t) {
    switch (t) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case EXP: return "EXP";
        case DIGIT: return "DIGIT";
        case ALPHA: return "ALPHA";
        default:
            printf("Error getting token type.  Got: %d\n", t);
            return "ERROR";
    }
}

ETokenType get_ETokenType(char p) {
	if (isdigit(p)) return DIGIT;
	if (isalpha(p)) return ALPHA;

	switch(p){
		case '+':  return PLUS;
		case '-':  return MINUS;
		case '*':  return MUL;
		case '/':  return DIV;
		case '^':  return EXP;
		case '(':  return LP;
		case ')':  return RP;
		case '&':  return AND;
		case '{':  return LB;
		case '}':  return RB;
		case '%':  return MOD;
		case '\0': return END_OF_FILE;
		default:   return ERROR;
	}
}

char *ETokenType_as_string(char *p){
	if (isdigit(*p)) return "DIGIT";
	if (isalpha(*p)) return "ALPHA";

	switch(*p){
		case '+':  return "PLUS";
		case '-':  return "MINUS";
		case '*':  return "MUL";
		case '/':  return "DIV";
		case '(':  return "LP";
		case ')':  return "RP";
		case '^':  return "EXP";
		case '\0': return "END_OF_FILE";
		default:   return "ERROR";
	}
}

Variable symbol_table[SYMBOL_TABLE_SIZE];
int symbol_count = 0;

void assign_variable(char *name, float value) {
    // Look for the variable in the symbol table
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            // Update the existing variable
            symbol_table[i].value = value;
            return;
        }
    }
    // If not found, add a new variable to the symbol table
    strncpy(symbol_table[symbol_count].name, name, TOKEN_SIZE_LIMIT);
    symbol_table[symbol_count].value = value;
    symbol_count++;
}

float get_variable_value(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].value;
        }
    }
    fprintf(stderr, "Variable '%s' not defined\n", name);
    exit(EXIT_FAILURE);
}

