#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#define MAX_VAL_LEN 256

typedef enum { S_CLAUSE, S_PROGRAM, S_HEAD, S_BODY, S_PREDICATE, S_LIST, S_CONDITION,
				S_VARIABLE, S_EXPR, S_ELEM, S_COMP, S_STRUCT, S_CONSTANT, S_NONE } syn_node_type_t;

typedef struct syn_node_t {
	struct syn_node_t *left;
	struct syn_node_t *right;
	char value[MAX_VAL_LEN];
	syn_node_type_t type;
} syn_node_t;

void syn_node_traverse(syn_node_t *node);

void syn_node_to_code(syn_node_t *tree, char code[]);

#endif
