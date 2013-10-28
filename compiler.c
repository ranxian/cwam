#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define is_anum(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
#define SEQL(s1, s2) (strcmp((s1), (s2)) == 0)
#define is_comp(s) (SEQL((s), ">") || SEQL((s), "<") || SEQL((s), ">=") || SEQL((s), "<=") || \
					SEQL((s), "!=") || SEQL((s), "=="))
#define PUSH_PARSE(tree, toks) (\
	int _old_idx = (toks)->idx; \
	(tree)->left = malloc(sizeof(syn_node_t)); \
	(tree)->right = malloc(sizeof(syn_node_t));)
#define POP_PARSE(tree, toks) (\
	free((tree)->right); \
	free((tree)->left); \
	(toks)->idx = _old_idx;)

/* test if s is a constant */
int is_constant(const char *s)
{
	int len = strlen(s), i;
	if (s[0] == '[' && s[1] == ']' && s[2] == 0)
		return 1;
	if (s[0] >= 'a' && s[0] <= 'z') {
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
	int i;
	if (s[0] == '_' && s[1] == 0)
		return 1;

	if (s[0] >= 'A' && s[0] <= 'Z') {
		int len = strlen(s);
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

int program(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	if (clause(toks, tree->left)) {
		if (program(toks, tree->right))
			return 1;
		free(tree->right);
		tree->right = NULL;
		return 1;
	}

	free(tree->right);
	free(tree->left);
	tree->right = tree->left = NULL;
	toks->idx = old_idx;
}
int predicate(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int constant(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int clause(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_CLAUSE;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	if (head(toks, tree->left)) {
		if (token(toks, ".")) {
			free(tree->right);
			tree->right = NULL;
			return 1;
		} else if (token(toks, ":") && token(toks, "-") && body(toks, tree->right)) {
			return 1;
		}
	}

	free(tree->right);
	free(tree->left);
	tree->right = tree->left = NULL;
	toks->idx = old_idx;
}
int head(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_HEAD;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	if (predicate(toks, tree->left)) {
		int old_idx2 = toks->idx;
		if (token(toks, "(") && list(toks, tree->right) && token(toks, ")")) {
			return 1;
		}
		toks->idx = old_idx2;
		free(tree->right);
		tree->right = NULL;
		return 1;
	}

	free(tree->right);
	free(tree->left);
	tree->right = tree->left = NULL;
	toks->idx = old_idx;
}
int body(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int list(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int condition(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int expression(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int comparator(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int structure(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}
int variable(tok_stream_t *toks, syn_node_t *tree)
{
	tree->type = S_PROGRAM;
	int old_idx = toks->idx;
	tree->left = malloc(sizeof(syn_node_t));
	tree->right = malloc(sizeof(syn_node_t));

	free(tree->right);
	free(tree->left);
	toks->idx = old_idx;
}

int token(tok_stream_t *toks, const char *token)
{
	if (strcmp(toks->tokens[toks->idx], token)) {
		return -1;
	} else {
		toks->idx += 1;
		return 1;
	}
}
int toks_add_token(tok_stream_t *toks, const char *token, int len)
{
	toks->tokens[toks->len] = malloc(len + 1);
	strncpy(toks->tokens[toks->len], token, len);
	toks->tokens[toks->len][len] = 0;
	toks->len += 1;
}

int toks_init_from_string(tok_stream_t *toks, const char *string)
{
	toks->idx = 0;
	toks->len = 0;

	int i, len = strlen(string);
	char c, temp[256], tlen = 0;

	for (i = 0; i < len; i++)
	{
		c = string[i];
		if (c == '\'') {
			if (tlen != 0) {
				printf("init token stream error!\n");
				return -1;
			}
			temp[tlen++] = '\'';
			do {
				i += 1;
				temp[tlen++] = string[i];
				if (string[i] == '\'') break;
			} while (i < len - 1);
			if (string[i] != '\'') {
				printf("' not closed, init token stream error!\n");
				return -1;
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

	return 1;
}

void toks_info(tok_stream_t *toks)
{
	int i;
	printf("token stream info:\n");
	printf("tokens count: %d\n", toks->len);
	for (i = 0; i < toks->len; i++) {
		printf("%03d: %s\n", i, toks->tokens[i]);
	}
}

int toks_destroy(tok_stream_t *toks)
{
	int i;
	for (i = 0; i < toks->len; i++) {
		free(toks->tokens[i]);
	}
	return 1;
}

