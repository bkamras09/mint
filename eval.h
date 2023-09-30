/* OVERVIEW: parse a doubly linked list of symbols into an AST. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "common.h"

struct tnode {
	char token[128];
	symbol symbol;
	struct tnode *parent;
	struct tnode *left;
	struct tnode *right;
};

typedef struct tnode tnode;

tnode * maketnode() {return malloc(sizeof(tnode));}

tnode *eval(node *n){
	while (n->next){
		switch (n->symbol){
			case RP:
				eval(n);
				break;
			case LP:
				eval(n);
				break;
			case MUL:
			case DIV:
			case NEG:
			case ADD:
			case VAR:
			case CONST:
			default:
				return NULL;
				break;
		}
	}
	return NULL;
}
