#include "lisp.h"
#include "inst.h"
#include <mcheck.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_HISTORY 10

extern int code_type;
extern Code codelist[CODE_MAX];
extern int code_index;

void exe_lisp(char *input){
	Token token;
	cons_t *c;
	token.input = input;
	while((c = create_list(&token)) != NULL){
		Code *begin = &codelist[code_index];
		if(compile(c, NULL, 0, 0)){
			printf("CodeGen Error\n");
		}else{
			if(code_type == -1) continue;

			if(code_type == TYPE_INT){
				add_code(PRINT_I, 0, 0);
			}else if(code_type == TYPE_T){
				add_code(PRINT_TNIL, 0, 0);
			}
			add_code(END, 0, 0);
			exec(begin);
		}
		free_cons(c);
	}
}

// concat s0 to s1
char *str_cat(char *s0, char *s1)
{
	int len1 = strlen(s1);
	if(s0 == NULL){
		s0 = (char *)malloc(len1 + 1);
		strcpy(s0, s1);
		return s0;
	}else{
		int len0 = strlen(s0);
		s0 = (char *)realloc(s0, len0 + len1 + 1);
		strcat(s0, s1);
		return s0;
	}
}

void open_file(char *name){
	FILE *fp;
	char in[256];
	char *line = NULL;
	if((fp = fopen(name, "r")) == NULL){
		printf("file open error:%s\n", name);
		return;
	}
	while((fgets(in, 256, fp)) != NULL){
		if(strchr(in, '\n') == NULL){
			line = str_cat(line, in);
		}else{
			if(line == NULL){
				exe_lisp(in);
			}else{
				line = str_cat(line, in);
				exe_lisp(line);
				free(line);
				line = NULL;
			}
		}
	}
	fclose(fp);
	if(line == NULL) free(line);
}

int main(int argc, char *argv[]){
	int i;
	mtrace();
	puts("WELCOME TO LOW(Lisp Of Wakamatsu) VM Version !!");
	// get g_jtable
	exec(NULL);
	// read file
	for(i=1; i<argc; i++){
		open_file(argv[i]);
		return 0;
	}
	// interactive mode
	while(1){
		char *input = readline(">>>");
		if(input == NULL) break;
		if(strcmp(input, "exit")==0 || strcmp(input, "bye")==0){
			break;
		}
		add_history(input);
		exe_lisp(input);
		free(input);
	}
	return 0;
}

