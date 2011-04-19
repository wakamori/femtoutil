#include "lisugimoto.h"

int main(int argc, char *argv[])
{
	const char *prompt = ">>> ";
	char *line = NULL;
	cons_t *root = NULL;
	cons_t result ={0,{ NULL},NULL};
	if (argc == 1) {
		while (1) {
			line = readline(prompt);
			root = sgmt_read(line);
			result = sgmt_eval(root);
			sgmt_print(&result);
		}
	} else {
		FILE *fp = NULL;
		char str[80] = "";
		fp = fopen(argv[1], "r");
		if (fp == NULL) return 1;
		while(fgets(str,80,fp)!=NULL) {
			str[strlen(str) - 1] = '\0';
			line=str;
			root = sgmt_read(line);
			result = sgmt_eval(root);
			sgmt_print(&result);
		}
	}
	return 0;
}
