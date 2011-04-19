#include "lisp.h"

void print_cons(cons_t *c){
	switch(c->type){
	case TYPE_INT: 
		printf("(int)%d\n", c->v.i);
		break;
	case TYPE_T:
		printf("T\n");
		break;
	case TYPE_NIL:
		printf("NIL\n");
		break;
	case TYPE_DEFUN:
		printf("DEFUN\n");
		break;
	default:
		printf("(?)value=%d\n", c->v.i);
	}
}

void exe_lisp(char *input){
	Token token;
	cons_t *c;
	cons_t r;

	token.input = input;
	while((c = create_list(&token)) != NULL){
		r = eval(c, NULL);
		print_cons(&r);
		//free_cons(c);
	}
}

void fib_test(){
	exe_lisp("(defun fib (n) (if (< n 3) 1 (+ (fib (- n 1)) (fib (- n 2)))))");
	exe_lisp("(fib 36)");
}

#define FILE_MAX (256 * 1024)

void open_file(char *name){
	FILE *fp;
	int size;
	char *buffer;

	if((fp = fopen(name, "r")) == NULL){
		printf("file open error:%s\n", name);
		return;
	}

	buffer = (char *)malloc(FILE_MAX);
	size = fread(buffer, 1, FILE_MAX, fp);

	fclose(fp);

	printf("read file %s\n", name);
	exe_lisp(buffer);
	free(buffer);
}

int main(int argc, char *argv[]){
	char input[256];
	int i;
	puts("WELCOME TO LOW(Lisp Of Wakamatsu)!");

	// allocate var_t array
	low_allocator();

	for(i=1; i<argc; i++){
		if(strcmp(argv[i], "-fib") == 0){
			fib_test();
			return 0;
		}else{
			open_file(argv[i]);
			return 0;
		}
	}

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

