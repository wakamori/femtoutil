#include "lisugimoto.h"

void sgmt_print(cons_t *result)
{
	switch(result->type){
		case TYPE_T:
			printf("T");
			break;
		case TYPE_NIL:
			printf("NIL");
			break;
		case TYPE_INT:
			printf("%d",result->ivalue);
			break;
	}
	printf("\n");
}
