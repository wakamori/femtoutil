#include "lisp.h"

Cons eval(Cons *list){
	Cons c;
	int n = 0;

	if(list->type == TYPE_PLUS){
		list = list->cdr;
		while(list != NULL){
			if(list->type == TYPE_INT){
				n += list->data.i;
			}else if(list->type == TYPE_LIST){
				n += eval(list->data.car).data.i;
			}else{
				printf("ERROR");
				exit(0);
			}
			list = list->cdr;
		}	
		c.data.i = n;
	}

	return c;
}

