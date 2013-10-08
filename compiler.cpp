#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define is_anum(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
/* test if s is a constant */
bool is_constant(const char *s)
{
	if (s[0] == '[' && s[1] == ']' && s[2] == 0)
		return true;
	if (s[0] >= 'a' && s[0] <= 'z') {
		int len = strlen(s);
		for (int i = 1; i < len; i++) {
			if (!is_anum(s[i])) return false;
		}
		return true;
	}
	if (s[0] == '\'' && s[len-1] == '\'')
		return true;
	if (is_num(s))
		return true;
}
/* test if s is a number */
bool is_num(const char *s)
{
	bool point = 0;
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		char c = s[i];
		if (c == '.') {
			if (!point) point = true;
			else return false;
		} else if (c >= '0' && c <= '9') {
			;
		} else {
			return false;
		}
	}
}
/* test if s is a variable */
bool is_variable(const char *s)
{
	if (s[0] == '_' && s[1] == 0) 
		return true;

	if (s[0] >= 'A' && s[0] <= 'Z') {
		int len = strlen(s);
		for (int i = 1; i < len; i++) {
			if (!is_anum(s[i])) return false;
		}
		return true;
	}
	return false;
}
/* test if s is a predicate */
bool is_predicate(const char *s)
{
	return is_constant(s) && !is_num(s) && !(s[0] == '_' && s[1] == 0);
}
