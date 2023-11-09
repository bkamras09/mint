#include "expr.c"
#include "io.c"
#include "shared.c"
#include "lex.c"

int main(void) {
	char input[INPUT_SIZE_LIMIT];
	char *current_token = input;
	printf("Welcome to k.  So simple it doesn't even do variables yet.\n");

	for(;;) {
		printf("> ");
		current_token = get_line(input);
		node *head = lex(input);
		if (head->token_type == EXIT) { break; }
		float result = expr(&head);
		printf("%.6f\n", result);
	}

	printf("bye\n");

	return 0;
}
