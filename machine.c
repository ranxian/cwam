#include "machine.h"

char *OP_NAMES(wam_op_t op)
{
	static char *names[] = 
	{
		"alloc",
		"dealloc",
		"call",
		"not_call",
		"create_variable",
		"get_const",
	   	"get_value",
		"get_variable",
		"get_level",
		"put_value",
		"put_variable",
		"put_const",
		"halt",
		"noop",
		"proceed",
		"rtry_me_else",
		"trust_me",
		"try_me_else",
		"unification_list",
		"unification_structrue",
		"unification_variable"
	};

	return names[op];
}
