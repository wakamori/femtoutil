#include "liso.h"

int (*eval_fp[FPA_SIZE]) (cons_t *p) = {fl_k, NULL, add, sub, mul, dev, fstr, \
										error, froot, lif, equ, lt, gt, elt, \
										egt, setq, def, lfunc, error, rfunc, child_ans};


int get_map(char *key)
{
  int count = 0;
  
  while (1) {
	  if (g_qa[count].key == NULL || strcmp(g_qa[count].key, key) == 0) {
		  return count;
	  }
	  count++;
  }
}

int get_func(char *key)
{
	int count = 0;

	while (1) {
		if (g_fa[count].key == NULL || strcmp(key, g_fa[count].key) == 0) {
			return count;
		}
		count++;
	}
}

void find_func(cons_t *root, char *key, int count)
{
	switch (root->type) {
	case L_K:
		if (root->car != NULL) {
			find_func(root->car, key, count);
		}
		if (root->cdr != NULL) {
			find_func(root->cdr, key, count);
		}
		break;
	case FUNC:
		if (strcmp(root->cvalue, key) == 0) {
			root->type = RFUNC;
			root->ivalue = count;
			g_fa[count].rec_locate [g_fa[count].rec_num ++] = root;
		}
		if (root->cdr != NULL) {
			find_func(root->cdr, key, count);
		}
		break;
	default:
		if (root->cdr != NULL) {
			find_func(root->cdr, key, count);
		}
	}
}

void find_arg(cons_t *root, cons_t *arg_name)
{
	int arg_number = 0;
	cons_t *search_name = NULL;
	switch (root->type) {
	case L_K:
		if (root->car != NULL) {
			find_arg(root->car, arg_name);
		}
		if (root->cdr != NULL) {
			find_arg(root->cdr, arg_name);
		}
		break;
	case STR:
		search_name = arg_name;
		while (search_name != NULL) {
			if (strcmp(root->cvalue, search_name->cvalue) == 0) {
				root->type = ARG;
				root->ivalue = arg_number;
				break;
			}
			search_name = search_name->cdr;
			arg_number++;
		}
		if (root->cdr != NULL) {
			find_arg(root->cdr, arg_name);
		}
		break;
	default:
		if (root->cdr != NULL) {
			find_arg(root->cdr, arg_name);
		}
	}
}

void set_arg(cons_t *arg_value,int layer)
{
	int arg_number = 0;
	while (arg_value != NULL) {
		g_arga[layer + 1][arg_number] = get_value(arg_value);
		arg_number++;
		arg_value = arg_value->cdr;
	}
}


int get_value(cons_t *next)
{
	switch (next->type) {
	case L_K:
		if (next->car != NULL) {
			return eval(next->car);
		} else {
			return eval(next->cdr);
		}		
	case ARG:
		return g_arga[g_argl][next->ivalue];
	case INT:
		return next->ivalue;
	case STR:
		return g_qa[get_map(next->cvalue)].value;
	default:
		return 0;
	}
}


int add(cons_t *next)
{
	cons_t *now;
	now = next;
	int answer = 0;
	do {
		next = next->cdr;
		answer += get_value(next);
	} while (next->cdr != NULL);
	now->result = answer;	
	return now->result;
}

int mul(cons_t *next)
{
	cons_t *now;
	now = next;
	int answer = 1;
	do {
		next = next->cdr;
		answer *= get_value(next);
	} while (next->cdr != NULL);
	now->result = answer;
	return now->result;
}

int sub(cons_t *next)
{
	cons_t *now;
	now = next;
	next = next->cdr;
	int answer = get_value(next);
	do {
		next = next->cdr;
		answer -= get_value(next);
	} while(next->cdr != NULL);	
	now->result = answer;
	return now->result;
}

int dev(cons_t *next)
{
	cons_t *now;
	now = next;
	next = next->cdr;
	int answer = get_value(next);
	do {
		next = next->cdr;
		answer /= get_value(next);
	} while(next->cdr != NULL);	
	now->result = answer;
	return now->result;
}

int lif(cons_t *next)
{
	cons_t *now;
	int v;
	now = next;
	next = next->cdr;
	v = get_value(next);
	
	if (v != 0) {
		next = next->cdr;
		now->result = get_value(next);
		return now->result;
	} else {
		next = next->cdr;
		next = next->cdr;
		now->result = get_value(next);
		return now->result;
	}
}

int equ(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
		first = second;
		second = first->cdr;

		first->result = get_value(first);
		second->result = get_value(second);

		if (first->result == second->result) {
			now->result = TRUE;
		} else {
			now->result = FALSE;
			return now->result;
		}
	} while (second->cdr != NULL);
	
	return now->result;
}


int lt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;
	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		if (get_value(first) < get_value(second)) {
			;
		} else {
			now->result = FALSE;
			return now->result;
		}
    } while (second->cdr != NULL);

	now->result = TRUE;	
	return now->result;
}

