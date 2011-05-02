#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

void search_s_bracket(cons_t* p)
{
	ans_alloc();
	ans_head->ans = eval(p->car);
	return;
}

void search_name(cons_t* p)
{
	while(n_s_head != NULL){
		if(strcmp(p->value, n_s_head->name) == 0){
			switch(n_s_head->type){
				case variable:
					ans_alloc();
					ans_head->ans = n_s_head->ivalue;
					break;
	
				case func:
					{
						arg_list* arg_temp = (arg_list*)malloc(sizeof(arg_list));
						if(p->cdr->type == number)
							arg_temp->arg = p->cdr->ivalue;
						else{
							if(p->cdr->type == s_bracket)
								search_s_bracket(p->cdr);
							if(p->cdr->type == string)
								search_name(p->cdr);
								arg_temp->arg = ans_head->ans;
								ans_free();
						}
						arg_temp->next = n_s_head->next->arg_head;
						n_s_head->next->arg_head = arg_temp;
						p->cdr->cdr->cdr = n_s_head->func_head;
						n_s_head = n_s_h_hold;
						ans_alloc();
						ans_head->ans = eval(p->cdr->cdr->cdr);
						while(strcmp(p->value, n_s_head->name) != 0)
							n_s_head = n_s_head->next;
						n_s_head = n_s_head->next;
						n_s_head->arg_head = n_s_head->arg_head->next;
						break;
					}

				case argument:
					ans_alloc();
					ans_head->ans = n_s_head->arg_head->arg;
					break;
			}
			n_s_head = n_s_h_hold;
			break;
		}
		n_s_head = n_s_head->next;
	}
	return;
}

