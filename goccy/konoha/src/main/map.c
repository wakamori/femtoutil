/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us.
 *
 * (1) GNU General Public License 3.0 (with K_UNDER_GPL)
 * (2) Konoha Non-Disclosure License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#ifndef K_INCLUDE_BUILTINAPI

#define USE_bytes_strcmp      1
//#define USE_bytes_strcasecmp  1
#define USE_bytes_equals      1
#define USE_hash              1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [HashMap] */

#define knh_map_hmap(m)      ((knh_hmap_t*)m)
#define knh_map_hentry(m)    (((knh_hmap_t*)m)->hentry)

static knh_map_t *hmap_init(CTX ctx, size_t init, const char *path, void *option)
{
	knh_hmap_t *hmap = (knh_hmap_t*)KNH_MALLOC(ctx, sizeof(knh_hmap_t));
	knh_bzero(hmap, sizeof(knh_hmap_t));
	if(init < K_HASH_INITSIZE) init = K_HASH_INITSIZE;
	hmap->hentry = (knh_hentry_t**)KNH_REALLOC(ctx, hmap->DBGNAME, NULL, 0, init, sizeof(knh_hentry_t*));
	hmap->hmax = init;
	hmap->size = 0;
	hmap->factor = KNH_HASH_FACTOR(hmap->hmax);
	return (knh_map_t*)hmap;
}

static knh_hmap_t *hmap_rehash(CTX ctx, knh_hmap_t *hmap)
{
	size_t i, newhmax = hmap->hmax * 2 + 1;
	knh_hentry_t **hentry = knh_map_hentry(hmap);
	knh_hentry_t **newhentry = (knh_hentry_t**)KNH_REALLOC(ctx, hmap->DBGNAME, NULL, 0, newhmax, sizeof(knh_hentry_t*));
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			knh_hentry_t *p = e;
			knh_hashcode_t ni = p->hcode % newhmax;
			e = e->next;
			p->next = newhentry[ni];
			newhentry[ni] = p;
		}
	}
	KNH_FREE(ctx, hentry, hmap->hmax * sizeof(knh_hentry_t*));
	hmap->hentry = newhentry;
	hmap->hmax = newhmax;
	hmap->factor = KNH_HASH_FACTOR(newhmax);
	return hmap;
}

static void hmap_reftrace(CTX ctx, knh_map_t *m FTRARG)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	size_t i;
	KNH_ENSUREREF(ctx, hmap->size * 2);
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hmap->hentry[i];
		while(e != NULL) {
			KNH_ADDREF(ctx, e->key);
			KNH_ADDREF(ctx, e->value);
			e = e->next;
		}
	}
	KNH_SIZEREF(ctx);
}

static void hmap_free(CTX ctx, knh_map_t *m)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	size_t i;
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hmap->hentry[i];
		while(e != NULL) {
			knh_hentry_t *p = e;
			e = e->next;
			KNH_FREE(ctx, p, sizeof(knh_hentry_t));
		}
	}
	KNH_FREE(ctx, hmap->hentry, sizeof(knh_hentry_t*)*(hmap->hmax));
	KNH_FREE(ctx, hmap, sizeof(knh_hmap_t));
}

static knh_bool_t hmap_get(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_bytes_t k = S_tobytes(ksfp[0].s);
	knh_hashcode_t hcode = knh_hash(0, k.text, k.len);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		knh_bytes_t k2 = S_tobytes(e->skey);
		if(e->hcode == hcode && knh_bytes_equals(k, k2)) {
			KNH_SETv(ctx, rsfp[0].o, e->value);
			return 1;
		}
		e = e->next;
	}
	return 0;
}

