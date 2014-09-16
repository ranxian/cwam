#include "compiler.h"
#include "syntaxtree.h"
#include "kv.h"
#include <string.h>
#include <stdio.h>

char var_prefix;

int main(int argc, char *argv[])
{
	compiler_begin();
	var_prefix = 'Q';

	char *constant = "foo";
	char *var = "Bar";
	printf("is_constant('%s'): %d\n", constant, is_constant(constant));
	printf("is_constant('%s'): %d\n", var, is_constant(var));

	FILE *file = fopen(argv[1], "r");
	char line[MAX_LINE_LEN] = {};
	char buf[MAX_PROGRAM_LEN] = {};

	while (fgets(line, MAX_LINE_LEN, file) != NULL) {
		strcat(buf, line);
	}

	fclose(file);

	printf("code: %s\n", buf);
	toks_t *toks = toks_init(buf);
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

	return 0;
}
