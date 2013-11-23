#include "machine.h"
#include "stmt.h"
#include <stdio.h>

char *OP_NAMES(wam_op_t op)
{
	static char *names[] = 
	{
		"allocate",
		"deallocate",
		"call",
		"not_call",
		"create_variable",
		"get_constant",
	   	"get_value",
		"get_variable",
		"get_level",
		"put_value",
		"put_variable",
		"put_constant",
		"halt",
		"noop",
		"proceed",
		"rtry_me_else",
		"trust_me",
		"try_me_else",
		"unify_list",
		"unify_structrue",
		"unify_variable"
	};

	return names[op];
}

int create_variable(wam_t *wam, char *regname, char *varname) { return 0; }
int get_variable(wam_t *wam, char *regname1, char *regname2) { return 0; }
int get_constant(wam_t *wam, char *constname, char *regname) { return 0; }
int get_value(wam_t *wam, char *regname, char* valname) { return 0; }
int unify_variable2(wam_t *wam, variable_t *v1, variable_t *v2) { return 0; }
int unify_struc2(wam_t *wam, variable_t *struc, variable_t *head, variable_t* tail) { return 0; }
int unify_variable(wam_t *wam, char *v1name, char *v2name) { return 0; }
int unify_struc(wam_t *wam,char *strucname, char *headname, char* tailname) { return 0; }
int unify_list(wam_t *wam, char *listname, char *headname, char *tailname) { return 0; }
int put_constant(wam_t *wam, char *constname, char *regname) { return 0; }
int put_list(wam_t *wam, char *headname, char *tailname, char *argname) { return 0; }
int put_value(wam_t *wam, char *varname, char *argname) { return 0; }
int put_variable(wam_t *wam, char *varname, char *argname) { return 0; }
int try_me_else(wam_t *wam, int next) { return 0; }
int proceed(wam_t *wam) { return 0; }
int is_bound(wam_t *wam, variable_t *var) { return 0; }
int allocate(wam_t *wam) { return 0; }
int deallocate(wam_t *wam) { return 0; }
int call(wam_t *wam, int target) { return 0; }

int wam_reset(wam_t *wam) { return 0; }
int wam_backtrack(wam_t *wam) { return 0; }
int wam_intpred(wam_t *wam, wam_call_t *call) { return 0; }
int wam_load(wam_t *wam, char *filename) { return 0; }
int wam_consult(wam_t *wam, char *filename) { return 0; }

int wam_run_query(wam_t *wam, char *query_str) { return 1; }

wam_t *wam_init(prog_t *prog) { return NULL; }

int wam_run(wam_t *wam)
{
	wam->failed = 1;
	wam->opcnt = 0;
	while (wam->pc > 0) {
		wam->failed = 0;
		stmt_t *stmt = wam->p->stmts[wam->pc];
		printf("PC: %d, stmt: %s\n", wam->pc, stmt->label);
		if (wam->opcnt > wam->maxopcnt) {
			printf("panic: maximum opcnt reached\n");
			wam->failed = 1;
			break;
		}

		switch (stmt->op) {
			case OP_ALLOC:
				allocate(wam); break;
			case OP_DEALLOC:
				deallocate(wam); break;
			case OP_CALL:
				call(wam, stmt->jump); break;
			case OP_CREATE_VAR:
				create_variable(wam, stmt->args[0], stmt->args[1]); break;
			case OP_GET_CONST:
				get_constant(wam, stmt->args[0], stmt->args[1]); break;
			case OP_GET_VAL:
				get_value(wam, stmt->args[0], stmt->args[1]); break;
			case OP_GET_VAR:
				get_variable(wam, stmt->args[0], stmt->args[1]); break;
			case OP_PUT_VAL:
				put_value(wam, stmt->args[0], stmt->args[1]); break;
			case OP_PUT_VAR:
				put_variable(wam, stmt->args[0], stmt->args[1]); break;
			case OP_PUT_CONST:
				put_constant(wam, stmt->args[0], stmt->args[1]); break;
			case OP_HALT:
				break;
			case OP_NOOP:
				wam->pc += 1; break;
			case OP_PROCEED:
				proceed(wam); break;
			case OP_RTRY_ME_ELSE:
				try_me_else(wam, stmt->jump); break;
			case OP_TRUST_ME:
				wam->pc++; break;
			case OP_UNI_STRUC:
				unify_struc(wam, stmt->args[0], stmt->args[1], stmt->args[2]); break;
			case OP_UNI_VAR:
				unify_variable(wam, stmt->args[0], stmt->args[1]); break;
			case OP_UNI_LIST:
				unify_list(wam, stmt->args[0], stmt->args[1], stmt->args[2]); break;
			default:
				printf("unknown wam op in line %d!\n", wam->pc); wam_backtrack(wam); break;
		}

		if (wam->failed) {
			while (wam->cp != NULL) wam_backtrack(wam);
			wam_backtrack(wam);
		}
	}
	return 0;
}
