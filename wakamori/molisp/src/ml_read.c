#include "molisp.h"

#define new_Cell(pool) mp_alloc(pool, sizeof(cons_t))

/**
 * count the number of '(' and ')'
 */
int countBracket(char *line)
{
	int bnum = 0;
	while (*line != '\0' && *line != '\n' && bnum >= 0) {
		if (*line == '(')
			bnum++;
		else if (*line == ')')
			bnum--;
		line++;
	}
	return bnum;
}

/**
 * read line and make AST
 */
cons_t *ml_read(mpool_t *pool, char **line, state_t state)
{
	while (**line == ' ' || **line == '\n')
		(*line)++;
	DBG("%c", **line);
	if (**line == '\0' || **line == ')')
		return NULL;
	cons_t *cell = new_Cell(pool);
	switch (**line) {
		case '\0':
			return NULL;
			break;
		case '(':
			cell->type = T_START;
			(*line)++;
			cell->car = ml_read(pool, line, S_HEAD);
			break;
		case ')':
			break;
		case '+':
			if (state == S_HEAD) {
				if (*(*line + 1) == ' ')
					cell->type = T_ADD;
				else
					DBG("+ is not func");
			}
			break;
		case '-':
			if (state == S_HEAD) {
				if (*(*line + 1) == ' ')
					cell->type = T_SUB;
				else
					DBG("- is not func");
			}
			break;
		case '*':
			if (state == S_HEAD) {
				if (*(*line + 1) == ' ')
					cell->type = T_MUL;
				else
					DBG("* is not func");
			}
			break;
		case '/':
			if (state == S_HEAD) {
				if (*(*line + 1) == ' ')
					cell->type = T_DIV;
				else
					DBG("/ is not func");
			}
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			cell->type = T_INT;
			cell->ival = atoi(*line);
			DBG("ival=%d", cell->ival);
			while ('0' <= *(*line + 1) && *(*line + 1) <= '9')
				(*line)++;
			break;
		default:
			break;
	}
	(*line)++;
	cell->cdr = ml_read(pool, line, S_BODY);
	return cell;
}

/**
 * generate bytecode from AST
 */
void ml_genCode(code_t *code, int *i, cons_t *cell)
{
	int n, j = 0;
	if (cell == NULL)
		return;
	switch (cell->type) {
		case T_START:
			DBG("START\n");
			ml_genCode(code, i, cell->car);
			DBG("RET\n");
			code[*i].op = RET;
			break;
		case T_INT:
			DBG("%d\n", cell->ival);
			if (cell->ival == 0) {
				code[*i].op = PUSH0;
			} else if (cell->ival == 1) {
				code[*i].op = PUSH1;
			} else if (cell->ival == 2) {
				code[*i].op = PUSH2;
			} else {
				code[*i].op = PUSH;
				code[*i].opr = cell->ival;
			}
			(*i)++;
			break;
		case T_ADD:
			n = 0;
			cell = cell->cdr;
			while (cell != NULL) {
				ml_genCode(code, i, cell);
				cell = cell->cdr;
				n++;
			}
			for (j = 0; j < n - 1; j++) {
				DBG("ADD\n");
				code[(*i)++].op = ADD;
			}
			break;
		case T_SUB:
			n = 0;
			cell = cell->cdr;
			while (cell != NULL) {
				ml_genCode(code, i, cell);
				cell = cell->cdr;
				n++;
			}
			for (j = 0; j < n - 1; j++) {
				DBG("SUB\n");
				code[(*i)++].op = SUB;
			}
			break;
		case T_MUL:
			n = 0;
			cell = cell->cdr;
			while (cell != NULL) {
				ml_genCode(code, i, cell);
				cell = cell->cdr;
				n++;
			}
			for (j = 0; j < n - 1; j++) {
				DBG("MUL\n");
				code[(*i)++].op = MUL;
			}
			break;
		case T_DIV:
			n = 0;
			cell = cell->cdr;
			while (cell != NULL) {
				ml_genCode(code, i, cell);
				cell = cell->cdr;
				n++;
			}
			for (j = 0; j < n - 1; j++) {
				DBG("DIV\n");
				code[(*i)++].op = DIV;
			}
			break;
		default:
			break;
	}
}
