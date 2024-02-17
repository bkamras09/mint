#include "shared.h"

char
*ETokenType_as_string(char *p)
{
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