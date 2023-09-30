/* OVERVIEW: parse a doubly linked list of symbols into an AST. */

/*
Logic for implementing grammar:
CONST or VAR:
	1.  Write value into current node
	2.  Ascend to parent node

Operator -, +, /, *, ^:
	1.  Ascend to parent node
	2a.  If priority of parent node is less than current token, break tree and insert.
	2b.  If priority of parent node is greater than current token, insert.
	3.  Descend right.

(:
	1.  Descend right.
	2.  Write '(' into current node.
	3.  Leave left child as NULL.
	4.  Descend right.

):
	1.  Ascend until ( is found.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "common.h"

tnode *parse(node *n){
	tnode *start = maketnode();
	tnode *cur = maketnode();
	cur->parent = start;
	// would do something like "nextsym" to avoid having to use a linked list intermediary
	//while (nextsym(*in)){}
	while (n->next){
		switch (n->symbol){
			case RP:
				break;
			case LP:
				break;
			case MUL:
			case DIV:
			case NEG:
			case ADD:
			
			case VAR:
			case CONST:
				strcpy(n->token, cur->token);
				cur = cur->parent;
				break;
			default:
				return NULL;
				break;
		}
	}
	return NULL;
}
