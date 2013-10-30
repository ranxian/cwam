#ifndef PROGRAM_H
#define PROGRAM_H
#define MAX_STATEMENTS_NUM 1024
#define MAX_LABEL_LEN 128

typedef enum {
	Call_Write,         
	Call_WriteLn,       
	Call_NewLine,       
	Call_Consult,       
	Call_Reconsult,     
	Call_Load,          
	Call_Assert,        
	Call_RetractOne,    
	Call_RetractAll,    
	Call_IsInteger,     
	Call_IsAtom,        
	Call_IsBound,       
	Call_ReadLn,        
	Call_Call
} call_t;

typedef struct  {
	stmt_t stmts[MAX_STATEMENTS_NUM];
	char labels[MAX_LABEL_LEN];
	int nlabel;
} prog_t;

prog_t prog_init();

int prog_add_prog(prog_t *prog, prog_t *adding);

int prog_add_stmt(prog_t *prog, stmt_t *stmt);

int prog_add_stmt_at(prog_t *prog, int pos);

int prog_del_from_line(prog_t *prog, int lineno);

int prog_del_from_label(prog_t *prog, const char *label);

int prog_get_last_clause(prog_t *prog, const char *proc_name, int butone);

int prog_add_clause(prog_t *prog, const char *label, prog_t *adding);

int prog_locate_label(prog_t *prog, const char *label);

int prog_update_label(prog_t *prog);

#endif
