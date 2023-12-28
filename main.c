
#include "expr.c"
#include "io.c"
#include "shared.c"
#include "lex.c"
#include <math.h>

int main(void) {
	char input[INPUT_SIZE_LIMIT];
	char *current_token = input;
	print_greeting();

  for (;;) {
    printf("> ");
    current_token = get_line(input);
    node *head = lex(input);
    if (head->token_type == EXIT) {
      break;
    }
    AstNode *result = expr_AstNode(&head);
    float computed_result = visit(result); // Compute the result
    printf("%.6f\n", computed_result);    // Print the result
  }

	printf("bye\n");

	return 0;
}

