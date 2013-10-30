#include "program.h"
#include "compiler.h"
#include <string.h>
#include <stdio.h>

prog_t *prog_init()
{
	prog_t *prog = malloc(sizeof(prog_t));
	prog->nlabel = prog->nstmt = 0;

	return prog;
}

void prog_destroy(prog_t *prog)
{
	int i;

	for (i = 0; i < prog->nstmt; i++)
		stmt_destroy(prog->stmts[i]);

	free(prog);
}

static int prog_contains_label(prog_t *prog, char *label)
{
	int i;
	for (i = 0; i < prog->nlabel; i++) {
		if (strcmp(prog->labels[i], label) == 0)
			return 1;
	}
	return 0;
}

int prog_add_node(prog_t *prog, syn_node_t *node)
{
	if (node == NULL) return -1;
	prog_t *p = syn_node_to_prog(node);

	int can_add = 1;
	int i;

	for (i = 0; i < p->nstmt; i++) {
		stmt_t *stmt = p->stmts[i];
		char *label = stmt->label;
		if (strlen(label) > 0) {
			if (prog_contains_label(prog, label)) {
				can_add = 0;
				break;
			}
		} else {
			strcpy(prog->labels[prog->nlabel], label);
			prog->nlabel += 1;
			can_add = 1;
		}
		if (can_add) {
			prog_add_stmt(prog, stmt);
		}
	}

	prog_destroy(p);
	return 0;
}

int prog_add_stmt(prog_t *prog, stmt_t *stmt)
{
	prog->stmts[prog->nstmt] = stmt_copy(stmt);
	prog->nstmt += 1;
	return 0;
}

int prog_add_stmt_at(prog_t *prog, stmt_t *stmt, int pos)
{
	int i;
	for (i = prog->nstmt; i > pos; i--)
		prog->stmts[i] = prog->stmts[i - 1];
	prog->stmts[pos] = malloc(sizeof(stmt_t));
	memcpy(prog->stmts[pos], stmt, sizeof(stmt_t));
	prog->nstmt += 1;
	return 0;
}

int prog_del_from_line(prog_t *prog, int lineno){ return 0; }

int prog_del_from_label(prog_t *prog, const char *label){ return 0; }

int prog_get_last_clause(prog_t *prog, const char *proc_name, int butone){ return 0; }

int prog_add_clause(prog_t *prog, const char *label, prog_t *adding){ return 0; }

int prog_locate_label(prog_t *prog, const char *label){ return 0; }

int prog_update_label(prog_t *prog){ return 0; }

void prog_info(prog_t *prog)
{
	int i;
	for (i = 0; i < prog->nstmt; i++) {
		printf("%3d:\t", i);
		stmt_info(prog->stmts[i]);
	}
}
