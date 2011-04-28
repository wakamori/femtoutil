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
	n_s_head = NULL;
	ans_free_head = NULL;
	ans_list = (ans_stack*)malloc(sizeof(ans_stack) * 5000);
	ans_counter = 0;
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
		ans_head = NULL;
		make_cons(p, fp);
		while(s_b_s_head->next != NULL)
			s_b_s_head = s_b_s_head->next;
		p = s_b_s_head->ps;
		print(eval(p));
	}
	return;
}

