#include "compiler.h"
#include <stdio.h>

char var_prefix = 'Y';

int main(int argc, char *argv[])
{
	printf("testing program compiler...\n");

	prog_t *prog = compile_program(argv[1]);

	prog_info(prog);

	return 0;
}
