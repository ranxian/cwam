#include "machine.h"

var_t *var_init_as_unbound(char *name) { return NULL; }
var_t *var_init_as_const(char *name, char *value) { return NULL; }
var_t *var_init_as_bounded(char *name, var_t *v) { return NULL; }
int var_copy(var_t *src, var_t *dst) { return 0; }
var_t *deref(var_t *var) { return NULL; }
void var_print(var_t *var) { return; }