#include "compiler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int query(toks_t *toks, syn_node_t *tree)
{
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->type = S_HEAD;
	if (body(toks, tree->left) && token(toks, "."))
	{
		tree->left->type = S_HEAD;
		tree->left->left = malloc(sizeof(syn_node_t));
		tree->left->left->type = S_PREDICATE;
		strcpy(tree->left->left->value, "query$~1/1");
		tree->right = NULL;
		return 1;
	}
	free(tree->left);
	tree->left = NULL;
	return 0;
}

int compile_query(char *query_str)
{
	toks_t *toks = toks_init(query_str);
	prog_t *prog;
	syn_node_t *root = syn_node_init();

	if (query(toks, root)) {
		if (toks->idx > toks->len) {
			printf("panic: query not fully parsed!\n");
			toks_destroy(toks);
			syn_node_destroy(root);
			return -1;
		}
		prog = syn_node_to_prog(root);
	}

	toks_destroy(toks);
	syn_node_destroy(root);
	return 0;
}
