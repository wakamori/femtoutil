#include "lisugimoto.h"
void p_indent(char *ope,int depth);
cons_t *sgmt_read(char *line, int *pos)
{
	cons_t *cell = NULL;
	//-----------check space------------
	while(line[*pos]==' '){
		(*pos)++;
	}
	//-----------check parse end--------
	if(line[*pos]=='\0' || line[*pos]==')') return NULL;
	cell =(cons_t *)malloc(sizeof(cons_t));
	switch (line[*pos]) {
		//-----------parse'('---------------
		case '(':
			cell->type=START;
			(*pos)++;
			cell->car=sgmt_read(line, pos);
			break;
			//-----------parse'operand'---------
		case '=':
			cell->type=EQ;
			break;
		case '+':
			if (line[*pos+1] == ' ') cell->type=PLUS;
			else if (isdigit(line[*pos+1])) {
				cell->ivalue=atoi((line + *pos+1));
				cell->type=INT;
				while (isdigit(line[*pos + 1])) (*pos)++;
			}
			break;
		case '-':
			if (line[*pos+1] == ' ') cell->type=MINUS;
			else if (isdigit(line[*pos+1])) {
				cell->ivalue=-atoi((line + *pos+1));
				cell->type=INT;
				while (isdigit(line[*pos + 1])) (*pos)++;
			}
			break;
		case '*':
			cell->type = MULTI;
			break;
		case '/':
			cell->type = DIVID;
			break;
		case '<':
			if (line[*pos+1] == ' ') {
				cell->type=LT;
			} else if (line[*pos+1] == '=') {
				cell->type=LEQ;
				(*pos)++;
			}
			break;
		case '>':
			if (line[*pos+1] == ' ') {
				cell->type=GT;
			} else if (line[*pos+1] == '=') {
				cell->type=GEQ;
				(*pos)++;
			}
			break;
			//----------parse'digit'----------
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
			int len1 = 0,len2 = 0;
			while(line[*pos+len1] != ' ' && line[*pos+len1] !=')')len1++;
			while(isdigit(line[*pos+len2]))len2++;
			if(len1 == len2){
			cell->ivalue=atoi((line + *pos));
			cell->type=INT;
			while (isdigit(line[*pos + 1])) (*pos)++;
			}
				else{
				int len = 0;
				while(line[*pos+len] !=' ' && line[*pos+len]!=')')len++;
				cell->symbol=strndup((line+*pos),len);
				cell->type=SYMBOL;
				(*pos)=(*pos)+len-1;
			}
			}
			break;
			//----------Parse'if'-------------
		case 'i':
			if(line[*pos+1] == 'f'){
				cell->type=IF;
				(*pos)++;
			}
			else{
				int len = 0;
				while(line[*pos+len] !=' ' && line[*pos+len]!=')')len++;
				cell->symbol=strndup((line+*pos),len);
				cell->type=SYMBOL;
				(*pos)=(*pos)+len-1;
			}
			break;
		case 'T':
			cell->type=T;
			break;
		case 'N':
			if(line[*pos+1]=='I' && line[*pos+2]=='L')
				cell->type=NIL;
			(*pos)=(*pos)+2;
			break;
			//-----------Parse'setq'-----------
		case 's':
			if(line[*pos+1]=='e' && line[*pos+2]=='t' && line[*pos+3]=='q'){
				cell->type=SETQ;
				(*pos)=(*pos)+3;
			}
			else{
				int len = 0;
				while(line[*pos+len] !=' ' && line[*pos+len]!=')')len++;
				cell->symbol=strndup((line+*pos),len);
				cell->type=SYMBOL;
				(*pos)=(*pos)+len-1;
			}
			break;
			//------------Parse'defun'---------
		case'd':{
				if(line[*pos+1]=='e' && line[*pos+2]=='f' && line[*pos+3]=='u' && line[*pos+4]=='n'){
					cell->type=DEFUN;
					(*pos)=(*pos)+4;
				}
				else{
					int len = 0;
					while(line[*pos+len] !=' ' && line[*pos+len]!=')')len++;
					cell->symbol=strndup((line+*pos),len);
					cell->type=SYMBOL;
					(*pos)=(*pos)+len-1;
				}
			}
			break;
		default: {
			int len = 0;
			while(line[*pos+len] !=' ' && line[*pos+len] != ')')len++;
			cell->symbol=strndup((line+*pos),len);
			cell->type=SYMBOL;
			(*pos)=(*pos)+len;
			break;
		}
	}
	(*pos)++;
	cell->cdr=sgmt_read(line,pos); 
	return cell;
}

void dump(cons_t *cell,int depth){

	int i=0;
	if(cell==NULL){
		return;
	}
	switch(cell->type) {
		case START:
			dump(cell->car,depth + 1);
			p_indent("(\n",depth);
			dump(cell->cdr,depth + 1);
			return;
		case INT:
			dump(cell->cdr,depth + 1);
			for(i=0;i<depth;i++){
				printf("\t");
			}
			printf("%d",cell->ivalue);
			break;
		case PLUS:
			dump(cell->cdr,depth+1);
			p_indent("+",depth);
			break;
		case MINUS:
			dump(cell->cdr,depth+1);
			p_indent("-",depth);
			break;
		case MULTI:
			dump(cell->cdr,depth+1);
			p_indent("*",depth);
			break;
		case DIVID:
			dump(cell->cdr,depth+1);
			p_indent("/",depth);
			break;
		case GT:
			dump(cell->cdr,depth+1);
			p_indent(">",depth);
			break;
		case LT:
			dump(cell->cdr,depth+1);
			p_indent("<",depth);
			break;
		case GEQ:
			dump(cell->cdr,depth+1);
			p_indent(">=",depth);
			break;
		case LEQ:
			dump(cell->cdr,depth+1);
			p_indent("<=",depth);
			break;
		case IF:
			dump(cell->cdr,depth+1);
			p_indent("if",depth);
			break;
		case T:
			dump(cell->cdr,depth+1);
			p_indent("T",depth);
			break;
		case NIL:
			dump(cell->cdr,depth+1);
			p_indent("NIL",depth);
			break;
		case SETQ:
			dump(cell->cdr,depth+1);
			p_indent("setq",depth);
			break;
		case SYMBOL:
			dump(cell->cdr,depth+1);
			p_indent(cell->symbol,depth);
			break;
		case DEFUN:
			dump(cell->cdr,depth+1);
			p_indent("defun",depth);
			break;
		default:
			return;
	}
	printf("\n");
}


void error(){
	printf("parsing error !!\n");
}

void p_indent(char *ope,int depth){
	int i;
	for(i=0;i<depth;i++){
		printf("\t");
	}
	printf("%s",ope);
}
