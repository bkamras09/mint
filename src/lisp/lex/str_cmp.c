#include <stdio.h>

bool
str_cmp(char a[], char b[])
{
        while (*a != '\0' && *b != '\0') if (*a++ != *b++) return false;

        return true;
}
