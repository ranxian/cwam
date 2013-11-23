#ifndef MACHINE_H
#define MACHINE_H
#include "defs.h"
#include "program.h"
#define MAX_VAR_CNT 256

char *OP_NAMES(wam_op_t op);

typedef enum { REF, CON, LIS, STR } tag_t;

typedef struct var_t {
	tag_t tag;
	char value[MAX_WORD_LEN];
	struct var_t *ref;
	char name[MAX_WORD_LEN];
} var_t;

char *TAG_NAMES(tag_t tag);
var_t *var_init_as_unbound(char *name);
var_t *var_init_as_const(char *name, char *value);
var_t *var_init_as_bounded(char *name, var_t *v);
int var_copy(var_t *src, var_t *dst);
var_t *deref(var_t *var);
void var_print(var_t *var);

typedef struct environ_t {
	var_t *vars[MAX_VAR_CNT];
	struct environ_t *lastenv;
	int retA;
} environ_t;

environ_t *env_init(int retA, environ_t *lastenv);

struct choicepoint_t;
typedef struct {
	prog_t *prog;
	struct choicepoint_t *cp;
	int failed;
	int pc;
	environ_t *env;
	var_t *args[MAX_VAR_CNT];
	var_t *qvars[MAX_VAR_CNT];
	int narg;
	int nqvar;
	int opcnt;
	int maxopcnt;
} wam_t;

wam_t *wam_init(prog_t *prog);
var_t *wam_get_ref(wam_t *wam, char *name);

typedef struct choicepoint_t {
	var_t *args;
	environ_t *lastenv;
	int retA;
	struct choicepoint_t *lastcp;
	int nextclause;
	int trailptr;
} choicepoint_t;

choicepoint_t *cp_init(var_t *args, int trailptr, int retA);
// byte code interpretation begins

int create_variable(wam_t *wam, char *regname, char *varname);
int get_variable(wam_t *wam, char *regname1, char *regname2);
int get_constant(wam_t *wam, char *constname, char *regname);
int get_value(wam_t *wam, char *regname, char* valname);
int unify_variable2(wam_t *wam, var_t *v1, var_t *v2);
int unify_struc2(wam_t *wam, var_t *struc, var_t *head, var_t* tail);
int unify_variable(wam_t *wam, char *v1name, char *v2name);
int unify_struc(wam_t *wam,char *strucname, char *headname, char* tailname);
int unify_list(wam_t *wam, char *listname, char *headname, char *tailname);
int put_constant(wam_t *wam, char *constname, char *regname);
int put_list(wam_t *wam, char *headname, char *tailname, char *argname);
int put_value(wam_t *wam, char *varname, char *argname);
int put_variable(wam_t *wam, char *varname, char *argname);
int try_me_else(wam_t *wam, int next);
int proceed(wam_t *wam);
int is_bound(wam_t *wam, var_t *var);
int allocate(wam_t *wam);
int deallocate(wam_t *wam);
int call(wam_t *wam, int target);

int wam_reset(wam_t *wam);
int wam_backtrack(wam_t *wam);
int wam_intpred(wam_t *wam, int call);
int wam_load(wam_t *wam, char *filename);
int wam_consult(wam_t *wam, char *filename);

int wam_run(wam_t *wam);
int wam_run_query(wam_t *wam, char *query_str);
#endif
