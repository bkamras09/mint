#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex.h"
#include "shared.h"

char *get_digits(char *p) {
	uint i;
	char result[TOKEN_SIZE_LIMIT];
	char *r = result;
	for (i = 0; (isdigit(*p) || *p == '.') && i<TOKEN_SIZE_LIMIT; i++) {
		result[i] = *p++;
	}
	result[i] = '\0';

	return r;
}

char *get_alphas(char *p) {
	uint i;
	char result[TOKEN_SIZE_LIMIT];
	char *r = result;
	while (isalpha(*p)) {
		result[i] = *p++;
	}
	result[i] = '\0';

	return r;
}

node *make_node() {
	node *n = calloc(1, (sizeof(node)));
	if (!n) return NULL;
	n->prev = n->next = NULL;
	return n;
}

node *add_node(node *n) {
	node *new_node = make_node();
	n->next = new_node;
	new_node->prev = n;
	return new_node;
}

void update_node(node *c, node *n, char *i, ETokenType t) {
	strncpy(c->token, i, 1);
	c->token_type = t;
	c->next = n;
	n->prev = c;
	c = n;
}

void remove_spaces(char* s) {
	 char *d = s;
	 do { while (*d == ' ') { ++d;}
    } while ((*s++ = *d++));
}

bool str_cmp(char a[], char b[]) {
	while (*a != '\0' && *b != '\0') {
		if (*a++ != *b++) { return false; }
	}

	return true;
}

node *lex (char *in) {
	remove_spaces(in);
	uint i = 0;
	node *cur = make_node();
	node *head = make_node();

	if (!cur || !head) return NULL;

	head = cur;
	head->prev = cur->prev = NULL;
	
	while(*in){
		node *n = make_node();

		if (!n) return NULL;

		ETokenType t = get_ETokenType((*in));

		switch(t){
			case PLUS: case DIV: case MUL: case EXP:
			case RP:   case LP:  case LB:  case RB: case EQ:
				update_node(cur, n, in, t);
				in++; 
				break;
			case MINUS:
				in--;
				if (*in == ')' || isdigit(*in)) {
					in++;
					update_node(cur, n, in, t);
				} else {
					in++;
					cur->token[0] = *in;
					in++;
					for (i = 1; isdigit(*in) || *in == '.'; i++) cur->token[i] = *in++;
					cur->token[i] = '\0';
					cur->token_type = DIGIT;
				}
				in++;
				break;
			case DIGIT:
				for (i = 0; isdigit(*in) || *in == '.'; i++) cur->token[i] = *in++;
				cur->token[i] = '\0';
				cur->token_type = DIGIT;
				break;
			case ALPHA:
					char id[TOKEN_SIZE_LIMIT];
					int j = 0;
					while (isalnum(*in) || *in == '_') {  // Variable names can include underscores
						id[j++] = *in++;
					}
					id[j] = '\0';
					strncpy(cur->token, id, TOKEN_SIZE_LIMIT);
					cur->token_type = IDENTIFIER;
					break;
			default:
				printf("Error lexing: %d\n", *in);
				in++;
				break;
		}
		cur = add_node(cur);
	}
	cur->next = NULL;
	
	return head;
}
