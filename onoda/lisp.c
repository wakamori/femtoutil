#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <math.h>

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
#define LAYER 4096
#define SIZE 4096

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
int g_qc = 0;

typedef struct func{
  char *key;
  char *arg;
  cons_t *exp;
} func;

func g_fa[SIZE];
int g_fc = 0;

int g_arga[LAYER];
int g_argl = 0;

int g_funcl = 0;


void dump_token(char **token);
char **tokenizer(char *line);
cons_t *maketree(char **token);
int typechange(char *chartoken);
void dump_tree(cons_t *root);
int eval(cons_t *root);
int findmap(char *key);
func findfunc(char *key);
void findarg(cons_t *root, func name);
void clean(cons_t *root,int layer);


int main(int argc, char *argv[])
{
  if (argc == 2) {
	char input[64];
	char **token = {NULL};
	FILE *fp = NULL;

	fp = fopen(argv[1],"r");

	if(fp == NULL){
	  printf("file not open");
	  return 0;
	}

	while ( fgets(input, 64, fp) != NULL) {
	  
	  printf("input\n%s\n", input);
	  
	  token = tokenizer(input);
	  dump_token(token);
	  
	  cons_t *root = NULL;
	  root = maketree(token);
	  printf("tree\n");
	  dump_tree(root);
	  
	  int answer;
	  answer = eval(root);
	  printf("answer=%d\n", answer);
	}

	fclose(fp);
	  
	return 0;

  } else {
	char *input = NULL;
	char **token = {NULL};

	while (1) {  
	  input = readline(">>>");
	  add_history(input);
	  
	  printf("readline\n%s\n", input);
	  
	  if (strncmp(input,"quit", sizeof("quit")) == 0 ||
		  strncmp(input, "exit", sizeof("exit")) == 0) {
		break;
	  }
	  
	  token = tokenizer(input);
	  dump_token(token);
	  
	  cons_t *root = NULL;
	  root = maketree(token);
	  printf("tree\n");
	  dump_tree(root);

	  int answer;
	  answer = eval(root);
	  printf("answer=%d\n", answer);

	}
	return 0;
  }
}


void dump_token(char **token)
{
  printf("token\n");
  int i = 0;
  while (token[i] != NULL) {
	printf("%s\n", token[i]);
	i++;
  }

}

char **tokenizer(char *line)
{
  
  char **t;
  t = (char**)malloc(sizeof(char*)*strlen(line));

  char *linec;
  linec = (char*)malloc(sizeof(char)*(strlen(line)));
  strcpy(linec,line);

  char *next;
  next = linec;

  int l,m = 0;

  for (l = 0; linec[l] != 0; l++) {
	if (linec[l] == '(') {
	  t[m] = (char*)malloc(sizeof(char));
	  t[m] = "(";
	  m++;
	  next = &linec[l+1];
	}else if (linec[l] == ' '){
	  if (strcmp(t[m-1],")") == 0 && (*next == ')' || *next == ' ')) {
		next = &linec[l+1];
	  } else {
		linec[l] = '\0';
		t[m] = (char*)malloc(strlen(next) + 1);
		strcpy(t[m],next);
		m++;
		next = &linec[l+1];
	  }
	} else if (linec[l] == ')') {
	  if (strcmp(t[m-1],")") == 0 && *next == ')') {
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char));
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  } else {	 
		linec[l] = '\0';
		t[m] = (char*)malloc(strlen(next) + 1);
		strcpy(t[m],next);
		m++;
		t[m] = (char*)malloc(sizeof(char));
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  }
	}
  }
  t[m] = (char*)malloc(sizeof(char));
  t[m] = NULL;

  return t;
}

int typechange(char *chartoken)
{
  int size, intoken = 0, i = 0, digit;  
  size = strlen(chartoken);
  size--;
  for (digit = 1; size>0; size--) {
	digit = digit * 10;
  }
  while (chartoken[i] != '\0') {
	intoken = intoken + digit * ((int)chartoken[i] - 48);
	i++;
	digit = digit / 10;
  }
  return intoken;
}
 
