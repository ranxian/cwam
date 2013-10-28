#include "compiler.h"
#include <stdio.h>

int main()
{
	char *constant = "foo";
	char *var = "Bar";
	char *num = "12.2";
	printf("is_constant('%s'): %d\n", constant, is_constant(constant));
	printf("is_constant('%s'): %d\n", var, is_constant(var));
}
