#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

int decode(int n)
{
	int x;
	x = n - '0';
	return x;
}

void define_defun_info(cons_t* p)
{
	cons_t* temp;
	temp = p->u.car;
	while(temp->type == STRING){
		name_stack* n_s_temp = allocate(sizeof(name_stack));
		format_name_stack(n_s_temp);
		n_s_temp->type = ARGUMENT;
		n_s_temp->name = temp->u.value;
		n_s_temp->func_key = func_key_hold;
		n_s_temp->u.arg_head = NULL;
		temp = temp->cdr;
		n_s_t_hold->next = n_s_temp;
		n_s_t_hold = n_s_temp;
	}
	return;
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
		printf("%s: %d\n", name_temp, x);
		setq_flag = 0;
	}
	else if(defun_flag == 1){
		printf("defun %s\n", name_temp);
		defun_flag = 0;
	}
	else
		printf("%d\n", x);
	return;
}

