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
