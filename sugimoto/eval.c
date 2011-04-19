#include "lisugimoto.h"

cons_t sgmt_eval(cons_t *cell)
{
	cons_t result = {0, {NULL}, NULL};
	switch (cell->type) {
		case TYPE_START:
			result = sgmt_eval(cell->car);
			break;
		case TYPE_INT:
			result.ivalue = cell->ivalue;
			break;
		case TYPE_CHAR:
			break;
		case TYPE_PLUS:
			while (cell->cdr != NULL) {
				result.ivalue += sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case TYPE_MINUS:
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
		case TYPE_MULTI:
			result.ivalue=1;
			while(cell->cdr != NULL) {
				result.ivalue = result.ivalue*sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case TYPE_DIVID:
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
		case TYPE_GT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue <= sgmt_eval(cell->cdr).ivalue) {
					result.type = TYPE_NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != TYPE_NIL)
				result.type = TYPE_T;
			break;
		case TYPE_LT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue >= sgmt_eval(cell->cdr).ivalue) {
					result.type = TYPE_NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != TYPE_NIL)
				result.type = TYPE_T;
			break;
		case TYPE_GEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue < sgmt_eval(cell->cdr).ivalue) {
					result.type = TYPE_NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != TYPE_NIL)
				result.type = TYPE_T;
			break;
		case TYPE_LEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue > sgmt_eval(cell->cdr).ivalue) {
					result.type = TYPE_NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != TYPE_NIL)
				result.type = TYPE_T;
			break;
		case TYPE_FUNC:
			break;
		case TYPE_END:
			break;
	}
	return result;
}
