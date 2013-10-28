#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define is_anum(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
/* test if s is a constant */
int is_constant(const char *s)
{
	int len = strlen(s), i;
	if (s[0] == '[' && s[1] == ']' && s[2] == 0)
		return 1;
	if (s[0] >= 'a' && s[0] <= 'z') {
		for (i = 1; i < len; i++) {
			if (!is_anum(s[i])) return 0;
		}
		return 1;
	}
	if (s[0] == '\'' && s[len-1] == '\'')
		return 1;
	if (is_num(s))
		return 1;
	return 0;
}
/* test if s is a number */
int is_num(const char *s)
{
	int point = 0;
	int len = strlen(s);
	int i;
	for (i = 0; i < len; i++) {
		char c = s[i];
		if (c == '.') {
			if (!point) point = 1;
			else return 0;
		} else if (c >= '0' && c <= '9') {
			;
		} else {
			return 0;
		}
	}
	return 1;
}
/* test if s is a variable */
int is_variable(const char *s)
{
	int i;
	if (s[0] == '_' && s[1] == 0)
		return 1;

	if (s[0] >= 'A' && s[0] <= 'Z') {
		int len = strlen(s);
		for (i = 1; i < len; i++) {
			if (!is_anum(s[i])) return 0;
		}
		return 1;
	}
	return 0;
}
/* test if s is a predicate */
int is_predicate(const char *s)
{
	return is_constant(s) && !is_num(s) && !(s[0] == '_' && s[1] == 0);
}
