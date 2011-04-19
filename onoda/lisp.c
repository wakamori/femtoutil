#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<math.h>

typedef struct cons_t{
  int type;
  union{
    struct cons_t *car;
    int ivalue;
    char *cvalue;
  };
  struct cons_t *cdr;
  int result;
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
int eval(cons_t *root);


int main(int 
argc,char *argv[])
{
  if(argc==2){
	char input[64];
	char **token={NULL};
	FILE *fp=NULL;

	fp = fopen(argv[1],"r");

	if(fp==NULL){
	  return 0;
	}
	fgets(input,64,fp);

	printf("input\n%s\n",input);
	
	token = tokenizer(input);
	dump_token(token);

	cons_t *root=NULL;
	root = maketree(token);
	printf("tree\n");
	dump_tree(root);
	fclose(fp);

	return 0;

  }else{
	char *input=NULL;
	char **token={NULL};

	while(1){  
	  input = readline(">>>");
	  add_history(input);
	  
	  printf("readline\n%s\n",input);
	  
	  if(strcmp(input,"quit")==0 || strcmp(input,"exit")==0){
		break;
	  }
	  
	  token = tokenizer(input);
	  dump_token(token);
	  
	  cons_t *root=NULL;
	  root = maketree(token);
	  printf("tree\n");
	  dump_tree(root);

	  int answer;
	  answer = eval(root);
	  printf("answer=%d\n",answer);	  
	}
	return 0;
  }
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
	  if(strcmp(t[m-1],")")==0 && (*next<=47 || 58<=*next)){
		next = &linec[l+1];
	  }else{
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char)*(strlen(next)+1));
		strcpy(t[m],next);
		m++;
		next = &linec[l+1];
	  }
	}else if(linec[l]==')'){
	  if(strcmp(t[m-1],")")==0 && *next==')'){
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char));
		t[m] = ")";
		m++;
		next = &linec[l+1];
	  }else{	 
		linec[l] = '\0';
		t[m] = (char*)malloc(sizeof(char)*(strlen(next)+1));
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
  int size,intoken=0,i=0,digit;  
  size = strlen(chartoken);
  size--;
  for(digit=1;size>0;size--){
	digit = digit*10;
  }
  while(chartoken[i]!='\0'){
	intoken = intoken + digit*((int)chartoken[i]-48);
	i++;
	digit = digit/10;
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
	  next->result = 0;
	}else if(strcmp(token[i],"-")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = SUB;
	  next->result = 0;
	}else if(strcmp(token[i],"*")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next = next->car;
	  next->type = MUL;
	  next->result = 0;
	}else if(strcmp(token[i],"/")==0){
	  next->car = (cons_t*)malloc(sizeof(cons_t));
	  next =  next->car;
	  next->type = DEV;
	  next->result = 0;
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

int eval(cons_t *root)
{
  cons_t *next;
  next = root;

  if(next->type==ROOT){
	next = root->cdr;
  }
  
  if(next->type==L_K){
	return	eval(next->car);
  }
  if(next->type==ADD){
	cons_t *now;
	now = next;
	do{
	  next = next->cdr;
	  if(next->type==INT){
		now->result += next->ivalue;
	  }else if(next->type==L_K){
		next->result = eval(next->car);
		now->result += next->result;
	  }
	}while(next->cdr!=NULL);	
	return now->result;
	
  }else if(next->type==MUL){
	cons_t *now;
	now = next;
	now->result = 1;
	do{
	  next = next->cdr;
	  if(next->type==INT){
		now->result *= next->ivalue;
	  }else if(next->type==L_K){
		next->result = eval(next->car);
		now->result *= next->result;
	  }
	}while(next->cdr!=NULL);	
	return now->result;

  }else if(next->type==SUB){
	cons_t *now;
	now = next;
	next = next->cdr;
	if(next->type==INT){
	  now->result = next->ivalue;
	}else if(next->type==L_K){
	  now->result = eval(next->car);
	}
	do{
	  next = next->cdr;
	  if(next->type==INT){
		now->result -= next->ivalue;
	  }else if(next->type==L_K){
		next->result = eval(next->car);
		now->result -= next->result;
	  }
	}while(next->cdr!=NULL);	
	return now->result;
 
  }else if(next->type==DEV){
	cons_t *now;
	now = next;
	next = next->cdr;
	if(next->type==INT){
	  now->result = next->ivalue;
	}else if(next->type==L_K){
	  now->result = eval(next->car);
	}
	do{
	  next = next->cdr;
	  if(next->type==INT){
		now->result /= next->ivalue;
	  }else if(next->type==L_K){
		next->result = eval(next->car);
		now->result /= next->result;
	  }
	}while(next->cdr!=NULL);	

	return now->result;
  }

  return 0;
}
