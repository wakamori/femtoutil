#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

typedef enum cons_type {L_K, R_K, ADD, SUB, MUL, DEV, STR, INT, ROOT, IF, EQU,\
						LT, GT, ELT, EGT, SETQ, DEF, FUNC, ARG, RFUNC, CHILD_ANS} cons_type;
#define MAX_DEPTH 32
#define TRUE 1
#define FALSE 0
#define FPA_SIZE 32
#define MAP_SIZE 16
#define FUNC_SIZE 128
#define REC_FUNC_SIZE 8
#define ARG_SIZE 128
#define LAYER 1024

typedef struct cons_t{
	cons_type type;
	union{
		struct cons_t *car;
		int ivalue;
		char *cvalue;
	};
	struct cons_t *cdr;
	int result;
} cons_t;

typedef struct map{
	int value;
	char *key;
	int hash;
} map;

map g_qa[MAP_SIZE];

typedef struct func{
	char *key;
	cons_t *exp;
	int rec_num;
	cons_t *rec_locate[REC_FUNC_SIZE];
} func;

func g_fa[FUNC_SIZE];

int g_arga[LAYER][ARG_SIZE];
int g_argl;

void find_func(cons_t *root, char *key, int count);
void find_arg(cons_t *root, cons_t *arg_name);
int get_value(cons_t *next);
int get_map(char *key);
int get_func(char *key);
int eval(cons_t *root);
char **tokenizer(char *line);
void dump_token(char **token);
void free_token(char **token);
cons_t *maketree(char **token);
int typechange(char *chartoken);
void dump_tree(cons_t *root);
void free_tree(cons_t *root);
int add(cons_t *next);
int mul(cons_t *next);
int sub(cons_t *next);
int dev(cons_t *next);
int lif(cons_t *next);
int equ(cons_t *next);
int lt(cons_t *next);
int gt(cons_t *next);
int elt(cons_t *next);
int egt(cons_t *next);
int setq(cons_t *next);
int def(cons_t *next);
int lfunc(cons_t *next);
int rfunc(cons_t *next);
int froot(cons_t *next);
int fl_k(cons_t *next);
int error(cons_t *next);
int fstr(cons_t *next);
int child_ans(cons_t *next);
void rec_cal(int func_count);
