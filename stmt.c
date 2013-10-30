#include "stmt.h"
#include <string.h>
#include <stdio.h>

stmt_t *stmt_init(char *label, wam_op_t op, int narg, ...)
{
	stmt_t *stmt;
	strcpy(stmt->label, label);
	stmt->op = op;

	va_list ap;
	va_start(ap, narg);

	int i;
	for (i = 0; i < narg; i++) {
		char *arg = va_arg(ap, char *);
		stmt->args[i] = malloc(sizeof(arg) + 1);
		strcpy(stmt->args[i], arg);
	}
	stmt->narg = narg;
	return stmt;
}

void stmt_destroy(stmt_t *stmt)
{
	int i;
	for (i = 0; i < stmt->narg; i++) {
		free(stmt->args[i]);
	}
	free(stmt);
}

void stmt_info(stmt_t *stmt)
{
	printf("not implemented\n");
}
