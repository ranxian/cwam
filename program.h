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

// 初始化 prog
prog_t *prog_init();
// 销毁 prog
void prog_destroy(prog_t *prog);
// 在指令序列的最后 append 一条指令
int prog_add_stmt(prog_t *prog, stmt_t *stmt);
// 在 pos 处插入一条指令
int prog_add_stmt_at(prog_t *prog, stmt_t *stmt, int pos);
// 删除第 line 行的指令
int prog_del_from_line(prog_t *prog, int lineno);
// 删除 label 所在的指令块（如一个 Clause 产生的所有指令）
int prog_del_from_label(prog_t *prog, char *label);
// 返回 label 在指令序列中最后一次出现的地址，如果 butone 为 1 就返回倒数第二次
// 出现的地址
int prog_get_last_clause(prog_t *prog, char *prog_name, int butone);
// 添加整个 Clause 的指令序列
int prog_add_clause(prog_t *prog, char *label, prog_t *adding);
// 定位 label 指向的指令所在的地址
int prog_locate_label(prog_t *prog, char *label);
// 根据 label 的所在的位置更新指令序列中跳转目标
int prog_update_label(prog_t *prog);
// 在指令序列的最后添加 node 所生成的所有指令
int prog_add_node(prog_t *prog, syn_node_t *node);
// 合并两个指令序列
int prog_add_prog(prog_t *prog, prog_t *p);

void prog_info(prog_t *prog);
#endif