cons_t *maketree(char **token)
{
  int depth = 0,i = 0;
  cons_t *ad[MAX_DEPTH] = {NULL};

  cons_t *root = NULL, *next = NULL;
  root = (cons_t*)malloc(sizeof(cons_t));
  root->type = ROOT;
  root->car = NULL;
  root->cdr = NULL;
  next = root;

  while (token[i] != NULL) {
	if (strcmp(token[i],"(") == 0) {
	  next->cdr = (cons_t*)malloc(sizeof(cons_t));
	  next = next->cdr;  
	  next->type = L_K;
	  next->car = NULL;
	  next->cdr = NULL;
	  ad[depth] = next;
	  depth++;
	} else if (strcmp(token[i], ")") == 0) {
	  next->cdr = NULL;
	  depth--;
	  next = ad[depth];
	} else if (strcmp(token[i], "+") == 0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = ADD;
	} else if (strcmp(token[i],"-") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = SUB;
	} else if (strcmp(token[i], "*") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = MUL;
	} else if (strcmp(token[i], "/") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next =  next->car;
	  next->type = DEV;
	} else if (strcmp(token[i],"if") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = IF;
	} else if (strcmp(token[i], "=") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = EQU;	
	} else if (strcmp(token[i], "<") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = LT;
	} else if (strcmp(token[i], ">") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = GT;
	} else if (strcmp(token[i], "<=") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = ELT;
	} else if (strcmp(token[i], ">=") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = EGT;
	} else if (strcmp(token[i], "setq") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = SETQ;
	} else if (strcmp(token[i], "defun") == 0) {
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = DEF;
	} else if (48 <= token[i][0] && token[i][0] <= 57) {
	  next->cdr = (cons_t*)malloc(sizeof(cons_t));
	  next = next->cdr;
	  next->type = INT;
	  next->ivalue = typechange(token[i]);
	} else {
	  if (next->type == L_K && next->car ==NULL) {
		next->car = (cons_t*)malloc(sizeof(cons_t));
		next = next->car;
		next->type = FUNC;
	  } else {
		next->cdr = (cons_t*)malloc(sizeof(cons_t));
		next = next->cdr;
		next->type = STR;
	  }
	  next->cvalue = token[i];
	}
	i++;
  }
  next = NULL;
  return root;
}

void dump_tree(cons_t *root)
{
  if (root->type==ROOT) {
	dump_tree(root->cdr);
  } else if (root->type == L_K) {
	printf("(\n");
	if(root->car != NULL){
	  dump_tree(root->car);
	}
	if(root->cdr != NULL){
	  dump_tree(root->cdr);
	}
  } else if (root->type == STR) {
	printf("%s\n", root->cvalue);
	if (root->cdr != NULL) {
	  dump_tree(root->cdr);
	} else {
	  printf(")\n");
	}
  } else if (root->type == INT) {
	printf("%d\n", root->ivalue);
	if (root->cdr != NULL) {
	  dump_tree(root->cdr);
	} else {
	  printf(")\n");
	}
  } else if (root->type == ADD) {
	printf("+\n");
	dump_tree(root->cdr);
  } else if (root->type == SUB) {
	printf("-\n");
	dump_tree(root->cdr);
  } else if (root->type == MUL) {
	printf("*\n");
	dump_tree(root->cdr);
  } else if (root->type == DEV) {
	printf("/\n");
	dump_tree(root->cdr);
  } else if (root->type == IF) {
	printf("if\n");
	dump_tree(root->cdr);
  } else if (root->type == EQU) {
	printf("=\n");
	dump_tree(root->cdr);
  } else if (root->type == LT) {
	printf("<\n");
	dump_tree(root->cdr);
  } else if (root->type == GT) {
	printf(">\n");
	dump_tree(root->cdr); 
  } else if (root->type == ELT) {
	printf("<=\n");
	dump_tree(root->cdr); 
  } else if (root->type == EGT) {
	printf(">=\n");
	dump_tree(root->cdr);
  } else if (root->type == SETQ) {
	printf("setq\n");
	dump_tree(root->cdr);
  } else if (root->type == DEF) {
	printf("defun\n");
	dump_tree(root->cdr);
  } else if (root->type == FUNC) {
	printf("%s\n", root->cvalue);
	if (root->cdr!=0) {
	  dump_tree(root->cdr);
	}
  }
}

