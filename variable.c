#include "machine.h"
#include <string.h>
#include <stdio.h>
static char temp[256];

var_t *var_init()
{
	var_t *var = malloc(sizeof(var_t));
	var->tag = REF;
	var->ref = var;
	var->display = 0;
	return var;
}

var_t *var_init_as_unbound(char *name)
{
	var_t *var = var_init();
	strcpy(var->name, name);
	var->display = 0;
	return var;
}

var_t *var_init_as_const(char *name, char *value)
{
	var_t *var = malloc(sizeof(var_t));
	var->tag = CON;
	strcpy(var->value, value);
	strcpy(var->name, name);
	var->display = 0;
	return var;
}

var_t *var_init_as_bounded(char *name, var_t *v)
{
	var_t *var = malloc(sizeof(var_t));
	var->tag = REF;
	var->ref = v;
	strcpy(var->name, name);
	var->display = 0;
	return var;
}

int var_copy(var_t *dst, var_t *src)
{
	memcpy(dst, src, sizeof(var_t));
	return 0;
}

var_t *deref(var_t *var)
{
	if (var->tag == REF && var->ref != var) {
		var_t *result = var->ref;
		while (result->tag == REF && result->ref != result) {
			result = result->ref;
		}
		return result;
	} else {
		return var;
	}
}

void var_print(var_t *var)
{
	char t[64];
	var_info(var, t);
	printf("%s\n", t);
	return;
}

void var_info2(var_t *var, char *res)
{
	if (var->tag == LIS) {
		var_info(var->head, res);
		if (var->tail != NULL && var->tail->tag != CON) {
			char t[MAX_WORD_LEN];
			var_info2(var->tail, t);
			sprintf(res, "%s, %s", res, t);
		}
	}
	res[0] = 0;
}

void var_info(var_t *var, char *res)
{
	switch (var->tag) {
		case CON:
			strcpy(res, var->value);
			break;
		case LIS:
			var_info2(var, res);
			break;
		case STR:
		{
			char t1[64];
			char t2[64];
			var_info2(var->head, t1);
			var_info2(var->tail, t2);
			sprintf(res, "%s(%s)", t1, t2);
			break;
		}
		case REF:
			if (var->ref == var)
				strcpy(res, "_");
			else var_info(deref(var), res);
			break;
		default:
			printf("var_info panic!\n");
	}
}
