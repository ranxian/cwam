#include "program.h"

prog_t *prog_init() { return NULL; }

void prog_destroy(prog_t *prog) {  }

int prog_add_prog(prog_t *prog, prog_t *adding){ return 0; }

int prog_add_stmt(prog_t *prog, stmt_t *stmt){return 0; }

int prog_add_stmt_at(prog_t *prog, int pos){ return 0; }

int prog_del_from_line(prog_t *prog, int lineno){ return 0; }

int prog_del_from_label(prog_t *prog, const char *label){ return 0; }

int prog_get_last_clause(prog_t *prog, const char *proc_name, int butone){ return 0; }

int prog_add_clause(prog_t *prog, const char *label, prog_t *adding){ return 0; }

int prog_locate_label(prog_t *prog, const char *label){ return 0; }

int prog_update_label(prog_t *prog){ return 0; }
