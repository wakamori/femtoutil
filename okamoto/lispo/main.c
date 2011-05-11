#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

int main(int argc, char* argv[])
{	
	setq_flag = 0;
	defun_flag = 0;
	char_flag = 0;
	func_key_hold = 0;
	n_s_head = (name_stack*)malloc(sizeof(name_stack));
	n_s_h_hold = n_s_head;
	n_s_t_hold = n_s_head;
	ans_free_head = NULL;
	ans_list = (ans_stack*)malloc(sizeof(ans_stack) * 1024);
	arg_pool = (arg_list*)malloc(sizeof(arg_list) * 1024);
	ans_counter = 0;
	arg_counter = 0;
	FILE* fp;
	if(argc == 2){
		if((fp = fopen(argv[1], "r")) == NULL){
			printf("file open eror!!\n");
			return;
		}
	}
	while(1){
		cons_t* p = malloc(sizeof(cons_t));
		s_b_s_head = NULL;
		ans_stack* ans_head = NULL;
		make_cons(p, fp);
		while(s_b_s_head->next != NULL)
			s_b_s_head = s_b_s_head->next;
		p = s_b_s_head->ps;
		print(eval(p, ans_head));
	}
	return;
}
