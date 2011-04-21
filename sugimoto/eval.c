#include "lisugimoto.h"
/*-----Symbol list--------*/
struct slist{
	int key;
	int data;
	struct slist *next;
};
/*-----Function List-------*/
//struct flist{
//				//
//};
struct slist *stable[T_SIZE];
//struct flist *ftable[T_SIZE];
int getkey(char *name);
int hash(int key);
int add(int key,int data);
int* search(int key);
void clear(void);

cons_t sgmt_eval(cons_t *cell)
{
	cons_t result = {0, {NULL}, NULL};
	switch (cell->type) {
		case START:
			result = sgmt_eval(cell->car);
			break;
		case INT:
			result.ivalue = cell->ivalue;
			break;
		case T:
			result.type=T;
		case NIL:
			result.type=NIL;
		case PLUS:
			while (cell->cdr != NULL) {
				result.ivalue += sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case MINUS:
			if (cell->cdr != NULL && cell->cdr->cdr != NULL) {
				result.ivalue = sgmt_eval(cell->cdr).ivalue;
			} else {
				result.ivalue = -sgmt_eval(cell->cdr).ivalue;
			}
			cell = cell->cdr;
			while(cell->cdr != NULL) {
				result.ivalue -= sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case MULTI:
			result.ivalue=1;
			while(cell->cdr != NULL) {
				result.ivalue = result.ivalue*sgmt_eval(cell->cdr).ivalue;
				cell = cell->cdr;
			}
			break;
		case DIVID:
			result.ivalue = sgmt_eval(cell->cdr).ivalue;
			cell = cell->cdr;
			int div;
			while(cell->cdr != NULL) {
				div=sgmt_eval(cell->cdr).ivalue;
				if(div==0) {
					printf("division by zero !\n");
					result.ivalue = 0;
					result.car = NULL;
					result.cdr = NULL;
					break;
				}
				result.ivalue /= div;
				cell = cell->cdr;
			}
			break;
		case GT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue <= sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case LT:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue >= sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case GEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue < sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case LEQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue > sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case EQ:
			cell=cell->cdr;
			while (cell->cdr != NULL) {
				if(sgmt_eval(cell).ivalue != sgmt_eval(cell->cdr).ivalue) {
					result.type = NIL;
					break;
				}
				cell = cell->cdr;
			}
			if(result.type != NIL)
				result.type = T;
			break;
		case IF:
			cell=cell->cdr;
			if(sgmt_eval(cell).type == T){
				result=sgmt_eval(cell->cdr);
			} else if(sgmt_eval(cell).type == NIL) {
				result=sgmt_eval(cell->cdr->cdr);
			}
			break;
		case SETQ:{
				  int key, data;
				  key = getkey(cell->cdr->symbol);
				  data = sgmt_eval(cell->cdr->cdr).ivalue;
				  result.ivalue=data;
				  if(add(key, data) == 1) { 
					  printf("The Symbol's key have already been used\n");
				  }
					 // else{
					  
					  break;
			  }
		case SYMBOL:
			  if(search(getkey(cell->symbol))!=NULL){
				  result.ivalue=*search(getkey(cell->symbol));
					}
					else{
					result.ivalue=sgmt_eval(cell->cdr).ivalue;
					}
				break;
		case FUNC:
			break;
		case END:
			break;
	}
	return result;
}

/*-----Get Key--------------*/
int getkey(char *name){
	int key = 0;
	while(*name != '\0')
		key += *name++;
	return key;
}
/*-----Hash Function--------*/
int hash(int key)
{
	return key % T_SIZE;
}

/*-----Add Symbol Data------*/
int add(int key, int data)
{
	struct slist *p;
	int bucket;
	if( search( key ) != NULL )
	return 1;
	p =malloc(sizeof(struct slist));
	bucket = hash( key );
	p->key = key;
	p->data = data;
	p->next =stable[bucket];
	stable[bucket] = p;
	return 0;
}
/*------Search Data---------*/
int* search(int key)
{
	struct slist *p;
	int bucket;
	bucket = hash( key );
	for( p=stable[bucket]; p!=NULL; p=p->next )
	{
if( key == p->key )
return &p->data;
 }
	return NULL;
}

/*-------Clear Hash Table----*/
void clear(void)
{
	struct slist *p;
	struct slist *q;
	int i;
	for( i=0; i<T_SIZE; ++i )
	{
	  p = stable[i];
  while( p != NULL ){
		  q = p;
		  p = p->next;
		  free( q );
	  }
	  stable[i] = NULL;
  }
}

