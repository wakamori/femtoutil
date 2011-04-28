#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

int eval(cons_t* p)
{
		int x, y; 
		char_flag = 0;
		setq_flag = 0;

		switch(p->type){
				case s_bracket:
						search_s_bracket(p);
						x = ans_head->ans;
						ans_free();
						break;

				case string:
						search_name(p);
						x = ans_head->ans;
						ans_free();
						break;

				case add:
						p = p->cdr;
						if(p->type == number)
								x = p->ivalue;
						else{
								if(p->type == s_bracket)
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x + p->ivalue;
								else{
										if(p->type == s_bracket)
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = x + ans_head->ans;
										ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x - p->ivalue;
								else{
										if(p->type == s_bracket)
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = x - ans_head->ans;
										ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x * p->ivalue;
								else{
										if(p->type == s_bracket)
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = x * ans_head->ans;
										ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						while(p->type != e_bracket){
								if(p->type == number)
										x = x / p->ivalue;
								else{
										if(p->type == s_bracket)
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = x / ans_head->ans;
										ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						if(p->type == number)
								y = p->ivalue;
						else{
								if(p->type == s_bracket)
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								y = ans_head->ans;
								ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
						}
						p = p->cdr;
						if(p->type == number)
								y = p->ivalue;
						else{
								if(p->type == s_bracket)
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								y = ans_head->ans;
								ans_free();
						}
						if(x > y)
								x = 'T';
						else
								x = 'F';	
						break;

				case IF:
						p = p->cdr;
						search_s_bracket(p);
						char_flag = 0;
						x = ans_head->ans;
						ans_free();
						if(x == 'T'){
								p = p->cdr;
								if(p->type == number)
										x = p->ivalue;
								else{
										if(p->type == s_bracket)
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = ans_head->ans;
										ans_free();
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
												search_s_bracket(p);
										else if(p->type == string)
												search_name(p);
										x = ans_head->ans;
										ans_free();
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
										search_s_bracket(p);
								else if(p->type == string)
										search_name(p);
								x = ans_head->ans;
								ans_free();
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

