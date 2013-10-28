#include "compiler.h"
#include <stdio.h>

int main()
{
	char *constant = "foo";
	char *var = "Bar";
	char *num = "12.2";
	printf("is_constant('%s'): %d\n", constant, is_constant(constant));
	printf("is_constant('%s'): %d\n", var, is_constant(var));

	char *code = "father(X, Y) := son(Y, X). %this is comment\nson(x).\nson('Alice')";
	tok_stream_t toks;
	init_toks_from_string(&toks, code);
	toks_info(&toks);
	toks_destroy(&toks);
}
