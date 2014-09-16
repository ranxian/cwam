#include "compiler.h"
#include <stdio.h>

char var_prefix = 'Q';

int main()
{
	prog_t *prog;

	prog = compile_query("member(a, [b, c, d, a, e]).");

	prog_info(prog);

	return 0;
}
