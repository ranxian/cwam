#ifndef MACHINE_H
#define MACHINE_H

typedef enum {
	OP_ALLOC,
	OP_DEALLOC,
	OP_CALL,
	OP_NOT_CALL,
	OP_CREATE_VAR,
	OP_GET_CONST,
   	OP_GET_VAL,
	OP_GET_VAR,
	OP_GET_LVL,
	OP_PUT_VAL,
	OP_PUT_VAR,
	OP_PUT_CONST,
	OP_HALT,
	OP_NOOP,
	OP_PROCEED,
	OP_RTRY_ME_ELSE,
	OP_TRUST_ME,
	OP_TRY_ME_ELSE,
	OP_UNI_LIST,
	OP_UNI_STRUC,
	OP_UNI_VAR
} wam_op_t;

typedef enum {
	CALL
} wam_call_t;

char *OP_NAMES(wam_op_t op);

#ifndef VARIABLE_H
#define VARIABLE_H
#include "defs.h"
typedef enum { REF, CON, LIS, STR } tag_t;

typedef struct cell_t {
	tag_t tag;
	char value[MAX_WORD_LEN];
	struct cell_t *ref;
	char name[MAX_WORD_LEN];
} cell_t;

char *TAG_NAMES(tag_t tag);
cell_t *cell_init_as_unbound(char *name);
cell_t *cell_init_as_const(char *name, char *value);
cell_t *cell_init_as_bounded(char *name, cell_t *v);
int cell_copy(cell_t *src, cell_t *dst);
cell_t *deref(cell_t *cell);
void cell_print(cell_t *cell);
#endif


#endif
