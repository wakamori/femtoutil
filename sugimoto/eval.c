#include "lisugimoto.h"
void setarg(cons_t *cell);
cons_t sgmt_eval(cons_t *cell)
{
	cons_t result = {0, {NULL}, NULL};
	switch (cell->type) {
		case START:
					if(cell->car->type == SYMBOL && cell->car->type != DEFUN && cell->car->type != IF && cell->car->type != SETQ){
		cell->car->type = FUNC;
		}
					result = sgmt_eval(cell->car);
			break;
		case INT:
			result.ivalue = cell->ivalue;
			break;
		case T:
			result.type=T;
		case NIL:
			result.type=NIL;
		case FUNC:{
				cons_t *func_arg_tmp = cell->cdr;
					a_push(func_arg_tmp);
					result=sgmt_eval(ftable[hash(cell->symbol)]->oroot);
					a_pop();
					}
			break;
		case ARG:
			result.ivalue=a_get(get_arg_count(cell));
			break;
		case PLUS:
			while (cell->cdr != NULL) {
				result.ivalue += sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case MINUS:
			if (cell->cdr != NULL && cell->cdr->cdr != NULL) {
				result.ivalue = sgmt_eval(cell->cdr).ivalue;
			} else {
				result.ivalue = -sgmt_eval(cell->cdr).ivalue;
			}
			cell = cell->cdr;
			while(cell->cdr != NULL) {
				result.ivalue -= sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case MULTI:
			result.ivalue=1;
			while(cell->cdr != NULL) {
				result.ivalue = result.ivalue*sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case DIVID:
			result.ivalue = sgmt_eval(cell->cdr).ivalue;
			cell = cell->cdr;
			int div;
			while(cell->cdr != NULL) {
				div=sgmt_eval(cell->cdr).ivalue;
				if(div==0) {
					printf("division by zero !\n");
					result.ivalue = 0;
					result.car = NULL;
					result.cdr = NULL;
					break;
				}
				result.ivalue /= div;
				cell = cell->cdr;
			}
			break;
		case GT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue <= sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case LT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue >= sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case GEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue < sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case LEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue > sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case EQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue != sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case IF:
			cell=cell->cdr;
			if(sgmt_eval(cell).type == T){
				result=sgmt_eval(cell->cdr);
			} else if(sgmt_eval(cell).type == NIL) {
				result=sgmt_eval(cell->cdr->cdr);
			}
			break;
		case SETQ:
				vhash(cell);
				result.type = SETQ;
				result.ivalue = vtable[hash(cell->cdr->symbol)]->data;;
			break;
		case SYMBOL:
					result.ivalue = vtable[hash(cell->symbol)]->data;
			break;
		case DEFUN:{
				int i = 0;
				cons_t *tmp = cell;
				cons_t *tmp_o;
				result.type = DEFUN;
				cell = cell->cdr;
				result.symbol=cell->symbol;
				cell = cell->cdr;
				cons_t *tmp_a = cell->car;
				tmp_o = cell;
						while (tmp_a != NULL) {
					arg_a2[i][0] = tmp_a->symbol;
					tmp_a = tmp_a->cdr;
					i++;
				}
				setarg(tmp_o);
						fhash(tmp);
			break;
						}
		case END:
			break;
	}
	return result;
}

void setarg(cons_t *cell){
	int i= 0;
	if (cell == NULL) return;
	switch (cell->type){
		case START :
			if(cell->car->type == SYMBOL && cell->car->type != DEFUN && cell->car->type != IF && cell->car->type != SETQ){
				cell->car->type = FUNC;
			}
			setarg(cell->car);
			setarg(cell->cdr);
			break;
		case SYMBOL :
			for(i = 0;i < ARG_SIZE ; i ++ ){
						if(arg_a2[i][0] != NULL && strncmp(cell->symbol, arg_a2[i][0], sizeof(arg_a2[i][0])) == 0)
							cell->type = ARG;
						printf("argument is %s\n",arg_a2[i][0]);
					}
				setarg(cell->cdr);
				break;
		case END :
				break;
		default:
				setarg(cell->cdr);
				break;
	}
}

