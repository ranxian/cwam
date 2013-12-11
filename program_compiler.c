#include "compiler.h"
#include "syntaxtree.h"
#include "kv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int preprocess_names(syn_node_t *root)
{

	kv_tbl_t *table = kv_tbl_init();

	syn_node_t *prog_n, *pred_n;
	int i;
	if (root->type == S_PROGRAM && root->left != NULL) {
		prog_n = root;
		while (prog_n != NULL) {
			pred_n = prog_n->left->left->left;

			kv_t *kv = NULL;
			if (!kv_tbl_contains(table, pred_n->value)) {
				kv = kv_tbl_insert(table, pred_n->value, 0, NULL);
			} else {
				kv = kv_tbl_lookup(table, pred_n->value);
			}

			sprintf(pred_n->value, "%s~%d", pred_n->value, kv->intval+1);

			kv->intval += 1;
			prog_n = prog_n->right;

		}

		prog_n = root;
		while (prog_n != NULL) {
			pred_n = prog_n->left->left->left;

			char value[256];
			for (i = 0; pred_n->value[i]; i++)
				if (pred_n->value[i] == '~') break;
				else value[i] = pred_n->value[i];
			value[i] = 0;
			kv_t *kv = kv_tbl_lookup(table, value);

			sprintf(pred_n->value, "%s/%d", pred_n->value, kv->intval);
			prog_n = prog_n->right;
		}
	}

	kv_tbl_destroy(table);
	return 0;
}

static prog_t *compile(char *code)
{
	compiler_begin();
	var_prefix = 'Y';
	syn_node_t *root = syn_node_init();
	toks_t *toks = toks_init(code);
	if (program(toks, root) && toks->idx == toks->len) {
		preprocess_names(root);
	}
	toks_destroy(toks);
	compiler_end();
	return syn_node_to_prog(root);
}

prog_t *compile_program(char *filename)
{
	FILE *file = fopen(filename, "r");

	char line[MAX_PROGRAM_LEN] = {};
	char buf[MAX_LINE_LEN] = {};
	while (fgets(line, MAX_LINE_LEN, file) != NULL) {
		strcat(buf, line);
	}
	fclose(file);
	prog_t *prog = compile(buf);
	return prog;
}