static void hmap_set(CTX ctx, knh_map_t* m, knh_sfp_t *kvsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_bytes_t k = S_tobytes(kvsfp[0].s);
	knh_hashcode_t hcode = knh_hash(0, k.text, k.len);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		knh_bytes_t k2 = S_tobytes(e->skey);
		if(e->hcode == hcode && knh_bytes_equals(k, k2)) {
			KNH_SETv(ctx, e->value, kvsfp[1].o);
			return;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
		idx = hcode % hmap->hmax;
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	KNH_INITv(e->key, kvsfp[0].o);
	KNH_INITv(e->value, kvsfp[1].o);
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static void hmap_remove(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	KNH_TODO("HashMap.remove");
//	 static void hmap_remove(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp)
//	 {
//	-       KNH_TODO("HashMap.remove");
//	+       knh_hmap_t *hmap = knh_map_hmap(m);
//	+       knh_hentry_t **hentry = knh_map_hentry(m);
//	+       knh_bytes_t k = S_tobytes(ksfp[0].s);
//	+       knh_hashcode_t hcode = knh_hash(0, k.text, k.len);
//	+       size_t idx = hcode % hmap->hmax;
//	+       knh_hentry_t *e = hentry[idx];
//	+       while(e != NULL) {
//	+               knh_bytes_t k2 = S_tobytes(e->skey);
//	+               if(e->hcode == hcode && knh_bytes_equals(k, k2)) {
//	+                       KNH_SETv(ctx, e->value, KNH_NULVAL(O_cid(e->value)));
//	+                       hmap->size -= 1;
//	+                       return;
//	+               }
//	+               e = e->next;
//	+       }
//	 }
}

static size_t hmap_size(CTX ctx, knh_map_t* m)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	return hmap->size;
}

static knh_bool_t hmap_next(CTX ctx, knh_map_t *m, knh_mapitr_t *mitr, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t *e = (knh_hentry_t*)mitr->ptr;
	if(e == NULL) {
		while(mitr->index < hmap->hmax) {
			e = hmap->hentry[mitr->index];
			mitr->index += 1;
			if(e != NULL) goto L_ENTRY;
		}
		return 0;
	}
	L_ENTRY:;
	KNH_SETv(ctx, rsfp[0].o, e->key);
	KNH_SETv(ctx, rsfp[1].o, e->value);
	mitr->ptr = e->next;
	return 1;
}

static const knh_MapDSPI_t* hmap_config(CTX ctx, knh_class_t p1, knh_class_t p2);

static const knh_MapDSPI_t HMAP_StringObject = {
	K_DSPI_MAP, "hash",
	hmap_config, hmap_init, hmap_reftrace, hmap_free,
	hmap_get, hmap_set, hmap_remove, hmap_size,
	hmap_next,
};

/* ---- */

static void hmap_reftracedata(CTX ctx, knh_map_t *m FTRARG)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	size_t i;
	KNH_ENSUREREF(ctx, hmap->size);
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hmap->hentry[i];
		while(e != NULL) {
			KNH_ADDREF(ctx, e->key);
			e = e->next;
		}
	}
	KNH_SIZEREF(ctx);
}

static knh_bool_t hmap_getdata(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_bytes_t k = S_tobytes(ksfp[0].s);
	knh_hashcode_t hcode = knh_hash(0, k.text, k.len);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		knh_bytes_t k2 = S_tobytes(e->skey);
		if(e->hcode == hcode && knh_bytes_equals(k, k2)) {
			rsfp[0].ndata = e->nvalue;
			return 1;
		}
		e = e->next;
	}
	rsfp[0].ndata = 0;
	return 0;
}

