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

#define K_DEFINE_API2
#include"commons.h"

#ifdef K_USING_ICONV
#include<iconv.h>
#endif

#define K_USING_LOADDATA
#include"../../include/konoha1/konohalang.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [ContextTable] */

static knh_context_t* new_hcontext(CTX ctx0)
{
	knh_context_t *ctx;
	static volatile size_t ctxid_counter = 0;
	if(ctx0 == NULL) {
		ctx = (knh_context_t*)malloc(sizeof(knh_context_t));
		knh_bzero(ctx, sizeof(knh_context_t));
//		ctx0 = ctx;
	}
	else {
		KNH_ASSERT_CTX0(ctx0);
		ctx = (knh_context_t*)KNH_MALLOC(ctx0, sizeof(knh_context_t));
		knh_bzero(ctx, sizeof(knh_context_t));
	}
	ctx->flag = 0;
	ctx->ctxid = ctxid_counter;
	ctxid_counter++;
	ctx->freeObjectList = NULL;
	ctx->freeMemoryList = NULL;
	ctx->parent = (knh_context_t*)ctx;
	ctx->api = knh_getExportsAPI();
	ctx->api2 = getapi2();
	{
		knh_uintptr_t i = 0, ch;
		knh_uint_t t = knh_rand();
		ch = t % 26;
		ctx->trace[i] = 'A' + ch;
		for(i = 1; i < 9; i++) {
			t = t / 36;
			if (t == 0) t = knh_rand();
			ch = t % 36;
			ctx->trace[i] = (ch < 10) ? '0' + ch : 'A' + (ch - 10);
		}
	}
	ctx->seq = 0;
	return ctx;
}

/* ------------------------------------------------------------------------ */
/* [ContextCommon] */

static void knh_CommonContext_init(CTX ctx, knh_context_t *o)
{
	KNH_ASSERT_CTX0(ctx);
	DBG_ASSERT(o->sys != NULL);
	DBG_ASSERT(o->script != NULL);
	DBG_ASSERT(o->gma != NULL);
	KNH_INITv(o->enc, DP(ctx->sys)->enc);
	KNH_INITv(o->in,  DP(ctx->sys)->in);
	KNH_INITv(o->out, DP(ctx->sys)->out);
	KNH_INITv(o->err, DP(ctx->sys)->err);
	KNH_INITv(o->e, KNH_NULL);
	KNH_INITv(o->evaled, KNH_NULL);
	KNH_INITv(o->symbolDictMap, new_DictMap0(ctx, 256, 0/*isCaseMap*/, "Context.symbolDictMap"));
	KNH_INITv(o->constPools, new_Array0(ctx, 0));
	o->ctxlock = knh_mutex_malloc(ctx);
}

static knh_Object_t** knh_CommonContext_reftrace(CTX ctx, knh_context_t *ctxo FTRARG)
{
	size_t i;
	KNH_ADDREF(ctx, ctxo->symbolDictMap);
	KNH_ADDREF(ctx, ctxo->constPools);
	KNH_ADDREF(ctx, ctxo->e);
	KNH_ADDREF(ctx, ctxo->evaled);
	KNH_ADDREF(ctx, (ctxo->script));
	KNH_ADDREF(ctx, (ctxo->enc));
	KNH_ADDREF(ctx, (ctxo->in));
	KNH_ADDREF(ctx, (ctxo->out));
	KNH_ADDREF(ctx, (ctxo->err));
	KNH_ADDREF(ctx, ctxo->gma);
	// stack
	KNH_ADDREF(ctx, (ctxo->bufa));
	KNH_ADDREF(ctx, (ctxo->bufw));
#ifdef K_USING_RCGC
	KNH_ENSUREREF(ctx, ctxo->stacksize);
	for(i = 0; i < ctxo->stacksize; i++) {
		KNH_ADDREF(ctx, ctxo->stack[i].o);
	}
#else
	{
		size_t stacksize = (ctxo->esp - ctx->stack) + 8;
		KNH_ENSUREREF(ctx, stacksize);
		for(i = 0; i < stacksize; i++) {
			KNH_ADDREF(ctx, ctxo->stack[i].o);
		}
	}
#endif
#ifdef K_USING_CSTACK_TRAVERSE_
	return NULL;
#else
	return tail_;
#endif
}

