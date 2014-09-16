#include "machine.h"
#include "defs.h"
#include "program.h"
#include <stdio.h>
#include <string.h>
char var_prefix;

int main()
{
	int i;
	char line[MAX_LINE_LEN];
	printf("This is XR's prolog interpreter\n");
	printf("Type help to get help\n");
	wam_t *wam = wam_init(prog_init());
	while (1) {
		char *p;
		printf("?- ");
		p = fgets(line, MAX_LINE_LEN, stdin);
		if (p) {
			for (i = strlen(p); i >= 0; i--)
				if (p[i] == '\n') p[i] = 0;
			if (!wam_run_query(wam, p)) {
				break;
			}
		} else {
			break;
		}
	}
	printf("Bye.\n");

	return 0;
}
