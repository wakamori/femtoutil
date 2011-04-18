#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<math.h>

typedef struct cons_t{
  int type;
  union{
    struct cons_t *car;
    int ivalue;
    char *cvalue;
  };
  struct cons_t *cdr;
} cons_t;

#define MAX_DEPTH 32
#define L_K 0
#define R_K 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DEV 5
#define STR 6
#define INT 7
#define ROOT 8

void dump_token(char **token);
char **tokenizer(char *line);
cons_t *maketree(char **token);
int typechange(char *chartoken);
void dump_tree(cons_t *root);


int main()
{
  char *input;
  char **token;
  
  input = readline(">>>");

  printf("readline\n%s\n",input);

  token = tokenizer(input);
  dump_token(token);

  cons_t *root;
  root = maketree(token);
  printf("tree\n");
  dump_tree(root);

  return 0;  
}

void dump_token(char **token)
{
  printf("token\n");
  int i=0;
  while(token[i]!=NULL){
	printf("%s\n",token[i]);
	i++;
  }

}

char **tokenizer(char *line)
{
  
  char **t;
  t = (char**)malloc(sizeof(char*)*strlen(line));

  char *linec;
  linec = (char*)malloc(sizeof(char)*(strlen(line)));
  strcpy(linec,line);

  char *next;
  next = linec;

  int l,m=0;

  for(l=0;linec[l]!=0;l++){
	if(linec[l]=='('){
	  t[m] = (char*)malloc(sizeof(char));
	  t[m] = "(";
	  m++;
	  next = &linec[l+1];
	}else if(linec[l]==' '){
	  if(strcmp(t[m-1],")")==0){
		next = &linec[l+1];
	  }else{
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char)*(strlen(next)));
		strcpy(t[m],next);
		m++;
		next = &linec[l+1];
	  }
	}else if(linec[l]==')'){
	  if(strcmp(t[m-1],")")==0){
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char)*2);
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  }else{	 
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char)*(strlen(next)));
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

int typechange(char *chartoken)
{
  int size,intoken=0,i=0;  
  size = strlen(chartoken);
  size--;
  while(chartoken[i]!='\0'){
	intoken = intoken + ((int)chartoken[i]-48);
	i++;
  }
  return intoken;
}
 
cons_t *maketree(char **token)
{
  int depth=0,i=0;
  cons_t *ad[MAX_DEPTH] = {NULL};

  cons_t *root=NULL,*next=NULL;
  root = (cons_t*)malloc(sizeof(cons_t));
  root->type = ROOT;
  root->car = NULL;
  root->ivalue = 0;
  root->cvalue = 0;
  root->cdr = NULL;
  next = root;

  while(token[i]!=NULL){
	if(strcmp(token[i],"(")==0){
	  next->cdr = (cons_t*)malloc(sizeof(cons_t));
	  next = next->cdr;  
	  next->type = L_K;
	  next->cdr = NULL;
	  ad[depth] = next;
	  depth++;
	}else if(strcmp(token[i],")")==0){
	  next->cdr = NULL;
	  depth--;
	  next = ad[depth];
	}else if(strcmp(token[i],"+")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = ADD;
	}else if(strcmp(token[i],"-")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = SUB;
	}else if(strcmp(token[i],"*")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = MUL;
	}else if(strcmp(token[i],"/")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next =  next->car;
	  next->type = DEV;
	}else  if(('a'<token[i][0] && token[i][0]<'z') \
			  || ('A'<token[i][0] && token[i][0]<'Z')){
	  next->cdr = (cons_t*)malloc(sizeof(cons_t));
	  next = next->cdr;
	  next->type = STR;
	  next->cvalue = token[i];
	}else{
	  next->cdr = (cons_t*)malloc(sizeof(cons_t));
	  next = next->cdr;
	  next->type = INT;
	  next->ivalue = typechange(token[i]);
	}
	i++;
  }
  next = NULL;
  return root;
}

void dump_tree(cons_t *root)
{
  if(root->type==ROOT){
	dump_tree(root->cdr);
  }else if(root->type==L_K){
	printf("(\n");
	dump_tree(root->car);
	if(root->cdr!=NULL){
	  dump_tree(root->cdr);
	}
  }else	if(root->type==STR){
	printf("%s\n",root->cvalue);
  }else if(root->type==INT){
	printf("%d\n",root->ivalue);
	if(root->cdr!=NULL){
	  dump_tree(root->cdr);
	}else{
	  printf(")\n");
	}
  }else if(root->type==ADD){
	printf("+\n");
	dump_tree(root->cdr);
  }else if(root->type==SUB){
	printf("-\n");
	dump_tree(root->cdr);
  }else if(root->type==MUL){
	printf("*\n");
	dump_tree(root->cdr);
  }else if(root->type==DEV){
	printf("/\n");
	dump_tree(root->cdr);
  }
}
