#include "program_compiler.h"
#include <stdio.h>

char var_prefix;

int main(int argc, char *argv[])
{
	var_prefix = 'Y';
	printf("testing program compiler...\n");
	compile_program(argv[1]);

	return 0;
}
