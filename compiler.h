#ifndef COMPILER_H
#define COMPILER_H
#define MAX_TOK_LEN 1024
#include "syntaxtree.h"

typedef struct {
	char *tokens[MAX_TOK_LEN];
	int idx;
	int len;
} tok_stream_t;

int is_constant(const char *s);
int is_num(const char* s);
int is_variable(const char *s);
int is_predicate(const char *s);

int predicate(tok_stream_t *toks, syn_node_t *tree);
int constant(tok_stream_t *toks, syn_node_t *tree);
int program(tok_stream_t *toks, syn_node_t *tree);
int clause(tok_stream_t *toks, syn_node_t *tree);
int head(tok_stream_t *toks, syn_node_t *tree);
int body(tok_stream_t *toks, syn_node_t *tree);
int list(tok_stream_t *toks, syn_node_t *tree);
int condition(tok_stream_t *toks, syn_node_t *tree);
int expression(tok_stream_t *toks, syn_node_t *tree);
int comparator(tok_stream_t *toks, syn_node_t *tree);
int structure(tok_stream_t *toks, syn_node_t *tree);
int variable(tok_stream_t *toks, syn_node_t *tree);

int toks_init_from_string(tok_stream_t *toks, const char *string);
int token(tok_stream_t *toks, const char *token);
int toks_add_token(tok_stream_t *toks, const char *token, int len);
void toks_info(tok_stream_t *toks);
int toks_destroy(tok_stream_t *toks);
#endif
