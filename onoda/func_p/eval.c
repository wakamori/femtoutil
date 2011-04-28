#include "liso.h"

int (*func_p[FPA_SIZE]) (cons_t *p) = {fl_k, NULL, add, sub, mul, dev, NULL, \
								   NULL, froot, lif, equ, lt, gt, elt, egt, setq, def, lfunc, NULL, rfunc};


int getmap(char *key)
{
  int count;
  
  for (count = 0; strncmp(key, g_qa[count].key, sizeof(key)) != 0; count++) {
	;
  }
  return g_qa[count].value;
}

func getfunc(char *key)
{
	int count;

	for (count = 0; strcmp(key, g_fa[count].key) != 0; count++) {
		;
	}
	return g_fa[count];
}

void findfunc(cons_t *root, char *key)
{
	switch (root->type) {
	case L_K:
		if (root->car != NULL) {
			findfunc(root->car, key);
		}
		if (root->cdr != NULL) {
			findfunc(root->cdr, key);
		}
		break;
	case FUNC:
		if (strcmp(root->cvalue, key) == 0) {
			root->type = RFUNC;
			root->ivalue = g_fc;
		}
		if (root->cdr != NULL) {
			findfunc(root->cdr, key);
		}
		break;
	default:
		if (root->cdr != NULL) {
			findfunc(root->cdr, key);
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
		return getmap(next->cvalue);
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
	next = next->cdr;
	g_qa[g_qc].key = next->cvalue;
	next = next->cdr;
	g_qa[g_qc].value = getvalue(next);
	printf("%s = ", g_qa[g_qc].key);
	g_qc++;
	return getvalue(next);
}

int def(cons_t *next)
{
	cons_t *now = NULL;
	
	now = next->cdr;
	g_fa[g_fc].key = now->cvalue;

	next = now->cdr;
	g_fa[g_fc].exp = next->cdr;
	findfunc(next->cdr, now->cvalue);
 
	now = next->cdr;
	next = next->car;
	g_fa[g_fc].arg = next->cvalue;
	findarg(now, next->cvalue);
	now->cdr = NULL;

	g_fc++;
	return 0;
}

int lfunc(cons_t *next)
{
	cons_t *now;
	int nextarg;
	
	now = next;
	next = next->cdr;

	nextarg = getvalue(next);

	g_funcl++;
	g_argl++;

	g_arga[g_argl] = nextarg;

	now->result[g_funcl-1] = eval(getfunc(now->cvalue).exp);

	g_argl--;	
	g_funcl--;

	return now->result[g_funcl];
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
	return eval(next->cdr);
}

int fl_k(cons_t *next)
{
	return eval(next->car);
}

int eval(cons_t *root)
{
  cons_t *next;
  next = root;

  return (*func_p[next->type]) (next);
}