static void knh_CommonContext_free(CTX ctx, knh_context_t *ctxo)
{
	KNH_FREE(ctx, ctxo->stack, sizeof(knh_sfp_t) * ctxo->stacksize);
	ctxo->stack = NULL;
	ctxo->esp = NULL;
	ctxo->stacktop = NULL;
	ctxo->stacksize = 0;
	KNH_FREE(ctx, ctxo->mtdcache,  K_MTDCACHE_SIZE * sizeof(knh_mtdcache_t));
	KNH_FREE(ctx, ctxo->tmrcache, K_TMAPCACHE_SIZE * sizeof(knh_tmrcache_t));
	ctxo->mtdcache  = NULL;
	ctxo->tmrcache = NULL;
	if(ctxo->queue_capacity > 0) {
		KNH_FREE(ctx, ctxo->queue,  ctxo->queue_capacity * sizeof(knh_Object_t*));
		ctxo->queue_capacity = 0;
	}
	if(ctx->ref_capacity > 0) {
		KNH_FREE(ctx, ctxo->ref_buf, ctxo->ref_capacity * sizeof(knh_Object_t*));
		ctxo->ref_capacity = 0;
	}
	knh_mutex_free(ctxo, ctxo->ctxlock);
	ctxo->ctxlock = NULL;
	ctxo->bufa = NULL;
}

/* ------------------------------------------------------------------------ */
/* [RootContext] */

static void knh_expandClassTBL(CTX ctx)
{
	size_t max = ctx->share->capacityClassTBL * 2;
	const knh_ClassTBL_t **newt = (const knh_ClassTBL_t**)
		KNH_REALLOC(ctx, "ClassTBL", ctx->share->ClassTBL, (ctx->share->capacityClassTBL), max, sizeof(knh_ClassTBL_t*));
	((knh_share_t*)ctx->share)->ClassTBL = newt;
	((knh_share_t*)ctx->share)->capacityClassTBL = max;
}

knh_class_t new_ClassId(CTX ctx)
{
	knh_class_t newid = ctx->share->sizeClassTBL;
	KNH_ASSERT_CTX0(ctx);
	if(ctx->share->sizeClassTBL == ctx->share->capacityClassTBL) {
		knh_expandClassTBL(ctx);
	}
	DBG_ASSERT(ctx->share->ClassTBL[newid] == NULL);
	{
		knh_ClassTBL_t *ct = (knh_ClassTBL_t*)KNH_MALLOC(ctx, sizeof(knh_ClassTBL_t));
		knh_bzero(ct, sizeof(knh_ClassTBL_t));
		ct->cid    = newid;
		ct->keyidx = -1;
		ct->metaidx = -1;
		ctx->share->ClassTBL[newid] = ct;
	}
	((knh_share_t*)ctx->share)->sizeClassTBL = newid + 1;
	return newid;
}

/* ------------------------------------------------------------------------ */

void knh_expandEventTBL(CTX ctx)
{
	size_t s = ctx->share->sizeEventTBL, max = ctx->share->capacityEventTBL * 2;
	knh_EventTBL_t *newt = (knh_EventTBL_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(max));
	knh_bzero(newt, SIZEOF_TEXPT(max));
	knh_memcpy(newt, ctx->share->EventTBL, SIZEOF_TEXPT(s));
	((knh_share_t*)ctx->share)->EventTBL = newt;
	((knh_share_t*)ctx->share)->capacityEventTBL = max;
}

static void initServiceSPI(knh_ServiceSPI_t *spi);
void knh_loadScriptAliasTokenData(CTX ctx);

static knh_context_t* new_RootContext(void)
{
	knh_context_t *ctx = (knh_context_t*)new_hcontext(NULL);
	const knh_PackageLoaderAPI_t *kapi = knh_getPackageLoaderAPI();
	knh_share_t *share = (knh_share_t*)malloc(sizeof(knh_share_t) + sizeof(knh_stat_t) + sizeof(knh_ServiceSPI_t));
	ctx->share = share;
	knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t) + sizeof(knh_ServiceSPI_t));
	ctx->stat = (knh_stat_t*)((share+1));
	ctx->spi = (const knh_ServiceSPI_t*)(ctx->stat + 1);
	initServiceSPI((knh_ServiceSPI_t*)ctx->spi);

	share->ObjectArenaTBL = (knh_ObjectArenaTBL_t*)KNH_MALLOC(ctx, K_ARENATBL_INITSIZE * sizeof(knh_ObjectArenaTBL_t));
	knh_bzero(share->ObjectArenaTBL, K_ARENATBL_INITSIZE * sizeof(knh_ObjectArenaTBL_t));
	share->sizeObjectArenaTBL = 0;
	share->capacityObjectArenaTBL = K_ARENATBL_INITSIZE;

	share->MemoryArenaTBL = (knh_MemoryArenaTBL_t*)KNH_MALLOC(ctx, K_ARENATBL_INITSIZE * sizeof(knh_MemoryArenaTBL_t));
	knh_bzero(share->MemoryArenaTBL, K_ARENATBL_INITSIZE * sizeof(knh_MemoryArenaTBL_t));
	share->sizeMemoryArenaTBL = 0;
	share->capacityMemoryArenaTBL = K_ARENATBL_INITSIZE;

