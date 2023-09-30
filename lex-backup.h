/* OVERVIEW: read a stream of characters and put into a doubly linked list of tokens. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define CMD_SIZE 1024
#define BUF_SIZE 256
#define TOK_SIZE 32

typedef enum {OPERAND, LPAREN, RPAREN, ADD, SUB, MUL, DIV, EXP} tokentypes;

struct node {
	char token[TOK_SIZE];
	tokentypes type;
	unsigned int key;
	struct node *next;
	struct node *prev;
};

typedef struct node node;
size_t NODE_SIZE = sizeof(node);

node * makenode() {
	node *new = (node *)malloc(NODE_SIZE);
	if(new != NULL) {
		new->key = 0;
		return new;
	}
	else return NULL;
}

void remove_spaces(char* s) {
	 char* d = s;
	 do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

void updatenode(node *cur, char *token, int key, node *next){
	strcpy(cur->token, token);
	cur->key = key;
	cur->next = next;
	next->prev = cur;
	cur = next;
}

node * lex (char *input) {
	remove_spaces(input);
	int key = 0;
	char *p;
	char *t;
	node *cur = makenode();
	// so we don't have to walk back up to the head when we retern
	node *head = makenode();
	head = cur;
	head->key = key;
	cur->prev = NULL;

	while(*input != '\0'){
		//printf("next char: %c\n", *input);
		node *new = makenode();
		switch(*input){
			case '(':
			case ')':
			case '+':
			case '*':
			case '/':
			case '^':
				//updatenode(cur, input, key++, new);
				strncpy(cur->token, input, 1);
				cur->next = new;new->prev = cur;cur = new;input++;
				break;
			case '-':
				printf("SUB\n");
				input--;
				if (isdigit(*input) || *input == ')') {
					input++;
					printf("Adding SUB.\n");
					strncpy(cur->token, "-", 1);
					cur->type = SUB;
					cur->next = new;
					new->prev = cur;
					cur = new;
					input++;
				} else {
					input+=2;
					cur->token[0] = '-';
					uint i;
					for (i = 1; isdigit(*input); i++){
						printf("for input: %c\n", *input);
						cur->token[i] = *input++;
					}
					cur->token[i] = '\0';
					
					/*printf("neg OPERAND\n");
					uint l = 0;
					
					t = input;
					printf("input: %c\n", *input);
					*t++ = *input++;
					while (isdigit(*input)){
						
						l++;
						*t++ = *input++;
					}
					
					if (strlen(input) < 2) {
						//p[l+1] = '\0';
						strncpy(cur->token, p, l);
					} else {
						//p[l+2] = '\0';
						strncpy(cur->token, p, l+1);
					}
					printf("p: %s\n", p);
					*/
					
					
					cur->type = OPERAND;
					cur->key = key++;
					cur->next = new;
					new->prev = cur;
					cur = new;
					input++;
				}
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				uint l = 0; t = input; p = t;
				while (isdigit(*input)){l++; input++;}
				strncpy(cur->token, p, l);
				cur->type = OPERAND;
				cur->key = key++;
				cur->next = new;
				new->prev = cur;
				cur = new;
				break;
			default:
				printf("uh oh\n");
				break;
		}
		t = input;	
	}

	free(cur);

	return head;
}
