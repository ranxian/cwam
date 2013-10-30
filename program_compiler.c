#include "program_compiler.h"
#include "compiler.h"
#include "syntaxtree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char key[256];
	int intval;
	char strval[256];
} key_val_t;

static int preprocess_names(syn_node_t *root)
{
	printf("start processing names\n");

	key_val_t keyvals[128];
	int nkey = 0;

	syn_node_t *prog_n, *pred_n;
	int i, num;
	if (root->type == S_PROGRAM && root->left != NULL) {
		prog_n = root;
		while (prog_n != NULL) {
			pred_n = prog_n->left->left->left;

			for (i = 0; i < nkey; i++)
				if (strcmp(keyvals[i].key, pred_n->value) == 0) break;
			if (i == nkey) {
				strcpy(keyvals[nkey].key, pred_n->value);
				keyvals[i].intval = 0;
				nkey++;
			}
			sprintf(pred_n->value, "%s~%d", pred_n->value, keyvals[i].intval);
			keyvals[i].intval += 1;
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
			for (i = 0; i < nkey; i++)
				if (strcmp(keyvals[i].key, value) == 0) break;

			sprintf(pred_n->value, "%s/%d", pred_n->value, keyvals[i].intval);
			prog_n = prog_n->right;
		}
	}
	syn_node_traverse(root);

	printf("predicate node names updated\n");
	return 0;
}

static int compile(char *code)
{
	syn_node_t *root = syn_node_init();
	toks_t *toks = toks_init(code);
	if (program(toks, root) && toks->idx == toks->len) {
		printf("program compile success.\n");
		syn_node_traverse(root);
		preprocess_names(root);
	}
	return 0;
}

int compile_program(char *filename)
{
	FILE *file = fopen(filename, "r");

	char line[MAX_PROGRAM_LEN] = {};
	char buf[MAX_LINE_LEN] = {};
	while (fgets(line, MAX_LINE_LEN, file) != NULL) {
		strcat(buf, line);
	}
	compile(buf);

	fclose(file);
	return 0;
}
