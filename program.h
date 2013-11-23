#ifndef PROGRAM_H
#define PROGRAM_H
#define MAX_STATEMENTS_NUM 1024
#define MAX_LABEL_LEN 128
#include "stmt.h"
#include "syntaxtree.h"
#include "defs.h"
#include "kv.h"

typedef struct  {
	stmt_t *stmts[MAX_STATEMENTS_NUM];
	kv_tbl_t *labels;
	int nstmt;
} prog_t;

prog_t *prog_init();

void prog_destroy(prog_t *prog);

int prog_add_stmt(prog_t *prog, stmt_t *stmt);

int prog_add_stmt_at(prog_t *prog, stmt_t *stmt, int pos);

int prog_del_from_line(prog_t *prog, int lineno);

int prog_del_from_label(prog_t *prog, char *label);

int prog_get_last_clause(prog_t *prog, char *proc_name, int butone);

int prog_add_clause(prog_t *prog, char *label, prog_t *adding);

int prog_locate_label(prog_t *prog, char *label);

int prog_update_label(prog_t *prog);

int prog_add_node(prog_t *prog, syn_node_t *node);

int prog_add_prog(prog_t *prog, prog_t *p);

void prog_info(prog_t *prog);
#endif
