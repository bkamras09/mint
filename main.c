#include "sys/ioctl.h"
#include "unistd.h"
#include "expr.c"
#include "io.c"
#include "shared.c"
#include "lex.c"

int main(void) {
	//struct winsize w;
	//w.ws_col
	//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	char input[INPUT_SIZE_LIMIT];
	char *current_token = input;
	// probably abstract the window stuff to its own function eventually
	char *title = " k REPL ";
	printf("%s", title);
	char *banner = "_";  // ━
	char *flavor_text = "enter \"exit\" without quotes to exit the REPL\n";
	int len = (strlen(flavor_text) - strlen(title)) / strlen(banner); 
	
	for (int i = 0; i < len; i++) { printf("%s", banner); }
	printf("|\n"); // ┓
	printf(flavor_text);

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
