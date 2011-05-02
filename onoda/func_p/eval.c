#include "liso.h"

int (*func_p[FPA_SIZE]) (cons_t *p) = {fl_k, NULL, add, sub, mul, dev, fstr, \
									   error, froot, lif, equ, lt, gt, elt, \
									   egt, setq, def, lfunc, error, rfunc};


int getmap(char *key)
{
  int count = 0;
  
  while (1) {
	  if (g_qa[count].key == NULL || strcmp(g_qa[count].key, key) == 0) {
		  return count;
	  }
	  count++;
  }
}

int getfunc(char *key)
{
	int count = 0;

	while (1) {
		if (g_fa[count].key == NULL || strcmp(key, g_fa[count].key) == 0) {
			return count;
		}
		count++;
	}
}

void findfunc(cons_t *root, char *key, int count)
{
	switch (root->type) {
	case L_K:
		if (root->car != NULL) {
			findfunc(root->car, key, count);
		}
		if (root->cdr != NULL) {
			findfunc(root->cdr, key, count);
		}
		break;
	case FUNC:
		if (strcmp(root->cvalue, key) == 0) {
			root->type = RFUNC;
			root->ivalue = count;
		}
		if (root->cdr != NULL) {
			findfunc(root->cdr, key, count);
		}
		break;
	default:
		if (root->cdr != NULL) {
			findfunc(root->cdr, key, count);
		}
	}
}

void findarg(cons_t *root, char *key)
{
	switch (root->type) {
	case L_K:
		if (root->car != NULL) {
			findarg(root->car, key);
		}
		if (root->cdr != NULL) {
			findarg(root->cdr, key);
		}
		break;
	case STR:
		if (strcmp(root->cvalue, key) == 0) {
			root->type = ARG;
		}
		if (root->cdr != NULL) {
			findarg(root->cdr, key);
		}
		break;
	default:
		if (root->cdr != NULL) {
			findarg(root->cdr, key);
		}
	}
}
int getvalue(cons_t *next)
{
	switch (next->type) {
	case L_K:
		if (next->car != NULL) {
			return eval(next->car);
		} else {
			return eval(next->cdr);
		}		
	case ARG:
		return g_arga[g_argl];
	case INT:
		return next->ivalue;
	case STR:
		return g_qa[getmap(next->cvalue)].value;
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
		answer += getvalue(next);
	} while (next->cdr != NULL);
	now->result[g_funcl] = answer;	
	return now->result[g_funcl];
}

int mul(cons_t *next)
{
	cons_t *now;
	now = next;
	int answer = 1;
	do {
		next = next->cdr;
		answer *= getvalue(next);
	} while (next->cdr != NULL);
	now->result[g_funcl] = answer;
	return now->result[g_funcl];
}

int sub(cons_t *next)
{
	cons_t *now;
	now = next;
	next = next->cdr;
	int answer = getvalue(next);
	do {
		next = next->cdr;
		answer -= getvalue(next);
	} while(next->cdr != NULL);	
	now->result[g_funcl] = answer;
	return now->result[g_funcl];
}

int dev(cons_t *next)
{
	cons_t *now;
	now = next;
	next = next->cdr;
	int answer = getvalue(next);
	do {
		next = next->cdr;
		answer /= getvalue(next);
	} while(next->cdr != NULL);	
	now->result[g_funcl] = answer;
	return now->result[g_funcl];
}

int lif(cons_t *next)
{
	cons_t *now;
	int v;
	now = next;
	next = next->cdr;
	v = getvalue(next);
	
	if (v != 0) {
		next = next->cdr;
		now->result[g_funcl] = getvalue(next);
		return now->result[g_funcl];
	} else {
		next = next->cdr;
		next = next->cdr;
		now->result[g_funcl] = getvalue(next);
		return now->result[g_funcl];
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

		first->result[g_funcl] = getvalue(first);
		second->result[g_funcl] = getvalue(second);

		if (first->result[g_funcl] == second->result[g_funcl]) {
			now->result[g_funcl] = TRUE;
		} else {
			now->result[g_funcl] = FALSE;
			return now->result[g_funcl];
		}
	} while (second->cdr != NULL);
	
	return now->result[g_funcl];
}