#if defined(K_USING_FASTMALLOC2)
	share->MemoryX2ArenaTBL = (knh_MemoryX2ArenaTBL_t*)KNH_MALLOC(ctx, K_ARENATBL_INITSIZE * sizeof(knh_MemoryX2ArenaTBL_t));
	knh_bzero(share->MemoryX2ArenaTBL, K_ARENATBL_INITSIZE * sizeof(knh_MemoryX2ArenaTBL_t));
	share->MemoryX2ArenaTBLSize = 0;
	share->capacityMemoryX2ArenaTBL = K_ARENATBL_INITSIZE;
#endif
#if defined(K_USING_FASTMALLOC256)
	share->Memory256ArenaTBL = (knh_Memory256ArenaTBL_t*)KNH_MALLOC(ctx, K_ARENATBL_INITSIZE * sizeof(knh_Memory256ArenaTBL_t));
	knh_bzero(share->Memory256ArenaTBL, K_ARENATBL_INITSIZE * sizeof(knh_Memory256ArenaTBL_t));
	share->Memory256ArenaTBLSize = 0;
	share->capacityMemory256ArenaTBL = K_ARENATBL_INITSIZE;
#endif

	share->ClassTBL = (const knh_ClassTBL_t**)KNH_MALLOC((CTX)ctx, sizeof(knh_ClassTBL_t*)*(K_CLASSTABLE_INIT));
	knh_bzero(share->ClassTBL, sizeof(knh_ClassTBL_t*)*(K_CLASSTABLE_INIT));
	share->sizeClassTBL = 0;
	share->capacityClassTBL  = K_CLASSTABLE_INIT;

	share->EventTBL = (knh_EventTBL_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(K_EVENTTBL_INIT));
	knh_bzero((void*)share->EventTBL, SIZEOF_TEXPT(K_EVENTTBL_INIT));
	share->sizeEventTBL = 0;
	share->capacityEventTBL  = K_EVENTTBL_INIT;
	knh_loadScriptSystemStructData(ctx, kapi);

	{
		knh_Object_t *p = (knh_Object_t*)new_hObject_(ctx, ClassTBL(CLASS_Object));
		Object_setNullObject(p, 1);
		KNH_INITv(share->constNull, p);
	}
	{
		knh_Boolean_t *o = new_H(Boolean);
		o->n.bvalue = 1;
		KNH_INITv(share->constTrue, o);
	}
	{
		knh_Boolean_t *o = new_H(Boolean);
		o->n.bvalue = 0;
		KNH_INITv(share->constFalse, o);
	}
	{
		knh_Int_t *io = new_H(Int);
		(io)->n.ivalue = 0;
		KNH_INITv(share->constInt0, io);
	}
	{
		knh_Float_t *fo = new_H(Float);
		(fo)->n.fvalue = K_FLOAT_ZERO;
		KNH_INITv(share->constFloat0, fo);
	}
	{
		static const knh_dim_t dimINIT = {0};
		knh_Array_t *a = new_H(Array);
		(a)->size = 0;
		(a)->dim = &dimINIT;
		KNH_INITv(share->emptyArray, a);
	}
	share->tString = (knh_String_t**)KNH_MALLOC(ctx, SIZEOF_TSTRING);
	knh_bzero(share->tString, SIZEOF_TSTRING);
	knh_loadScriptSystemString(ctx);

	/* These are not shared, but needed to initialize System*/
	knh_stack_initexpand(ctx, NULL, K_STACKSIZE);
	KNH_INITv(ctx->sys, new_(System));
	KNH_INITv(share->rootns, new_(NameSpace));
	knh_loadScriptSystemData(ctx, share->rootns, kapi);
	knh_System_initPath(ctx, ctx->sys);       // require rootns
	KNH_INITv(ctx->script, new_(Script));     // require rootns
	KNH_INITv(ctx->gma, new_(Gamma));         // require script
	knh_loadScriptSystemKonohaCode(ctx);      // require gamma
	knh_loadScriptSystemMethod(ctx, kapi);
	knh_CommonContext_init(ctx, ctx);
	knh_loadScriptTokenData(ctx);
	knh_loadScriptAliasTokenData(ctx);
	share->ctx0 = ctx;
	return ctx;
}

