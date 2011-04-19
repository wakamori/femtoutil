#include "lisugimoto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//cons_t *parsing(char *);
void dump(cons_t *root,int depth);
void error();

cons_t *sgmt_read(char *line, int *pos)
{
	printf("pos=%d\n", *pos);
	cons_t *cell = NULL;
	// char *def="defun";
	//int blcount=0,brcount=0,opcount=0,errcount=0;
	//-----------check parse end--------
	if(line[*pos]=='\0' || line[*pos]==')') return NULL;
	//-----------check space------------
	while(line[*pos]==' '){
		(*pos)++;
	}
	//-----------parse'('---------------
	if (line[*pos] == '(') {
		cell =(cons_t *)malloc(sizeof(cons_t));
		cell->type=TYPE_START;
		printf("pos1=%d\n", *pos);
		(*pos)++;
		printf("pos2=%d\n", *pos);
		int tmp = *pos;
		printf("token=(\n");
		cell->car=sgmt_read(line, pos);
		(*pos)++;
		cell->cdr=sgmt_read(line, pos);
	}
	//------------parse')'---------------
	else if (line[*pos] == ')') {
		cell->cdr = NULL;
		printf("%dのとなりにNULL\n",cell->ivalue);
		printf("token =)\n");
	}//-----------parse'operand'---------
	else if(line[*pos]=='+'||line[*pos]=='-'||line[*pos]=='*'||line[*pos]=='/'||line[*pos]=='<'||line[*pos]=='>') {
		cell = (cons_t *)malloc(sizeof(cons_t));
		cell->value=line[*pos];
		if(line[*pos]=='+')cell->type=TYPE_PLUS;
		else if(line[*pos]=='-')cell->type=TYPE_MINUS;
		else if(line[*pos]=='*')cell->type=TYPE_MULTI;
		else if(line[*pos]=='/')cell->type=TYPE_DIVID;
		else if(line[*pos]=='<')cell->type=TYPE_CRE;
		else if(line[*pos]=='>')cell->type=TYPE_DECRE;
		(*pos)++;
		printf("token =%c\n", cell->value);
		cell->cdr=sgmt_read(line,pos);
	}
		//----------parse'digit'----------
	else if (isdigit(line[*pos])) {
			cell =(cons_t *)malloc(sizeof(cons_t));
			cell->ivalue=atoi((line + *pos));
			cell->type=TYPE_INT;
			(*pos)++;
			printf("token =%d\n",cell->ivalue);
			cell->cdr=sgmt_read(line,pos);
		}
	//------------parse'function'-------
		/*else if(strncmp(line[*pos],def,5)==0){//declare'function'
			cell=(cons_t *)malloc(sizeof(cons_t));
			cell->type=TYPE_DEFUN;
			if(isalpha(line[*pos])){
				cell->cdr=(cons_t *)malloc(sizeof(cons_t));
				cell = cell->cdr;
				cell->func=ps;
				printf("function was defined=%s\n",cell->func);
				// cell =cell ->cdr;
				while(isalpha(line[*pos]))ps++;
				while(line[*pos]==' ')ps++;
				if(line[*pos]=='(');
			}
			else error();
		}
		*/
		/*else{//check error
			if(errcount==0) {
				error();
				errcount++;
			}
		}
		*/
	

	return cell;
}

void dump(cons_t *cell,int depth){

	int i=0;
	if(cell==NULL){
		for(i=0;i<depth;i++){
			printf("\t");
		}
		printf(")\n");
		return;
	}
	// printf("depth=%d,operatestatus=%d\n",depth,cell->type);
	/*if(cell->type==TYPE_START){
	dump(cell->car,depth++);
	printf("depth=%d",depth);
	for (i = 0; i < depth; i++) {
	printf("\t");
	}
	printf("(");
	}
	*/

	/*printf("depth=%d",depth);
	if (cell->cdr != NULL)
	dump(cell->cdr,depth++);
	printf("\t");*/
	switch(cell->type) {
		case TYPE_START:
			dump(cell->car,depth + 1);
			for(i=0;i<depth;i++){
				printf("\t");
			}
			printf("(,depth=%d\n",depth);
			dump(cell->cdr,depth+1);
			break;
		case TYPE_INT:
			dump(cell->cdr,depth + 1);
			for(i=0;i<depth;i++){
				printf("\t");
			}
			printf("%d,depth=%d\n",cell->ivalue,depth);
			break;
		case TYPE_PLUS:
			dump(cell->cdr,depth+1);
			for(i=0;i<depth;i++){
				printf("\t");
			}
			printf("+,depth=%d\n",depth);
			break;
	}
}

void error(){
	printf("parsing error !!\n");
}
