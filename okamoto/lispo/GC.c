#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lispo.h"

void *allocate(long unsigned int size)
{
	void *ret;
	if (from_heap_counter < HEAP_SIZE / 2) {
		if (from_heap_counter + size >= HEAP_SIZE / 2)
			gc();
		ret = heap + from_heap_counter;
		from_heap_counter += size;
		return ret;
	}
	else {
		if (from_heap_counter + size >= HEAP_SIZE)
			gc();
		ret = heap + from_heap_counter;
		from_heap_counter += size;
		return ret;
	}
}

void copy_string(char* str)
{
	if(str == NULL)
		return;
	str = strncpy(heap + to_heap_counter, str, strlen(str));
	to_heap_counter += strlen(str);
	return;
}

void format_cons_t(cons_t *p)
{
	p->h.size = sizeof(cons_t);
	p->h.copied_flag = 0;
	p->cdr = NULL;
	return;
}

void copy_cons_t(cons_t *p)
{
	if (p == NULL)
		return;
	if (p->h.copied_flag == 1)
		return;
	else {
		p = memcpy(heap + to_heap_counter, p, p->h.size);
		to_heap_counter += p->h.size;
		p->h.copied_flag = 1;
		if (p->type == S_BRACKET)
			copy_cons_t(p->u.car);
		else {
			if (p->type = E_BRACKET) {
				if (p->u.ivalue == FUNC)
					copy_cons_t(p->cdr);
				return;
			}
			if (p->type == STRING)
				copy_string(p->u.value);
			copy_cons_t(p->cdr);
		}
		return;
	}
}

void format_s_b_stack(s_b_stack *s)
{
	s->h.size = sizeof(s_b_stack);
	s->h.copied_flag = 0;
	s->next = NULL;
	s->ps = NULL;
	return;
}

void copy_s_b_stack(s_b_stack *s)
{
	if (s == NULL)
		return;
	if (s->h.copied_flag == 1)
		return;
	else {
		s = memcpy(heap + to_heap_counter, s, s->h.size);
		to_heap_counter += s->h.size;
		s->h.copied_flag = 1;
		copy_cons_t(s->ps);
		copy_s_b_stack(s->next);
		return;
	}
}

void format_arg_list(arg_list *arg)
{
	arg->h.size = sizeof(arg_list);
	arg->h.copied_flag = 0;
	arg->next = NULL;
	return;
}

void copy_arg_list(arg_list *arg)
{
	if (arg == NULL)
		return;
	if (arg->h.copied_flag == 1)
		return;
	else {
		arg = memcpy(heap + to_heap_counter, arg, arg->h.size);
		to_heap_counter += arg->h.size;
		arg->h.copied_flag = 1;
		copy_arg_list(arg->next);
		return;
	}
}

void format_name_stack(name_stack *n)
{
	n->h.size = sizeof(name_stack);
	n->h.copied_flag = 0;
	n->next = NULL;
	return;
}

void copy_name_stack(name_stack *n)
{
	if (n == NULL)
		return;
	if (n->h.copied_flag == 1)
		return;
	else {
		n = memcpy(heap + to_heap_counter, n, n->h.size);
		to_heap_counter += n->h.size;
		n->h.copied_flag = 1;
		copy_string(n->name);
		if (n->type == FUNC)
			copy_cons_t(n->u.func_head);
		else if (n->type == ARGUMENT)
			copy_arg_list(n->u.arg_head);
		copy_name_stack(n->next);
		return;
	}
}

void format_ans_stack(ans_stack *ans)
{
	ans->h.size = sizeof(ans_stack);
	ans->h.copied_flag = 0;
	ans->next = NULL;
	return;
}

void copy_ans_stack(ans_stack *ans)
{
	if (ans == NULL)
		return;
	if (ans->h.copied_flag == 1)
		return;
	else {
		ans = memcpy(heap + to_heap_counter, ans, ans->h.size);
		to_heap_counter += ans->h.size;
		ans->h.copied_flag = 1;
		copy_ans_stack(ans->next);
		return;
	}
}

void gc(void)
{
	if (from_heap_counter < HEAP_SIZE / 2)
		to_heap_counter = HEAP_SIZE / 2;
	else
		to_heap_counter = 0;
	copy_s_b_stack(s_b_s_head);
	copy_name_stack(n_s_h_hold);
	from_heap_counter = to_heap_counter;
	return;
}
