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

#define USE_array_index  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* Array */

static const knh_dim_t dimINIT = {
	0, 0, 0, 0, 0, 0, 0,
};

KNHAPI2(knh_Array_t*) new_Array(CTX ctx, knh_class_t p1, size_t capacity)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Array, p1);
	knh_Array_t *a = (knh_Array_t*)new_Object_init2(ctx, ClassTBL(cid));
	if(capacity > 0) {
		knh_Array_grow(ctx, a, capacity, 8);
	}
	return a;
}

void knh_Array_grow(CTX ctx, knh_Array_t *a, size_t newsize, size_t reqsize)
{
	size_t capacity = a->dim->capacity;
	if(newsize < reqsize) newsize = reqsize;
	if(newsize == 0) return;
	if(capacity == 0) {
		size_t wsize = (Array_isNDATA(a)) ? sizeof(knh_ndata_t) : sizeof(Object*);
		a->dim = new_dim(ctx, newsize, wsize);
		DBG_ASSERT(a->list == NULL);
	}
	else {
		((knh_dim_t*)a->dim)->capacity = newsize;
	}
	a->list = (knh_Object_t**)KNH_REALLOC(ctx, "Array", a->list, capacity, newsize, a->dim->wsize);
}

KNHAPI2(void) knh_Array_add_(CTX ctx, knh_Array_t *a, knh_Object_t *value)
{
	size_t capacity = a->dim->capacity;
	if(a->size == capacity) {
		knh_Array_grow(ctx, a, k_grow(capacity), a->size + 1);
	}
	KNH_INITv(a->list[a->size], value);
	a->size++;
}

KNHAPI2(void) knh_Array_swap(CTX ctx, knh_Array_t *a, size_t n, size_t m)
{
	DBG_ASSERT(n < a->size);
	DBG_ASSERT(m < a->size);
#ifdef K_USING_RCGC
	Object *temp = a->list[n];
	a->list[n] = a->list[m];
	a->list[m] = temp;
#else
	knh_sfp_t *esp1 = ctx->esp + 1;
	esp1[0].o = a->list[n];
	a->list[n] = a->list[m];
	a->list[m] = esp1[0].o;
#endif
}

void knh_Array_clear(CTX ctx, knh_Array_t *a, size_t n)
{
	if(!Array_isNDATA(a)) {
		size_t i;
		for(i = n; i < a->size; i++) {
			KNH_FINALv(ctx, a->list[i]);
		}
	}
	a->size = n;
}

/* ------------------------------------------------------------------------ */
/* [api] */

static void Farray_getO(CTX ctx, knh_sfp_t *sfp, size_t n2 _RIX)
{
	knh_Array_t *a = sfp[0].a;
	RETURN_(a->list[n2]);
}

static void Farray_getN(CTX ctx, knh_sfp_t *sfp, size_t n2 _RIX)
{
	knh_Array_t *a = sfp[0].a;
	RETURNd_(a->nlist[n2]);
}

static void Farray_setO(CTX ctx, knh_Array_t *a, size_t n2, knh_sfp_t *vsfp)
{
	KNH_SETv(ctx, a->list[n2], vsfp[0].o);
}

static void Farray_setN(CTX ctx, knh_Array_t *a, size_t n2, knh_sfp_t *vsfp)
{
	a->nlist[n2] = vsfp[0].ndata;
}

static void Farray_addO(CTX ctx, knh_Array_t *a, knh_sfp_t *v)
{
	size_t i, n = knh_stack_argc(ctx, v);
	size_t capacity = a->dim->capacity;
	if(!(a->size + n < capacity)) {
		knh_Array_grow(ctx, a, k_grow(a->size), a->size + n);
	}
	for(i = 0; i < n; i++) {
		KNH_INITv(a->list[a->size+i], v[i].o);
	}
	a->size += n;
}

static void Farray_addN(CTX ctx, knh_Array_t *a, knh_sfp_t *v)
{
	size_t i, n = knh_stack_argc(ctx, v);
	size_t capacity = a->dim->capacity;
	if(!(a->size + n < capacity)) {
		knh_Array_grow(ctx, a, k_grow(a->size), a->size + n);
	}
	for(i = 0; i < n; i++) {
		a->nlist[a->size+i] = v[i].ndata;
	}
	a->size += n;
}

