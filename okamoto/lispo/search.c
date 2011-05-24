#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

ans_stack* search_s_bracket(cons_t* p, ans_stack* ans_head)
{
	ans_stack *temp = allocate(sizeof(ans_stack));
	format_ans_stack(temp);
	temp->next = ans_head;
	ans_head = temp;
	ans_head->ans = eval(p->u.car, ans_head);
	return ans_head;
}

ans_stack* search_name(cons_t* p, ans_stack* ans_head)
{
	int arg_match_flag;
	ans_stack *ans_temp;
	while(n_s_head != n_s_t_hold){
		arg_match_flag = 0;
		n_s_head = n_s_head->next;
		if(strcmp(p->u.value, n_s_head->name) == 0){
			switch(n_s_head->type){
				case VARIABLE:
					ans_temp = allocate(sizeof(ans_stack));
					format_ans_stack(ans_temp);
					ans_temp->next = ans_head;
					ans_head = ans_temp;
					ans_head->ans = n_s_head->u.ivalue;
					break;

				case FUNC:
					{
						func_key = n_s_head->func_key;
						cons_t* temp = p->cdr;
						name_stack* func_name = n_s_head;
						name_stack* n_s_temp = n_s_head->next;
						while(temp->type != E_BRACKET){
							arg_list* arg_temp = allocate(sizeof(arg_list));
							format_arg_list(arg_temp);
							if(temp->type == NUMBER)
								arg_temp->arg = temp->u.ivalue;
	 						else{
								if(temp->type == S_BRACKET)
									ans_head = search_s_bracket(temp, ans_head);
								if(temp->type == STRING)
									ans_head = search_name(temp, ans_head);
							arg_temp->arg = ans_head->ans;
							ans_head = ans_head->next;
							}
							arg_temp->next = n_s_temp->u.arg_head;
							n_s_temp->u.arg_head = arg_temp;
							temp = temp->cdr;
							n_s_temp = n_s_temp->next;
						}
						temp->cdr = func_name->u.func_head;
						ans_temp = allocate(sizeof(ans_stack));
						format_ans_stack(ans_temp);
						ans_temp->next = ans_head;
						ans_head = ans_temp;
						ans_head->ans = eval(temp->cdr, ans_head);
						n_s_head = func_name->next;
						while(n_s_head != n_s_temp){
							n_s_head->u.arg_head = n_s_head->u.arg_head->next;
							n_s_head = n_s_head->next;
						}
						break;
					}

				case ARGUMENT:
				 	if(n_s_head->func_key != func_key){
						arg_match_flag = 1;
						break;
					}
					ans_temp = allocate(sizeof(ans_head));
					format_ans_stack(ans_temp);
					ans_temp->next = ans_head;
					ans_head = ans_temp;
					ans_head->ans = n_s_head->u.arg_head->arg;
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
