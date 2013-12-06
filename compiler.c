#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "defs.h"
#include "kv.h"
#include "machine.h"
#define is_anum(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
#define SEQL(s1, s2) (strcmp((s1), (s2)) == 0)
#define is_comp(s) (SEQL((s), ">") || SEQL((s), "<") || SEQL((s), ">=") || SEQL((s), "<=") || \
					SEQL((s), "!=") || SEQL((s), "=="))
#define CUR_TOK(toks) ((toks)->tokens[(toks)->idx])

static char temp[MAX_WORD_LEN];

static char *last_var = NULL;

static int body_calls = 0;

kv_tbl_t *table;

char *cnt_arg(int count)
{
	sprintf(temp, "A%d", count);
	return temp;
}

int inline push_stat(toks_t *toks, syn_node_t *tree)
{
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));
	return old_idx;
}
int inline pop_stat(toks_t *toks, syn_node_t *tree, int old_idx)
{
	free(tree->right);
	free(tree->left);
	tree->right = tree->left = NULL;
	toks->idx = old_idx;

	return 0;
}
/* test if s is a constant */
int is_constant(const char *s)
{
	int len = strlen(s);
	if (s[0] == '[' && s[1] == ']' && s[2] == 0)
		return 1;
	if (s[0] >= 'a' && s[0] <= 'z') {
		int i;
		for (i = 1; i < len; i++) {
			if (!is_anum(s[i])) return 0;
		}
		return 1;
	}
	if (s[0] == '\'' && s[len-1] == '\'')
		return 1;
	if (is_num(s))
		return 1;
	return 0;
}
/* test if s is a number */
int is_num(const char *s)
{
	int point = 0;
	int len = strlen(s);
	int i;
	for (i = 0; i < len; i++) {
		char c = s[i];
		if (c == '.') {
			if (!point) point = 1;
			else return 0;
		} else if (c >= '0' && c <= '9') {
			;
		} else {
			return 0;
		}
	}
	return 1;
}
/* test if s is a variable */
int is_variable(const char *s)
{
	if (s[0] == '_' && s[1] == 0)
		return 1;

	if (s[0] >= 'A' && s[0] <= 'Z') {
		int len = strlen(s), i;
		for (i = 1; i < len; i++) {
			if (!is_anum(s[i])) return 0;
		}
		return 1;
	}
	return 0;
}
/* test if s is a predicate */
int is_predicate(const char *s)
{
	return is_constant(s) && !is_num(s) && !(s[0] == '_' && s[1] == 0);
}

int program(toks_t *toks, syn_node_t *tree)
{
	// printf("test program: %s\n", CUR_TOK(toks));
	if (toks->idx >= toks->len) return 0;
	tree->type = S_PROGRAM;
	int old_idx = push_stat(toks, tree);

	if (clause(toks, tree->left)) {
		if (program(toks, tree->right))
			return 1;
		free(tree->right);
		tree->right = NULL;
		return 1;
	}

	pop_stat(toks, tree, old_idx);

	return 0;
}
int predicate(toks_t *toks, syn_node_t *tree)
{
	if (is_predicate(CUR_TOK(toks))) {
		tree->type = S_PREDICATE;
		tree->left = tree->right = NULL;
		strcpy(tree->value, CUR_TOK(toks));
		toks->idx += 1;
		return 1;
	}
	return 0;
}
int constant(toks_t *toks, syn_node_t *tree)
{
	// printf("test constant: %s\n", CUR_TOK(toks));
	if (is_constant(CUR_TOK(toks))) {
		tree->type = S_CONSTANT;
		tree->left = tree->right = NULL;
		strcpy(tree->value, CUR_TOK(toks));
		toks->idx += 1;
		return 1;
	}

	int old_idx = toks->idx;
	if (token(toks, "[") && token(toks, "]")) {
		tree->type = S_CONSTANT;
		strcpy(tree->value, "[]");
		return 1;
	}
	toks->idx = old_idx;
	return 0;
}
int clause(toks_t *toks, syn_node_t *tree)
{
	// printf("test clause: %s\n", CUR_TOK(toks));
	tree->type = S_CLAUSE;
	int old_idx = push_stat(toks, tree);

	if (head(toks, tree->left)) {
		if (is_token(toks, ".")) {
			token(toks, ".");
			free(tree->right), tree->right = NULL;
			return 1;
		} else if (is_token(toks, ":")) {
			if (token(toks, ":") && token(toks, "-") && body(toks, tree->right) && token(toks, "."))
				return 1;
		}
	}

	pop_stat(toks, tree, old_idx);

	return 0;
}
int head(toks_t *toks, syn_node_t *tree)
{
	// printf("test head:%s\n", CUR_TOK(toks));
	tree->type = S_HEAD;
	int old_idx = push_stat(toks, tree);

	if (predicate(toks, tree->left)) {
		if (is_token(toks, "(")) {
			if (token(toks, "(") && list(toks, tree->right) && token(toks, ")"))
				return 1;
		} else {
			free(tree->right), tree->right = NULL;
			return 1;
		}
	}

	pop_stat(toks, tree, old_idx);
	return 0;
}
int body(toks_t *toks, syn_node_t *tree)
{
	tree->type = S_BODY;
	int old_idx = push_stat(toks, tree);
	// printf("test body: %s\n", CUR_TOK(toks));
	if (condition(toks, tree->left)) {
		if (is_token(toks, ",")) {
			if (token(toks, ",") && body(toks, tree->right))
				return 1;
		} else {
			free(tree->right), tree->right = NULL;
			return 1;
		}
	}

	pop_stat(toks, tree, old_idx);
	return 0;
}
int list(toks_t *toks, syn_node_t *tree)
{
	// printf("test list: %s\n", CUR_TOK(toks));
	tree->type = S_LIST;
	int old_idx = push_stat(toks, tree);

	if (element(toks, tree->left)) {
		if (is_token(toks, "|")) {
			token(toks, "|");
			if (element(toks, tree->right))
				return 1;
		} else if (is_token(toks, ",")) {
			token(toks, ",");
			if (list(toks, tree->right))
				return 1;
		} else {
			free(tree->right), tree->right = NULL;
			return 1;
		}
	}

	pop_stat(toks, tree, old_idx);
	return 0;
}
int condition(toks_t *toks, syn_node_t *tree)
{
	tree->type = S_CONDITION;
	int old_idx = push_stat(toks, tree);
	if (predicate(toks, tree->left)) {
		if (is_token(toks, "(")) {
			if (token(toks, "(") && list(toks, tree->right) && token(toks, ")")) {
				return 1;
			}
		} else {
			free(tree->right), tree->right = NULL;
			return 1;
		}
	}

	pop_stat(toks, tree, old_idx);
	return 0;
}
// not implemented.
int expression(toks_t *toks, syn_node_t *tree)
{
	return 0;
}
// not implemented.
int comparator(toks_t *toks, syn_node_t *tree)
{
	return 0;
}

