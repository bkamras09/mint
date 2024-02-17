#include "shared.h"

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
