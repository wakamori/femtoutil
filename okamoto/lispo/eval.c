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
		name_stack* n_s_temp;

		switch(p->type){
				case S_BRACKET:
						ans_head = search_s_bracket(p, ans_head);
						x = ans_head->ans;
						ans_head = ans_head->next;
						break;

				case STRING:
						ans_head = search_name(p, ans_head);
						x = ans_head->ans;
						ans_head = ans_head->next;
						break;

				case ADD:
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						while(p->type != E_BRACKET){
								if(p->type == NUMBER)
										x = x + p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = x + ans_head->ans;
										ans_head = ans_head->next;
								}
								p = p->cdr;
						}
						break;

				case SUB:
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						while(p->type != E_BRACKET){
								if(p->type == NUMBER)
										x = x - p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = x - ans_head->ans;
										ans_head = ans_head->next;
								}
								p = p->cdr;
						}
						break;

				case MULT:
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						while(p->type != E_BRACKET){
								if(p->type == NUMBER)
										x = x * p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = x * ans_head->ans;
										ans_head = ans_head->next;
								}
								p = p->cdr;
						}
						break;

				case DEV:
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						while(p->type != E_BRACKET){
								if(p->type == NUMBER)
										x = x / p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = x / ans_head->ans;
										ans_head = ans_head->next;
								}
								p = p->cdr;
						}
						break;

				case L_THAN:
						char_flag = 1;
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						if(p->type == NUMBER)
								y = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								y = ans_head->ans;
								ans_head = ans_head->next;
						}
						if(x < y)
								x = 'T';
						else
								x = 'F';
						break;

				case M_THAN:
						char_flag = 1;
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						p = p->cdr;
						if(p->type == NUMBER)
								y = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								y = ans_head->ans;
								ans_head = ans_head->next;
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
						ans_head = ans_head->next;
						if(x == 'T'){
								p = p->cdr;
								if(p->type == NUMBER)
										x = p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = ans_head->ans;
										ans_head = ans_head->next;
								}
								break;
						}
						else{
								p = p->cdr;
								p = p->cdr;
								if(p->type == NUMBER)
										x = p->u.ivalue;
								else{
										if(p->type == S_BRACKET)
												ans_head = search_s_bracket(p, ans_head);
										else if(p->type == STRING)
												ans_head = search_name(p, ans_head);
										x = ans_head->ans;
										ans_head = ans_head->next;
								}
								break;
						}

				case SETQ:
					{
						p = p->cdr;
						n_s_temp = allocate(sizeof(name_stack));
						format_name_stack(n_s_temp);
						n_s_temp->type = VARIABLE;
						n_s_temp->name = p->u.value;
						p = p->cdr;
						if(p->type == NUMBER)
								x = p->u.ivalue;
						else{
								if(p->type == S_BRACKET)
										ans_head = search_s_bracket(p, ans_head);
								else if(p->type == STRING)
										ans_head = search_name(p, ans_head);
								x = ans_head->ans;
								ans_head = ans_head->next;
						}
						n_s_temp->u.ivalue = x;
						n_s_t_hold->next = n_s_temp;
						n_s_t_hold = n_s_temp;
						name_temp = n_s_t_hold->name;
						setq_flag = 1;
						break;
					}

				case DEF:
					{
						func_key_hold++;
						p = p->cdr;
						n_s_temp = allocate(sizeof(name_stack));
						format_name_stack(n_s_temp);
						n_s_temp->type = FUNC;
						n_s_temp->name = p->u.value;
						n_s_temp->func_key = func_key_hold;
						name_temp = p->u.value;
						defun_flag = 1;
						p = p->cdr;
						n_s_temp->u.func_head = p->cdr->u.car;
						n_s_t_hold->next = n_s_temp;
						n_s_t_hold = n_s_temp;
						define_defun_info(p);
						x = 0;
						break;
					}
		}
		return x;
}
