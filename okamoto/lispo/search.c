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
	while(n_s_head != NULL){
		if(strcmp(p->value, n_s_head->name) == 0){
			switch(n_s_head->type){
				case variable:
					ans_head = ans_alloc(ans_head);
					ans_head->ans = n_s_head->ivalue;
					break;
	
				case func:
					{
						arg_list* temp = arg_alloc(n_s_head->next->arg_head);
						if(p->cdr->type == number)
							temp->arg = p->cdr->ivalue;
						else{
							if(p->cdr->type == s_bracket)
								ans_head = search_s_bracket(p->cdr, ans_head);
							if(p->cdr->type == string)
								ans_head = search_name(p->cdr, ans_head);
							temp->arg = ans_head->ans;
							ans_head = ans_free(ans_head);
						}
						temp->next = n_s_head->next->arg_head;
						n_s_head->next->arg_head = temp;
						p->cdr->cdr->cdr = n_s_head->func_head;
						n_s_head = n_s_h_hold;
						ans_head = ans_alloc(ans_head);
						ans_head->ans = eval(p->cdr->cdr->cdr, ans_head);
						while(strcmp(p->value, n_s_head->name) != 0)
							n_s_head = n_s_head->next;
						n_s_head = n_s_head->next;
						n_s_head->arg_head = arg_free(n_s_head->arg_head);
						break;
					}

				case argument:
					ans_head = ans_alloc(ans_head);
					ans_head->ans = n_s_head->arg_head->arg;
				 	break;
			}
			n_s_head = n_s_h_hold;
			break;
		}
		n_s_head = n_s_head->next;
	}
	return ans_head;
}
