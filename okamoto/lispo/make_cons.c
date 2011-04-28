#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

void make_cons(cons_t* p, FILE* fp)
{
	int i = 0;
	int x = 0;
	int b_counter = -1;
	int minus_flag = 0;
	int e_b_flag = 0;
	char* line_temp;
	char* line;
	char line1[100];
	
	if(fp != NULL){
		if((fgets(line1, 100, fp)) == NULL)
			exit(0);
		line = strndup(line1, strlen(line1));
	}
	else
		line = readline(">>> ");
	
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
				p->value = line;
				if(strncmp(p->value, "if", 2) == 0)
					p->type = IF;
				else if(strncmp(p->value, "defun", 5) == 0)
					p->type = def;
				else if(strncmp(p->value, "setq", 4) == 0)
					p->type = setq;
				else
					p->type = string;
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
			s_b_s_head = s_b_s_head->next;
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
				if(minus_flag == 1)
					x = x * 10 - decode(line[i]);
				else
					x = x * 10 + decode(line[i]);
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
				p->value = line;
				if(strncmp(p->value, "if", 2) == 0)
					p->type = IF;
				else if(strncmp(p->value, "defun", 5) == 0)
					p->type = def;
				else if(strncmp(p->value, "setq", 4) == 0)
					p->type = setq;
				else
					p->type = string;
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


