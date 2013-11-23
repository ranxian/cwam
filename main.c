#include "machine.h"
#include "defs.h"
#include "program.h"
#include <stdio.h>
#include <string.h>
char var_prefix = 'Q';

int main()
{
	int i;
	char line[MAX_LINE_LEN];
	char *p;
	printf("This is XR's prolog interpreter\n");
	printf("Type help to get help\n");
	wam_t *wam = wam_init(prog_init());
	do {
		printf("?- ");
		p = fgets(line, MAX_LINE_LEN, stdin);
		if (p) {
			for (i = strlen(p); i >= 0; i--)
				if (p[i] == '\n') p[i] = 0;
		}
	} while (p != NULL && wam_run_query(wam, p));
	printf("Bye.\n");
}
