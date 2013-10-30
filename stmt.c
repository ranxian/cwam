#include "stmt.h"
#include <string.h>
#include <stdio.h>

stmt_t *stmt_init(char *label, wam_op_t op, int narg, ...)
{
	stmt_t *stmt;
	strcpy(stmt->label, label);
	stmt->op = op;
	stmt->jump = -1;

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
	if (strcmp(stmt->label, ";") == 0)
		printf("; %d\n", stmt->op);

	char buf[MAX_LINE_LEN] = {};

	if (stmt->label[0] != 0) {
		sprintf(buf, "%12s: ", stmt->label);
	} else {
		sprintf(buf, "            ");
	}

	sprintf(buf, "%s%d", buf, stmt->op);
	int i;

	for (i = 0; i < stmt->narg; i++) {
		sprintf(buf, "%s %s", buf, stmt->args[i]);
	}

	if (stmt->jump >= 0)
		sprintf(buf, "%s (%d)", buf, stmt->jump);

	printf("%s\n", buf);
}
