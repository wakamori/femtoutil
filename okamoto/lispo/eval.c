#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

int eval(cons_t* p, ans_stack* ans_head)
{
		int x, y; 
		char_flag = 0;
		setq_flag = 0;

		switch(p->type){
				case s_bracket:
						ans_head = search_s_bracket(p, ans_head);
						x = ans_head->ans;
						ans_head = ans_free(ans_head);
						break;

				case string:
						ans_head = search_name(p, ans_head);
						x = ans_head->ans;
						ans_head = ans_free(ans_head);
						break;

				case add:
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x + p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = x + ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								p = p->cdr;
						}	
						break;

				case sub:
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x - p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = x - ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								p = p->cdr;
						}
						break;	

				case mult:
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x * p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = x * ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								p = p->cdr;
						}	
						break;

				case dev:
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x / p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = x / ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								p = p->cdr;
						}
						break;	

				case l_than:
						char_flag = 1;
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						if(p->type == number)
								y = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								y = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						if(x < y)
								x = 'T';
						else
								x = 'F';	
						break;

				case m_than:
						char_flag = 1;
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						p = p->cdr;
						if(p->type == number)
								y = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								y = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						if(x > y)
								x = 'T';
						else
								x = 'F';	
						break;

				case IF:
						p = p->cdr;
						ans_head = search_s_bracket(p, ans_head);
						char_flag = 0;
						x = ans_head->ans;
						ans_head = ans_free(ans_head);
						if(x == 'T'){
								p = p->cdr;
								if(p->type == number)
										x = p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								break;
						}
						else{
								p = p->cdr;
								p = p->cdr;
								if(p->type == number)
										x = p->ivalue;
								else{
										if(p->type == s_bracket)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == string)
												ans_head = search_name(p, ans_head);
										x = ans_head->ans;
										ans_head = ans_free(ans_head);
								}
								break;
						}

				case setq:
						p = p->cdr;
						name_stack* n_s_temp = (name_stack*)malloc(sizeof(name_stack));
						n_s_temp->type = variable;
						n_s_temp->name = p->value;
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == string)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_free(ans_head);
						}
						n_s_temp->ivalue = x;
						n_s_temp->next = n_s_head;
						n_s_head = n_s_temp;
						name_temp = n_s_head->name;
						setq_flag = 1;
						n_s_h_hold = n_s_head;
						break;

				case def:
						p = p->cdr;
						name_stack* n_s_temp1 = (name_stack*)malloc(sizeof(name_stack));
						n_s_temp1->type = func;
						n_s_temp1->name = p->value;
						name_temp = p->value;
						defun_flag = 1;
						p = p->cdr;
						name_stack* n_s_temp2 = (name_stack*)malloc(sizeof(name_stack));
						n_s_temp2->type = argument;
						n_s_temp2->name = p->car->value;
						n_s_temp2->arg_head = NULL;
						p = p->cdr;
						n_s_temp1->func_head = p->car;
						n_s_temp2->next = n_s_head;
						n_s_head = n_s_temp2;
						n_s_temp1->next = n_s_head;
						n_s_head = n_s_temp1;
						n_s_h_hold = n_s_head;
						x = 0;
						break;
		}
		return x;
}