int eval(cons_t *root)
{
  cons_t *next;
  next = root;

  if (next->type == ROOT) {
	next = root->cdr;
  }
  
  if (next->type == L_K) {
	return	eval(next->car);
  }
  if (next->type == ADD) {
	cons_t *now;
	now = next;
	do {
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] += next->ivalue;
	  } else if (next->type == L_K) {
		if (next->car != NULL) {
		  next->result[g_funcl] = eval(next->car);
		  now->result[g_funcl] += next->result[g_funcl];
		} else {
		  next->result[g_funcl] = eval(next->cdr);
		  now->result[g_funcl] += next->result[g_funcl];
		}
	  } else if (next->type == STR) {
		now->result[g_funcl] += findmap(next->cvalue);
	  } else if (next->type == ARG) {
		now->result[g_funcl] += g_arga[g_argl];
	  }
	} while (next->cdr != NULL);	
	return now->result[g_funcl];
	
  } else if (next->type == MUL) {
	cons_t *now;
	now = next;
	now->result[g_funcl] = 1;
	do {
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] *= next->ivalue;
	  } else if (next->type == L_K) {
		next->result[g_funcl] = eval(next->car);
		now->result[g_funcl] *= next->result[g_funcl];
	  } else if (next->type == STR) {
		now->result[g_funcl] *= findmap(next->cvalue);
	  } else if (next->type == ARG) {
		now->result[g_funcl] *= g_arga[g_argl];
	  }
	} while (next->cdr != NULL);	
	return now->result[g_funcl];

  } else if (next->type == SUB) {
	cons_t *now;
	now = next;
	next = next->cdr;
	if (next->type == INT) {
	  now->result[g_funcl] = next->ivalue;
	} else if (next->type == L_K) {
	  now->result[g_funcl] = eval(next->car);
	} else if (next->type == STR) {
	  now->result[g_funcl] = findmap(next->cvalue);
	} else if (next->type == ARG) {
	  now->result[g_funcl] = g_arga[g_argl];
	}
	do {
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] -= next->ivalue;
	  } else if (next->type == L_K) {
		next->result[g_funcl] = eval(next->car);
		now->result[g_funcl] -= next->result[g_funcl];
	  } else if (next->type == STR){
		now->result[g_funcl] -= findmap(next->cvalue);
	  } else if (next->type == ARG) {
		now->result[g_funcl] -= g_arga[g_argl];
	  }
	} while(next->cdr != NULL);	
	return now->result[g_funcl];
 
  } else if (next->type == DEV) {
	cons_t *now;
	now = next;
	next = next->cdr;
	if (next->type == INT) {
	  now->result[g_funcl] = next->ivalue;
	} else if (next->type == L_K) {
	  now->result[g_funcl] = eval(next->car);
	} else if (next->type == STR) {
	  now->result[g_funcl] = findmap(next->cvalue);
	} else if (next->type == ARG) {
	  now->result[g_funcl] = g_arga[g_argl];
	}
	do {
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] /= next->ivalue;
	  } else if (next->type == L_K) {
		next->result[g_funcl] = eval(next->car);
		now->result[g_funcl] /= next->result[g_funcl];
	  } else if (next->type == STR){
		now->result[g_funcl] /= findmap(next->cvalue);
	  } else if (next->type == ARG) {
		now->result[g_funcl] /= g_arga[g_argl];
	  }
	} while(next->cdr != NULL);	
	return now->result[g_funcl];

  } else if (next->type == IF) {
	cons_t *now;
	int v;

	now = next;

	next = next->cdr;
	if (next->type == INT) {
	  v = next->ivalue;
	} else if (next->type == L_K) {
	  v = eval(next->car);
	} else if (next->type == STR) {
	  v = findmap(next->cvalue);
	} else if (next->type == ARG) {
	  v	= g_arga[g_argl];
	}

	if (v != 0) {
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] = next->ivalue;
		return now->result[g_funcl];
	  } else if (next->type == L_K) {
		now->result[g_funcl] = eval(next->car);
		return now->result[g_funcl];
	  } else if (next->type == STR) {
		now->result[g_funcl] = findmap(next->cvalue);
		return now->result[g_funcl];
	  } else if (next->type == ARG) {
		now->result[g_funcl] = g_arga[g_argl];
	  }
	} else {
	  next = next->cdr;
	  next = next->cdr;
	  if (next->type == INT) {
		now->result[g_funcl] = next->ivalue;
		return now->result[g_funcl];
	  } else if (next->type == L_K) {
		now->result[g_funcl] = eval(next->car);
		return now->result[g_funcl];
	  } else if (next->type == STR) {
		now->result[g_funcl] = findmap(next->cvalue);
	  } else if (next->type == ARG) {
		now->result[g_funcl] = g_arga[g_argl];
	  }
	}

  } else if (next->type == EQU) {
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
	  first = second;
	  second = first->cdr;
	  if (first->type == INT) {
		first->result[g_funcl] = first->ivalue;
	  } else if (first->type == L_K) {
		first->result[g_funcl] = eval(first->car);
	  } else if (first->type == STR) {
		first->result[g_funcl] = findmap(first->cvalue);
	  } else if (first->type == ARG) {
		first->result[g_funcl] = g_arga[g_argl];
	  }
	  if (second->type == INT) {
		second->result[g_funcl] = second->ivalue;
	  } else if (second->type == L_K){
		second->result[g_funcl] = eval(second->car);
	  } else if (second->type == STR) {
		second->result[g_funcl] = findmap(second->cvalue);
	  } else if (second->type == ARG) {
		second->result[g_funcl] = g_arga[g_argl];
	  }
	  if (first->result[g_funcl] == second->result[g_funcl]) {
		now->result[g_funcl] = TRUE;
	  } else {
		now->result[g_funcl] = FALSE;
		return 0;
	  }
	} while (second->cdr != 0);
	
	return now->result[g_funcl];
	
  } else if (next->type == LT) {
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
	   first = second;
	   second = first->cdr;
	   if (first->type == INT) {
		 first->result[g_funcl] = first->ivalue;
	   } else if (first->type == L_K) {
		 first->result[g_funcl] = eval(first->car);
	   } else if (first->type == STR) {
		 first->result[g_funcl] = findmap(first->cvalue);
	   } else if (first->type == ARG) {
		 first->result[g_funcl] = g_arga[g_argl];
	   }
	   if (second->type == INT) {
		 second->result[g_funcl] = second->ivalue;
	   } else if (second->type == L_K){
		 second->result[g_funcl] = eval(second->car);
	   } else if (second->type == STR) {
		 second->result[g_funcl] = findmap(second->cvalue);
	   } else if (second->type == ARG) {
		 second->result[g_funcl] = g_arga[g_argl];
	   }
	   if (first->result[g_funcl] < second->result[g_funcl]) {
		 now->result[g_funcl] = TRUE;
	   } else {
		 return 0;
	   }
    } while (second->cdr != 0);

	return now->result[g_funcl];
  
  } else if (next->type == GT) {
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do{
	   first = second;
	   second = first->cdr;
	   if (first->type == INT) {
		 first->result[g_funcl] = first->ivalue;
	   } else if (first->type == L_K) {
		 first->result[g_funcl] = eval(first->car);
	   } else if (first->type == STR) {
		 first->result[g_funcl] = findmap(first->cvalue);
	   } else if (first->type == ARG) {
		 first->result[g_funcl] = g_arga[g_argl];
	   }
	   if (second->type == INT) {
		 second->result[g_funcl] = second->ivalue;
	   } else if (second->type == L_K) {
		 second->result[g_funcl] = eval(second->car);
	   } else if (second->type == STR) {
		 second->result[g_funcl] = findmap(second->cvalue);
	   } else if (second->type == ARG) {
		 second->result[g_funcl] = g_arga[g_argl];
	   }
	   if (first->result[g_funcl] > second->result[g_funcl]) {
		 now->result[g_funcl] = TRUE;
	   } else {
		 return 0;
	   }
    } while (second->cdr != 0);

	return now->result[g_funcl];  

  } else if (next->type == ELT) {
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
	   first = second;
	   second = first->cdr;
	   if (first->type == INT) {
		 first->result[g_funcl] = first->ivalue;
	   } else if (first->type == L_K) {
		 first->result[g_funcl] = eval(first->car);
	   } else if (first->type == STR) {
		 first->result[g_funcl] = findmap(first->cvalue);
	   } else if (first->type == ARG) {
		 first->result[g_funcl] = g_arga[g_argl];
	   }
	   if (second->type == INT) {
		 second->result[g_funcl] = second->ivalue;
	   } else if (second->type == L_K) {
		 second->result[g_funcl] = eval(second->car);
	   } else if (second->type == STR) {
		 second->result[g_funcl] = findmap(second->cvalue);
	   } else if (second->type == ARG) {
		 second->result[g_funcl] = g_arga[g_argl];
	   }
	   if (first->result[g_funcl] <= second->result[g_funcl]) {
		 now->result[g_funcl] = TRUE;
	   } else {
		 return 0;
	   }
    } while (second->cdr != 0);

	return now->result[g_funcl];

  } else if (next->type == EGT) {
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
	   first = second;
	   second = first->cdr;
	   if (first->type == INT) {
		 first->result[g_funcl] = first->ivalue;
	   } else if (first->type == L_K) {
		 first->result[g_funcl] = eval(first->car);
	   } else if (first->type == STR) {
		 first->result[g_funcl] = findmap(first->cvalue);
	   } else if (first->type == ARG) {
	   }
		 first->result[g_funcl] = g_arga[g_argl];
	   if (second->type == INT) {
		 second->result[g_funcl] = second->ivalue;
	   } else if (second->type == L_K) {
		 second->result[g_funcl] = eval(second->car);
	   } else if (second->type == STR) {
		 second->result[g_funcl] = findmap(second->cvalue);
	   } else if (second->type == ARG) {
		 second->result[g_funcl] = g_arga[g_argl];
	   }
	   if (first->result[g_funcl] > second->result[g_funcl]) {
		 now->result[g_funcl] = TRUE;
	   } else {
		 return 0;
	   }
    } while (second->cdr != 0);

	return now->result[g_funcl];

  } else if (next->type == SETQ) {
	next = next->cdr;
	g_qa[g_qc].key = next->cvalue;
	next = next->cdr;
	if (next->type == INT) {
	  g_qa[g_qc].value = next->ivalue;
	} else if (next->type == L_K) {
	  g_qa[g_qc].value = eval(next->car);
	}
	printf("%s = %d\n", g_qa[g_qc].key, g_qa[g_qc].value);
	g_qc++;

  } else if (next->type == DEF) {
	cons_t *now = NULL;

	now = next->cdr;
	g_fa[g_fc].key = now->cvalue;

	now = now->cdr;
	next = now->car;
	g_fa[g_fc].arg = next->cvalue;
	g_fa[g_fc].exp = now->cdr;

	findarg(now->cdr, g_fa[g_fc]);
 
	now->cdr = NULL;

	g_fc++;

  } else if (next->type == FUNC) {
	cons_t *now;
	int nextarg;

	now = next;
	next = next->cdr;

	if (next->type == INT){
	  nextarg = next->ivalue;
	} else if (next->type == L_K) {
	  nextarg = eval(next);
	}

	g_funcl++;
	g_argl++;

	g_arga[g_argl] = nextarg;

	now->result[g_funcl-1] = eval(findfunc(now->cvalue).exp);
	clean(findfunc(now->cvalue).exp,g_funcl);

	g_argl--;	
	g_funcl--;

	return now->result[g_funcl];
  }

  return 0;
}