int element(toks_t *toks, syn_node_t *tree)
{
	// printf("test element: %s\n", CUR_TOK(toks));
	tree->type = S_ELEM;
	int old_idx = toks->idx;

	if (structure(toks, tree))
		return 1;
	if (variable(toks, tree))
		return 1;
	if (constant(toks, tree))
		return 1;
	if (token(toks, "[") && list(toks, tree) && token(toks, "]"))
		return 1;

	toks->idx = old_idx;

	return 0;
}
int structure(toks_t *toks, syn_node_t *tree)
{
	// printf("test structure: %s\n", CUR_TOK(toks));
	tree->type = S_STRUCT;
	int old_idx = push_stat(toks, tree);

	if (predicate(toks, tree->left) && token(toks, "(") && list(toks, tree->right) && token(toks, ")")) {
	   	return 1;
	}
	toks->idx = old_idx;
	if (variable(toks, tree->left) && token(toks, "(") && list(toks, tree->right) && token(toks, ")")) {
		return 1;
	}

	pop_stat(toks, tree, old_idx);

	return 0;
}

int variable(toks_t *toks, syn_node_t *tree)
{
	// printf("test variable: %s\n", CUR_TOK(toks));
	if (is_variable(CUR_TOK(toks))) {
		tree->type = S_VARIABLE;
		tree->left = tree->right = NULL;
		strcpy(tree->value, CUR_TOK(toks));
		toks->idx += 1;
		return 1;
	}
	return 0;
}

int token(toks_t *toks, const char *token)
{
	if (strcmp(toks->tokens[toks->idx], token)) {
		return 0;
	} else {
		toks->idx += 1;
		return 1;
	}
}

int toks_add_token(toks_t *toks, const char *token, int len)
{
	toks->tokens[toks->len] = malloc(len + 1);
	strncpy(toks->tokens[toks->len], token, len);
	toks->tokens[toks->len][len] = 0;
	toks->len += 1;
	return 0;
}

