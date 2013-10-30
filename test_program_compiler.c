#include "program_compiler.h"
#include <stdio.h>

char var_prefix;

int main()
{
	var_prefix = 'Q';
	printf("testing program compiler...\n");
	char *filename = "code.pr";
	compile_program(filename);

	return 0;
}
