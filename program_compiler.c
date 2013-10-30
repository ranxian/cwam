#include "program_compiler.h"
#include "compiler.h"
#include "syntaxtree.h"
#include "kv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int preprocess_names(syn_node_t *root)
{
	printf("start processing names\n");

	kv_tbl_t *table = kv_tbl_init();
	int nkey = 0;

	syn_node_t *prog_n, *pred_n;
	int i, num;
	if (root->type == S_PROGRAM && root->left != NULL) {
		prog_n = root;
		while (prog_n != NULL) {
			pred_n = prog_n->left->left->left;

			kv_t *kv = NULL;
			if (!kv_tbl_contains(table, pred_n->value)) {
				printf("%x\n", (int)kv);
				
				kv = kv_tbl_insert(table, pred_n->value, 0, NULL);
				printf("%x\n", (int)kv);
			} else {
				printf("%x\n", (int)kv);
				kv = kv_tbl_lookup(table, pred_n->value);
				printf("%x\n", (int)kv);

			}
			printf("zhzha\n");

			sprintf(pred_n->value, "%s~%d", pred_n->value, kv->intval);
			printf("zhzha\n");

			kv->intval += 1;
			prog_n = prog_n->right;
			printf("zhzha\n");

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
	syn_node_traverse(root);

	printf("predicate node names updated\n");
	kv_tbl_destroy(table);
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