static const char* LOG__(int p)
{
	switch(p) {
	case LOG_EMERG:   return "PANIC ";
	case LOG_ALERT:   return "ALERT ";
	case LOG_CRIT:    return "CRIT ";
	case LOG_ERR:     return "ERROR ";
	case LOG_WARNING: return "WARNING ";
	case LOG_NOTICE:  return "NOTICE ";
	case LOG_INFO:    return "INFO ";
	case LOG_DEBUG:   return "DEBUG ";
	}
	return "DEBUG2 ";
}

void pseudo_vsyslog(int p, const char *fmt, va_list ap)
{
	fprintf(stderr, "%s", LOG__(p));
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
}

void pseudo_syslog(int p, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fprintf(stderr, "%s", LOG__(p));
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

static int _lock(knh_mutex_t *m DBG_TRACE)
{
	TRACE_P("UNLOCK mutex=%p", m);
	return 0;
};


static int _unlock(knh_mutex_t *m DBG_TRACE)
{
	TRACE_P("UNLOCK mutex=%p", m);
	return 0;
};

#ifndef K_USING_ICONV
iconv_t iconv_open(const char *t, const char *f)
{
	return (iconv_t)(-1);
}
size_t iconv(iconv_t i, char **t, size_t *ts, char **f, size_t *fs)
{
	return 0;
}
int iconv_close(iconv_t i)
{
	return 0;
}
#endif

static void initServiceSPI(knh_ServiceSPI_t *spi)
{
	spi->syncspi = "nothread";
	spi->lock = _lock;
	spi->unlock = _unlock;
	spi->syslogspi = "fprintf(stderr)";
	spi->syslog = pseudo_syslog;
	spi->vsyslog = pseudo_vsyslog;
	spi->iconv_open = iconv_open;
	spi->iconv = (size_t (*)(iconv_t, char**, size_t*, char**, size_t*))iconv;
	spi->iconv_close = iconv_close;
#ifdef K_USING_ICONV
	spi->iconvspi = "iconv";
#else
	spi->iconvspi = "noiconv";
#endif
}

/* ------------------------------------------------------------------------ */

static knh_Object_t **knh_share_reftrace(CTX ctx, knh_share_t *share FTRARG)
{
	size_t i;
	KNH_ADDREF(ctx, share->constNull);
	KNH_ADDREF(ctx, share->constTrue);
	KNH_ADDREF(ctx, share->constFalse);
	KNH_ADDREF(ctx, (share->constInt0));
	KNH_ADDREF(ctx, (share->constFloat0));
	KNH_ADDREF(ctx, (share->emptyArray));
	KNH_ADDREF(ctx, (ctx->sys));
	KNH_ADDREF(ctx, (share->rootns));
	KNH_ENSUREREF(ctx, K_TSTRING_SIZE);
	for(i = 0; i < K_TSTRING_SIZE; i++) {
		KNH_ADDREF(ctx, (share->tString[i]));
	}
	KNH_ENSUREREF(ctx, share->sizeEventTBL);
	for(i = 0; i < share->sizeEventTBL; i++) {
		if(EventTBL(i).name != NULL) {
			KNH_ADDREF(ctx, (EventTBL(i).name));
		}
	}
	/* tclass */
	KNH_ENSUREREF(ctx, share->sizeClassTBL * 10);
	for(i = 0; i < share->sizeClassTBL; i++) {
		const knh_ClassTBL_t *t = ClassTBL(i);
		DBG_ASSERT(t->lname != NULL);
		KNH_ADDNNREF(ctx,  (t->typeNULL));
		KNH_ADDREF(ctx, (t->methods));
		KNH_ADDREF(ctx, t->typemaps);
		KNH_ADDNNREF(ctx,  t->cparam);
		KNH_ADDNNREF(ctx,  t->defnull);
		KNH_ADDNNREF(ctx,  t->constDictCaseMapNULL);
		KNH_ADDREF(ctx, t->sname);
		KNH_ADDREF(ctx, t->lname);
		if(t->bcid == CLASS_Object && t->cid > t->bcid) {
			KNH_ADDREF(ctx, t->protoNULL);
		}
	}
#ifdef K_USING_CSTACK_TRAVERSE_
	return NULL;
#else
	return tail_;
#endif
}

static void knh_share_free(CTX ctx, knh_share_t *share)
{
	size_t i;
	KNH_FREE(ctx, (void*)share->EventTBL, SIZEOF_TEXPT(ctx->share->capacityEventTBL));
	share->EventTBL = NULL;
	KNH_FREE(ctx, share->tString, SIZEOF_TSTRING);
	share->tString = NULL;
	//((knh_context_t*)ctx)->fsweep = knh_Object_finalSweep;

	DBG_ASSERT(share->ObjectArenaTBL != NULL);
	for(i = 0; i < share->sizeObjectArenaTBL; i++) {
		knh_ObjectArenaTBL_t *t = share->ObjectArenaTBL + i;
		knh_ObjectObjectArenaTBL_free(ctx, t);
	}
	for(i = 0; i < share->sizeClassTBL; i++) {
		knh_ClassTBL_t *t = varClassTBL(i);
		if(t->fcapacity > 0) {
			KNH_FREE(ctx, t->fields, sizeof(knh_fields_t) * t->fcapacity);
			t->fields = NULL;
		}
		KNH_FREE(ctx, t, sizeof(knh_ClassTBL_t));
	}
	KNH_FREE(ctx, (void*)share->ClassTBL, sizeof(knh_ClassTBL_t*)*(share->capacityClassTBL));
	share->ClassTBL = NULL;
	DBG_ASSERT(share->ObjectArenaTBL != NULL);
	for(i = 0; i < share->sizeObjectArenaTBL; i++) {
		knh_ObjectArenaTBL_t *oat = share->ObjectArenaTBL + i;
		DBG_ASSERT(K_MEMSIZE(oat->bottom, oat->head) == oat->arenasize);
		KNH_FREE(ctx, oat->bitmap, oat->bitmapsize * K_NBITMAP);
		KNH_VFREE(ctx, oat->head, oat->arenasize);
	}
	KNH_FREE(ctx, share->ObjectArenaTBL, share->capacityObjectArenaTBL * sizeof(knh_ObjectArenaTBL_t));
	share->ObjectArenaTBL = NULL;
	for(i = 0; i < share->sizeMemoryArenaTBL; i++) {
		knh_MemoryArenaTBL_t *at = share->MemoryArenaTBL + i;
		KNH_FREE(ctx, at->head, K_MEMSIZE(at->bottom, at->head));
	}
	KNH_FREE(ctx, share->MemoryArenaTBL, share->capacityMemoryArenaTBL * sizeof(knh_MemoryArenaTBL_t));
	share->MemoryArenaTBL = NULL;
#if defined(K_USING_FASTMALLOC2)
	for(i = 0; i < share->MemoryX2ArenaTBLSize; i++) {
		knh_MemoryX2ArenaTBL_t *at = share->MemoryX2ArenaTBL + i;
		KNH_FREE(ctx, at->head, K_MEMSIZE(at->bottom, at->head));
	}
	KNH_FREE(ctx, share->MemoryX2ArenaTBL, share->capacityMemoryX2ArenaTBL * sizeof(knh_MemoryX2ArenaTBL_t));
	share->MemoryX2ArenaTBL = NULL;
#endif
#if defined(K_USING_FASTMALLOC256)
	for(i = 0; i < share->Memory256ArenaTBLSize; i++) {
		knh_Memory256ArenaTBL_t *at = share->Memory256ArenaTBL + i;
		KNH_FREE(ctx, at->head, K_MEMSIZE(at->bottom, at->head));
	}
	KNH_FREE(ctx, share->Memory256ArenaTBL, share->capacityMemory256ArenaTBL * sizeof(knh_Memory256ArenaTBL_t));
	share->Memory256ArenaTBL = NULL;
#endif
	if(ctx->stat->gcCount > 0) {
		KNH_MEMINFO(ctx, "GC %d times, marking_time=%dms, sweeping_time=%dms total=%fs",
				(int)ctx->stat->gcCount, (int)ctx->stat->markingTime, (int)ctx->stat->sweepingTime,
				((double)ctx->stat->gcTime) / 1000.0);
	}
	if(ctx->stat->usedMemorySize != 0) {
		KNH_WARN(ctx, "memory leaking size=%ldbytes", (long)ctx->stat->usedMemorySize);
	}
	knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t) + sizeof(knh_ServiceSPI_t));
	free(share);
}

