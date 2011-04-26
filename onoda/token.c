#include "liso.h"

char **tokenizer(char *line)
{
  
  char **t;
  t = (char**)malloc(sizeof(char*)*strlen(line));

  char *linec;
  linec = (char*)malloc(sizeof(char)*(strlen(line)));
  strcpy(linec,line);

  char *next;
  next = linec;

  int l,m = 0;

  for (l = 0; linec[l] != 0; l++) {
	if (linec[l] == '(') {
	  t[m] = (char*)malloc(sizeof(char));
	  t[m] = "(";
	  m++;
	  next = &linec[l+1];
	}else if (linec[l] == ' '){
	  if (strcmp(t[m-1],")") == 0 && (*next == ')' || *next == ' ')) {
		next = &linec[l+1];
	  } else {
		linec[l] = '\0';
		t[m] = (char*)malloc(strlen(next) + 1);
		strcpy(t[m],next);
		m++;
		next = &linec[l+1];
	  }
	} else if (linec[l] == ')') {
	  if (strcmp(t[m-1],")") == 0 && *next == ')') {
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char));
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  } else {	 
		linec[l] = '\0';
		t[m] = (char*)malloc(strlen(next) + 1);
		strcpy(t[m],next);
		m++;
		t[m] = (char*)malloc(sizeof(char));
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  }
	}
  }
  t[m] = (char*)malloc(sizeof(char));
  t[m] = NULL;

  return t;
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
