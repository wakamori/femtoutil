#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TYPE_INT  0 //123...
#define TYPE_CHAR 1 
#define TYPE_PLUS  21 //+,-,*....
#define TYPE_MINUS 22
#define TYPE_MULTI 23
#define TYPE_DIVID 24
#define TYPE_CRE   25
#define TYPE_DECRE 26
#define TYPE_FUNC 3 //defun func
#define TYPE_START 4 // (
#define TYPE_END 5  // )
typedef struct cons_t {
	int type;
	union {
		struct cons_t *car;
		char value;
		int ivalue;
		char *func;
	};
	struct cons_t *cdr;
} cons_t;

/*typedef struct list_t{
	int type;
	int listnum;
	char value;
	int ivalue;
	char *funcname;
}list_t;
*/

cons_t *sgmt_read(char *line);
cons_t sgmt_eval(cons_t *cell);
void sgmt_print(cons_t *root);
