#include "lisugimoto.h"
void setarg(cons_t *cell);
inline cons_t func_eval(cons_t *cell,cons_t result);
inline cons_t if_eval(cons_t *cell,cons_t result);
cons_t setq_eval(cons_t *cell,cons_t result);
cons_t defun_eval(cons_t *cell,cons_t result);
int a_get(int argcount){
	//printf("%s=%d", arg_a2[argcount][0], arg_stack[argcount][stack_num - 1]);$
	return arg_s[argcount][stack_num - 1];
}
char *func_name_tmp;
cons_t sgmt_eval(cons_t *cell)
{
	cons_t result = {0, {NULL}, NULL};
	switch (cell->type) {
		case START:
			if (cell->car->type == SYMBOL) {
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
		case IF:
			result = if_eval(cell,result);
			break;
		case FUNC:
			result = func_eval(cell,result);
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
		case SETQ:
			result = setq_eval(cell,result);
			break;
		case SYMBOL:
			result.ivalue = vtable[hash(cell->symbol)]->data;
			break;
		case DEFUN:
			result = defun_eval(cell,result);
			break;
		case END:
			break;
	}
	// printf("%d\n", result.ivalue);
	return result;
}

//char *arg_name_tmp = NULL;
void setarg(cons_t *cell){
	int i= 0;
	if (cell == NULL) return;
	switch (cell->type){
		case START :
			if(cell->car->type == SYMBOL){
				cell->car->type = FUNC;
			}
			setarg(cell->car);
			setarg(cell->cdr);
			break;
		case SYMBOL :
			for(i = 0;i < argsize ; i ++ ){
				if(ftable[hash(func_name_tmp)]->arg_name[i] != '\0' && strncmp(cell->symbol, ftable[hash(func_name_tmp)]->arg_name[i], sizeof(cell->symbol)) == 0) {
					cell->type = ARG;
					printf("setarg OK !!\n");
				}
			}
			setarg(cell->cdr);
			break;
		default:
			setarg(cell->cdr);
			break;
	}
}



cons_t func_eval(cons_t *cell,cons_t result) {
	count ++;
	cons_t *func_arg_tmp = cell->cdr;
	a_push(func_arg_tmp);
	result=sgmt_eval(ftable[hash(cell->symbol)]->oroot);
	a_pop();
	return result;
}

cons_t if_eval(cons_t *cell,cons_t result){
	cell=cell->cdr;
	cons_t tmp = sgmt_eval(cell);
	if(tmp.type == T){
		result=sgmt_eval(cell->cdr);
	} else if(tmp.type == NIL) {
		result=sgmt_eval(cell->cdr->cdr);
	}
	return result;
}

cons_t setq_eval(cons_t *cell,cons_t result){
	vhash(cell);
	result.type = SETQ;
	result.ivalue = vtable[hash(cell->cdr->symbol)]->data;
	return result;
}

int get_arg_count(cons_t *cell){
	int i = 0;
	for (i = 0;i < argsize;i ++ ) {
		if (ftable[hash(func_name_tmp)]->arg_name[i] != '\0' && strncmp(cell->symbol, ftable[hash(func_name_tmp)]->arg_name[i], sizeof(ftable[hash(func_name_tmp)]->arg_name[i])) == 0)
			return i;
	}
	return argsize + 1;
}

cons_t defun_eval(cons_t *cell,cons_t result){
	cons_t *tmp = cell;
	cons_t *tmp_o;
	result.type = DEFUN;
	cell = cell->cdr;
	func_name_tmp = cell->symbol;
	result.symbol = cell->symbol;
	cell = cell->cdr;
	argset(cell->car);
	tmp_o = cell->cdr;
	fhash(tmp);
	setarg(tmp_o);
	return result;
}
