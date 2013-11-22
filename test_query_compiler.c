#include "compiler.h"
#include <stdio.h>

char var_prefix = 'Q';

int main()
{
	prog_t *prog;

	prog = compile_query("boy(Who).");

	prog_info(prog);
}
