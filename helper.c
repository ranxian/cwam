#include "helper.h"
#include <string.h>

int indexof(char *str, char c)
{
	int res = -1;
	int i = 0, len = strlen(str);
	for (i = 0; i < len; i++)
		if (str[i] == c) return i;
	return res;
}

char *substr(char *str, int begin, int end, char *to)
{
	int i, m = 0;
	for (i = begin; i <= end; i++)
		to[m++] = str[i];
	to[m] = 0;
	return to;
}
