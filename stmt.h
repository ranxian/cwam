#ifndef STMT_H
#define STMT_H

#include "machine.h"
#include "defs.h"
#include "stdarg.h"
#include <stdlib.h>
#define MAX_OPR_NUM 16

typedef struct {
	char label[MAX_WORD_LEN];
	wam_op_t op;
	char **args;
	int narg;
	int jump;
} stmt_t;

stmt_t *stmt_init(char *label, wam_op_t op, int narg, ...);

void stmt_destroy(stmt_t *stmt);

void stmt_info(stmt_t *stmt);

#endif
