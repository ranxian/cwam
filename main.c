#include "machine.h"
#include "defs.h"
#include "program.h"
#include <stdio.h>
char var_prefix = 'Q';

int main()
{
	char line[MAX_LINE_LEN];
	char *p;
	printf("This is XR's prolog interpreter\n");
	printf("Type help to get help\n");
	wam_t *wam = wam_init(prog_init());
	do {
		printf("?- ");
		p = fgets(line, MAX_LINE_LEN, stdin);
	} while (p != NULL && wam_run_query(wam, p));
	printf("Bye.\n");
}
