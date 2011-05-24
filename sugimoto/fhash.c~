#include"lisugimoto.h"
void f_add(int key,cons_t *argcell,cons_t *oroot,int argsize);

/*------Function Hash-------*/
void fhash(cons_t *cell){
	char *fname;
	cons_t *oroot;
	cell = cell->cdr;
	fname = cell->symbol;
	cell = cell->cdr;
	oroot = cell->cdr;
	f_add(hash(fname),cell->car,oroot,argsize);
}
/*-----Add Function Data------*/
void f_add(int key, cons_t *argcell,cons_t *oroot,int argsize)
{
	int i = 0;
	struct flist *p = NULL;
	if (search(key) != NULL) {
		p = ftable[key];
		p->argsize =argsize;
		for(i = 0;i < p->argsize;i ++){
		p->arg_name[i] = argcell->symbol;
		argcell = argcell->cdr;
		}
		p->oroot = oroot;
	}
	else{
		p =malloc(sizeof(struct flist)+argsize);
		p->arg_name = (char **)malloc(argsize);
		p->argsize = argsize;
		for(i = 0;i < p->argsize ;i ++){
		p->arg_name[i] = (char *)malloc(sizeof(argcell->symbol));
		p->arg_name[i] = argcell->symbol;
		argcell = argcell->cdr;
		}
		p->oroot = oroot;
		p->key = key;
		p->next =ftable[key];
		ftable[key] = p;
	}
}
/*------Clear Table---------*/
void fclear(void)
{
	struct flist *p;
	struct flist *q;
	int i;
	for( i=0; i<T_SIZE; ++i )
	{
	p = ftable[i];
	while( p != NULL ){
		q = p;
		p = p->next;
		free( q );
	}
	ftable[i] = NULL;
	}
}
