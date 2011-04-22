#include "lisugimoto.h"

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
				result.type=SETQ;
				result.ivalue = vtable[*search(hash(cell->cdr->symbol))]->data;
			break;
		case VAR: 
					result.ivalue = vtable[*search(hash(cell->symbol))]->data;
		case SYMBOL:
				cell->type = VAR;
			break;
		case DEFUN:{
					fhash(cell);
					result.type=DEFUN;
					result.symbol=cell->cdr->symbol;
		}
			break;
		case FUNC:
			break;
		case END:
			break;
	}
	return result;
}

