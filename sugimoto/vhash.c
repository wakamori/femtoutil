#include"lisugimoto.h"
void add(int key,int data);
void clear(void);

void vhash(cons_t *cell){
				char *vname;
				int data;
				cell=cell->cdr;
				vname = cell->symbol;
				data = sgmt_eval(cell->cdr).ivalue;
				v_add(hash(vname),data);
}
/*-----Hash Function--------*/
int hash(char *name)
{
	int key = 0;
	while(*name != '\0')
		key = key + *name++;
	return key % T_SIZE;
}

/*-----V_Add Symbol Data------*/
void v_add(int key, int data)
{
	struct vlist *p;
	if( search(key) != NULL ){
		vtable[key]->data=data;
	}
	else{
		p =malloc(sizeof(struct vlist));
		p->key = key;
		p->data = data;
		p->next =vtable[key];
		vtable[key] = p;
	}
}
/*------Search Data---------*/
int *search(int key)
{
	struct vlist *p;
	int bucket;
	bucket = key;
	for( p=vtable[bucket]; p!=NULL; p=p->next )
	{
	  if( key == p->key )
		  return &p->key;
}
	return NULL;
}
/*-------Clear Hash Table----*/
void clear(void)
{
	struct vlist *p;
	struct vlist *q;
	int i;
	for( i=0; i<T_SIZE; ++i )
	{
	  p = vtable[i];
	  while( p != NULL ){
		  q = p;
		  p = p->next;
		  free( q );
	  }
	  vtable[i] = NULL;
  }
}