static void hmap_setdata(CTX ctx, knh_map_t* m, knh_sfp_t *kvsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_bytes_t k = S_tobytes(kvsfp[0].s);
	knh_hashcode_t hcode = knh_hash(0, k.text, k.len);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		knh_bytes_t k2 = S_tobytes(e->skey);
		if(e->hcode == hcode && knh_bytes_equals(k, k2)) {
			e->nvalue = kvsfp[1].ndata;
			return ;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	KNH_INITv(e->key, kvsfp[0].o);
	e->nvalue = kvsfp[1].ndata;
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static knh_bool_t hmap_nextdata(CTX ctx, knh_map_t *m, knh_mapitr_t* mitr, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t *e = (knh_hentry_t*)mitr->ptr;
	if(e == NULL) {
		while(mitr->index < hmap->hmax) {
			e = hmap->hentry[mitr->index];
			mitr->index += 1;
			if(e != NULL) goto L_ENTRY;
		}
		return 0;
	}
	L_ENTRY:;
	KNH_SETv(ctx, rsfp[0].o, e->key);
	rsfp[1].ndata = e->nvalue;
	mitr->ptr = e->next;
	return 1;
}

static const knh_MapDSPI_t HMAP_StringNDATA = {
	K_DSPI_MAP, "hash",
	hmap_config, hmap_init, hmap_reftracedata, hmap_free,
	hmap_getdata, hmap_setdata, hmap_remove, hmap_size, hmap_nextdata,
};

static const knh_MapDSPI_t* hmap_config(CTX ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tstr(p1)) {
		if(IS_Tunbox(p2)) {
			return &HMAP_StringNDATA;
		}
		return &HMAP_StringObject;
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */
/* dmap */

#define UNSORTED 8
#define knh_map_dmap(m)        ((knh_dmap_t*)m)
#define knh_map_dentry(m)      (((knh_dmap_t*)m)->dentry)

static knh_uint64_t knh_struint64(knh_bytes_t t)
{
	const unsigned char *p = (const unsigned char*)t.text;
	knh_uint64_t n = 0;
	if(t.len > 0) {
		switch(t.len) {
		default: n |= ((knh_uint64_t)p[7]);
		case 7: n |= (((knh_uint64_t)p[6]) << (64-56));
		case 6: n |= (((knh_uint64_t)p[5]) << (64-48));
		case 5: n |= (((knh_uint64_t)p[4]) << (64-40));
		case 4: n |= (((knh_uint64_t)p[3]) << (64-32));
		case 3: n |= (((knh_uint64_t)p[2]) << (64-24));
		case 2: n |= (((knh_uint64_t)p[1]) << (64-16));
		case 1: n |= (((knh_uint64_t)p[0]) << (64-8));
		}
	}
	return n;
}

static int dentry_strcmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	K_USE_FASTDMAP(if(e->ukey == e2->ukey))
		return knh_bytes_strcmp(S_tobytes(e->key), S_tobytes(e2->key));
	K_USE_FASTDMAP(return (e->ukey < e2->ukey) ? -1 : 1;)
}

static knh_map_t *dmap_init(CTX ctx, size_t init, const char *path, void *option)
{
	knh_dmap_t *dmap = (knh_dmap_t*)KNH_MALLOC(ctx, sizeof(knh_dmap_t));
	if(init < K_HASH_INITSIZE) init = 4;
	dmap->dentry = (knh_dentry_t*)KNH_REALLOC(ctx, NULL, NULL, 0, init, sizeof(knh_dentry_t));
	dmap->capacity = init;
	dmap->size = 0;
	dmap->sorted = 0;
	dmap->strcmpr = knh_bytes_strcmp;
	K_USE_FASTDMAP(dmap->strkeyuint = knh_struint64;)
	dmap->dentrycmpr = dentry_strcmp;
	return dmap;
}

static void dmap_reftrace(CTX ctx, knh_map_t *m FTRARG)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_dentry_t *dentry = knh_map_dentry(m);
	size_t i;
	KNH_ENSUREREF(ctx, dmap->size * 2);
	for(i = 0; i < dmap->size; i++) {
		KNH_ADDREF(ctx, dentry[i].key);
		KNH_ADDREF(ctx, dentry[i].value);
	}
	KNH_SIZEREF(ctx);
}

static void dmap_free(CTX ctx, knh_map_t *m)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	KNH_FREE(ctx, dmap->dentry, sizeof(knh_dentry_t)*dmap->capacity);
	KNH_FREE(ctx, dmap, sizeof(knh_dmap_t));
}

static size_t dmap_size(CTX ctx, knh_map_t* m)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	return dmap->size;
}

