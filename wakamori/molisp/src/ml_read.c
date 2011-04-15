#include "molisp.h"

int countBracket(char *line)
{
	int bnum = 0;
	while (*line != TK_LF) {
		if (*line == TK_BRACKET_OPEN)
			bnum++;
		else if (*line == TK_BRACKET_CLOSE)
			bnum--;
	}
	return bnum;
}