toks_t *toks_init(const char *string)
{
	toks_t *toks = malloc(sizeof(toks_t));
	toks->idx = 0;
	toks->len = 0;

	int i, len = strlen(string);
	char temp[256], tlen = 0;

	for (i = 0; i < len; i++)
	{
		char c = string[i];
		if (c == '\'') {
			if (tlen != 0) {
				printf("init token stream error!\n");
				free(toks);
				return NULL;
			}
			temp[tlen++] = '\'';
			do {
				i += 1;
				temp[tlen++] = string[i];
				if (string[i] == '\'') break;
			} while (i < len - 1);
			if (string[i] != '\'') {
				printf("' not closed, init token stream error!\n");
				free(toks);
				return NULL;
			}
		} else if (c != ' ' && c != '\n' && c != '\t') {
			if (c == '(' || c == ')' || c == '[' || c == ']' ||
				c == ',' || c == '.' || c == '|' || c == '=' ||
		   		c == '<' || c == '>' || c == '%' || c == '\\' ||
			    c == '+' || c == '-' || c == '*' || c == '/')
			{
				if (tlen > 0)
					toks_add_token(toks, temp, tlen);
				temp[0] = c;
				toks_add_token(toks, temp, 1);
				tlen = 0;
			} else {
				temp[tlen++] = c;
			}
		} else {
			if (tlen > 0)
				toks_add_token(toks, temp, tlen);
			tlen = 0;
		}
	}
	if (tlen > 0)
		toks_add_token(toks, temp, tlen);
	tlen = 0;

	return toks;
}

void toks_info(toks_t *toks)
{
	int i;
	printf("token stream info:\n");
	printf("tokens count: %d\n", toks->len);
	for (i = 0; i < toks->len; i++) {
		printf("%03d: %s\n", i, toks->tokens[i]);
	}
}

int toks_destroy(toks_t *toks)
{
	int i;
	for (i = 0; i < toks->len; i++) {
		free(toks->tokens[i]);
	}
	free(toks);
	return 1;
}

int is_token(toks_t *toks, const char *token)
{
	return strcmp(toks->tokens[toks->idx], token) == 0;
}

char *deco_var(char *var)
{
	kv_t *kv;

	if (strlen(var) > 0 && strcmp(var, "_") != 0) {
		kv = kv_tbl_lookup(table, var);
		if (kv != NULL) {
			last_var = kv->strval;
			return kv->strval;
		}
	}

	char value[MAX_WORD_LEN];
	sprintf(value, "%c%d", var_prefix, table->len);
	kv = kv_tbl_insert(table, var, 0, value);
	last_var = kv->strval;

	return last_var;
}

int first_occur(char *var)
{
	return !kv_tbl_contains(table, var);
}

