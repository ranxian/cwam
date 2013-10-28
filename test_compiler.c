#include "compiler.h"
#include "syntaxtree.h"
#include <stdio.h>

int main()
{
	char *constant = "foo";
	char *var = "Bar";
	char *num = "12.2";
	printf("is_constant('%s'): %d\n", constant, is_constant(constant));
	printf("is_constant('%s'): %d\n", var, is_constant(var));

	char *code = "son(paul, jinny). father(X, Y) :- son(Y, X).";
	printf("code: %s\n", code);
	tok_stream_t toks;
	toks_init_from_string(&toks, code);
	toks_info(&toks);

	syn_node_t root;
	root.left = root.right = NULL;
	root.type = S_NONE;

	if (program(&toks, &root)) {
		printf("parse success\n");
		syn_node_traverse(&root);
	}

	toks_destroy(&toks);
}
