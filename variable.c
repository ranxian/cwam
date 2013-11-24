#include "machine.h"
#include <string.h>
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

void var_print(var_t *var) { return; }

char *var_info(var_t *var)
{
	return "NOT_IMPLEMENTED";
}
