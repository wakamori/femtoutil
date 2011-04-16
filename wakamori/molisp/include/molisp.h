#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ml_config.h"
#include "ml_debug.h"
#include "ml_token.h"
#include "ml_struct.h"

void molisp_readfile(const char *);
void molisp_interactive();
int countBracket(char *line);
cons_t *ml_read(char *line);
void ml_error(char *message);
