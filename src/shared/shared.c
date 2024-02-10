#ifndef SHARED
#define SHARED
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
//#include "sys/ioctl.h"
//#include "unistd.h"
#include "shared.h"

char *type_as_string(ETokenType t) {
        switch (t) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case EXP: return "EXP";
        case LP: return "LP";
        case RP: return "RP";
        case DIGIT: return "DIGIT";
        case ALPHA: return "ALPHA";
        case FN: return "FN";
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case NEG: return "NEG";
        case POS: return "POS";
        case EQ: return "EQ";
        case LET: return "LET";
        case DEFINE: return "DEFINE";
        default:
                printf("Error getting token type.  Got: %d\n", t);
                return "ERROR";
        }
}

ETokenType get_ETokenType(char p) {
        if (isdigit(p)) return DIGIT;
        if (isalpha(p)) return ALPHA;
        //if (strcmp(&p, "fn")) return FN;
        //if (strcmp(&p, "macro")) return MACRO;

        switch(p){
        case '+':  return PLUS;
        case '-':  return MINUS;
        case '*':  return MUL;
        case '/':  return DIV;
        case '^':  return EXP;
        case '(':  return LP;
        case ')':  return RP;
        case '&':  return AND;
        case '=':  return EQ;
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
        case ' ': return "SPACE";
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

char *ETokenType_to_string(ETokenType t){
        switch(t) {
        case SPACE: return "SPACE";
        case PLUS:
        case POS:   return "+";
        case MINUS:
        case NEG:   return "-";
        case MUL:   return "*";
        case DIV:   return "/";
        case EXP:   return "^";
        case DIGIT: return "DIGIT";
        case ALPHA: return "ALPHA";
        default:    return "ERROR";
        }
}

Variable symbol_table[SYMBOL_TABLE_SIZE];
int symbol_count = 0;

void assign_variable(char *name, float value) {
        for (int i = 0; i < symbol_count; i++) {
                if (strcmp(symbol_table[i].name, name) == 0) {
                        // Update the existing variable
                        symbol_table[i].value = value;
                        return;
                }
        }
        // variable wasn't found, add it to the symbol table
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

void print_greeting() {
        //struct winsize w;
        //w.ws_col
        //ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        char *title = " mint REPL ";
        printf("%s", title);
        char *banner = "_";
        char *flavor_text = "enter \"exit\" without quotes to exit the REPL\n";
        int len = (strlen(flavor_text) - strlen(title)) / strlen(banner); 
        for (int i = 0; i < len; i++) {
                printf("%s", banner);
        }
        printf("]\n");
        printf("%s", flavor_text);
}

void print_all_tokens(node *head) {
        node *current = head;
        while (current != NULL) {
                printf("%s ", current->token);
                current = current->next;
        }
        printf("\n");
}

// Function to find a variable's value in the map.
float lookupVar(VarMap **map, const char* varName) {
        VarMap *temp = (*map);
        while (temp->next != NULL) {
                if (strcmp(temp->varName, varName) == 0) {
                        return temp->value;
                }
                temp = temp->next;
        }

        fprintf(stderr, "Variable not found: %s\n", varName);
        exit(EXIT_FAILURE);
}

// Function to add a new variable or update an existing one in the map.
void updateVarMap(VarMap **map, const char* varName, float value) {
    VarMap *temp = (*map);
    while (temp != NULL) {
        if (strcmp(temp->varName, varName) == 0) {
            temp->value = value;
            return;
        }
        temp = temp->next;
    }
    // If not found, add new variable.
    VarMap* newVar = (VarMap*)malloc(sizeof(VarMap));
    if (!newVar) return; // Handle allocation failure gracefully.
    strcpy(newVar->varName, varName);
    newVar->value = value;
    newVar->next = *map;
    *map = newVar;
}

//print all variables in the map
void printVarMap(VarMap **map) {
    printf("printing var map\n");
    VarMap *temp = (*map);
    while (temp != NULL) {
        printf("varName: %s, value: %f\n", temp->varName, temp->value);
        temp = temp->next;
    }
}

// Free the memory allocated for the map.
void freeVarMap(VarMap **map) {
    VarMap *temp = (*map);
    while (temp != NULL) {
        VarMap *next = temp->next;
        free(temp);
        temp = next;
    }
    *map = NULL;
}

#endif