static knh_ArrayAPI_t ArrayOAPI = {
	knh_array_index,
	Farray_getO,
	Farray_setO,
	Farray_addO,
};

knh_Array_t* new_Array0(CTX ctx, size_t capacity)
{
	knh_Array_t *a = new_H(Array);
	a->dim = &dimINIT;
	a->api = &ArrayOAPI;
	a->list = NULL;
	if(capacity > 0) {
		knh_Array_grow(ctx, a, capacity, 8);
	}
	a->size = 0;
	return a;
}

static knh_ArrayAPI_t ArrayNAPI = {
	knh_array_index,
	Farray_getN,
	Farray_setN,
	Farray_addN,
};

void knh_Array_initAPI(CTX ctx, knh_Array_t *a)
{
	knh_class_t p1 = O_p1(a);
	if(IS_Tunbox(p1)) {
		Array_setNDATA(a, 1);
		a->api = &ArrayNAPI;
	}
	else {
		a->api = &ArrayOAPI;
	}
}

/* ------------------------------------------------------------------------ */
/* Iterator */

static ITRNEXT Fitrnext_end(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	ITREND_();
}

/* ------------------------------------------------------------------------ */

void knh_Iterator_close(CTX ctx, knh_Iterator_t *it)
{
	DBG_ASSERT(IS_bIterator(it));
	if(it->fnext_1 != Fitrnext_end) {
		KNH_SETv(ctx, DP(it)->source, KNH_NULL);
		if(DP(it)->freffree != NULL) {
			if(DP(it)->nptr != NULL) {
				DP(it)->freffree(DP(it)->nptr);
			}
			DP(it)->freffree = NULL;
		}
		DP(it)->nptr = NULL;
		it->fnext_1   = Fitrnext_end;
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI2(knh_Iterator_t*) new_Iterator(CTX ctx, knh_class_t p1, knh_Object_t *source, knh_Fitrnext fnext)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Iterator, p1);
	knh_Iterator_t *it = new_O(Iterator, cid);
	if(IS_NULL(source)) fnext = Fitrnext_end;
	KNH_SETv(ctx, DP(it)->source, source);
	it->fnext_1 = fnext;
	return it;
}

/* ------------------------------------------------------------------------ */
/* [ArrayIterator] */

static ITRNEXT Array_nextO(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	DBG_ASSERT(IS_bIterator(sfp[0].it));
	knh_Iterator_t *itr = ITR(sfp);
	knh_Array_t *a = (knh_Array_t*)DP(itr)->source;
	size_t pos = (size_t)DP(itr)->mitr.index;
	if(pos < a->size) {
		DP(itr)->mitr.index = pos + 1;
		ITRNEXT_(a->list[pos]);
	}
	ITREND_();
}

static ITRNEXT Array_nextN(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	DBG_ASSERT(IS_bIterator(sfp[0].it));
	knh_Iterator_t *itr = ITR(sfp);
	knh_Array_t *a = (knh_Array_t*)DP(itr)->source;
	size_t pos = (size_t)DP(itr)->mitr.index;
	if(pos < a->size) {
		DP(itr)->mitr.index = pos+1;
		ITRNEXTd_(a->nlist[pos]);
	}
	ITREND_();
}

knh_Iterator_t* new_ArrayIterator(CTX ctx, knh_Array_t *a)
{
	knh_class_t cid = O_p1(a);
	knh_Fitrnext fnext = Array_isNDATA(a) ? Array_nextN : Array_nextO;
	return new_Iterator(ctx, cid, UPCAST(a), fnext);
}

knh_bool_t knh_isArrayIterator(knh_Iterator_t *itr)
{
	return (itr->fnext_1 == Array_nextO || itr->fnext_1 == Array_nextN);
}

