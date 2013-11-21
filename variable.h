#ifndef VARIABLE_H
#define VARIABLE_H
#include "defs.h"
typedef enum { REF, CON, LIS, STR } tag_t;

typedef struct variable_t {
	tag_t tag;
	char value[MAX_WORD_LEN];
	struct variable_t *ref;
	char name[MAX_WORD_LEN];
} variable_t;

variable_t *var_init_as_unbound(char *name);
variable_t *var_init_as_const(char *name, char *value);
variable_t *var_init_as_bounded(char *name, variable_t *v);
int var_copy(variable_t *src, variable_t *dst);
variable_t *deref(variable_t *var);
void var_print(variable_t *var);
#endif
