#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lispo.h"

void *allocate(long unsigned int size)
{
	printf("allocate : %lubytes\n", size);
	void *ret;
	if (from_heap_counter < HEAP_SIZE / 2) {
		if (from_heap_counter + size >= HEAP_SIZE / 2)
			gc();
		ret = heap + from_heap_counter;
		from_heap_counter += size;
		object_counter++;
		return ret;
	}
	else {
		if (from_heap_counter + size >= HEAP_SIZE)
			gc();
		ret = heap + from_heap_counter;
		from_heap_counter += size;
		object_counter++;
		return ret;
	}
}

char *copy_string(char* str)
{
	if(str == NULL)
		return str;
	live_object_counter++;
	str = memcpy(heap + to_heap_counter, str, strlen(str) + 1);
	to_heap_counter += (strlen(str) + 1);
	return str;
}

void format_cons_t(cons_t *p)
{
	p->h.size = sizeof(cons_t);
	if (from_heap_counter < HEAP_SIZE / 2)
		p->h.copied_flag = 0;
	else
		p->h.copied_flag = 1;
	p->type = 0;
	p->u.car = NULL;
	p->u.ivalue = 0;
	p->cdr = NULL;
	return;
}

cons_t *copy_cons_t(cons_t *p)
{
	if (p == NULL)
		return p;
	if (((from_heap_counter < HEAP_SIZE / 2) && (p->h.copied_flag == 1)) || ((HEAP_SIZE / 2 <= from_heap_counter) && (p->h.copied_flag == 0)))
		return p;
	else {
		live_object_counter++;
		p = memcpy(heap + to_heap_counter, p, p->h.size);
		to_heap_counter += p->h.size;
		if (from_heap_counter < HEAP_SIZE / 2)
			p->h.copied_flag = 1;
		else
			p->h.copied_flag = 0;
		if (p->type == S_BRACKET)
			p->u.car = copy_cons_t(p->u.car);
		else {
			if (p->type = E_BRACKET) {
				if (p->u.ivalue == FUNC)
					p->cdr = copy_cons_t(p->cdr);
				return;
			}
			if (p->type == STRING)
				p->u.value = copy_string(p->u.value);
			p->cdr = copy_cons_t(p->cdr);
		}
		return p;
	}
}

void format_s_b_stack(s_b_stack *s)
{
	s->h.size = sizeof(s_b_stack);
	if (from_heap_counter < HEAP_SIZE /2)
		s->h.copied_flag = 0;
	else
		s->h.copied_flag = 1;
	s->next = NULL;
	s->ps = NULL;
	return;
}

s_b_stack *copy_s_b_stack(s_b_stack *s)
{
	if (s == NULL)
		return s;
	if (((from_heap_counter < HEAP_SIZE / 2) && (s->h.copied_flag == 1)) || ((HEAP_SIZE / 2 <= from_heap_counter) && (s->h.copied_flag == 0)))

		return s;
	else {
		live_object_counter++;
		s = memcpy(heap + to_heap_counter, s, s->h.size);
		to_heap_counter += s->h.size;
		if (from_heap_counter < HEAP_SIZE / 2)
			s->h.copied_flag = 1;
		else
			s->h.copied_flag = 0;
//		s->ps = copy_cons_t(s->ps);
		s->next = copy_s_b_stack(s->next);
		return s;
	}
}

void format_arg_list(arg_list *arg)
{
	arg->h.size = sizeof(arg_list);
	if (from_heap_counter < HEAP_SIZE / 2)
		arg->h.copied_flag = 0;
	else
		arg->h.copied_flag = 1;
	arg->next = NULL;
	return;
}

arg_list *copy_arg_list(arg_list *arg)
{
	if (arg == NULL)
		return arg;
	if (((from_heap_counter < HEAP_SIZE / 2) && (arg->h.copied_flag == 1)) || ((HEAP_SIZE / 2 <= from_heap_counter) && (arg->h.copied_flag == 0)))

		return arg;
	else {
		live_object_counter++;
		arg = memcpy(heap + to_heap_counter, arg, arg->h.size);
		to_heap_counter += arg->h.size;
		if (from_heap_counter < HEAP_SIZE / 2)
			arg->h.copied_flag = 1;
		else
			arg->h.copied_flag = 0;
		arg->next = copy_arg_list(arg->next);
		return arg;
	}
}

void format_name_stack(name_stack *n)
{
	n->h.size = sizeof(name_stack);
	if (from_heap_counter < HEAP_SIZE / 2)
		n->h.copied_flag = 0;
	else
		n->h.copied_flag = 1;
	n->type = 0;
	n->name = NULL;
	n->u.arg_head = NULL;
	n->next = NULL;
	return;
}

name_stack *copy_name_stack(name_stack *n)
{
	if (n == NULL)
		return n;
	if (((from_heap_counter < HEAP_SIZE / 2) && (n->h.copied_flag == 1)) || ((HEAP_SIZE / 2 <= from_heap_counter) && (n->h.copied_flag == 0)))

		return n;
	else {
		live_object_counter++;
		n = memcpy(heap + to_heap_counter, n, n->h.size);
		to_heap_counter += n->h.size;
		if (from_heap_counter < HEAP_SIZE / 2)
			n->h.copied_flag = 1;
		else
			n->h.copied_flag = 0;
		n->name = copy_string(n->name);
		if (n->type == FUNC)
			n->u.func_head = copy_cons_t(n->u.func_head);
		else if (n->type == ARGUMENT)
			n->u.arg_head = copy_arg_list(n->u.arg_head);
		n->next = copy_name_stack(n->next);
		return n;
	}
}

void format_ans_stack(ans_stack *ans)
{
	ans->h.size = sizeof(ans_stack);
	if (from_heap_counter < HEAP_SIZE / 2)
		ans->h.copied_flag = 0;
	else
		ans->h.copied_flag = 1;
	ans->next = NULL;
	return;
}

ans_stack *copy_ans_stack(ans_stack *ans)
{
	if (ans == NULL)
		return ans;
	if (((from_heap_counter < HEAP_SIZE / 2) && (ans->h.copied_flag == 1)) || ((HEAP_SIZE / 2 <= from_heap_counter) && (ans->h.copied_flag == 0)))

		return ans;
	else {
		live_object_counter++;
		ans = memcpy(heap + to_heap_counter, ans, ans->h.size);
		to_heap_counter += ans->h.size;
		if (from_heap_counter < HEAP_SIZE / 2)
			ans->h.copied_flag = 1;
		else
			ans->h.copied_flag = 0;
		ans->next = copy_ans_stack(ans->next);
		return ans;
	}
}

void gc(void)
{
	live_object_counter = 0;

	if (from_heap_counter < HEAP_SIZE / 2)
		to_heap_counter = HEAP_SIZE / 2;
	else
		to_heap_counter = 0;
	if (s_b_s_head != NULL) {
		s_b_stack *temp = s_b_s_head;
		while(temp->next != NULL)
			temp = temp->next;
		temp->ps = copy_cons_t(temp->ps);
	}
	line = copy_string(line);
	s_b_s_head = copy_s_b_stack(s_b_s_head);
	n_s_h_hold = copy_name_stack(n_s_h_hold);
	from_heap_counter = to_heap_counter;
	printf("-----------Start GC program----------\n");
	printf("    live object : %d\n", live_object_counter);
	printf("    dead object : %d\n", object_counter - live_object_counter);
	printf("-----------Finish GC program---------\n");
	object_counter = live_object_counter;
	return;
}