static knh_bool_t dmap_next(CTX ctx, knh_map_t *m, knh_mapitr_t* mitr, knh_sfp_t *rsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	if(mitr->index < dmap->size) {
		knh_dentry_t *dentry = knh_map_dentry(m);
		KNH_SETv(ctx, rsfp[0].o, dentry[mitr->index].key);
		KNH_SETv(ctx, rsfp[1].o, dentry[mitr->index].value);
		mitr->index += 1;
		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* String */

static knh_index_t dmap_index(knh_dmap_t *dmap, size_t sp, size_t ep, knh_bytes_t key)
{
	knh_dentry_t *a = dmap->dentry;
	knh_uint64_t ukey = dmap->strkeyuint(key);
	L_TAIL:;
	if(ep - sp < UNSORTED) {
		size_t i;
		for(i = sp; i < ep; i++) {
			if(a[i].ukey == ukey) {
				knh_bytes_t k = S_tobytes(a[i].key);
				if(key.len < 8) {
					//DBG_ASSERT(key.len == k.len);
					return i;
				}
				if(dmap->strcmpr(k, key) == 0) return i;
			}
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		if(a[cp].ukey < ukey) {
			sp = cp + 1;
		}
		else if(a[cp].ukey > ukey) {
			ep = cp;
		}
		else {
			knh_bytes_t k = S_tobytes(a[cp].key);
			int res = dmap->strcmpr(k, key);
			if(res == 0) {return cp; }
			else if(res > 0) { ep = cp; }
			else { sp = cp + 1; }
		}
		goto L_TAIL;
	}
}

static knh_bool_t dmap_get(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) return 0;
	}
	KNH_SETv(ctx, rsfp[0].o, dmap->dentry[loc].value);
	return 1;
}

#define dmap_grow(ctx, dmap) {\
		if(!(dmap->size < dmap->capacity)) {\
			size_t newsize = k_grow(dmap->capacity);\
			dmap->dentry = (knh_dentry_t*)KNH_REALLOC(ctx, dmap->DBGNAME, dmap->dentry, dmap->capacity, newsize, sizeof(knh_dentry_t));\
			dmap->capacity = newsize;\
		}\
	}\

#define dmap_sort(dmap) \
	if(!((dmap->size - dmap->sorted) < UNSORTED)) {\
		/*DBG_P("SORTED %s sorted=%d, size=%d", dmap->DBGNAME, dmap->sorted, dmap->size); */\
		knh_qsort(dmap->dentry, dmap->size, sizeof(knh_dentry_t), dmap->dentrycmpr);\
		dmap->sorted = dmap->size;\
	}\

void dmap_sort_(knh_dmap_t *dmap, int isforced)
{
	if(isforced || dmap->size != dmap->sorted) {
		//DBG_P("SORTED* %s, sorted=%d, size=%d", dmap->DBGNAME, dmap->sorted, dmap->size);
		knh_qsort(dmap->dentry, dmap->size, sizeof(knh_dentry_t), dmap->dentrycmpr);
		dmap->sorted = dmap->size;
	}
}

static void dmap_add(CTX ctx, knh_dmap_t *dmap, knh_sfp_t *kvsfp)
{
	size_t loc = dmap->size;
	dmap_grow(ctx, dmap);
	KNH_INITv(dmap->dentry[loc].key, kvsfp[0].s);
	KNH_INITv(dmap->dentry[loc].value, kvsfp[1].o);
	K_USE_FASTDMAP(dmap->dentry[loc].ukey = dmap->strkeyuint(S_tobytes(kvsfp[0].s)));
	dmap->size++;
}

static void dmap_set(CTX ctx, knh_map_t* m, knh_sfp_t* kvsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(kvsfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) {
			dmap_add(ctx, dmap, kvsfp);
			dmap_sort(dmap);
			return;
		}
	}
	KNH_SETv(ctx, dmap->dentry[loc].value, kvsfp[1].o);
}

static void dmap_remove(CTX ctx, knh_map_t* m, knh_sfp_t *kvsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(kvsfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) {
			return;
		}
	}
	KNH_FINALv(ctx, dmap->dentry[loc].key);
	KNH_FINALv(ctx, dmap->dentry[loc].value);
	if((size_t)loc < dmap->sorted) {
		dmap->sorted -= 1;
	}
	memmove(dmap->dentry + loc, dmap->dentry + loc + 1, (dmap->size - loc - 1)*sizeof(knh_dentry_t));
	dmap->size--;
}

static const knh_MapDSPI_t* dmap_config(CTX ctx, knh_class_t p1, knh_class_t p2);

static const knh_MapDSPI_t DMAP_StringObject = {
	K_DSPI_MAP, "dict",
	dmap_config, dmap_init, dmap_reftrace, dmap_free,
	dmap_get, dmap_set, dmap_remove, dmap_size, dmap_next,
};