int findmap(char *key)
{
  int count;
  
  for (count = 0; strncmp(key, g_qa[count].key, sizeof(key)) != 0; count++) {
	;
  }
  
  return g_qa[count].value;
}

func findfunc(char *key)
{
  int count;

  for (count = 0; strncmp(key, g_fa[count].key, sizeof(key)) != 0; count++) {
	;
  }
  return g_fa[count];
}

void findarg(cons_t *root, func name)
{
  if (root->type == L_K) {
	if (root->car != NULL) {
	  findarg(root->car,name);
	}
	if (root->cdr != NULL) {
	  findarg(root->cdr,name);
	}
  } else if (root->type == STR) {
	if (strcmp(root->cvalue, name.arg) == 0) {
	  root->type = ARG;
	}
	if (root->cdr != NULL) {
	  findarg(root->cdr,name);
	}
  } else {
	if (root->cdr != NULL) {
	  findarg(root->cdr,name);
	}
  }
}

void clean(cons_t *root,int layer)
{
  if (root->type == L_K) {
	root->result[layer] = 0;
	
	if (root->car != NULL) {
	  clean(root->car,layer);
	}
	if (root->cdr != NULL) {
	  clean(root->cdr,layer);
	}
  }else {
	root->result[layer] = 0;
    if (root->cdr != NULL) {
	  clean(root->cdr,layer);
	}
  }
}
