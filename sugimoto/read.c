#include "lisugimoto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

cons_t *parsing(char *);
void error();

cons_t *sgmt_read(char *line)
{
	cons_t *cell = NULL;
	char *ps;
	char *def="defun";
	ps=line;
	int blcount=0,brcount=0,opcount=0,errcount=0;
	struct cons_t *root = NULL;
	struct cons_t *next = NULL;
	struct cons_t *tmp = NULL;
	while (*ps != '\0') {
		if(*ps==' '){
			ps++;
		}
		else if (*ps == '(') { //check'('
			cell =(cons_t *)malloc(sizeof(cons_t));
			if(root==NULL)root=cell;
			cell->type=TYPE_START;
			if(blcount >= 1)tmp = cell;
			next = cell->cdr;
			printf("token =(\n");
			ps++;
			blcount++;
		} else if (*ps == ')') {//check')'
			cell->cdr = NULL;
			printf("%dのとなりにNULL\n",cell->ivalue);
			if(tmp != NULL){
				tmp->cdr=(cons_t *)malloc(sizeof(cons_t));
				cell = tmp->cdr;
				tmp=NULL;
			}
			printf("token =)\n");
			brcount++;
			ps++;
		} else if(blcount !=0 && (*ps=='+'||*ps=='-'||*ps=='*'||*ps=='/'||*ps=='<'||*ps=='>')) {//check'operator'
			cell->car = (cons_t *)malloc(sizeof(cons_t));
			cell = cell->car;
			cell->value=*ps;
			if(*ps=='+')cell->type=TYPE_PLUS;
			else if(*ps=='-')cell->type=TYPE_MINUS;
			else if(*ps=='*')cell->type=TYPE_MULTI;
			else if(*ps=='/')cell->type=TYPE_DIVID;
			else if(*ps=='<')cell->type=TYPE_CRE;
			else if(*ps=='>')cell->type=TYPE_DECRE;
			printf("token =%c\n", cell->value);
			opcount++;
			ps++;
		} else if (blcount != 0 && blcount ==opcount && isdigit(*ps)) {//check'digit'
			cell->cdr =(cons_t *)malloc(sizeof(cons_t));
			cell = cell->cdr;
			cell->ivalue=atoi(ps);
			cell->type=TYPE_INT;
			printf("token =%d\n",cell->ivalue);
			while(isdigit(*ps)) {
				ps++;
			}
		} else if(blcount != 0 && strncmp(ps,def,5)==0){//declare'function'
			opcount++;
			cell->cdr=(cons_t *)malloc(sizeof(cons_t));
			cell = cell->cdr;
			cell->type=TYPE_FUNC;
			ps=ps+5;
			while(*ps==' ')ps++;
			if(isalpha(*ps)){
				cell->cdr=(cons_t *)malloc(sizeof(cons_t));
				cell = cell->cdr;
				cell->func=ps;
				printf("function was defined=%s\n",cell->func);
				// cell =cell ->cdr;
				while(isalpha(*ps))ps++;
				while(*ps==' ')ps++;
				if(*ps=='(');
			}
			else error();
		}
		else{//check error
			if(errcount==0) {
				error();
				errcount++;
			}
		}
	}
	if(blcount != brcount && errcount==0){
		error();
	}
	return root;
}

void error(){
	printf("parsing error !!\n");
}