static void dmap_reftracedata(CTX ctx, knh_map_t *m FTRARG)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_dentry_t *dentry = knh_map_dentry(m);
	size_t i;
	KNH_ENSUREREF(ctx, dmap->size);
	for(i = 0; i < dmap->size; i++) {
		KNH_ADDREF(ctx, dentry[i].key);
	}
	KNH_SIZEREF(ctx);
}

static knh_bool_t dmap_nextdata(CTX ctx, knh_map_t *m, knh_mapitr_t* mitr, knh_sfp_t *rsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	if(mitr->index < dmap->size) {
		knh_dentry_t *dentry = knh_map_dentry(m);
		KNH_SETv(ctx, rsfp[0].o, dentry[mitr->index].key);
		rsfp[1].ndata = dentry[mitr->index].nvalue; /* thanks, ide */
		mitr->index += 1;
		return 1;
	}
	return 0;
}

static knh_bool_t dmap_getdata(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) return 0;
	}
	rsfp[0].ndata = dmap->dentry[loc].nvalue;
	return 1;
}

static void dmap_adddata(CTX ctx, knh_dmap_t *dmap, knh_sfp_t *kvsfp)
{
	size_t loc = dmap->size;
	dmap_grow(ctx, dmap);
	KNH_INITv(dmap->dentry[loc].key, kvsfp[0].s);
	dmap->dentry[loc].nvalue = kvsfp[1].ndata;
	K_USE_FASTDMAP(dmap->dentry[loc].ukey = dmap->strkeyuint(S_tobytes(kvsfp[0].s)));
	dmap->size++;
}

static void dmap_setdata(CTX ctx, knh_map_t* m, knh_sfp_t* kvsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(kvsfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) {
			dmap_adddata(ctx, dmap, kvsfp);
			dmap_sort(dmap);
			return;
		}
	}
	dmap->dentry[loc].nvalue = kvsfp[1].ndata;
}

static void dmap_removedata(CTX ctx, knh_map_t* m, knh_sfp_t *kvsfp)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_bytes_t key = S_tobytes(kvsfp[0].s);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
		if(loc == -1) {
			return;
		}
	}
	KNH_FINALv(ctx, dmap->dentry[loc].key);
	if((size_t)loc < dmap->sorted) {
		dmap->sorted -= 1;
	}
	memmove(dmap->dentry + loc, dmap->dentry + loc + 1, (dmap->size - loc - 1)*sizeof(knh_dentry_t));
	dmap->size--;
}

static const knh_MapDSPI_t DMAP_StringNDATA = {
	K_DSPI_MAP, "dict",
	dmap_config, dmap_init, dmap_reftracedata, dmap_free,
	dmap_getdata, dmap_setdata, dmap_removedata, dmap_size, dmap_nextdata,
};

static const knh_MapDSPI_t* dmap_config(CTX ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tstr(p1)) {
		if(IS_Tunbox(p2)) {
			return &DMAP_StringNDATA;
		}
		return &DMAP_StringObject;
	}
	return NULL;
}

/* ------------------------------------------------------------------------- */
/* [casecmp] */

static knh_uint64_t knh_strcaseuint64(knh_bytes_t t)
{
	int ch[8] = {0};
	knh_uint64_t n = 0;
	size_t i, c = 0;
	for(i = 0; i < 8; i++) {
		while(t.text[c] == '_') c++;
		if(c >= t.len) break;
		ch[i] = toupper(t.text[c]); c++;
	}
	if(i > 0) {
		switch(i) {
		default: n |= ((knh_uint64_t)ch[7]);
		case 7: n |= (((knh_uint64_t)ch[6]) << (64-56));
		case 6: n |= (((knh_uint64_t)ch[5]) << (64-48));
		case 5: n |= (((knh_uint64_t)ch[4]) << (64-40));
		case 4: n |= (((knh_uint64_t)ch[3]) << (64-32));
		case 3: n |= (((knh_uint64_t)ch[2]) << (64-24));
		case 2: n |= (((knh_uint64_t)ch[1]) << (64-16));
		case 1: n |= (((knh_uint64_t)ch[0]) << (64-8));
		}
	}
	return n;
}

