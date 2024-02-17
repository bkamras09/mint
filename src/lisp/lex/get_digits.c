#include <stdio.h>
#include "../../shared/shared.h"

char *get_digits(char *p) {
        unsigned int i;
        char result[TOKEN_SIZE_LIMIT];
        char *r = result;
        for (i = 0; (isdigit(*p) || *p == '.') && i<TOKEN_SIZE_LIMIT; i++) {
                result[i] = *p++;
        }
        result[i] = '\0';

        return r;
}
