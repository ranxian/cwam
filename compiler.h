#ifndef COMPILER_H
#define COMPILER_H
#include "syntaxtree.h"
#include "stmt.h"
#include "program.h"

typedef struct {
	char *tokens[MAX_TOK_LEN];
	int idx;
	int len;
} toks_t;

int is_constant(const char *s);
int is_num(const char* s);
int is_variable(const char *s);
int is_predicate(const char *s);

int predicate(toks_t *toks, syn_node_t *tree);
int constant(toks_t *toks, syn_node_t *tree);
int program(toks_t *toks, syn_node_t *tree);
int clause(toks_t *toks, syn_node_t *tree);
int head(toks_t *toks, syn_node_t *tree);
int body(toks_t *toks, syn_node_t *tree);
int list(toks_t *toks, syn_node_t *tree);
int condition(toks_t *toks, syn_node_t *tree);
int expression(toks_t *toks, syn_node_t *tree);
int comparator(toks_t *toks, syn_node_t *tree);
int structure(toks_t *toks, syn_node_t *tree);
int variable(toks_t *toks, syn_node_t *tree);
int element(toks_t *toks, syn_node_t *tree);

toks_t *toks_init(const char *string);
int token(toks_t *toks, const char *token);
int is_token(toks_t *toks, const char *token);
int toks_add_token(toks_t *toks, const char *token, int len);
void toks_info(toks_t *toks);
int toks_destroy(toks_t *toks);

int push_stat(toks_t *toks, syn_node_t *tree);
int pop_stat(toks_t *toks, syn_node_t *tree, int old_idx);

char var_prefix = 'Q';
prog_t *syn_node_to_prog(syn_node_t *tree);

void compiler_begin();
void compiler_end();

#endif
