#include "compiler.h"
#include "syntaxtree.h"
#include "kv.h"
#include <stdio.h>

char var_prefix;

int main()
{
	compiler_begin();
	var_prefix = 'Q';

	char *constant = "foo";
	char *var = "Bar";
	char *num = "12.2";
	printf("is_constant('%s'): %d\n", constant, is_constant(constant));
	printf("is_constant('%s'): %d\n", var, is_constant(var));

	char *code = "son(paul, jinny). father(X, Y) :- son(Y, X).";
	printf("code: %s\n", code);
	toks_t *toks = toks_init(code);
	toks_info(toks);


	syn_node_t *root = syn_node_init();
	root->left = root->right = NULL;
	root->type = S_NONE;

	if (program(toks, root)) {
		printf("parse success\n");
		syn_node_traverse(root);
	}

	toks_destroy(toks);
	syn_node_destroy(root);
	compiler_end();
}
