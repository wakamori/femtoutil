#include "lisugimoto.h"

void a_push(int arg){
	// printf("push %d\n", arg);
as_data[stack_num] = arg;
stack_num ++;
}

void a_pop(){
	// printf("pop %d\n", as_data[stack_num]);
stack_num --;
}

int a_get(){
	// printf("get %d\n", as_data[stack_num]);
return as_data[stack_num - 1];
}
