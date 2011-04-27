#include "liso.h"

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
