#include<stdio.h>

#define S_BRACKET '('
#define E_BRACKET ')'
#define SPACE ' '
#define COMMA ','
#define NUMBER 'n'
#define STRING 's'
#define ADD '+'
#define SUB '-'
#define MULT '*'
#define DEV '/'
#define L_THAN '<'
#define M_THAN '>'
#define VARIABLE 'v'
#define ARGUMENT 'a'
#define FUNC 'f'
#define DEF 'd'
#define IF 'i'
#define SETQ 'S'

typedef struct cons_t{
	int type;
	union{
		struct cons_t* car;
		char* value;
		int ivalue;
	};
	struct cons_t* cdr;
}cons_t;	

typedef struct s_b_stack{
	cons_t* ps;
	struct s_b_stack* next;
}s_b_stack;

typedef struct arg_list{
	int arg;
	struct arg_list* next;
}arg_list;

typedef struct name_stack{
	int type;
	char* name;
	int func_key;
	union{
		int ivalue;
		cons_t* func_head;
		arg_list* arg_head;
	};
	struct name_stack* next;
}name_stack;

typedef struct ans_stack{
	int ans;
	struct ans_stack* next;
}ans_stack;

ans_stack* ans_list;
ans_stack* ans_free_head;
arg_list* arg_pool;
arg_list* arg_free_head;
s_b_stack* s_b_pointer;
s_b_stack* s_b_s_head;
name_stack* n_s_head;
name_stack* n_s_h_hold;
name_stack* n_s_t_hold;

char* name_temp;
int setq_flag;
int defun_flag;
int char_flag;
int ans_counter;
int arg_counter;
int func_key;
int func_key_hold;

int decode(int x);
void make_cons(cons_t* p, FILE* fp);
ans_stack*  search_s_bracket(cons_t* p, ans_stack* ans_head);
ans_stack* search_name(cons_t* p, ans_stack* ans_head);
int eval(cons_t* p, ans_stack* ans_head);
void print(int x);
void define_defun_info(cons_t* p);
ans_stack* ans_alloc(ans_stack* ans_head);
ans_stack* ans_free(ans_stack* ans_head);
arg_list* arg_alloc(arg_list* arg_head);
arg_list* arg_free(arg_list* arg_head);
