#include "lisp.h"

void exe_lisp(char *input){
	Cons c;
	Cons *l = compile(input);
	c = eval(l);
	printf("%d\n", c.data.i);
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

