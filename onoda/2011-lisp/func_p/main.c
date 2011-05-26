#include "liso.h"
#include"mcheck.h"
int main(int argc, char *argv[])
{
	mtrace();
	if (argc == 2) {
		char **token = {NULL};
		char *input = NULL;
		char char_check;
		int size_count = 0;
		FILE *fp_input = NULL;
		FILE *fp_count = NULL;

		fp_input = fopen(argv[1], "r");
		fp_count = fopen(argv[1], "r");

		if (fp_input == NULL) {
			printf("file not open");
			return 0;
		}

		while (1) {

			do {
				char_check = getc(fp_count);
				size_count++;
			} while(char_check != '\n' && char_check != EOF);

			if (char_check == EOF) {
				break;
			}

			input = (char*)malloc(sizeof(char) * size_count);  
			fgets(input, size_count + 1, fp_input); 

			if (input[0] == '\n') {
				goto EXIT;
			}

//			printf("input\n%s\n", input);

			token = tokenizer(input);
//			dump_token(token);

			cons_t *root = NULL;
			root = maketree(token);
//			printf("tree\n");
//			dump_tree(root);

			int answer;
			answer = eval(root);

			printf("answer = %d\n", answer);

			free_tree(root);
			free_token(token);
		EXIT:
			free(input);
			input = NULL;

		}

		fclose(fp_input);
		fclose(fp_count);

	} else {

		char *input = NULL;
		char **token = {NULL};

		while (1) {  
			input = readline(">>>");
			add_history(input);

//			printf("readline\n%s\n", input);

			if (strncmp(input,"quit", sizeof("quit")) == 0 ||
					strncmp(input, "exit", sizeof("exit")) == 0) {
				break;
			}

			token = tokenizer(input);
//			dump_token(token);

			cons_t *root = NULL;
			root = maketree(token);
//			printf("tree\n");
//			dump_tree(root);

			int answer; 
			answer = eval(root);
			printf("answer = %d\n", answer);

			free_tree(root);
			free_token(token);
			free(input);
		}
		clear_history();
	}

	int d = 0;
	while (g_fa[d].key != NULL) {
		free(g_fa[d].key);
		free(g_fa[d].exp);
		d++;
	}
 
	muntrace();
	return 0;
}