knh_Context_t* toContext(CTX ctx)
{
	if(ctx->ctxobjNC == NULL) {
		knh_Context_t *cx = new_H(Context);
		cx->ctx = (knh_context_t*)ctx;
		((knh_context_t*)ctx)->ctxobjNC = cx;
	}
	return ctx->ctxobjNC;
}

/* ------------------------------------------------------------------------ */

static knh_context_t* knh_getRootContext(CTX ctx)
{
	knh_context_t *ctx0 = (knh_context_t*)ctx;
	while(ctx0->parent != ctx0) {
		ctx0 = ctx0->parent;
	}
	return ctx0;
}

void knh_context_reftrace(CTX ctx, knh_context_t *o FTRARG)
{
#ifdef K_USING_CSTACK_TRAVERSE_
	knh_CommonContext_reftrace(ctx, (knh_context_t*)o FTRDATA);
	if(knh_getRootContext(ctx) == (CTX)o) {
		knh_share_reftrace(ctx, (knh_share_t*)o->share FTRDATA);
	}
#else
	tail_ = knh_CommonContext_reftrace(ctx, (knh_context_t*)o FTRDATA);
	if(knh_getRootContext(ctx) == (CTX)o) {
		tail_ = knh_share_reftrace(ctx, (knh_share_t*)o->share FTRDATA);
	}
	KNH_SIZEREF(ctx);
#endif
}