knh_Array_t* knh_Iterator_toArray(CTX ctx, knh_Iterator_t *itr)
{
	knh_Array_t *a = new_Array(ctx, O_cTBL(itr)->p1, 0);
	BEGIN_LOCAL(ctx, lsfp, 2);
	KNH_SETv(ctx, lsfp[1].o, itr);
	while(itr->fnext_1(ctx, lsfp + 1, -1)) {
		a->api->add(ctx, a, lsfp);
	}
	END_LOCAL_(ctx, lsfp);
	return a;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#else /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */
//## method T1 Array.get(Int n);
//## method T1 ArrayIm.get(Int n);

static METHOD Array_get(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	size_t n2 = a->api->index(ctx, sfp, Int_to(size_t, ctx->esp[-1]), a->size);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1 Array.get2(Int x, Int y);
//## @Hidden method T1 ArrayIm.get2(Int x, Int y);

static METHOD Array_get2(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = a->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1 Array.get3(Int x, Int y, Int z);
//## @Hidden method T1 ArrayIm.get3(Int x, Int y, Int z);

static METHOD Array_get3(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = a->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x) + (sfp[3].ivalue * dim->xy);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1 Array.get4(Int x, Int y, Int z, Int w);
//## @Hidden method T1 ArrayIm.get4(Int x, Int y, Int z, Int w);

static METHOD Array_get4(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = a->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x) + (sfp[3].ivalue * dim->xy) + (sfp[4].ivalue * dim->xyz);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.set(Int n, T1 v);

static METHOD Array_set(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	knh_int_t n = sfp[1].ivalue;
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->set(ctx, sfp[0].a, n2, sfp+2);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.set2(Int x, Int y, T1 v);

static METHOD Array_set2(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = (sfp[0].a)->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->set(ctx, a, n2, sfp+3);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.set3(Int x, Int y, Int z, T1 v);

static METHOD Array_set3(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = (sfp[0].a)->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x) + (sfp[3].ivalue * dim->xy);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->set(ctx, a, n2, sfp+4);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.set4(Int x, Int y, Int z, Int, w, T1 v);

static METHOD Array_set4(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	const knh_dim_t *dim = (sfp[0].a)->dim;
	knh_int_t n = sfp[1].ivalue + (sfp[2].ivalue * dim->x) + (sfp[3].ivalue * dim->xy) + (sfp[4].ivalue * dim->xyz);
	size_t n2 = a->api->index(ctx, sfp, n, a->size);
	a->api->set(ctx, a, n2, sfp+5);
	a->api->get(ctx, sfp, n2, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.setAll(T1 v);

static METHOD Array_setAll(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t i;
	for(i = 0; i < (sfp[0].a)->size; i++) {
		(sfp[0].a)->api->set(ctx, sfp[0].a, i, sfp+1);
	}
	if((sfp[0].a)->size > 0) {
		(sfp[0].a)->api->get(ctx, sfp, 0, rix);
	}
	else {
		sfp[rix].ndata = sfp[1].ndata;
		RETURN_(sfp[1].o);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Array.add(T1 value, ...);
//## method void Array.send(T1 value, ...);

static METHOD Array_add(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	a->api->add(ctx, a, sfp+1);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Array.insert(Int n, T1 value);

static METHOD Array_insert(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	size_t size, n = a->api->index(ctx, sfp, Int_to(size_t, sfp[1]), a->size);
	const knh_dim_t *dim = a->dim;
	BEGIN_LOCAL(ctx, lsfp, 1);
	if(a->size == dim->capacity) {
		knh_Array_grow(ctx, a, k_grow(dim->capacity), a->size + 1);
	}
	if(Array_isNDATA(a)) {
		size = sizeof(knh_ndata_t);
		lsfp[0].ndata = a->nlist[a->size];
	}else {
		size = sizeof(knh_Object_t*);
		KNH_SETv(ctx, lsfp[0].o, a->list[a->size]);
	}
	a->size++;
	knh_memmove(a->list+(n+1), a->list+n, size * (a->size - n));
	a->api->set(ctx, a, n, sfp+2);
	END_LOCAL_(ctx, lsfp);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Array.clear();

static METHOD Array_clear(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_clear(ctx, sfp[0].a, 0);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */

static void knh_Array_remove_(CTX ctx, knh_Array_t *a, size_t n)
{
	DBG_ASSERT(n < a->size);
	size_t size;
	if (Array_isNDATA(a)) {
		size = sizeof(knh_ndata_t);
	} else {
		size = sizeof(knh_Object_t*);
		KNH_FINALv(ctx, a->list[n]);
	}
	knh_memmove(a->list+n, a->list+(n+1), size * (a->size - n - 1));
	a->size--;
}

/* ------------------------------------------------------------------------ */
//## method void Array.remove(Int n);

static METHOD Array_remove(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t n = knh_array_index(ctx, sfp, sfp[1].ivalue, o->size);
	knh_Array_remove_(ctx, o, n);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method T1! Array.pop();

static METHOD Array_pop(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	if (a->size > 0) {
		(a)->api->get(ctx, sfp, a->size - 1, rix);
		knh_Array_clear(ctx, a, a->size - 1);
	}
}
/* ------------------------------------------------------------------------ */
//## method Int Array.indexOf(T1 value);

static METHOD Array_indexOf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	knh_int_t res = -1;
	size_t i;
	if(Array_isNDATA(a)) {
		knh_ndata_t ndata = sfp[1].ndata;
		for(i = 0; i < a->size; i++) {
			if(a->nlist[i] == ndata) {
				res = i; break;
			}
		}
	}else {
		knh_Object_t *o = sfp[1].o;
		for(i = 0; i < a->size; i++) {
			if(knh_Object_compareTo(a->list[i], o) == 0) {
				res = i; break;
			}
		}
	}
	RETURNi_(res);
}
/* ------------------------------------------------------------------------ */
//## method Int Array.lastIndexOf(T1 value);

static METHOD Array_lastIndexOf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	long i;
	if(Array_isNDATA(a)) {
		knh_ndata_t ndata = sfp[1].ndata;
		for(i = a->size - 1; i >= 0; i--) {
			if(a->nlist[i] == ndata) {
				break;
			}
		}
	}else {
		knh_Object_t *o = sfp[1].o;
		for(i = a->size - 1; i >= 0; i--) {
			if(knh_Object_compareTo(a->list[i], o) == 0) {
				break;
			}
		}
	}
	RETURNi_(i);
}

///* ------------------------------------------------------------------------ */
///* [Collections] */
//
//typedef struct {
//	CTX ctx;
//	knh_sfp_t *sfp;
//} knh_env_t;
//
//static int knh_env_comp(knh_env_t *env, Object **a1, Object **a2)
//{
//	CTX ctx = env->ctx;
//	knh_sfp_t *lsfp = env->sfp + 2;
//	knh_putsfp(ctx, lsfp, 2, a1[0]);
//	knh_putsfp(ctx, lsfp, 3, a2[0]);
//	knh_Func_invokesfp(ctx, env->sfp[1].cc, lsfp, 1/*rtnidx*/, 2);
//	return (int)lsfp[0].ivalue;
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.sort(Cmpr? cc);
//
//static METHOD Array_sort(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//
//	knh_Array_t *o = sfp[0].a;
//	if(IS_NULL(sfp[1].o)) {
//		knh_qsort_r(o->list, o->size, sizeof(Object*),
//			(void*)ctx, (int (*)(void*, const void*, const void*))knh_Object_compareTo2);
//	}
//	else {
//		knh_env_t env = {ctx, sfp};
//		knh_qsort_r(o->list, o->size, sizeof(Object*), (void*)&env,
//				(int (*)(void *, const void* , const void*))knh_env_comp);
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
//
//static int qsort_icmp(const void* ap, const void* bp)
//{
//	knh_int_t a = *((knh_int_t*)ap);
//	knh_int_t b = *((knh_int_t*)bp);
//	if(a < b) return -1;
//	if(a > b) return 1;
//	return 0;
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.sort();
//
//static METHOD IArray_sort(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//
//	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
//	knh_qsort(o->ilist, o->size, sizeof(knh_int_t), qsort_icmp);
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
//
//static int qsort_fcmp(const void* ap, const void* bp)
//{
//	knh_float_t a = *((knh_float_t*)ap);
//	knh_float_t b = *((knh_float_t*)bp);
//	if(a < b) return -1;
//	if(a > b) return 1;
//	return 0;
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.sort();
//
//static METHOD FArray_sort(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//
//	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
//	knh_qsort(o->flist, o->size, sizeof(knh_float_t), qsort_fcmp);
//	RETURNvoid_();
//}
//
/* ------------------------------------------------------------------------ */

static inline void NArray_swap(CTX ctx, knh_Array_t *a, size_t n, size_t m)
{
	knh_ndata_t temp = a->nlist[n];
	a->nlist[n] = a->nlist[m];
	a->nlist[m] = temp;
}

static inline void OArray_swap(CTX ctx, knh_Array_t *a, size_t n, size_t m)
{
	knh_sfp_t *esp1 = ctx->esp + 1;
	esp1[0].o = a->list[n];
	a->list[n] = a->list[m];
	a->list[m] = esp1[0].o;
}

/* ------------------------------------------------------------------------ */
//## method void Array.swap(Int m, Int n);

static METHOD Array_swap(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	size_t m = knh_array_index(ctx, sfp, (sfp[1].ivalue), a->size);
	size_t n = knh_array_index(ctx, sfp, (sfp[2].ivalue), a->size);
	if(Array_isNDATA(a)) {
		NArray_swap(ctx, a, n, m);
	}
	else {
		OArray_swap(ctx, a, n, m);
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Array.shuffle();
static METHOD Array_shuffle(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t i;
	knh_Array_t *a = sfp[0].a;
	if(Array_isNDATA(a)) {
		for(i = 0; i < a->size * 2; i++) {
			size_t m = knh_rand() % a->size;
			size_t n = knh_rand() % a->size;
			NArray_swap(ctx, a, m, n);
		}
	}
	else {
		for(i = 0; i < a->size * 2; i++) {
			size_t m = knh_rand() % a->size;
			size_t n = knh_rand() % a->size;
			OArray_swap(ctx, a, m, n);
		}
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Array.reverse();

static METHOD Array_reverse(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t i;
	knh_Array_t *a = sfp[0].a;
	if(Array_isNDATA(a)) {
		for(i = 0; i < a->size / 2; i++) {
			size_t n = a->size - i - 1;
			NArray_swap(ctx, a, i, n);
		}
	}
	else {
		for(i = 0; i < a->size / 2; i++) {
			size_t n = a->size - i - 1;
			OArray_swap(ctx, a, i, n);
		}
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## mapper Iterator Iterator;

static TYPEMAP Iterator_Iterator(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KNH_TODO(__FUNCTION__);

}

/* ------------------------------------------------------------------------ */
//## mapper Array Array;

static TYPEMAP Array_Array(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KNH_TODO(__FUNCTION__);
//	knh_TypeMap_t *tmr = sfp[K_TMRIDX].tmrNC;
//	knh_Array_t *ta = sfp[0].a;
//	knh_Array_t *sa = new_ArrayCTBL(ctx, ClassTBL(tmr->tcid), knh_Array_size(ta));
//	size_t i;
//	if(IS_TypeMap(tmr->tmr1)) {
//		tmr = tmr->tmr1;
//		for(i = 0; i < knh_Array_size(ta); i++) {
//			ta->api->get(ctx, sfp, i, +1);
//			knh_TypeMap_exec(ctx, tmr, sfp+1, 0); // results are on sfp[1]
//			sa->api->add(ctx, sa, sfp+1);
//		}
//	}
//	RETURN_(sa);
}

/* ------------------------------------------------------------------------ */
//## mapper Iterator Array;

static TYPEMAP Iterator_Array(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(knh_Iterator_toArray(ctx, sfp[0].it));
}

/* ------------------------------------------------------------------------ */
/* [Array] */
//## mapper Array Iterator;
//## method T1.. Array.opITR();

static TYPEMAP Array_Iterator(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_ArrayIterator(ctx, sfp[0].a));
}

/* ------------------------------------------------------------------------ */

#endif/*K_INCLUDE_BUILTINAPI*/
