/* A simple PEMDAS interpreter for use as an interactive calculator.

   Lexes tokens into a linked list which are then parsed.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

int main() {
	char in[] = "55 * -4.6 * (-3 / 2.3) / 2 ^-2.2";
	node *cur = lex(in);

	printf("input string: %s\n", in);

	printf("lexed tokens: \n");
	
	while(cur->next != NULL) {
		printf(
			"token: %s\ttype: %s\tkey: %d\tpriority: %d\n",
			cur->token,
			symbol_as_string(cur->symbol),
			cur->key,
			cur->priority);
			
		cur = cur->next;
	}

	return 0;
}