void knh_Context_free(CTX ctx, knh_context_t* ctxo)
{
	knh_CommonContext_free(ctx, ctxo);
	if(knh_getRootContext(ctx) == (CTX)ctxo) {
		size_t i, j;
		for(i = 0; i < ctxo->share->sizeClassTBL; i++) {
			knh_ClassTBL_t *t = varClassTBL(i);
			knh_Array_t *a = t->methods;
			for(j = 0; j < knh_Array_size(a); j++) {
				knh_Method_toAbstract(ctx, a->methods[j]);
			}
		}
		knh_share_free(ctx, (knh_share_t*)ctxo->share);
		knh_bzero((void*)ctxo, sizeof(knh_context_t));
		free((void*)ctxo);
	}
	else {
		knh_bzero((void*)ctxo, sizeof(knh_context_t));
		knh_free(ctx, (void*)ctxo, sizeof(knh_context_t));
	}
}

/* ------------------------------------------------------------------------ */
/* [konohaapi] */

konoha_t konoha_open(size_t stacksize)
{
	konoha_t k = {KONOHA_MAGIC};
	konoha_init();
	{
		CTX ctx = new_RootContext();
		k.ctx = (knh_context_t*)ctx;
	}
	return k;
}

void knh_reftraceAll(CTX ctx FTRARG)
{
	CTX ctx0 = knh_getRootContext(ctx);
	knh_context_reftrace(ctx0, (knh_context_t*)ctx0 FTRDATA);
}

void konoha_close(konoha_t konoha)
{
	CTX ctx = (knh_context_t*)konoha.ctx;
	KONOHA_CHECK_(konoha);
	if(ctx->share->threadCounter > 1) {
		KNH_WARN(ctx, "still %d thread(s) running", (int)ctx->share->threadCounter);
		return;
	}
	knh_showMemoryStat(ctx);
#ifdef K_USING_RCGC
#ifdef K_USING_CSTACK_TRAVERSE_
#define ARG knh_Object_RCsweep
#else
#define ARG ctx->ref_buf
#endif
	knh_context_reftrace(ctx, (knh_context_t*)ctx, ARG);
#ifndef K_USING_CSTACK_TRAVERSE_
	//knh_RefTraverse(ctx, knh_Object_RCsweep);
#endif
#undef ARG
#endif
	((knh_context_t*)ctx)->bufa = NULL; // necessary for KNH_SYSLOG
	knh_Context_free(ctx, (knh_context_t*)ctx);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
