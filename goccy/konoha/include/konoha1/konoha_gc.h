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

#ifndef KONOHA_GC_H_
#define KONOHA_GC_H_

#include"konoha_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

#define K_RCGC_INIT           0

#ifdef K_USING_RCGC
#define knh_Object_RCinc(o)     (o)->h.refc ++
#define DBG_Object_RCinc(o)    {\
		(o)->h.refc ++; \
		if(O_cid(o) == CLASS_Tdynamic) {\
			DBG_P("p=%p refc=%d", o, o->h.refc);\
		}\
	}\

#define knh_Object_RCplus(o,n)  (o)->h.refc += n
#define knh_Object_DRCsweep(ctx, o)  knh_Object_sweep(ctx, o)
#define knh_Object_RCdec(o)     (o)->h.refc--
#define knh_Object_RCset(o,c)   (o)->h.refc = (c)
#define Object_isRC0(o)     ((o)->h.refc == 0)
#define RCGC_(STMT)             STMT
#define MSGC_(STMT)
#define MTGC_(STMT)

#define KNH_SWAPMOV(ctx, sfp, n, n2) {\
		knh_sfp_t sfpN = sfp[n];\
		sfp[n] = sfp[n2];\
		sfp[n2] = sfpN;\
	}\

#define KNH_GC(ctx)  DBG_(knh_System_gc(ctx))
#define O_toTenure(o)

#else/* K_USING_RCGC*/

#ifdef K_USING_MTGC
#define knh_Object_RCinc(o)       (o)->h.mscheck = ctx->mscheck
#define knh_Object_RCplus(o,n)    (o)->h.mscheck = ctx->mscheck
#define MTGC_(STMT)             STMT
#else
#define knh_Object_RCinc(o)
#define knh_Object_RCplus(o,n)
#define MTGC_(STMT)
#endif

#define knh_Object_RCdec(o)
#define Object_isRC0(o)    (0)
#define knh_Object_RCset(o,c)
#define knh_Object_DRCsweep(ctx, o)
#define RCGC_(STMT)
#define MSGC_(STMT)             STMT

#define KNH_SWAPMOV(ctx, sfp, n, n2) {\
		sfp[n] = sfp[n2];\
	}\

static inline int knh_System_checkGC(CTX ctx)
{
	knh_stat_t *ctxstat = ctx->stat;
	size_t used = ctxstat->usedObjectSize;
	if(!(used < ctx->share->gcBoundary)) {
		return 1;
	}
	return 0;
}

#define KNH_GC(ctx)   {\
		if (knh_System_checkGC(ctx)) {\
			knh_System_gc(ctx);\
		}\
	}

#define O_toTenure(o)  knh_Object_toTenure(ctx, o)

#endif/*K_USING_RCGC*/

#ifdef _MSC_VER
#define OBJECT_SET(v, h) {\
	knh_Object_t **v_ = (knh_Object_t**)&v; \
	v_[0] = (h_); \
}
#else
#define OBJECT_SET(v, h) v = (typeof(v))(h_)
#endif /*_MSC_VER*/

#define KNH_INITv(v,o) {\
		knh_Object_t *h_ = (knh_Object_t*)o; \
		DBG_ASSERT_ISOBJECT(h_); \
		knh_Object_RCinc(h_); \
		OBJECT_SET(v, h_);\
	}\

#define KNH_SETv(ctx,v,o) {\
		knh_Object_t *h_ = (knh_Object_t*)o; \
		DBG_ASSERT_ISOBJECT(v);  \
		DBG_ASSERT_ISOBJECT(h_); \
		knh_Object_RCinc(h_); \
		knh_Object_DRCsweep(ctx, (knh_Object_t*)v); \
		OBJECT_SET(v, h_);\
	}\

#define KNH_RCSETv(ctx,v,o) {\
		knh_Object_t *h_ = (knh_Object_t*)o; \
		DBG_ASSERT_ISOBJECT(v);  \
		DBG_ASSERT_ISOBJECT(h_); \
		knh_Object_RCinc(h_); \
		knh_Object_DRCsweep(ctx, (knh_Object_t*)v); \
		OBJECT_SET(v, h_);\
	}\

#define KNH_NGCSETv(ctx,v,o) {\
		knh_Object_t *h_ = (knh_Object_t*)o; \
		DBG_ASSERT_ISOBJECT(h_); \
		knh_Object_RCinc(h_); \
		knh_Object_RCdec(v); \
		OBJECT_SET(v, h_);\
	}\

#define KNH_FINALv(ctx, v) {\
		knh_Object_DRCsweep(ctx, UPCAST(v)); \
		v = NULL; \
	}\

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_GC_H_*/
