#include "liso.h"

char **tokenizer(char *line)
{ 
	char **token;
	int lc,tc = 0;
	char *next;
	token = (char**)malloc(sizeof(char*)*strlen(line));
	next = line;
	
	for (lc = 0; line[lc] != 0 && line[lc] != '\n'; lc++) {
		switch (line[lc]) {
		case '(':
			token[tc] = (char*)malloc(sizeof(char) + 1);
			strcpy(token[tc], "(");
			tc++;
			break;
		case ')':
			token[tc] = (char*)malloc(sizeof(char) + 1);
			strcpy(token[tc], ")");
			tc++;
			break;
		case ' ':
			break;
		default:
			next = &line[lc];
			do{
				lc++;
			} while (line[lc] != '(' && line[lc] != ')' && line[lc] != ' ' && line[lc] != 0);
			switch (line[lc]) {
			case '(': 
				token[tc + 1] = (char*)malloc(sizeof(char) + 1);
				strcpy(token[tc + 1], "(");
				line[lc] = '\0';
				token[tc] = (char*)malloc(strlen(next) + 1);
				strcpy(token[tc], next);
				tc = tc +2;
				break;
			case ')':
				token[tc + 1] = (char*)malloc(sizeof(char) + 1);
				strcpy(token[tc + 1], ")");
				line[lc] = '\0';
				token[tc] = (char*)malloc(strlen(next) + 1);
				strcpy(token[tc], next);
				tc = tc +2;
				break;
			case ' ':
				line[lc] = '\0';
				token[tc] = (char*)malloc(strlen(next) + 1);
				strcpy(token[tc], next);
				tc++;
				break;
			case 0:
				token[tc] = (char*)malloc(strlen(next) + 1);
				strcpy(token[tc], next);
				lc--;
				tc++;
				break;
			}
		}
	}
	token[tc] = NULL;
	return token;
}


void dump_token(char **token)
{
	printf("token\n");
	int i = 0;
	while (token[i] != NULL) {
		printf("%s\n", token[i]);
		i++;
	}
}

void free_token(char **token)
{
	int i;
	for(i=0; token[i] != NULL; i++) {
		free(token[i]);
		token[i] = NULL;
	}
	free(token);
	token = NULL;
}
