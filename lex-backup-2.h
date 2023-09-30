/* OVERVIEW: read a stream of ASCII characters and put into a doubly linked list of tokens. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define TOK_SIZE 128

typedef enum {CONST, VAR, CTL, UNK} tokentypes;

struct node {
	char token[TOK_SIZE];
	struct node *next;
	struct node *prev;
};

typedef struct node node;
size_t NODE_SIZE = sizeof(node);

node * makenode() {
	node *new = (node *)malloc(NODE_SIZE);
	if(new != NULL) {return new;}
	else return NULL;
}

void remove_spaces(char* s) {
	 char* d = s;
	 do { while (*d == ' ') { ++d;}
    } while ((*s++ = *d++));
}

tokentypes token_type(char *t){
	if (isalpha(*t)){return VAR;}
	else if (*t == '(' || *t == ')' || *t == '*'||
	         *t == '/' || *t == '^' || *t == '-'){ return CTL;}
	else if (isdigit(*t)){return CONST;}
	else {return UNK;}
}

node * lex (char *input) {
	remove_spaces(input);
	unsigned short i = 0;
	node *cur = makenode();
	node *head = makenode();
	head = cur;
	cur->prev = NULL;

	while(*input != '\0'){
		node *new = makenode();
		tokentypes type = token_type(input);
		printf("char: %c\ttype: %d\n", *input, type);
		switch(*input){
			case '(': case ')': case '+': case '*': case '/': case '^':
				strncpy(cur->token, input, 1);
				cur->next = new;new->prev = cur;cur = new; input++;
				break;
			case '-':
				input--;
				if (isdigit(*input) || *input == '.') {
					input++;
					strncpy(cur->token, input, 1);
					cur->next = new; new->prev = cur; cur = new; input++;
				} else {
					input++;
					cur->token[0] = *input++;
					for (i = 1; isdigit(*input) || *input == '.'; i++){cur->token[i] = *input++;}
					cur->token[i] = '\0';
					cur->next = new; new->prev = cur; cur = new;
				}
				break;
			case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': case '0':
				for (i = 0; isdigit(*input) || *input == '.'; i++){cur->token[i] = *input++;}
				cur->token[i] = '\0';
				cur->next = new; new->prev = cur; cur = new;
				break;
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
				input++;
				break;
			default:
				printf("uh oh: %d\n", *input);
				input++;
				break;
		}
	}
	free(cur);
	return head;
}
