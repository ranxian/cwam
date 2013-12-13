#include "machine.h"
#include "stmt.h"
#include "compiler.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "helper.h"
#define STREQL(s1, s2) (strcmp(s1, s2) == 0)

static void print_var_list(var_t *list[])
{
	#ifdef DEBUG
	int i;
	for (i = 0; i < MAX_VAR_CNT; i++) {
		var_t *var = list[i];
		if (var != NULL)
			printf("%d name: %s display: %d value: %s\n", i, var->name, var->display, var->value);
	}
	#endif
}
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

int create_variable(wam_t *wam, char *regname, char *varname)
{
	if (varname[0] != '_') {
		var_t *q = wam_get_ref(wam, regname);
		assert(q->tag == REF);
		assert(q == q->ref);
		strcpy(q->name, varname);
		q->display = 1;
		print_var_list(wam->qvars);
	}
	wam->pc += 1;
	return 0;
}
int get_variable(wam_t *wam, char *regname1, char *regname2)
{
	var_t *vn = wam_get_ref(wam, regname1);
	var_t *Ai = wam_get_ref(wam, regname2);
	var_copy(vn, Ai);

	wam->pc += 1;
	return 0;
}
int get_constant(wam_t *wam, char *constname, char *regname)
{
	var_t *v = deref(wam_get_ref(wam, regname));
	int failed = 1;
	if (v->tag == REF) {
		trail_add(wam->trail, v);
		v->tag = CON;
		strcpy(v->value, constname);
		failed = 0;
	} else if (v->tag == CON) {
		if (strcmp(constname, v->value) == 0)
			failed = 0;
	}
	if (failed)
		wam_backtrack(wam);
	else
		wam->pc += 1;
	return 0;
}
int get_value(wam_t *wam, char *regname, char* valname)
{
	unify_variable(wam, valname, regname);
	return 0;
}
int unify_variable2(wam_t *wam, var_t *v1, var_t *v2)
{
	if (v1 == NULL || v2 == NULL) return 0;
	v1 = deref(v1);
	v2 = deref(v2);
	if (v1 == v2) return 1;

	if (v1->tag == REF) {
		trail_add(wam->trail, v1);
		var_copy(v1, v2);
		return 1;
	}

	if (v2->tag == REF) {
		trail_add(wam->trail, v2);
		var_copy(v2, v1);
		return 1;
	}

	if (v1->tag == CON && v2->tag == CON) {
		if (strcmp(v1->value, v2->value) == 0)
			return 1;
		else
			return 0;
	}

	if ((v1->tag == LIS && v2->tag == LIS) ||
			(v1->tag == STR && v2->tag == STR)) {
		if (unify_variable2(wam, v1->head, v2->head) &&
				unify_variable2(wam, v1->tail, v2->tail))
			return 1;
	}
   	return 0;
}
int unify_struc2(wam_t *wam, var_t *struc, var_t *head, var_t* tail)
{
	if (struc->tag == REF) {
		trail_add(wam->trail, struc);
		struc->tag = STR;
		struc->head = head;
		struc->tail = tail;
		return 1;
	}
	if (struc->tag == STR) {
		if (unify_variable2(wam, head, struc->head))
			if (unify_variable2(wam, tail, struc->tail))
				return 1;
	}
	return 0;
}
int unify_variable(wam_t *wam, char *v1name, char *v2name)
{
	var_t *v1 = wam_get_ref(wam, v1name);
	var_t *v2 = wam_get_ref(wam, v2name);
	if (unify_variable2(wam, v1, v2)) {
		wam->pc += 1;
	} else
		wam_backtrack(wam);
	return 0;
}
int unify_struc(wam_t *wam,char *strucname, char *headname, char* tailname)
{
	var_t *struc = wam_get_ref(wam, strucname);
	var_t *head = wam_get_ref(wam, headname);
	var_t *tail = wam_get_ref(wam, tailname);

	if (unify_struc2(wam, struc, head, tail)) {
		wam->pc += 1;
	} else {
		wam_backtrack(wam);
	}
	return 0;
}

