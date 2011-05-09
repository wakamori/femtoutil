#define MPOOL_ALIGN(size)\
	((size + (MPOOL_ALIGN_SIZE - 1)) & ~(MPOOL_ALIGN_SIZE - 1))

#define MPOOL_CREATE_SIZE(size)\
	(size <= 0 ? MPOOL_SIZE : MPOOL_ALIGN(size))

#define MPOOL_MALLOC(p, siz)\
	do {\
		if (((p) = malloc(siz)) == NULL) {\
			ml_error("malloc failed");\
			exit(-1);\
		}\
	} while(0)

#define MPOOL_FREE(p)\
	do {\
		if (p != NULL) {\
			free(p);\
			(p) = NULL;\
		}\
	} while(0)

/**
 * list of memory pool
 */
typedef struct mlist_t {
	void *pool;
	struct mlist_t *next;
} mlist_t;

/**
 * memory pool
 */
typedef struct mpool_t {
	mlist_t *head;
	void *begin;
	size_t usize;
	size_t msize;
	mlist_t *mlist;
} mpool_t;

mpool_t *mp_create(size_t size);
void *mp_alloc(mpool_t *pool, size_t size);
void mp_clear(mpool_t *pool);
void mp_extend(mlist_t *p, size_t size, mpool_t *pool);
