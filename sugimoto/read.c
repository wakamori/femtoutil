#include "lisugimoto.h"
char *get_token(char *);

cons_t *sgmt_read(char *line)
{
	printf("%s\n", get_token(line));
	return NULL;
}

char *get_token(char *line)
{
	int size strlen(line);
	int i=0;
	while (line[i] != ' ') i++;
	//char *token = (char *)malloc(sizeof(char) * size);
	char *ps,*pg;//文字列のと終点
	ps=line;
	pg=token;
	for(i=0;i<size;i++){
		token[i]=*ps++;
		if(line[i]==' '){
			token[i] = '\n';
			//return token;
		}
	}
	return token;
}
