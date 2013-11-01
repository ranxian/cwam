#include "machine.h"

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
