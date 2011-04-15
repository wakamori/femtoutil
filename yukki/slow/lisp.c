#include "lisp.h"

void exe_lisp(char *input){
	cons_t *l = compile(input);
	l = eval(l);
	switch(l->type){
	case TYPE_INT: 
		printf("(int)%d\n", l->v.i);
		break;
	case TYPE_T:
		printf("T\n");
		break;
	case TYPE_NIL:
		printf("NIL\n");
		break;
	default:
		printf("(?)%d\n", l->v.i);
	}
}

int main(void){
	char input[256];
	while(1){
		printf(">>>");
		gets(input);
		exe_lisp(input);
	}
	return 0;
}

/*
// parser test
int main(void){
	char s[256];
	Token t;

	gets(s);
	t.input = s;
	
	while(1){
		next_token(&t);
		if(t.type == TOKEN_NULL){
			break;
		}else if(t.type == TOKEN_INT){
			printf("int %d\n", t.num);
		}else if(t.type == TOKEN_STR){
			printf("str %s\n", t.str);
		}else{
			printf("%c\n", t.type);
		}
	}
	return 0;
}
*/