int lt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;
	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		if (getvalue(first) < getvalue(second)) {
			;
		} else {
			now->result[g_funcl] = FALSE;
			return now->result[g_funcl];
		}
    } while (second->cdr != NULL);

	now->result[g_funcl] = TRUE;	
	return now->result[g_funcl];
}

int gt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do{
		first = second;
		second = first->cdr;
		first->result[g_funcl] = getvalue(first);
		second->result[g_funcl] = getvalue(second);
		if (first->result[g_funcl] > second->result[g_funcl]) {
			now->result[g_funcl] = TRUE;
		} else {
			now->result[g_funcl] = FALSE;
			return now->result[g_funcl];
		}
    } while (second->cdr != NULL);

	return now->result[g_funcl];  
}

int elt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;

	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		first->result[g_funcl] = getvalue(first);
		second->result[g_funcl] = getvalue(second);
		if (first->result[g_funcl] <= second->result[g_funcl]) {
			now->result[g_funcl] = TRUE;
		} else {
			return 0;
		}
    } while (second->cdr != 0);
	
	return now->result[g_funcl];
}

int egt(cons_t *next)
{
	cons_t *now,*first,*second;
	now = next;
	
	second = next->cdr;

	do {
		first = second;
		second = first->cdr;
		first->result[g_funcl] = getvalue(first);
		second->result[g_funcl] = getvalue(second);
		if (first->result[g_funcl] > second->result[g_funcl]) {
			now->result[g_funcl] = TRUE;
		} else {
			now->result[g_funcl] = FALSE;
			return now->result[g_funcl];
		}
    } while (second->cdr != NULL);

	return now->result[g_funcl];
}

int setq(cons_t *next)
{
	int count;
	next = next->cdr;
	count = getmap(next->cvalue);
	if (g_qa[count].key == NULL) {
		g_qa[count].key = (char*)malloc(strlen(next->cvalue) + 1);
		strcpy(g_qa[count].key, next->cvalue);
	}
	next = next->cdr;
	g_qa[count].value = getvalue(next);
	printf("%s = %d\n", g_qa[count].key, g_qa[count].value);
	return 0;
}

int def(cons_t *next)
{
	int count;
	cons_t *now = NULL;
	now = next->cdr;
	count = getfunc(now->cvalue);
	if (g_fa[count].key == NULL) {
		g_fa[count].key = (char*)malloc(strlen(now->cvalue) + 1);
		strcpy(g_fa[count].key, now->cvalue);
	}
	next = now;
	now = now->cdr;
	g_fa[count].exp = now->cdr;
	findfunc(now->cdr, next->cvalue, count);
	next = now->car;

	g_fa[count].arg = next->cvalue;
	findarg(now->cdr, next->cvalue);
	now->cdr = NULL;
	printf("define %s\n",g_fa[count].key);
	return 0;
}

int lfunc(cons_t *next)
{
	cons_t *now;
	now = next;
	int nextarg,fc;

	fc = getfunc(next->cvalue);
	if (g_fa[fc].key == NULL) {
		printf("syntax error\n");
		return 0;
	} else {	
		next = next->cdr;
		nextarg = getvalue(next);

		g_funcl++;
		g_argl++;

		g_arga[g_argl] = nextarg;
		now->result[g_funcl-1] = eval(g_fa[fc].exp);

		g_argl--;	
		g_funcl--;

		return now->result[g_funcl];
	}
}

int rfunc(cons_t *next)
{
	cons_t *now;
	int nextarg;
	
	now = next;
	next = next->cdr;

	nextarg = getvalue(next);

	g_funcl++;
	g_argl++;

	g_arga[g_argl] = nextarg;

	now->result[g_funcl-1] = eval(g_fa[now->ivalue].exp);

	g_argl--;	
	g_funcl--;

	return now->result[g_funcl];

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
	count =	getmap(next->cvalue);
	if (g_qa[count].key == NULL) {
		printf("syntax error\n");
	} else {
		printf("%s = %d\n",g_qa[count].key, g_qa[count].value);
	}
	return 0;
}

int eval(cons_t *root)
{
  cons_t *next;
  next = root;

  if (root == NULL) {
	  printf("syntax error\n");
	  return 0;
  } else {
	  return (*func_p[next->type]) (next);
  }
}