int unify_list2(wam_t *wam, var_t *list, var_t *head, var_t *tail)
{
	if (list->tag == REF) {
		trail_add(wam->trail, list);
		list->tag = LIS;
		list->head = head;
		list->tail = tail;
		return 1;
	}
	if (list->tag == LIS) {
		if (unify_variable2(wam, head, list->head)) {
			if (unify_variable2(wam, tail, list->tail))
				return 1;
		}
	}

	return 0;
}

int unify_list(wam_t *wam, char *listname, char *headname, char *tailname) {
	var_t *list = wam_get_ref(wam, listname);
	var_t *head = wam_get_ref(wam, headname);
	var_t *tail = wam_get_ref(wam, tailname);

	if (unify_list2(wam, list, head, tail)) {
		wam->pc += 1;
	} else {
		wam_backtrack(wam);
	}
	return 0;
}
int put_constant(wam_t *wam, char *constname, char *regname)
{
	var_t *Ai = wam_get_ref(wam, regname);
	Ai->tag = CON;
	strcpy(Ai->value, constname);
	wam->pc += 1;
	return 0;
}
// int put_list(wam_t *wam, char *headname, char *tailname, char *argname) { return 0; }
int put_value(wam_t *wam, char *varname, char *argname)
{
	var_t *Vi = wam_get_ref(wam, varname);
	var_t *An = wam_get_ref(wam, argname);
	var_copy(An, Vi);
	wam->pc += 1;
	return 0;
}
int put_variable(wam_t *wam, char *varname, char *argname)
{
	var_t *Vn = deref(wam_get_ref(wam, varname));
	var_t *Ai = wam_get_ref(wam, argname);
	Ai->tag = REF;
	Ai->ref = Vn;
	wam->pc += 1;
	return 0;
}
int try_me_else(wam_t *wam, int next)
{
	choicepoint_t *cp = cp_init(wam->args, wam->trail->n, wam->ctnptr);
	cp->lastcp = wam->cp;
	wam->cp = cp;
	cp->nextclause = next;
	cp->lastenv = wam->env;
	wam->pc += 1;
	return 0;
}
int proceed(wam_t *wam)
{
	wam->pc = wam->ctnptr;
	return 0;
}
int is_bound(wam_t *wam, var_t *var)
{
	var = deref(var);
	if (var->tag == REF) {
		wam_backtrack(wam);
	} else {
		wam->pc += 1;
	}
	return 0;
}
int allocate(wam_t *wam)
{
	environ_t *env = env_init(wam->ctnptr, wam->env);
	wam->env = env;
	wam->pc += 1;
	return 0;
}
int deallocate(wam_t *wam)
{
	wam->ctnptr = wam->env->retA;
	wam->env = wam->env->lastenv;
	wam->pc += 1;
	return 0;
}

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
	#ifdef DEBUG
	printf("backtracking\n");
	#endif
	wam->bpcnt++;
	wam->failed = 1;
	int i;
	if (wam->cp != NULL) {
		wam->ctnptr = wam->cp->retA;
		wam->pc = wam->cp->nextclause;
		wam->env = wam->cp->lastenv;

		int tp = wam->cp->trailptr;
		for (i = wam->trail->n - 1; i >= tp; i--) {
			trail_undo(wam->trail, i);
		}
		wam->trail->n = tp;
		memcpy(wam->args, wam->cp->args, sizeof(wam->args));
		wam->cp = wam->cp->lastcp;
	} else {
		for (i = wam->trail->n - 1; i >= 0; i--) {
			trail_undo(wam->trail, i);
		}
		wam->pc = -1;
	}
	#ifdef DEBUG
	printf("%d\n", wam->pc);
	printf("backtracked\n");
	#endif
	return 0;
}

int wam_intpred(wam_t *wam, int call)
{
	prog_t *prog = wam->prog;
	int result = 1;
	var_t *v = wam->args[0];
	if (call == CALL_CALL) {
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
	} else if (call == CALL_CONSULT) {
		char info[MAX_WORD_LEN];
		var_info(v, info);
		wam_consult(wam, info);
	} else {
		result = 0;
	}
	return result;
}

