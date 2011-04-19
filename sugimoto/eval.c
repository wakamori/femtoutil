#include "lisugimoto.h"

cons_t sgmt_eval(cons_t *cell)
{
	cons_t result = {0, {NULL}, NULL};
	switch (cell->type) {
	case TYPE_START:
		result.ivalue = sgmt_eval(cell->car).ivalue;
		printf("( is evaluated %d\n", result.ivalue);
		break;
	case TYPE_INT:
		result.ivalue = cell->ivalue;
		break;
	case TYPE_CHAR:
		break;
	case TYPE_PLUS:
		printf("+ is evaluated\n");
			while (cell->cdr != NULL) {
				result.ivalue += sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
				printf("now amount = %d\n",result.ivalue);
			}
			break;
	case TYPE_MINUS:
		break;
	case TYPE_MULTI:
		break;
	case TYPE_DIVID:
		break;
	case TYPE_CRE:
		break;
	case TYPE_DECRE:
		break;
	case TYPE_FUNC:
		break;
	case TYPE_END:
		break;
	}
	return result;
}
