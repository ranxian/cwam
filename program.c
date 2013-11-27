#include "program.h"
#include "compiler.h"
#include "kv.h"
#include <string.h>
#include <stdio.h>

prog_t *prog_init()
{
	prog_t *prog = malloc(sizeof(prog_t));
	prog->labels = kv_tbl_init();
	prog->nstmt = 0;
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
	return kv_tbl_contains(prog->labels, label);
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
			} else {
				kv_tbl_insert(prog->labels, label, prog->nstmt, NULL);
				can_add = 1;
			}
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

int prog_del_from_line(prog_t *prog, int line)
{
	int i;
	int delcnt = 1;
	for (i = line+1; ; i++) {
		if (prog->stmts[i]->label[0] != 0)
			break;
		delcnt += 1;
	}
	for (i = line; i < prog->nstmt - delcnt; i++) {
		prog->stmts[i] = prog->stmts[i+delcnt];
	}
	prog->nstmt -= delcnt;
	return delcnt;
}

int prog_del_from_label(prog_t *prog, char *label)
{
	int line = prog_locate_label(prog, label);
	int result = 0;
	printf("delete %s at line %d\n", label, line);
	if (line >= 0) {
		result = prog_del_from_line(prog, line);
		kv_tbl_remove(prog->labels, label);
	}
	return result;
}

int prog_get_last_clause(prog_t *prog, char *label, int butone)
{
	int last2 = -1;
	int line = prog_locate_label(prog, label);
	if (line >= 0) {
		stmt_t *stmt;
		do {
			stmt = prog->stmts[line];
			if (stmt->op == OP_TRY_ME_ELSE || stmt->op == OP_RTRY_ME_ELSE) {
				last2 = line;
				line = stmt->jump;
			} else break;
		} while (1);
	}
	if (butone) return last2;
	else return line;
}

int prog_add_clause(prog_t *prog, char *label, prog_t *adding)
{
	int line = prog_get_last_clause(prog, label, 0);
	int cnt = -1;
	if (line >= 0) {
		int index = 0;
		stmt_t *stmt = prog->stmts[line];
		for (index = 0; stmt->label[index] && stmt->label[index] != '~'; index++)
			;
		if (index < strlen(stmt->label)) {
			char no[16] = {};
			int i, m = 0;
			for (i = index + 1; stmt->label[i]; i++)
				no[m++] = stmt->label[i];
			no[m] = 0;
			cnt = atoi(no);
		} else {
			cnt = 2;
		}
		sprintf(adding->stmts[0]->label, "%s~%d", label, cnt);
		stmt->op = OP_TRY_ME_ELSE;
		strcpy(stmt->args[0], adding->stmts[0]->label);
		stmt->jump = prog->nstmt;
		prog_add_prog(prog, adding);
	} else {
		prog_add_prog(prog, adding);
		prog_update_label(prog);
	}
	return 0;
}

int prog_locate_label(prog_t *prog, char *label)
{
	kv_t *kv = kv_tbl_lookup(prog->labels, label);
	if (kv == NULL)
		return -1;
	else return kv->intval;
}

int prog_update_label(prog_t *prog)
{
	int i;
	prog_info(prog);
	kv_tbl_t *table = kv_tbl_init();
	for (i = 0; i < prog->nstmt; i++) {
		if (prog->stmts[i]->label[0]) { // has a label
			kv_tbl_insert(table, prog->stmts[i]->label, i, NULL);
		}
	}

	for (i = 0; i < prog->nstmt; i++) {
		stmt_t *stmt = prog->stmts[i];
		if (stmt->op == OP_CALL || stmt->op == OP_RTRY_ME_ELSE ||
			stmt->op == OP_TRY_ME_ELSE || stmt->op == OP_NOT_CALL) {
			char *label = stmt->args[0];
			stmt->jump = -1;
			if (kv_tbl_contains(table, label)) {
				stmt->jump = kv_tbl_lookup(table, label)->intval;
			} else {
				if (!strcmp(label, "call")) {
					stmt->jump = CALL_CALL;
				} else if (!strcmp(label, "consult")) {
					stmt->jump = CALL_CONSULT;
				} else if (!strcmp(label, "reconsult")) {
					stmt->jump = CALL_RECONSULT;
				} else if (!strcmp(label, "load")) {
					stmt->jump = CALL_LOAD;
				}
			}
		}
	}
	return 0;
}

int prog_add_prog(prog_t *prog, prog_t *p)
{
	int can_add = 1;
	int i;

	for (i = 0; i < p->nstmt; i++) {
		stmt_t *stmt = p->stmts[i];
		char *label = stmt->label;
		if (strlen(label) > 0) {
			if (prog_contains_label(prog, label)) {
				can_add = 0;
			} else {
				kv_tbl_insert(prog->labels, label, prog->nstmt, NULL);
				can_add = 1;
			}
		}
		if (can_add) {
			prog_add_stmt(prog, stmt);
		}
	}

	return 0;
}

void prog_info(prog_t *prog)
{
	int i;
	for (i = 0; i < prog->nstmt; i++) {
		printf("%3d:\t", i);
		stmt_info(prog->stmts[i]);
	}
}
