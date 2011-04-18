#include "lisp.h"

void exe_lisp(char *input){
	cons_t *l = compile(input);
	cons_t p = eval(l, NULL);
	switch(p.type){
	case TYPE_INT: 
		printf("(int)%d\n", p.v.i);
		break;
	case TYPE_T:
		printf("T\n");
		break;
	case TYPE_NIL:
		printf("NIL\n");
		break;
	default:
		printf("(?)%d\n", p.v.i);
	}
}

void fib_test(){
	exe_lisp("(defun fib (n) (if (< n 3) 1 (+ (fib (- n 1)) (fib (- n 2)))))");
	exe_lisp("(fib 36)");
}

int main(void){
	char input[256];

	puts("WELCOME TO LOW(Lisp Of Wakamatsu)!");

	//fib_test();
	
	while(1){
		printf(">>>");
		fgets(input, 256, stdin);
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

