#include "lisp.h"
#include "inst.h"

#define FILE_MAX (256 * 1024)

Code code[1024];
int code_index;

void exe_lisp(char *input){
	Token token;
	cons_t *c;
	token.input = input;
	while((c = create_list(&token)) != NULL){
		Code *begin = &code[code_index];
		compile(c);
		add_code(END, 0, 0);
		exe_code(begin);
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

	// get g_jtable
	exe_code(NULL);

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

