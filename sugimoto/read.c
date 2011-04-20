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
	//-----------parse'('---------------
	cell =(cons_t *)malloc(sizeof(cons_t));
	switch (line[*pos]) {
	case '(':
		cell->type=TYPE_START;
		(*pos)++;
		cell->car=sgmt_read(line, pos);
		break;
	//-----------parse'operand'---------
	case '=':
		cell->type=TYPE_EQ;
		break;
	case '+':
		if (line[*pos+1] == ' ') cell->type=TYPE_PLUS;
		else if (isdigit(line[*pos+1])) {
			cell->ivalue=atoi((line + *pos+1));
			cell->type=TYPE_INT;
			while (isdigit(line[*pos + 1])) (*pos)++;
		}
		break;
	case '-':
		if (line[*pos+1] == ' ') cell->type=TYPE_MINUS;
		else if (isdigit(line[*pos+1])) {
			cell->ivalue=-atoi((line + *pos+1));
			cell->type=TYPE_INT;
			while (isdigit(line[*pos + 1])) (*pos)++;
		}
		break;
	case '*':
		cell->type = TYPE_MULTI;
		break;
	case '/':
		cell->type = TYPE_DIVID;
		break;
	case '<':
		if (line[*pos+1] == ' ') {
			cell->type=TYPE_LT;
		} else if (line[*pos+1] == '=') {
			cell->type=TYPE_LEQ;
			(*pos)++;
		}
		break;
	case '>':
		if (line[*pos+1] == ' ') {
			cell->type=TYPE_GT;
		} else if (line[*pos+1] == '=') {
			cell->type=TYPE_GEQ;
			(*pos)++;
		}
		break;
		//----------parse'digit'----------
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		cell->ivalue=atoi((line + *pos));
		cell->type=TYPE_INT;
		while (isdigit(line[*pos + 1])) (*pos)++;
		break;
	case 'i':
		if(line[*pos+1] == 'f'){
			cell->type=TYPE_IF;
			(*pos)++;
		}
		break;
	case 'T':
		cell->type=TYPE_T;
		break;
	case 'N':
		if(line[*pos+1]=='I' && line[*pos+2]=='L')
			cell->type=TYPE_NIL;
			(*pos)=(*pos)+2;
		break;
	default:
		/*todo*/
		break;

	}
	(*pos)++;
		cell->cdr=sgmt_read(line,pos); 
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
		return;
	}
	switch(cell->type) {
		case TYPE_START:
			dump(cell->car,depth + 1);
			p_indent("(\n",depth);
			dump(cell->cdr,depth + 1);
			return;
		case TYPE_INT:
			dump(cell->cdr,depth + 1);
			for(i=0;i<depth;i++){
				printf("\t");
			}
			printf("%d",cell->ivalue);
			break;
		case TYPE_PLUS:
			dump(cell->cdr,depth+1);
			p_indent("+",depth);
			break;
		case TYPE_MINUS:
			dump(cell->cdr,depth+1);
			p_indent("-",depth);
			break;
		case TYPE_MULTI:
			dump(cell->cdr,depth+1);
			p_indent("*",depth);
			break;
		case TYPE_DIVID:
			dump(cell->cdr,depth+1);
			p_indent("/",depth);
			break;
		case TYPE_GT:
			dump(cell->cdr,depth+1);
			p_indent(">",depth);
			break;
		case TYPE_LT:
			dump(cell->cdr,depth+1);
			p_indent("<",depth);
			break;
		case TYPE_GEQ:
			dump(cell->cdr,depth+1);
			p_indent(">=",depth);
			break;
		case TYPE_LEQ:
			dump(cell->cdr,depth+1);
			p_indent("<=",depth);
			break;
		case TYPE_IF:
			dump(cell->cdr,depth+1);
			p_indent("if",depth);
			break;
		case TYPE_T:
			dump(cell->cdr,depth+1);
			p_indent("T",depth);
			break;
		case TYPE_NIL:
			dump(cell->cdr,depth+1);
			p_indent("NIL",depth);
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


