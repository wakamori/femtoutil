#include "molisp.h"

/*
 * readfile mode
 */
void molisp_readfile(const char *argv)
{
	fprintf(stderr, "argv=%s\n", argv);
}

/*
 * interactive (readline) mode
 */
void molisp_interactive()
{
	char *line = NULL;
	int history_num = 0;
	HIST_ENTRY *history = NULL;
	// cons_t *root = NULL;
	while (1) {
		int bnum = 0;
		line = readline(PROMPT);
		bnum = countBracket(line);
		while (bnum != 0)
			bnum = countBracket(readline(PROMPT_CONT));
		// root = ml_read(line);
		// ml_eval(root);
		if (strncmp(line, EXIT_STRING, strlen(EXIT_STRING)) == 0)
			break;
		add_history(line);
		if (++history_num > MAX_HISTORY_NUM) {
			history = remove_history(0);
			free(history);
		}
		free(line);
	}
	fprintf(stderr, "bye!\n");
}
