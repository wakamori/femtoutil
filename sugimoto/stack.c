#include "lisugimoto.h"

void a_push(cons_t *cell){
	int i = 0;
	while(cell != NULL) {
		arg_stack[i][stack_num] = sgmt_eval(cell).ivalue;
		cell = cell->cdr;
		i ++;
	}
	stack_num ++;
}

void a_pop(){
	stack_num --;
}

int a_get(int argcount){
	return arg_stack[argcount][stack_num - 1];
}

int get_arg_count(cons_t *cell){
	int i;
	for(i = 0;i < ARG_SIZE;i++){
		if(arg_a2[i][0] != NULL && strncmp(cell->symbol, arg_a2[i][0], sizeof(arg_a2[i][0])) == 0)
			return i;
	}
	return 2142144;
}
