nclude "lisugimoto.h" void a_add(int key, int data);
void clear3(void);

/*------Argument Hash-------*/
void ahash(cons_t * cell, char *fname)
{
    int argsize = 0;
    cell = cell->cdr;
    while (cell != NULL) {
	a_add(hash(fname), sgmt_eval(cell).ivalue);
	argsize++;
	cell = cell->cdr;

    }
}

/*-----Add Argument Data------*/
void a_add(int key, int data)
{
    struct alist *p;
    if (search(key) != NULL) {
	p = atable[key];
    } else {
	p = malloc(sizeof(struct alist));
	p->key = key;
	p->next = atable[key];
	atable[key] = p;
    }
}

/*------Clear Table---------*/
void clear3(void)
{
    struct alist *p;
    struct alist *q;
    int i;
    for (i = 0; i < T_SIZE; ++i) {
	p = atable[i];
	while (p != NULL) {
	    q = p;
	    p = p->next;
	    free(q);
	}
	atable[i] = NULL;
    }
}
