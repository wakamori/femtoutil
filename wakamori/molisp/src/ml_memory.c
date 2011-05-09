#include "molisp.h"

/**
 * create memory pool
 */
mpool_t *mp_create(size_t size)
{
	mpool_t *pool;
	size = MPOOL_CREATE_SIZE(size);
	MPOOL_MALLOC(pool, sizeof(mpool_t));
	MPOOL_MALLOC(pool->mlist, sizeof(mlist_t));
	MPOOL_MALLOC(pool->mlist->pool, size);
	memset(pool->mlist->pool, 0, size);
	if (!pool->mlist || !pool->mlist->pool)
		return NULL;
	pool->mlist->next = NULL;
	pool->begin = pool->mlist->pool;
	pool->head = pool->mlist;
	pool->usize = 0;
	pool->msize = size;
	return pool;
}

/**
 * allocate memory from memory pool
 */
void *mp_alloc(mpool_t *pool, size_t size)
{
	mlist_t **p = &pool->mlist;
	mlist_t *pp = *p;
	size_t usize = MPOOL_ALIGN(pool->usize + size);
	size_t msize = pool->msize;
	void *d = pool->begin;
	if (usize > msize) {
		mp_extend(pp, usize * 2 + 1, pool);
		pool->usize = 0;
		pool->msize = usize * 2;
		d = pool->begin;
		pool->begin += MPOOL_ALIGN(size);
		*p = pp->next;
	} else {
		pool->usize = usize;
		pool->begin += MPOOL_ALIGN(size);
	}
	return d;
}

/**
 * release memory pool
 */
void mp_clear(mpool_t *pool)
{
	mlist_t *p = pool->head;
	while (p != NULL) {
		mlist_t *current = p;
		mlist_t *next = p->next;
		MPOOL_FREE(current->pool);
		MPOOL_FREE(current);
		p = next;
	}
	MPOOL_FREE(pool);
}

/**
 * extend memory pool
 */
void mp_extend(mlist_t *p, size_t size, mpool_t *pool)
{
	size = MPOOL_CREATE_SIZE(size);
	mlist_t *pp;
	MPOOL_MALLOC(pp, sizeof(mlist_t));
	MPOOL_MALLOC(pp->pool, size);
	memset(pp->pool, 0, size);
	pp->next = NULL;
	p->next = pp;
	pool->begin = pp->pool;
}
