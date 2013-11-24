#include "machine.h"
#include "stmt.h"
#include "compiler.h"
#include <stdio.h>
#include <string.h>
#include "helper.h"
#define STREQL(s1, s2) (strcmp(s1, s2) == 0)
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
int unify_variable2(wam_t *wam, var_t *v1, var_t *v2) { return 0; }
int unify_struc2(wam_t *wam, var_t *struc, var_t *head, var_t* tail) { return 0; }
int unify_variable(wam_t *wam, char *v1name, char *v2name) { return 0; }
int unify_struc(wam_t *wam,char *strucname, char *headname, char* tailname) { return 0; }
int unify_list(wam_t *wam, char *listname, char *headname, char *tailname) { return 0; }
int put_constant(wam_t *wam, char *constname, char *regname) { return 0; }
int put_list(wam_t *wam, char *headname, char *tailname, char *argname) { return 0; }
int put_value(wam_t *wam, char *varname, char *argname) { return 0; }
int put_variable(wam_t *wam, char *varname, char *argname) { return 0; }
int try_me_else(wam_t *wam, int next) { return 0; }
int proceed(wam_t *wam) { return 0; }
int is_bound(wam_t *wam, var_t *var) { return 0; }
int allocate(wam_t *wam) { return 0; }
int deallocate(wam_t *wam) { return 0; }

int wam_call(wam_t *wam, int target)
{
	if (target >= 0) {
		wam->ctnptr = wam->pc + 1;
		wam->pc = target;
	} else if (!wam_intpred(wam, target)) {
		wam_backtrack(wam);
	}
	return 0;
}

int wam_backtrack(wam_t *wam)
{
	int i;
	wam->ctnptr++;
	wam->failed = 1;
	if (wam->cp != NULL) {
		wam->ctnptr = wam->cp->retA;
		wam->pc = wam->cp->nextclause;
		wam->env = wam->cp->lastenv;
		memcpy(wam->args, wam->cp->args, sizeof(wam->args));
		wam->cp = wam->cp->lastcp;
	} else {
		wam->pc -= 1;
	}
	return 0;
}

int wam_intpred(wam_t *wam, int call)
{
	prog_t *prog = wam->prog;
	int result = 1;
	var_t *v = wam->args[0];
	switch (call) {
		case CALL_CALL:
		{
			var_t *v2 = deref(v);
			int target = -1;
			if (v2->tag == CON) {
				kv_t *kv = kv_tbl_lookup(prog->labels, v2->value);
				if (kv != NULL)
					target = kv->intval;
			} else if (v2->tag == STR) {
				kv_t *kv = kv_tbl_lookup(prog->labels, v2->head->value);
				if (kv != NULL) {
					target = kv->intval;
					var_t *tail = v2->tail;
					int cnt = 0;
					while (tail != NULL) {
						char temp[64];
						sprintf(temp, "A%d", cnt);
						var_t *v3 = wam_get_ref(wam, temp);
						v3->tag = REF;
						v3->ref = tail->head;
						cnt++;
						tail = tail->tail;
					}
				}
			}
			if (target >= 0)
				wam_call(wam, target);
			else
				wam_backtrack(wam);
			break;
		}
		case CALL_CONSULT: wam_consult(wam, var_info(v)); break;
		default: result = 0; break;
	}
	return result;
}

int wam_consult(wam_t *wam, char *filename)
{
	prog_t *prog = compile_program(filename);
	if (prog == NULL)
		wam_backtrack(wam);
	else {
		prog_info(prog);
		prog_add_prog(wam->prog, prog);
		prog_update_label(wam->prog);
		wam->pc += 1;
	}
	return 0;
}

int wam_run_query(wam_t *wam, char *query_str)
{
	if (!strcmp(query_str, "quit.") || !strcmp(query_str, "exit.")) {
		return 0;
	} else if (!strcmp(query_str, "labels.")) {
		int i;
		for (i = 0; i < wam->prog->nstmt; i++)
			if (wam->prog->stmts[i]->label[0])
				printf("%s\n", wam->prog->stmts[i]->label);
		return 1;
	} else if (!strcmp(query_str, "procedures.")) {
		int i;
		for (i = 0; i < wam->prog->nstmt; i++) {
			char *label = wam->prog->stmts[i]->label;
			if (strlen(label) > 0 && indexof(label, '~') < 0)
				printf("%s\n", label);
		}
		return 1;
	} else if (!strcmp(query_str, "list.")) {
		prog_info(wam->prog);
		return 1;
	} else if (!strcmp(query_str, "help.")) {
		printf("sorry there is no help.\n");
		return 1;
	}

	prog_t *prog = compile_query(query_str);
	prog_info(prog);

	prog_add_prog(wam->prog, prog);
	prog_update_label(prog);

	wam->pc = prog_locate_label(prog, "query$");
	printf("pc: %d\n", wam->pc);

	wam_run(wam);

	if (wam->failed) {
		printf("failed.\n");
	} else {
		printf("success.\n");
		int i;
		for (i = 0; i < wam->nqvar; i++) {
			printf("%s=\n", wam->qvars[i]->name);
			var_print(wam->qvars[i]);
			printf("\n");
		}
	}

	wam_reset(wam);
	return 1;
}

wam_t *wam_init(prog_t *prog)
{
	wam_t *wam = malloc(sizeof(wam_t));
	wam->prog = prog;
	wam_reset(wam);
	return wam;
}

var_t *wam_get_ref(wam_t *wam, char *name) {
	var_t **array;
	switch (name[0]) {
		case 'Y': array = wam->env->vars; break;
		case 'A': array = wam->args; break;
		case 'Q': array = wam->qvars; break;
		default: return NULL;
	}
	int len = strlen(name);
	char *num = substr(name, 1, len-1);
	int index = atoi(num);
	if (array[index] == NULL) {
		array[index] = var_init();
	}
	return array[index];
}

environ_t *env_init(int retA, environ_t *lastenv)
{
	environ_t *env = malloc(sizeof(environ_t));
	env->retA = retA;
	env->lastenv  = lastenv;
	return env;
}

int wam_reset(wam_t *wam) {
	wam->narg = 0;
	wam->nqvar = 0;
	wam->env = env_init(99999999, NULL);
	wam->cp = NULL;
	return 0;
}

int wam_run(wam_t *wam)
{
	wam->failed = 1;
	wam->opcnt = 0;
	wam->bpcnt = 0;

	while (wam->pc >= 0) {
		getchar();
		wam->failed = 0;
		stmt_t *stmt = wam->prog->stmts[wam->pc];
		printf("PC: %d, stmt: %s\n", wam->pc, stmt->label);
		stmt_info(stmt);
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
				wam_call(wam, stmt->jump); break;
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
