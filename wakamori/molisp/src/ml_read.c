#include "molisp.h"

int countBracket(char *line)
{
	int bnum = 0;
	while (*line != '\0' && bnum >= 0) {
		if (*line == '(')
			bnum++;
		else if (*line == ')')
			bnum--;
		else if (bnum == 0 && *line != '\n' && *line != ' ') {
			return -1;
		}
		line++;
	}
	return bnum;
}

cons_t *ml_read(char line[])
{
	(void)line;
	return NULL;
}
