#include "syntaxtree.h"
#include <stdio.h>

void print_indent(int indent)
{
	int i;
	for (i = 0; i < indent; i++) { printf(" "); };
}

void print_node(syn_node_t *node, int indent)
{
	if (node == NULL) return;
	print_indent(indent);
	switch (node->type) {
		case S_PROGRAM:
			printf("PROGRAM\n"); break;
		case S_CLAUSE:
			printf("CLAUSE\n"); break;
		case S_HEAD:
			printf("HEAD\n"); break;
		case S_BODY:
			printf("BODY\n"); break;
		case S_PREDICATE:
			printf("PREDICATE:\t%s\n", node->value); break;
		case S_LIST:
			printf("LIST\n"); break;
		case S_CONDITION:
			printf("CONDITION\n"); break;
		case S_VARIABLE:
		   	printf("VARIABLE:\t%s\n", node->value); break;
		case S_ELEM:
			printf("ELEMENT\n"); break;
		case S_CONSTANT:
			printf("CONSTANT:\t%s\n", node->value); break;
		case S_NONE:
			printf("NONE\n"); break;
		default:
			printf("UNKNOWN NODE\n");
	}
	print_node(node->left, indent + 2);
	print_node(node->right, indent);
}

void syn_node_traverse(syn_node_t *node)
{
	print_node(node, 0);
}
