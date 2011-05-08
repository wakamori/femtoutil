#include "lisugimoto.h"

//int count = 0;
int main(int argc, char *argv[])
{
	count =0;
	const char *prompt = ">>> ";
	const char *quit = "(quit)";
	char *line = NULL;
	cons_t *root = NULL;
	cons_t result ={0,{ NULL},NULL};
	if (argc == 1) {
		while (1) {
			line = readline(prompt);
			add_history(line);
			if(strncmp(line,quit,6) == 0){
			return 0;
			}
			int pos = 0;
			if (sgmt_error(line, &pos) != 1){
			root = sgmt_read(line, &pos);
			dump(root,0);
			result = sgmt_eval(root);
			sgmt_print(&result);
		}
		}
		clear_history();
	} else {
		FILE *fp = NULL;
		char str[1024] = "";
		fp = fopen(argv[1], "r");
		if (fp == NULL) return 1;
		while(fgets(str,1024,fp)!=NULL) {
			str[strlen(str) - 1] = '\0';
			line=str;
			int pos = 0;
			if (sgmt_error(line,&pos) != 1){
			root = sgmt_read(line, &pos);
			dump(root,0);
			result = sgmt_eval(root);
			sgmt_print(&result);
			printf("number of calling function %d\n",count);
		}
		}
	}
	return 0;
}
