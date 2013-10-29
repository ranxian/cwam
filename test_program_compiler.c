#include "program_compiler.h"
#include <stdio.h>

int main()
{
	printf("testing program compiler...\n");
	char *filename = "code.pr";
	compile_program(filename);

	return 0;
}
