#include "lisugimoto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void parsing(char *);
void error();

cons_t *sgmt_read(char *line)
{
	parsing(line);
	return NULL;
}

void parsing(char *line)
{
	cons_t *cell = NULL;
	char *ps;
	char *def="defun";
	ps=line;
	int blcount=0,brcount=0,opcount=0,errcount=0,digicount=0;
	cell =(cons_t *)malloc(sizeof(cons_t));
	while (*ps != '\0') {
		if (*ps == '(') { //check'('
			cell->value=*ps;
			printf("token =%c\n", cell->value);
			blcount++;
		} else if (*ps == ')') {//check')'
			cell->cdr=NULL;
			printf("token =)\n");
			brcount++;
		} else if(blcount !=0 && (*ps=='+'||*ps=='-'||*ps=='*'||*ps=='/'||*ps=='<'||*ps=='>')) {//check'operator'
			cell = cell->cdr;
			cell =(cons_t *)malloc(sizeof(cons_t));
			cell->value=*ps;
			printf("token =%c\n", cell->value);
			opcount++;
		} else if (blcount != 0 && blcount ==opcount && isdigit(*ps)) {//check'digit'
			cell = cell->cdr;
			cell =(cons_t *)malloc(sizeof(cons_t));
			cell->ivalue=atoi(ps);
			printf("token =%d\n",cell->ivalue);
			while(isdigit(*(ps + 1))) {
				ps++;
			}
		} else if(blcount != 0 && strncmp(ps,def,5)==0){//declare'function'
			ps=ps+5;
			while(*ps==' ')ps++;
			if(isalpha(*ps)){
				cell =cell ->cdr;
				cell=(cons_t *)malloc(sizeof(cons_t));
				cell->func=ps;
				printf("function was defined=%s\n",cell->func);
				while(isalpha(*ps))ps++;
				while(*ps==' ')ps++;
				if(*ps=='(');			
			}
			else error();
		}
		else if(*ps ==' '){//check'space'
			ps++;
		}
		else{//check error
			if(errcount==0)	{
				error();
				errcount++;
			}
		}
		ps++;
	}
	if(blcount != brcount && errcount==0){
		error();

	}
	return;
}

void error(){
	printf("parsing error !!\n");
}
