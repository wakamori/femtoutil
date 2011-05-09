#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ml_config.h"
#include "ml_debug.h"
#include "ml_token.h"
#include "ml_struct.h"
#include "ml_memory.h"

/* ml_main.c */
void molisp_readfile(const char *);
void molisp_interactive();
int ml_loop(mpool_t *pool, char *line);

/* ml_read.c */
int countBracket(char *line);
cons_t *ml_read(mpool_t *pool, char *line[], state_t state);
void ml_genCode(code_t *code, int *i, cons_t *cell);

/* ml_error.c */
void ml_error(char *message);

/* ml_print.c */
void printTree(cons_t *root, int depth);
void printCode(code_t *pc);

/* ml_eval.c */
void replaceLabel(code_t *code, const void *inst[]);
void ml_eval(vm_t *vm, code_t *pc);
