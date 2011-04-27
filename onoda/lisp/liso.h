#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DEPTH 32
#define TRUE 1
#define FALSE 0
#define L_K 0
#define R_K 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DEV 5
#define STR 6
#define INT 7
#define ROOT 8
#define IF 9
#define EQU 10
#define LT 11
#define GT 12
#define ELT 13
#define EGT 14
#define SETQ 15
#define DEF 16
#define FUNC 17
#define ARG 18
#define LAYER 2048
#define SIZE 2048

typedef struct cons_t{
  int type;
  union{
    struct cons_t *car;
    int ivalue;
    char *cvalue;
  };
  struct cons_t *cdr;
  int result[LAYER];
} cons_t;

typedef struct map{
  int value;
  char *key;
  int hash;
} map;

map g_qa[SIZE];
int g_qc;

typedef struct func{
  char *key;
  char *arg;
  cons_t *exp;
} func;

func g_fa[SIZE];
int g_fc;

int g_arga[LAYER];
int g_argl;

int g_funcl;
 
int findmap(char *key);
func findfunc(char *key);
void findarg(cons_t *root, func mame);
void clean(cons_t *root, int layer);
int getvalue(cons_t *next);
int eval(cons_t *root);
char **tokenizer(char *line);
void dump_token(char **token);
cons_t *maketree(char **token);
int typechange(char *chartoken);
void dump_tree(cons_t *root);
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
void setq(cons_t *next);
void def(cons_t *next);
int lfunc(cons_t *next);
