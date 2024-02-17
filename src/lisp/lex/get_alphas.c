#include <stdio.h>
#include "../../shared/shared.h"

char *get_alphas(char *p) {
        unsigned int i = 0;
        char result[TOKEN_SIZE_LIMIT];
        char *r = result;
        while (isalpha(*p)) {
                result[i] = *p++;
        }
        result[i] = '\0';

        return r;
}