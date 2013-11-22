#include "syntaxtree.h"
#include "machine.h"
#include "defs.h"
#include "stmt.h"
#include <stdlib.h>
#include <stdio.h>

char *NODE_NAMES(syn_node_type_t type)
{
	static char *_NODE_NAMES[20] = {
		"clause",
		"program",
		"head",
		"body",
		"predicate",
		"list",
		"condition",
		"variable",
		"expression",
		"element",
		"comparison",
		"structure",
		"constant",
		"none",
		"call",
		"non_call",
		"query"
	};

	return _NODE_NAMES[type];
}

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
			printf("VARIABLE:\t\t%s\n", node->value); break;
		case S_ELEM:
			printf("ELEMENT\n"); break;
		case S_CONSTANT:
			printf("CONSTANT:\t\t%s\n", node->value); break;
		case S_NONE:
			printf("NONE\n"); break;
		case S_STRUCT:
			printf("STRUCTURE\n"); break;
		case S_QUERY:
			printf("QUERY\n"); break;
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


syn_node_t *syn_node_init() { return malloc(sizeof(syn_node_t)); }

void syn_node_destroy(syn_node_t *tree)
{
	if (tree == NULL) return;
	syn_node_destroy(tree->left);
	syn_node_destroy(tree->right);
	free(tree);
}
