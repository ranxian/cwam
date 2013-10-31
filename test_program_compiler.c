#include "program_compiler.h"
#include <stdio.h>

char var_prefix;

int main()
{
	var_prefix = 'Y';
	printf("testing program compiler...\n");
	char *filename = "code.pr";
	compile_program(filename);

	return 0;
}
