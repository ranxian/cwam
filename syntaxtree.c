#include "syntaxtree.h"
#include <stdlib.h>
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
		   	printf("VARIABLE:\t\t%s\n", node->value); break;
		case S_ELEM:
			printf("ELEMENT\n"); break;
		case S_CONSTANT:
			printf("CONSTANT:\t\t%s\n", node->value); break;
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

prog_t *syn_node_to_prog(syn_node_t *tree)
{
	if (tree = NULL) NULL;
	prog_t *prog = prog_init();

	switch (tree->type) {
		case S_PROGRAM:
			{
				if (tree->left == NULL) { free(prog); return NULL; };
				prog_t *p1 = syn_node_to_prog(tree->left);
				prog_t *p2 = syn_node_to_prog(tree->right);
				prog_add_prog(prog, p1), prog_add_prog(prog, p2);
				prog_destroy(p1), prog_destroy(p2);
			}
		case S_CONDITION:
			{
				if (tree->right != NULL) {
					syn_node_t *s = tree->right;

					while (s != NULL) {
						switch (s->left->type) {
							case S_CONSTANT:
								prog_add_stmt(prog, stmt_init("", OP_PUT_CONST, 2, s->left->value, ))
						}
					}

					syn_node_destroy(s);
				}
			}
	}

}

syn_node_t *syn_node_init() { return malloc(sizeof(syn_node_t)); }

void syn_node_destroy(syn_node_t *tree)
{
	if (tree = NULL) return;
	syn_node_destroy(tree->left);
	syn_node_destroy(tree->right);
	free(tree);
}
