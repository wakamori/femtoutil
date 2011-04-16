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
#define ADD_HISTORY(line, num) \
	add_history(line);\
	history_num++
	char line[MAX_INPUT_LEN] = "";
	char *lbuf = NULL;
	int history_num = 0;
	HIST_ENTRY *history = NULL;
	cons_t *root = NULL;
	int bnum = 0;
	while (1) {
		lbuf = readline(bnum == 0 ? PROMPT : PROMPT_CONT);
		if (strlen(line) + strlen(lbuf) >= MAX_INPUT_LEN) {
			ml_error("too long input");
			ADD_HISTORY(line, history_num);
			free(lbuf);
			bnum = 0;
			line[0] = '\0';
			continue;
		}
		bnum += countBracket(lbuf);
		strncat(line, lbuf, strlen(lbuf));
		strncat(line, "\n", 1);
		free(lbuf);
		if (bnum < 0) {
			ml_error("wrong syntax");
			ADD_HISTORY(line, history_num);
			bnum = 0;
			line[0] = '\0';
			continue;
		} else if (bnum > 0) {
			continue;
		}
		root = ml_read(line);
		// ml_eval(root);
		DBG2("line=%s, bnum=%d", line, bnum);
		if (strncmp(line, EXIT_STRING, strlen(EXIT_STRING)) == 0)
			break;
		ADD_HISTORY(line, history_num);
		if (++history_num > MAX_HISTORY_NUM) {
			history = remove_history(0);
			free(history);
		}
		line[0] = '\0';
	}
	fprintf(stderr, "bye!\n");
}