int knh_bytes_strcasecmp2(knh_bytes_t t1, knh_bytes_t t2)
{
	const char *p1 = t1.text, *e1 = t1.text + t1.len;
	const char *p2 = t2.text, *e2 = t2.text + t2.len;
	int ch1, ch2;
	while(1) {
		while(*p1 == '_') p1++;
		while(*p2 == '_') p2++;
		if(p1 == e1) return (p2 == e2) ? 0 : 1;
		if(p2 == e2) return -1;
		ch1 = toupper((unsigned char)*p1);
		ch2 = toupper((unsigned char)*p2);
		if(ch1 == ch2) {
			p1++; p2++; continue;
		}
		return (ch1 < ch2) ? -1 : 1;
	}
}

static int dentry_strcasecmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	K_USE_FASTDMAP(if(e->ukey == e2->ukey))
		return knh_bytes_strcasecmp2(S_tobytes(e->key), S_tobytes(e2->key));
	K_USE_FASTDMAP(return (e->ukey < e2->ukey) ? -1 : 1;)
}

static void dmap_case(knh_dmap_t *dmap)
{
	dmap->strkeyuint = knh_strcaseuint64;
	dmap->dentrycmpr = dentry_strcasecmp;
	dmap->strcmpr = knh_bytes_strcasecmp2;
}

/* ------------------------------------------------------------------------ */
/* DictMap */

knh_DictMap_t* new_DictMap0_(CTX ctx, size_t capacity, int isCaseMap, const char *DBGNAME)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &DMAP_StringObject;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	DBG_ASSERT(m->map != NULL);
	if(isCaseMap) dmap_case(m->dmap);
	m->dmap->DBGNAME = DBGNAME;
	return (knh_DictMap_t*)m;
}

knh_DictSet_t* new_DictSet0_(CTX ctx, size_t capacity, int isCaseMap, const char *DBGNAME)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &DMAP_StringNDATA;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	DBG_ASSERT(m->map != NULL);
	if(isCaseMap) dmap_case(m->dmap);
	m->dmap->DBGNAME = DBGNAME;
	return (knh_DictSet_t*)m;
}

KNHAPI2(knh_String_t*) knh_DictMap_keyAt(knh_DictMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dmap->dentry[n].key;
}

KNHAPI2(Object*) knh_DictMap_valueAt(knh_DictMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dmap->dentry[n].value;
}

knh_uintptr_t knh_DictSet_valueAt(knh_DictSet_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictSet_size(m));
	return (knh_uintptr_t)m->dmap->dentry[n].nvalue;
}

knh_index_t knh_DictMap_index(knh_DictMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
	}
	return loc;
}

Object *knh_DictMap_getNULL(CTX ctx, knh_DictMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? NULL : dmap->dentry[loc].value;
}

knh_uintptr_t knh_DictSet_get(CTX ctx, knh_DictSet_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_index_t loc = dmap_index(dmap, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = dmap_index(dmap, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? 0 : (knh_uintptr_t)dmap->dentry[loc].nvalue;
}

void DictMap_set_(CTX ctx, knh_DictMap_t *m, knh_String_t *key, dynamic *v)
{
	knh_sfp_t* kvsfp = ctx->esp;
	KNH_SETv(ctx, kvsfp[0].o, key);
	KNH_SETv(ctx, kvsfp[1].o, v);
	m->dspi->set(ctx, m->map, kvsfp);
}

void knh_DictSet_set(CTX ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* kvsfp = ctx->esp;
	KNH_SETv(ctx, kvsfp[0].o, key);
	kvsfp[1].ivalue = n;
	m->dspi->set(ctx, m->map, kvsfp);
}

void knh_DictMap_append(CTX ctx, knh_DictMap_t *m, knh_String_t *key, knh_Object_t *v)
{
	knh_sfp_t* kvsfp = ctx->esp;
	KNH_SETv(ctx, kvsfp[0].o, key);
	KNH_SETv(ctx, kvsfp[1].o, v);
	dmap_add(ctx, m->dmap, kvsfp);
}

void knh_DictSet_append(CTX ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* kvsfp = ctx->esp;
	KNH_SETv(ctx, kvsfp[0].o, key);
	kvsfp[1].ivalue = n;
	dmap_adddata(ctx, m->dmap, kvsfp);
}

void knh_DictSet_sort(CTX ctx, knh_DictSet_t *m)
{
	dmap_sort(m->dmap);
}

/* ------------------------------------------------------------------------ */

void knh_loadScriptDefaultMapDSPI(CTX ctx, knh_NameSpace_t *ns)
{
	knh_NameSpace_addDSPI(ctx, ns, "hash", (knh_DSPI_t*)&HMAP_StringObject);
	knh_hash(0, "", 0); // dummy
	knh_NameSpace_addDSPI(ctx, ns, "dict", (knh_DSPI_t*)&DMAP_StringObject);
}

/* ------------------------------------------------------------------------ */

const knh_MapDSPI_t *knh_NameSpace_getMapDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	if(path.len == 0) {
		return &DMAP_StringObject;
	}
	else {
		const knh_MapDSPI_t *p = (const knh_MapDSPI_t*)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_MAP, path);
		if(p == NULL) {
			//SYSLOG_UnknownPathType(ctx, path);
			p = &DMAP_StringObject;
		}
		return p;
	}
}

