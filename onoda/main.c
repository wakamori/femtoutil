#include "liso.h"

int main(int argc, char *argv[])
{
  if (argc == 2) {
	char input[64];
	char **token = {NULL};
	FILE *fp = NULL;

	fp = fopen(argv[1],"r");

	if(fp == NULL){
	  printf("file not open");
	  return 0;
	}

	while ( fgets(input, 64, fp) != NULL) {
	  
	  printf("input\n%s\n", input);
	  
	  token = tokenizer(input);
	  dump_token(token);
	  
	  cons_t *root = NULL;
	  root = maketree(token);
	  printf("tree\n");
	  dump_tree(root);
	  
	  int answer;
	  answer = eval(root);
	  printf("answer=%d\n", answer);
	}

	fclose(fp);
	  
	return 0;

  } else {
	char *input = NULL;
	char **token = {NULL};

	while (1) {  
	  input = readline(">>>");
	  add_history(input);
	  
	  printf("readline\n%s\n", input);
	  
	  if (strncmp(input,"quit", sizeof("quit")) == 0 ||
		  strncmp(input, "exit", sizeof("exit")) == 0) {
		break;
	  }
	  
	  token = tokenizer(input);
	  dump_token(token);
	  
	  cons_t *root = NULL;
	  root = maketree(token);
	  printf("tree\n");
	  dump_tree(root);

	  int answer;
	  answer = eval(root);
	  printf("answer=%d\n", answer);

	}
	return 0;
  }
}
