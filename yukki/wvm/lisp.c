#include "lisp.h"
#include "inst.h"

#define FILE_MAX (256 * 1024)

Code code[1024];
int code_index;

void print_code(Code *c){
	char *names[] = {
		"MOV_V", "MOV_R", "MOV_B",
		"ADD", "SUB", "MUL", "DIV", "MOD",
		"LT", "LE", "GT", "GE", "EQ",
		"CMP", "JMP", "PUSH", "POP", "CALL", "RET"
	};
	while(1){
		printf("%s %d, %d\n", names[c->inst], c->v1.i, c->v2.i);
		if(c->inst == RET) break;
		c++;
	}
}

void exe_lisp(char *input){
	Token token;
	cons_t *c;
	token.input = input;
	while((c = create_list(&token)) != NULL){
		code_index = 0;
		eval(c);
		add_code(RET, 0, 0);

		print_code(code);

		exe_code(code);
		print_regs();
	}
}

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
	puts("WELCOME TO LOW(Lisp Of Wakamatsu) VM Version !!");

	for(i=1; i<argc; i++){
		open_file(argv[i]);
		return 0;
	}

	while(1){
		printf(">>>");
		fgets(input, 256, stdin);
		exe_lisp(input);
	}
	return 0;
}