int wam_consult(wam_t *wam, char *filename)
{
	char fn2[256];
	strcpy(fn2, filename+1);
	fn2[strlen(fn2)-1] = 0;
	prog_t *prog = compile_program(fn2);
	if (prog == NULL)
		wam_backtrack(wam);
	else {
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
	} else if (!strcmp(query_str, "reset.")) {
		wam_reset(wam);
		return 1;
	} else if (!strcmp(query_str, "help.")) {
		printf("sorry there is no help.\n");
		return 1;
	}

	if (strlen(query_str) == 0)
		return 1;

	wam_reset(wam);
	prog_del_from_label(wam->prog, "query$");

	prog_t *prog = compile_query(query_str);

	prog_add_prog(wam->prog, prog);
	prog_update_label(wam->prog);

	wam->pc = prog_locate_label(wam->prog, "query$");

	while (1) {
		wam_run(wam);

		if (wam->failed) {
			printf("NO.\n");
		} else {
			printf("YES.\n");
			int i;
			for (i = 0; i < MAX_VAR_CNT; i++) {
				if (wam->qvars[i] != NULL) {
					if (wam->qvars[i]->display) {
						var_print(wam->qvars[i]);
					}
				}
			}
		}
		if (wam->cp != NULL) {
			char ans[MAX_WORD_LEN];
			printf("more? [Y]es, [N]o\n");
			scanf("%s", ans);
			if (ans[0] == 'Y' || ans[0] == 'y') 
				wam_backtrack(wam);
			else break;
		} else break;
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
		if (name[0] == 'A') {
			wam->narg += 1;
		} else if (name[0] == 'Q') {
			wam->nqvar += 1;
		}
	}
	return array[index];
}

environ_t *env_init(int retA, environ_t *lastenv)
{
	environ_t *env = malloc(sizeof(environ_t));
	memset(env->vars, 0, sizeof(env->vars));
	env->retA = retA;
	env->lastenv  = lastenv;
	return env;
}

choicepoint_t *cp_init(var_t *args[], int trailptr, int retA)
{
	int i;
	choicepoint_t *cp = malloc(sizeof(choicepoint_t));
	for (i = 0; i < MAX_VAR_CNT; i++) {
		if (args[i] != NULL) {
			cp->args[i] = malloc(sizeof(var_t));
			memcpy(cp->args[i], args[i], sizeof(var_t));
		} else {
			cp->args[i] = NULL;
		}
	}
	cp->retA = retA;
	cp->trailptr = trailptr;

	return cp;
}

int wam_reset(wam_t *wam) {
	int i;
	for (i = 0; i < wam->narg; i++)
		wam->args[i] = NULL;
	for (i = 0; i < wam->nqvar; i++)
		wam->qvars[i] = NULL;
	wam->narg = 0;
	wam->nqvar = 0;
	wam->env = env_init(99999999, NULL);
	wam->cp = NULL;
	wam->trail = trail_init();
	return 0;
}

int wam_run(wam_t *wam)
{
	wam->failed = 1;
	wam->opcnt = 0;
	wam->bpcnt = 0;

	int time = 0;
	int halted;
	while (wam->pc >= 0) {
		halted = 0;
		time += 1;

		wam->failed = 0;
		stmt_t *stmt = wam->prog->stmts[wam->pc];
		if (wam->opcnt > wam->maxopcnt) {
			printf("panic: maximum opcnt reached\n");
			wam->failed = 1;
			break;
		}

		if (time > 10000) {
			printf("> 10000\n");
			stmt->op = OP_HALT;
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
				halted = 1;
			case OP_NOOP:
				wam->pc += 1; break;
			case OP_PROCEED:
				proceed(wam); break;
			case OP_RTRY_ME_ELSE:
			case OP_TRY_ME_ELSE:
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
		if (halted) break;
	}
	if (halted) return 0;
	if (wam->failed) {
		while (wam->cp != NULL) wam_backtrack(wam);
		wam_backtrack(wam);
	}
	return 0;
}

trail_t *trail_init()
{
	trail_t *trail = malloc(sizeof(trail_t));
	memset(trail, 0, sizeof(trail->contents));
	trail->n = 0;
	return trail;
}
int trail_add(trail_t *trail, var_t *var)
{
	trail->contents[trail->n] = var;
	trail->n++;
	return 0;
}
int trail_undo(trail_t *trail, int index)
{
	var_t *v = trail->contents[index];
	if (v != NULL) {
		v->tag = REF;
		v->ref = v;
	}

	return 0;
}
