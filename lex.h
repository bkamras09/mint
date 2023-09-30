/* OVERVIEW: read an array of ASCII characters and put into a doubly linked list of tokens. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "common.h"

void remove_spaces(char* s) {
	 char* d = s;
	 do { while (*d == ' ') { ++d;}
    } while ((*s++ = *d++));
}

int setpriority(char c){
	switch(c){
		case '+': case '-':
			return 0;
			break;
		case '/': case '*':
			return 1;
			break;
		case '^':
			return 2;
			break;
		case '(':
			return 3;
			break;
		case ')':
			return 4;
			break;
		default:
			return -1;
			break;
	}
}

void update_node(node *c, node *n, char *i, symbol s, uint key, uint p, uint l)
{
	strncpy(c->token, i, l);
	c->symbol = s;
	c->key = key;
	c->priority = setpriority(p);
	c->next = n;
	n->prev = c;
	c = n;
}

node * lex (char *input)
{
	remove_spaces(input);
	unsigned short i = 0;
	unsigned int key = 0;
	node *cur = make_node();
	node *head = make_node();

	cur->key = head->key = key;
	
	if (cur == NULL || head == NULL) return NULL;
	
	head = cur;
	cur->prev = NULL;
	
	while(*input){
		node *new = make_node();
		if (new == NULL) return NULL;
		
		switch(*input){
			case '(':
				update_node(cur, new, input, LP, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case ')':
				update_node(cur, new, input, RP, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case '+':
				update_node(cur, new, input, ADD, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case '*':
				update_node(cur, new, input, MUL, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case '/':
				update_node(cur, new, input, DIV, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case '^':
				update_node(cur, new, input, EXP, key++, *input, 1);
				cur->next = new;
				new->prev = cur;
				cur = new;
				input++;
				break;
			case '-':
				input--;
				if (isdigit(*input)) {
					input++;
					update_node(cur, new, input, NEG, key++, *input, 1);
					input++;
				} else {
					input++;
					cur->token[0] = *input++;
					for (i = 1; isdigit(*input) || *input == '.'; i++){
						cur->token[i] = *input++;
					}
					cur->token[i] = '\0';
					cur->symbol = CONST;
					cur->key = key++;
					cur->priority = -2;
				}
				cur->next = new;
				new->prev = cur;
				cur = new;
				break;
			case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': case '0':
				for (i = 0; isdigit(*input) || *input == '.'; i++){
					cur->token[i] = *input++;
				}
				cur->token[i] = '\0';
				cur->symbol = CONST;
				cur->key = key++;
				cur->priority = -2;
				cur->next = new;
				new->prev = cur;
				cur = new;
				break;
			case 'a': case 'b': case 'c': case 'd': case 'e':
			case 'f': case 'g': case 'h': case 'i': case 'j':
			case 'k': case 'l': case 'm': case 'n': case 'o':
			case 'p': case 'q': case 'r': case 's': case 't':
			case 'u': case 'v': case 'w': case 'x': case 'y':
			case 'z':
				for (i = 0; isalpha(*input) || *input == '.'; i++){
					cur->token[i] = *input++;
				}
				cur->token[i] = '\0';
				cur->symbol = VAR;
				cur->key = key++;
				cur->priority = -2;
				cur->next = new;
				new->prev = cur;
				cur = new;
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
