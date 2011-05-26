#include<stdio.h>

#define HEAP_SIZE sizeof(cons_t) * 128
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

s_b_stack* s_b_s_head;
name_stack* n_s_head;
name_stack* n_s_h_hold;
name_stack* n_s_t_hold;

char *name_temp;
void *heap;
int from_heap_counter;
int to_heap_counter;
int setq_flag;
int defun_flag;
int char_flag;
int func_key;
int func_key_hold;

void *allocate(long unsigned int size);
void copy_string(char *str);
void format_cons_t(cons_t *p);
void format_s_b_stack(s_b_stack *s);
void format_arg_list(arg_list *arg);
void format_name_stack(name_stack *n);
void format_ans_stack(ans_stack *ans);
void copy_cons_t(cons_t *p);
void copy_s_b_stack(s_b_stack *s);
void copy_arg_list(arg_list *arg);
void copy_name_stack(name_stack *n);
void copy_ans_stack(ans_stack *ans);
void gc(void);
int decode(int x);
void make_cons(cons_t* p, FILE* fp);
ans_stack*  search_s_bracket(cons_t* p, ans_stack* ans_head);
ans_stack* search_name(cons_t* p, ans_stack* ans_head);
int eval(cons_t* p, ans_stack* ans_head);
void print(int x);
void define_defun_info(cons_t* p);
