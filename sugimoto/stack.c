#include "lisugimoto.h"

void argset(cons_t *cell){
	int i;
	cons_t *tmp;
	tmp = cell;
	while (tmp != NULL){
		argsize ++;
		tmp = tmp->cdr;
	}
	printf ("argsize = %d\n",argsize);
	arg_s = (int **)malloc(sizeof(int*)*argsize);
	for(i = 0;i<argsize ;i ++ ){
		arg_s[i] = (int *)malloc(sizeof(int)*AS_SIZE);
	}
}

void a_push(cons_t *cell){
	int i;
	int arg_tmp = ftable[hash(cell->symbol)]->argsize;
	cell = cell->cdr;
	for(i = 0; i < arg_tmp;i ++){
		arg_s[i][stack_num] = sgmt_eval(cell).ivalue;
		cell = cell->cdr;
	}
	stack_num ++;
}

void a_pop(){
	stack_num --;
}