const knh_MapDSPI_t *knh_getDefaultMapDSPI(CTX ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tstr(p1)) {
		if(IS_Tunbox(p2)) {
			return &DMAP_StringNDATA;
		}
		return &DMAP_StringObject;
	}
	return NULL;
}

#ifdef __cplusplus
}
#endif

#else/*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */
/* [Map] */

/* ------------------------------------------------------------------------ */
//## method Boolean Map.opHAS(T1 key);

static METHOD Map_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	RETURNb_(m->dspi->get(ctx, m->dmap, sfp + 1, sfp + rix));
}

/* ------------------------------------------------------------------------ */
//## method T2 Map.get(T1 key);

static METHOD Map_get(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	if(!m->dspi->get(ctx, m->dmap, sfp + 1, sfp + rix)) {
		RETURNa_(KNH_NULVAL(knh_class_p2(O_cid(m))));
	}
}

/* ------------------------------------------------------------------------ */
//## method void Map.set(T1 key, T2 value);

static METHOD DictMap_set(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	m->dspi->set(ctx, m->dmap, sfp + 1);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Map.remove(T1 key);

static METHOD DictMap_remove(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	m->dspi->remove(ctx, m->dmap, sfp + 1);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method T1[] Map.keys();

static METHOD Map_keys(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	size_t size = m->dspi->size(ctx, m->map);
	knh_class_t p1 = O_cTBL(m)->p1;
	knh_Array_t *a = new_Array(ctx, p1, size);
	knh_sfp_t *lsfp = ctx->esp;
	knh_mapitr_t mitrbuf = K_MAPITR_INIT, *mitr = &mitrbuf;
	while(m->dspi->next(ctx, m->map, mitr, lsfp)) {
		a->api->add(ctx, a, lsfp);
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */

static ITRNEXT Fnext_mapkey(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *itr = ITR(sfp);
	knh_Map_t *m = (knh_Map_t*)DP(itr)->source;
	knh_sfp_t *lsfp = ctx->esp;
	if(m->dspi->next(ctx, m->map, &(DP(itr)->mitr), lsfp)) {
		ITRNEXT_(lsfp[0].o);
	}
	else {
		ITREND_();
	}
}

static ITRNEXT Fnext_mapkeydata(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *itr = ITR(sfp);
	knh_Map_t *m = (knh_Map_t*)DP(itr)->source;
	knh_sfp_t *lsfp = ctx->esp;
	if(m->dspi->next(ctx, m->map, &(DP(itr)->mitr), lsfp)) {
		ITRNEXTd_(lsfp[0].ndata);
	}
	else {
		ITREND_();
	}
}


/* ------------------------------------------------------------------------ */
//## mapper Map Iterator!;
//## mapper Map T1..!;
//## method T1.. Map.opITR();

static TYPEMAP Map_T1__(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	knh_class_t p1 = O_cTBL(m)->p1;
	knh_Iterator_t *itr = new_Iterator(ctx, p1, UPCAST(m), IS_Tunbox(p1) ? Fnext_mapkeydata : Fnext_mapkey);
	RETURN_(itr);
}

/* ------------------------------------------------------------------------ */

#endif/*K_INCLUDE_BUILTINAPI*/
