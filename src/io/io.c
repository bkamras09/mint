#include <stdio.h>
#include "io.h"
#include "../shared/shared.h"

char
*get_line(char *s)
{
	int c, i;
	char *ps = s;
	for (i = 0; i<INPUT_SIZE_LIMIT-1 && (c = getchar()) != EOF; i++) {
		if (c == '\n') break;
		*ps++ = c;
	}
	*ps = '\0';

	if (i >INPUT_SIZE_LIMIT-1) {
		printf("ERROR: token was too large and was truncated.\n");
	}

	return s;
}
