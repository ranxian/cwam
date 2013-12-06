#include "helper.h"
#include <string.h>

static char temp[256];

int indexof(char *str, char c)
{
	int res = -1;
	int i = 0, len = strlen(str);
	for (i = 0; i < len; i++)
		if (str[i] == c) return i;
	return res;
}

char *substr(char *str, int begin, int end)
{
	int i, m = 0;
	for (i = begin; i <= end; i++)
		temp[m++] = str[i];
	temp[m] = 0;
	return temp;
}

char *TAG_NAME(tag_t tag)
{
	static char *_names[] = {
		"REF", "CON", "LIS", "STR"
	};
	return _names[tag];
}
