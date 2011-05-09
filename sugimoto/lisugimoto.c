#include "lisugimoto.h"
void parse_tree_free(cons_t *root);
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
				parse_tree_free(root);
				vclear();
				fclear();
				return 0;
			}
			int pos = 0;
			root = sgmt_read(line, &pos);
			//dump(root,0);
			result = sgmt_eval(root);
			sgmt_print(&result);
			argsize = 0;
			//parse_tree_free(root);
		}
		clear_history();
	} else {
		int c_len = 0;
		FILE *fp = NULL;
		char *str;
		fp = fopen(argv[1], "r");
		if (fp == NULL) return 1;
		fseek(fp,0,SEEK_END);
		c_len = ftell(fp);
		fseek(fp,0,SEEK_SET);
		//printf("The number of letter = %d\n",c_len);
		str = (char *)malloc(c_len + 1);
		if (str == NULL)
			return 0;
		while(fgets(str,c_len,fp)!=NULL) {
			str[strlen(str) - 1] = '\0';
			line=str;
			int pos = 0;
			root = sgmt_read(line, &pos);
			//dump(root,0);
			result = sgmt_eval(root);
			sgmt_print(&result);
			//printf("number of calling function %d\n",count);
			argsize = 0;
			//parse_tree_free(root);
		}
		free(str);
		//vclear();
		//fclear();
	}
	return 0;
}




void parse_tree_free(cons_t *cell){
	if(cell == NULL) return;
	switch(cell->type){
		case START:
			parse_tree_free(cell->car);
			parse_tree_free(cell->cdr);
			free( cell );
			break;
		default :
			parse_tree_free(cell->cdr);
			free( cell );
			break;
	}
}