prog_t *syn_node_to_prog(syn_node_t *tree)
{
	// printf("[syn_node_to_prog begin] %s\n", NODE_NAMES(tree->type));
	if (tree == NULL) NULL;
	prog_t *prog = prog_init();

	switch (tree->type) {
		case S_PROGRAM:
			{
				if (tree->left == NULL) { free(prog); return NULL; };
				prog_add_node(prog, tree->left);
			   	prog_add_node(prog, tree->right);
			   	break;
			}
		case S_CONDITION:
			{
				body_calls += 1;
				if (tree->right != NULL) {
					syn_node_t *s = tree->right;
					int argcount = 0;
					while (s != NULL) {
						switch (s->left->type) {
							case S_CONSTANT:
								prog_add_stmt(prog, stmt_init("", OP_PUT_CONST, 2, s->left->value, cnt_arg(argcount)));
								break;
							case S_VARIABLE:
								if (var_prefix == 'Q' && first_occur(s->left->value))
									prog_add_stmt(prog, stmt_init("", OP_CREATE_VAR, 2, deco_var(s->left->value), s->left->value));
								prog_add_stmt(prog, stmt_init("", OP_PUT_VAL, 2, deco_var(s->left->value), cnt_arg(argcount)));
								break;
							default:
								{
									prog_add_node(prog, tree->right);
									prog_add_stmt(prog, stmt_init("", OP_PUT_VAL, 2, last_var, cnt_arg(argcount)));
									break;
								}
						}
						argcount += 1;
						s = s->right;
					}
				}
				printf("hehehehehehe %s\n", tree->left->value);
				prog_add_stmt(prog, stmt_init("", OP_CALL, 1, tree->left->value));
				break;
			}
		case S_HEAD:
			{
				int i, pos, total, m;
				char name[MAX_WORD_LEN];
				syn_node_t *pred_n = tree->left;
				m = 0;
				for (i = 0; pred_n->value[i] != '~'; i++)
					name[m++] = pred_n->value[i];
				name[m] = 0;
				m = 0;
				for (i = i+1; pred_n->value[i] != '/'; i++)
					temp[m++] = pred_n->value[i];
				temp[m] = 0;
				pos = atoi(temp);
				m = 0;
				for (i = i+1; pred_n->value[i]; i++)
					temp[m++] = pred_n->value[i];
				temp[m] = 0;
				total = atoi(temp);
				if (total == 1)
					strcpy(pred_n->value, name);
				else
					sprintf(pred_n->value, "%s~%d", name, pos);
				if (pos < total) {
					sprintf(temp, "%s~%d", name, pos + 1);
					if (pos > 1) {
						prog_add_stmt(prog, stmt_init(pred_n->value, OP_RTRY_ME_ELSE, 1, temp));
					} else {
						prog_add_stmt(prog, stmt_init(pred_n->value, OP_TRY_ME_ELSE, 1, temp));
					}
				} else {
					prog_add_stmt(prog, stmt_init(pred_n->value, OP_TRUST_ME, 0));
				}

				if (tree->right != NULL) {
					syn_node_t *s = tree->right;
					int argcount = 0;

					while (s != NULL) {
						if (s->left->type == S_CONSTANT) {
							prog_add_stmt(prog, stmt_init("", OP_GET_CONST, 2, s->left->value, cnt_arg(argcount)));
						}
						else if (s->left->type == S_VARIABLE) {
							if (first_occur(s->left->value))
								prog_add_stmt(prog, stmt_init("", OP_GET_VAR, 2, deco_var(s->left->value), cnt_arg(argcount)));
							else prog_add_stmt(prog, stmt_init("", OP_GET_VAL, 2, deco_var(s->left->value), cnt_arg(argcount)));
						} else {
							char *decor = deco_var("");
							prog_add_stmt(prog, stmt_init("", OP_GET_VAR, 2, decor, cnt_arg(argcount)));

							prog_add_node(prog, s->left);
							prog_add_stmt(prog, stmt_init("", OP_UNI_VAR, 2, decor, last_var));
						}
						argcount++;
						s = s->right;
					}
				}
				break;
			}
		case S_CONSTANT:
			prog_add_stmt(prog, stmt_init("", OP_PUT_CONST, 2, tree->value, deco_var("")));
			break;
		case S_VARIABLE:
			if (var_prefix == 'Q' && first_occur(tree->value))
				prog_add_stmt(prog, stmt_init("", OP_CREATE_VAR, 2, deco_var(tree->value), tree->value));
			deco_var(tree->value);
			printf("variable exit\n");
			break;
		case S_LIST:
			if (tree->left != NULL) {
				prog_add_node(prog, tree->left);
				char lvar[MAX_WORD_LEN], rvar[MAX_WORD_LEN];
				if (tree->left->type == S_VARIABLE)
					strcpy(lvar, deco_var(tree->left->value));
				else strcpy(lvar, last_var);
				if (tree->right == NULL) {
					strcpy(rvar, deco_var(""));
					prog_add_stmt(prog, stmt_init("", OP_PUT_CONST, 2, "[]", rvar));
				} else {
					prog_add_node(prog, tree->right);
					strcpy(rvar, last_var);
				}
				prog_add_stmt(prog, stmt_init("", OP_UNI_LIST, 3, deco_var(""), lvar, rvar));
				return prog;
			} else {
				prog_add_stmt(prog, stmt_init("", OP_PUT_CONST, 2, "[]", deco_var("")));
			}
			break;
		case S_STRUCT:
			{
				char lvar[MAX_WORD_LEN], rvar[MAX_WORD_LEN];
				printf("begin add tree->left\n");
				prog_add_node(prog, tree->left);
				strcpy(lvar, last_var);
				printf("added tree->left\n");
				prog_add_node(prog, tree->right);
				printf("added tree->right\n");
				strcpy(rvar, last_var);
				prog_add_stmt(prog, stmt_init("", OP_UNI_STRUC, 3, deco_var(""), lvar, rvar));
				return prog;
			}
			break;
		case S_CLAUSE:
			{
				if (table != NULL) {
					kv_tbl_destroy(table);
					table = NULL;
				}
				table = kv_tbl_init();
				body_calls = 0;

				prog_add_node(prog, tree->left);
				prog_add_node(prog, tree->right);

				if (table->len > 0 || body_calls > 0) {
					prog_add_stmt_at(prog, stmt_init("", OP_ALLOC, 0), 1);
					prog_add_stmt(prog, stmt_init("", OP_DEALLOC, 0));
				}
				prog_add_stmt(prog, stmt_init("", OP_PROCEED, 0));
			}
			break;
		case S_BODY:
			{
				syn_node_t *s = tree;
				while (s != NULL) {
					prog_add_node(prog, s->left);
					s = s->right;
				}
			}
			break;
		case S_QUERY:
			{
				if (tree->left == NULL) {
					free(prog);
					return NULL;
				}
				prog_add_node(prog, tree->left);
				if (tree->right != NULL)
					prog_add_node(prog, tree->right);
				stmt_t *stmt = stmt_init("", OP_HALT, 0);
				prog_add_stmt(prog, stmt);
			}
			break;
		case S_PREDICATE:
			break;
		default:
			printf("syn_node_to_prog panic [%s]!\n", NODE_NAMES(tree->type));
			break;
	}

	return prog;
}

void compiler_begin()
{
	table = kv_tbl_init();
}

void compiler_end()
{
	if (table == NULL) {
		printf("compiler_end(): wierd, table is NULL.\n");
		return;
	}
}
