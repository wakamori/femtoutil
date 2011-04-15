#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>

#define s_bracket '('
#define e_bracket ')'
#define space ' '
#define LF '\n'
#define number 1
#define add '+'
#define sub '-'
#define mult '*'
#define dev '/'

typedef struct cons_t{
	int type;
	union{
		struct cons_t* car;
		char* value;
		int ivalue;
	};
	struct cons_t* cdr;
}cons_t;	

typedef struct list{
	cons_t* ps;
	struct list* next;
}list;

list* head;

int decode(int n)
{
	return n - '0';
}

void make_cons(cons_t* p)
{
	int i = 0;
	int x = 0;
	int b_counter = -1;
	int minus_flag = 0;
	char* line = readline(">>> ");

	if(strncmp(line, "quit", 4) == 0)
		exit(0);

	while(b_counter != 0){ 
		if(b_counter == -1)
			b_counter++;
		if(line[i] == space || line[i] == LF){
			if(minus_flag == 1){
				p->type = sub;
				cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
				p->cdr = temp;
				p = temp;
				i++;
				minus_flag = 0;
				continue;
			}
			i++;
			continue;
		}
		else if(line[i] == s_bracket){
			p->type = s_bracket;
			b_counter++;
			
			list* l_temp = (list*)malloc(sizeof(list));
			l_temp->ps = p;
			l_temp->next = head;
			head = l_temp;

			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->car = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == e_bracket){
			p->type = e_bracket;
			b_counter--;
			
			if(b_counter == 0)
				break;

			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			head->ps->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == add){
			p->type = add;
			
			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == sub){
			minus_flag = 1;
			i++;
			}
		else if(line[i] == mult){
			p->type = mult;
			
			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == dev){
			p->type = dev;
			
			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if('0' <= line[i] && line[i] <= '9'){
			while('0' <= line[i] && line[i] <= '9'){
				x = x * 10 + decode(line[i]);
				if(minus_flag == 1)
					x = x * (-1);
				i++;
			}
			
			p->type = number; 
			p->ivalue = x;
			x = 0;

			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			continue;
		}
		else break;
	}
	return;
}

int eval(cons_t* p)
{
	int x;
	while(head != NULL){
		p = head->ps;
		p = p->car;
		if(p->type == add){
			p = p->cdr;
			x = p->ivalue;
			p = p->cdr;
			while(p->type != e_bracket){
				x = x + p->ivalue;
				p = p->cdr;
			}	
			p = head->ps;
			p->ivalue = x;
			x = 0;
			head = head->next;
		}

		if(p->type == sub){
			p = p->cdr;
			x = p->ivalue;
			p = p->cdr;
			while(p->type != e_bracket){
				x = x - p->ivalue;
				p = p->cdr;
			}	
			p = head->ps;
			p->ivalue = x;
			x = 0;
			head = head->next;
		}
		if(p->type == mult){
			p = p->cdr;
			x = p->ivalue;
			p = p->cdr;
			while(p->type != e_bracket){
				x = x * p->ivalue;
				p = p->cdr;
			}	
			p = head->ps;
			p->ivalue = x;
			x = 0;
			head = head->next;
		}
		if(p->type == dev){
			p = p->cdr;
			x = p->ivalue;
			p = p->cdr;
			while(p->type != e_bracket){
				x = x / p->ivalue;
				p = p->cdr;
			}	
			p = head->ps;
			p->ivalue = x;
			x = 0;
			head = head->next;
		}
	}
	return p->ivalue;
}
			
int main(void)
{
	while(1){
		cons_t* p = malloc(sizeof(cons_t));
		head = NULL;
		make_cons(p);
		printf("%d\n\n",eval(p));
		free(p);
		free(head);
	}
	return;
}
