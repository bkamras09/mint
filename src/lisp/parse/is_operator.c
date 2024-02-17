#include <stdlib.h>

int
is_operator(ETokenType type)
{
        return (type == PLUS || type == MINUS || type == MUL || type == DIV || type == EXP);
}
