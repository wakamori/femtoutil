#include"lisugimoto.h"
void f_add(int key,cons_t *aroot,cons_t *oroot);
void clear2(void);

/*------Function Hash-------*/
void fhash(cons_t *cell){
	char *fname;
	cons_t *aroot;
	cons_t *oroot;
	fname=cell->cdr->symbol;
	aroot = cell->cdr->cdr->car;
	oroot = cell->cdr->cdr->cdr;
	f_add(hash(fname),aroot,oroot);
}
/*-----Add Function Data------*/
void f_add(int key, cons_t *aroot,cons_t *oroot)
{
	struct flist *p;
	if (search(key) != NULL) {
		p = ftable[key];
		p->aroot = aroot;
		p->oroot = oroot;
	}
	else{
		p =malloc(sizeof(struct flist));
		p->key = key;
		p->aroot = aroot;
		p->oroot = oroot;
		p->next =ftable[key];
		ftable[key] = p;
	}
}
/*------Clear Table---------*/
void clear2(void)
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
