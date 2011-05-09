#include "molisp.h"

/**
 * readfile mode
 */
void molisp_readfile(const char *fname)
{
	FILE *fp;
	mpool_t *pool = mp_create(MPOOL_SIZE);
	char line[MAX_INPUT_LEN] = "";
	char lbuf[MAX_INPUT_LEN] = "";
	int bnum = 0;
	if ((fp = fopen(fname, "r")) == NULL) {
		ml_error("no such file");
		return;
	}
	while (fgets(lbuf, MAX_INPUT_LEN, fp) != NULL) {
		if (strlen(line) + strlen(lbuf) >= MAX_INPUT_LEN) {
			ml_error("too long input text");
		} else {
			bnum += countBracket(lbuf);
			strncat(line, lbuf, strlen(lbuf));
			if (bnum < 0) {
				ml_error("wrong syntax");
			} else if (bnum == 0) {
				if (ml_loop(pool, line) == 1)
					break;
			} else {
				continue;
			}
		}
		bnum = 0;
		line[0] = '\0';
	}
	fclose(fp);
}

/**
 * interactive (readline) mode
 */
void molisp_interactive()
{
	mpool_t *pool = mp_create(MPOOL_SIZE);
	char line[MAX_INPUT_LEN] = "";
	char *lbuf = NULL;
	int bnum = 0;
	while (1) {
		lbuf = readline(bnum == 0 ? PROMPT : PROMPT_CONT);
		if (strlen(line) + strlen(lbuf) >= MAX_INPUT_LEN) {
			ml_error("too long input text");
			add_history(lbuf);
		} else {
			bnum += countBracket(lbuf);
			strncat(line, lbuf, strlen(lbuf));
			strncat(line, "\n", 1);
			add_history(line);
			if (bnum < 0) {
				ml_error("wrong syntax");
			} else if (bnum == 0) {
				if (ml_loop(pool, line) == 1)
					break;
			} else {
				continue;
			}
		}
		free(lbuf);
		bnum = 0;
		line[0] = '\0';
	}
	mp_clear(pool);
	clear_history();
	ml_print("bye!\n");
}

/**
 * start read-eval-print loop
 */
int ml_loop(mpool_t *pool, char *line)
{
	DBG("line=%s", line);
	code_t *code = mp_alloc(pool, sizeof(code_t) * 30);
	vm_t vm;
	vm.reg.a = 0;
	vm.sp = 0;
	if (strncmp(line, EXIT_STRING, strlen(EXIT_STRING)) == 0)
		return 1;
	cons_t *root = ml_read(pool, &line, S_OUT);
	if (root != NULL) {
		printTree(root, 0);
		int i = 0;
		ml_genCode(code, &i, root);
		printCode(code);
		ml_eval(&vm, code);
	}
	return 0;
}
