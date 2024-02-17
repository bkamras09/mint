#include "shared.h"

char
*ETokenType_to_string(ETokenType t)
{
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