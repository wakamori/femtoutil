#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

ans_stack* search_s_bracket(cons_t* p, ans_stack* ans_head)
{
	ans_head = ans_alloc(ans_head);
	ans_head->ans = eval(p->car, ans_head);
	return ans_head;
}

ans_stack* search_name(cons_t* p, ans_stack* ans_head)
{
	int arg_match_flag;	
	while(n_s_head != n_s_t_hold){
		arg_match_flag = 0;
		n_s_head = n_s_head->next;
		if(strcmp(p->value, n_s_head->name) == 0){
			switch(n_s_head->type){
				case VARIABLE:
					ans_head = ans_alloc(ans_head);
					ans_head->ans = n_s_head->ivalue;
					break;
	
				case FUNC:
					{
						func_key = n_s_head->func_key;
						cons_t* temp = p->cdr;
						name_stack* func_name = n_s_head;
						name_stack* n_s_temp = n_s_head->next;
						while(temp->type != E_BRACKET){
							arg_list* arg_temp = arg_alloc(n_s_temp->arg_head);
							if(temp->type == NUMBER)
								arg_temp->arg = temp->ivalue;
	 						else{
								if(temp->type == S_BRACKET)
									ans_head = search_s_bracket(temp, ans_head);
								if(temp->type == STRING)
									ans_head = search_name(temp, ans_head);
							arg_temp->arg = ans_head->ans;
							ans_head = ans_free(ans_head);
							}
							arg_temp->next = n_s_temp->arg_head;
							n_s_temp->arg_head = arg_temp;
							temp = temp->cdr;
							n_s_temp = n_s_temp->next;
						}
						temp->cdr = func_name->func_head;
						ans_head = ans_alloc(ans_head);
						ans_head->ans = eval(temp->cdr, ans_head);
						n_s_head = func_name->next;
						while(n_s_head != n_s_temp){
							n_s_head->arg_head = arg_free(n_s_head->arg_head);
							n_s_head = n_s_head->next;
						}
						break;
					}

				case ARGUMENT:
				 	if(n_s_head->func_key != func_key){
						arg_match_flag = 1;
						break;
					}	
					ans_head = ans_alloc(ans_head);
					ans_head->ans = n_s_head->arg_head->arg;
					break;
			}
			if(arg_match_flag == 1)
				continue;
			n_s_head = n_s_h_hold;
			break;
		}
	}
	return ans_head;
}