int gt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do{
		first = second;
		second = first->cdr;
		first->result = get_value(first);
		second->result = get_value(second);
		if (first->result > second->result) {
			now->result = TRUE;
		} else {
			now->result = FALSE;
			return now->result;
		}
    } while (second->cdr != NULL);

	return now->result;  
}

int elt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		first->result = get_value(first);
		second->result = get_value(second);
		if (first->result <= second->result) {
			now->result = TRUE;
		} else {
			return 0;
		}
    } while (second->cdr != 0);
	
	return now->result;
}

int egt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;
	
	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		first->result = get_value(first);
		second->result = get_value(second);
		if (first->result > second->result) {
			now->result = TRUE;
		} else {
			now->result = FALSE;
			return now->result;
		}
    } while (second->cdr != NULL);

	return now->result;
}

int setq(cons_t *next)
{
	int count;
	next = next->cdr;
	count = get_map(next->cvalue);
	if (g_qa[count].key == NULL) {
		g_qa[count].key = (char*)malloc(strlen(next->cvalue) + 1);
		strcpy(g_qa[count].key, next->cvalue);
	}
	next = next->cdr;
	g_qa[count].value = get_value(next);
	printf("%s = %d\n", g_qa[count].key, g_qa[count].value);
	return 0;
}

int def(cons_t *next)
{
	int count;
	cons_t *now = NULL;
	now = next->cdr;
	count = get_func(now->cvalue);
	if (g_fa[count].key == NULL) {
		g_fa[count].key = (char*)malloc(strlen(now->cvalue) + 1);
		strcpy(g_fa[count].key, now->cvalue);
	}
	next = now;
	now = now->cdr;
	g_fa[count].exp = now->cdr;
	find_func(now->cdr, next->cvalue, count);
	next = now->car;

	find_arg(now->cdr, next);
	
	now->cdr = NULL;
	printf("define %s\n",g_fa[count].key);
	return 0;
}

int lfunc(cons_t *next)
{
	cons_t *now;
	now = next;
	int fc;

	fc = get_func(next->cvalue);
	if (g_fa[fc].key == NULL) {
		printf("syntax error\n");
		return 0;
	} else {	
		next = next->cdr;
		set_arg(next, g_argl);

		int rec = g_fa[fc].rec_num;
		if (rec >= 2) {

			g_argl++;

			rec_cal(fc);
			now->result = eval(g_fa[fc].exp);



			g_argl--;

		} else {
			g_argl++;

			now->result = eval(g_fa[fc].exp);

			g_argl--;
		}	

		return now->result;
	}
}

int rfunc(cons_t *next)
{
	cons_t *now;
	now = next;
	next = next->cdr;
	set_arg(next, g_argl);

	g_argl++;

	now->result = eval(g_fa[now->ivalue].exp);

	g_argl--;	

	return now->result;

}

int froot(cons_t *next)
{
	if (next->cdr != NULL) {
		return eval(next->cdr);
	} else {
		return eval(next->car);
	}
}

int fl_k(cons_t *next)
{
	if (next->car != NULL) {
		return eval(next->car);
	} else {
		return eval(next->cdr);
	}
}

int error(cons_t *next)
{
	printf("syntax error\n");
	return 0;
}

int fstr(cons_t *next)
{
	int count;
	count =	get_map(next->cvalue);
	if (g_qa[count].key == NULL) {
		printf("syntax error\n");
	} else {
		printf("%s = %d\n",g_qa[count].key, g_qa[count].value);
	}
	return 0;
}

int child_ans(cons_t *next)
{
	return next->result;
}

int eval(cons_t *root)
{
  cons_t *next;
  next = root;

  if (root == NULL) {
	  printf("syntax error\n");
	  return 0;
  } else {
	  return (*eval_fp[next->type]) (next);
  }
}

void rec_cal(int fc)
{
	int rec_num = g_fa[fc].rec_num;
	int shared_mem_id;
	int *shared_mem_ptr;
	int *ans;
	int ans_count;
	shared_mem_id = shmget(IPC_PRIVATE, rec_num * sizeof(int), IPC_CREAT);
	if ((shared_mem_ptr = shmat(shared_mem_id, NULL, 0)) == (void*)-1) {
		return;
	}

	ans = (int*)shared_mem_ptr;

	for (ans_count = 0; ans_count < rec_num; ans_count++) {
		pid_t child_pid;
		child_pid = fork();
		if (child_pid == 0) {
			*(ans + ans_count) = rfunc(g_fa[fc].rec_locate[ans_count]);
//			printf("%d\n",*(ans + ans_count));
			exit(0);
		}
	}

	for (ans_count = 0; ans_count < rec_num; ans_count++) {
		wait(NULL);
	}

	for (ans_count = 0; ans_count < rec_num; ans_count++) {
		g_fa[fc].rec_locate[ans_count]->type = CHILD_ANS;
		g_fa[fc].rec_locate[ans_count]->result = *(ans + ans_count);
//		printf("ans %d\n", *(ans + ans_count));
	}
}
