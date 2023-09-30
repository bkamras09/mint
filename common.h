#include <stdlib.h>

typedef enum {CONST, VAR, CTL, UNK, LP, RP, ADD, NEG, DIV, MUL, EXP, FAC, SIN, COS, TAN, ACT, ACS} symbol;

struct node {
	char token[sizeof(short)];
	symbol symbol;
	uint key;
	int priority;
	struct node *next;
	struct node *prev;
};
typedef struct node node;

node * make_node() {return malloc(sizeof(node));}

struct tnode {
	char token[sizeof(short)];
	float value;
	symbol symbol;
	unsigned short priority;
	struct tnode *parent;
	struct tnode *left;
	struct tnode *right;
};

typedef struct tnode tnode;

tnode * maketnode() {return malloc(sizeof(tnode));}

char * symbol_as_string(symbol s){
	switch(s){
		case LP:
			return "LP";
			break;
		case RP:
			return "RP";
			break;
		case EXP:
			return "EXP";
			break;
		case MUL:
			return "MUL";
			break;
		case DIV:
			return "DIV";
			break;
		case ADD:
			return "ADD";
			break;
		case NEG:
			return "NEG";
			break;
		case VAR:
			return "VAR";
			break;
		case CONST:
			return "CONST";
			break;
		default:
			return "DEFAULT";
			break;
	}
}
