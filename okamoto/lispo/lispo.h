#include<stdio.h>

#define HEAP_SIZE 16384
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

typedef struct header {
	int size;
	int copied_flag;
}header;

typedef struct cons_t{
	header h;
	int type;
	union{
		struct cons_t* car;
		char* value;
		int ivalue;
	}u;
	struct cons_t* cdr;
}cons_t;

typedef struct s_b_stack{
	header h;
	cons_t* ps;
	struct s_b_stack* next;
}s_b_stack;

typedef struct arg_list{
	header h;
	int arg;
	struct arg_list* next;
}arg_list;

typedef struct name_stack{
	header h;
	int type;
	char* name;
	int func_key;
	union{
		int ivalue;
		cons_t* func_head;
		arg_list* arg_head;
	}u;
	struct name_stack* next;
}name_stack;

typedef struct ans_stack{
	header h;
	int ans;
	struct ans_stack* next;
}ans_stack;

s_b_stack* s_b_pointer;
s_b_stack* s_b_s_head;
name_stack* n_s_head;
name_stack* n_s_h_hold;
name_stack* n_s_t_hold;

char *name_temp;
void *heap;
int heap_counter;
int setq_flag;
int defun_flag;
int char_flag;
int func_key;
int func_key_hold;

void *allocate(long unsigned int size);
void GC(void);
int decode(int x);
void make_cons(cons_t* p, FILE* fp);
ans_stack*  search_s_bracket(cons_t* p, ans_stack* ans_head);
ans_stack* search_name(cons_t* p, ans_stack* ans_head);
int eval(cons_t* p, ans_stack* ans_head);
void print(int x);
void define_defun_info(cons_t* p);
