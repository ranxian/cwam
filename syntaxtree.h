#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include "defs.h"

typedef enum { S_CLAUSE, S_PROGRAM, S_HEAD, S_BODY, S_PREDICATE, S_LIST, S_CONDITION,
				S_VARIABLE, S_EXPR, S_ELEM, S_COMP, S_STRUCT, S_CONSTANT, S_NONE, S_CALL, S_NON_CALL, S_QUERY } syn_node_type_t;

typedef struct syn_node_t {
	struct syn_node_t *left;
	struct syn_node_t *right;
	char value[MAX_VAL_LEN];
	syn_node_type_t type;
} syn_node_t;

void syn_node_traverse(syn_node_t *node);

syn_node_t *syn_node_init();

void syn_node_destroy(syn_node_t *);

char *NODE_NAMES(syn_node_type_t type);

#endif
