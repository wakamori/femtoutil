#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>

#define s_bracket '('
#define e_bracket ')'
#define space ' '
#define LF '\n'
#define number 'n'
#define string 's'
#define add '+'
#define sub '-'
#define mult '*'
#define dev '/'
#define l_than '<'
#define m_than '>'
#define variable 'v'
#define func 'f'

typedef struct cons_t{
	int type;
	union{
		struct cons_t* car;
		char* value;
		int ivalue;
	};
	struct cons_t* cdr;
}cons_t;	

typedef struct s_b_stack{
	cons_t* ps;
	struct s_b_stack* next;
}s_b_stack;

typedef struct name_stack{
	int type;
	char* name;
	union{
		int ivalue;
		cons_t* func_head;
	};
	struct name_stack* next;
}name_stack;

s_b_stack* s_b_pointer;
s_b_stack* s_b_s_head;
name_stack* n_s_head;
name_stack* n_s_h_hold;
char* v_name_temp;
int setq_flag = 0;
int char_flag = 0;

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
	int e_b_flag = 0;
	char* line = readline(">>> ");
	char* line_temp;
	if(strncmp(line, "quit", 4) == 0){
		printf("\n");
		exit(0);
	}
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
			
			s_b_stack* s_b_s_temp = (s_b_stack*)malloc(sizeof(s_b_stack));
			s_b_s_temp->ps = p;
			s_b_s_temp->next = s_b_s_head;
			s_b_s_head = s_b_s_temp;

			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->car = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == e_bracket){
			if(e_b_flag == 1){
				line_temp = &line[i + 1];
				line[i] = '\0';
				i = 0;
				p->type = string;
				p->value = line;
				line = line_temp;
				cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
				p->cdr = temp;
				p = temp;
			}
			p->type = e_bracket;
			b_counter--;
			
			if(b_counter == 0)
				break;

			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			s_b_s_head->ps->cdr = temp;
			p = temp;
			if(e_b_flag != 1)
				i++;
			e_b_flag = 0;
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
		else if(line[i] == l_than){
			p->type = l_than;
			
			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == m_than){
			p->type = m_than;
			
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
		else{
			line = &line[i];
			i = 0;
			while(line[i] != space && line[i] != e_bracket)
				i++;
			if(line[i] == space){
				line_temp = &line[i + 1];
				line[i] = '\0';
				p->type = string;
				p->value = line;
				line = line_temp;
				i = 0;
			}
			else if(line[i] == e_bracket){
				e_b_flag = 1;
				continue;
			}
			cons_t* temp = (cons_t*)malloc(sizeof(cons_t));
			p->cdr = temp;
			p = temp;
			continue;
		}
	}
	return;
}

int	 search_s_bracket(cons_t* p)
{
	if(p->type == s_bracket){
			s_b_stack* s_b_p_temp = (s_b_stack*)malloc(sizeof(s_b_stack));
			s_b_p_temp->ps = p;
			s_b_p_temp->next = s_b_pointer;
			s_b_pointer = s_b_p_temp;
			p = p->car;
			s_b_pointer->ps->ivalue = eval(p);
			p = s_b_pointer->ps;
			s_b_pointer = s_b_pointer->next;
			return p->ivalue;
	}
	return 0;
}

void search_name(cons_t* p)
{
	if(p->type == string){
		if(strncmp(p->value, "if", 2) != 0 && strncmp(p->value, "setq", 4) != 0 && strncmp(p->value, "defun", 5) != 0){
			while(n_s_head != NULL){
				if(strcmp(p->value, n_s_head->name) == 0){
					if(n_s_head->type == variable){
						p->type = number;
						p->ivalue = n_s_head->ivalue;
					}
					n_s_head = n_s_h_hold;
					break;
				}
				n_s_head = n_s_head->next;
			}	
		}
	}
	return;
}

int eval(cons_t* p)
{
	int x;
	char_flag = 0;
	setq_flag = 0;

	search_s_bracket(p);
	if(p->type == add){
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		while(p->type != e_bracket){
			search_s_bracket(p);
			search_name(p);
			x = x + p->ivalue;
			p = p->cdr;
		}	
		return x;
	}
	else if(p->type == sub){
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		while(p->type != e_bracket){
			search_s_bracket(p);
			search_name(p);
			x = x - p->ivalue;
			p = p->cdr;
		}
		return x;	
	}
	else if(p->type == mult){
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		while(p->type != e_bracket){
			search_s_bracket(p);
			search_name(p);
			x = x * p->ivalue;
			p = p->cdr;
		}	
		return x;
	}
	else if(p->type == dev){
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		while(p->type != e_bracket){
			search_s_bracket(p);
			search_name(p);
			x = x / p->ivalue;
			p = p->cdr;
		}
		return x;	
	}
	else if(p->type == l_than){
		char_flag = 1;
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		if(x < (p->ivalue))
			x = 'T';
		else
			x = 'F';	
		return x;
	}
	else if(p->type == m_than){
		char_flag = 1;
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		x = p->ivalue;
		p = p->cdr;
		search_s_bracket(p);
		search_name(p);
		if(x > (p->ivalue))
			x = 'T';
		else
			x = 'F';	
		return x;
	}
	else if(p->type == string){
		if(strncmp(p->value, "if", 2) == 0){
			p = p->cdr;
			search_s_bracket(p);
			char_flag = 0;
		   	if(p->ivalue == 'T'){
				p = p->cdr;
				search_s_bracket(p);
				search_name(p);
				x = p->ivalue;
				return x;
			}
			else{
				p = p->cdr;
				p = p->cdr;
				search_s_bracket(p);
				search_name(p);
				x = p->ivalue;
				return x;
			}
		}
		else if(strncmp(p->value, "setq", 4) == 0){
			p = p->cdr;
			name_stack* n_s_temp = (name_stack*)malloc(sizeof(name_stack));
			n_s_temp->type = variable;
			n_s_temp->name = p->value;
			p = p->cdr;
			search_s_bracket(p);
			search_name(p);
			n_s_temp->ivalue = p->ivalue;
			x = p->ivalue;
			n_s_temp->next = n_s_head;
			n_s_head = n_s_temp;
			v_name_temp = n_s_head->name;
			setq_flag = 1;
			n_s_h_hold = n_s_head;
			return x;
		}
//			else if(strncmp(p->value, "defun", 5) == 0){
//
//			}				
	}
	return p->ivalue;
}

void print(int x)
{
	if(char_flag == 1){
		if(x == 'F')
			printf("NIL\n");
		else
			printf("%c\n",x);
		char_flag = 0;
	}
	else if(setq_flag == 1){
		printf("%s: %d\n", v_name_temp, x);
		setq_flag = 0;
	}
	else
		printf("%d\n", x); 
	return;
}

int main(void)
{	
	char_flag = 0;
	n_s_head = NULL;

	while(1){
		cons_t* p = malloc(sizeof(cons_t));
		s_b_s_head = NULL;
		s_b_pointer = NULL;
		make_cons(p);
		while(s_b_s_head->next != NULL)
			s_b_s_head = s_b_s_head->next;
		p = s_b_s_head->ps;
		print(eval(p));
	}
	return;
}
