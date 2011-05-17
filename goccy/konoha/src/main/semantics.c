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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_SEMANTICS

static
knh_class_t knh_addSpecializedType(CTX ctx, knh_class_t cid, knh_class_t bcid, knh_Semantics_t *u);

/* ------------------------------------------------------------------------ */

knh_Int_t* new_IntX(CTX ctx, knh_class_t cid, knh_int_t value)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	if(DP(u)->fichk(u, value)) {
		knh_Int_t *n = (knh_Int_t*)new_H(ctx, FLAG_Int, CLASS_Int, cid);
		n->n.ivalue = value;
		return n;
	}
	else {
		if(DP(u)->imin >= 0) {
			KNH_SYSLOG(ctx, LOG_WARNING, _("out of range: %u of %s"), value, CLASS__(cid));
		}
		else {
			KNH_SYSLOG(ctx, LOG_WARNING, _("out of range: %d of %s"), value, CLASS__(cid));
		}
		return DP(u)->ivalue;
	}
}

/* ------------------------------------------------------------------------ */

knh_Float_t* new_FloatX(CTX ctx, knh_class_t cid, knh_float_t value)
{
	DBG_ASSERT_cid(cid);
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	if(DP(u)->ffchk(u, value)) {
		knh_Float_t *f = (knh_Float_t*)new_H(ctx, FLAG_Float, CLASS_Float, cid);
		f->n.fvalue = value;
		return f;
	}
	else {
		KNH_SYSLOG(ctx, LOG_WARNING, _("out of range: %f of %s"), value, CLASS__(cid));
		return DP(u)->fvalue;
	}
}

/* ------------------------------------------------------------------------ */
/* [Int] */

static int knh_fichk__range(knh_Semantics_t *u, knh_int_t v)
{
	return (DP(u)->imin <= v && v <= DP(u)->imax);
}

static int knh_fichk__umax(knh_Semantics_t *u, knh_uint_t v)
{
	return (v <= DP(u)->umax);
}

static int knh_fichk__urange(knh_Semantics_t *u, knh_uint_t v)
{
	return (DP(u)->umin <= v && v <= DP(u)->umax);
}

static int knh_ficmp__unsigned(knh_Semantics_t *u, knh_uint_t v1, knh_uint_t v2)
{
	if(v1 == v2) return 0;
	return (v1 > v2) ? 1 : -1;
}

static int knh_ffchk__range(knh_Semantics_t *u, knh_float_t v)
{
	return (DP(u)->fmin <= v && v <= DP(u)->fmax);
}

static int knh_ffcmp__step(knh_Semantics_t *u, knh_float_t v1, knh_float_t v2)
{
	knh_float_t v = v1 - v2;
	if(v >= DP(u)->fstep) return 1;
	if(v <= -(DP(u)->fstep)) return -1;
	return 0;
}

/* ------------------------------------------------------------------------ */

static void knh_Semantics_initIntRange(CTX ctx, knh_Semantics_t *u, knh_int_t min, knh_int_t max)
{
	DP(u)->imin = min;
	DP(u)->fmin = (knh_float_t)min;
	DP(u)->imax = max;
	DP(u)->fmax = (knh_float_t)max;
	DP(u)->fstep = K_FLOAT_ONE;
	if(min >= 0) {
		DP(u)->ficmp = (knh_Ficmp)knh_ficmp__unsigned;
		if(min == 0) {
			DP(u)->fichk = (knh_Fichk)knh_fichk__umax;
		}
		else {
			DP(u)->fichk = (knh_Fichk)knh_fichk__urange;
		}
		DP(u)->ffchk = knh_ffchk__range;
	}
	else {
		if(min != K_INT_MIN || max != K_INT_MAX) {
			DP(u)->fichk = knh_fichk__range;
			DP(u)->ffchk = knh_ffchk__range;
		}
	}
	DP(u)->ffcmp = knh_ffcmp__step;
}

/* ------------------------------------------------------------------------ */

void knh_write_intx(CTX ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_int_t v)
{
	char buf[K_INT_FMTSIZ];
	char *fmt = K_INT_FMT;
	if(DP(u)->imin >= 0) {
		fmt = K_UINT_FMT;
	}
	knh_snprintf(buf, sizeof(buf), fmt, v);
	knh_write(ctx, w, B(buf));
	knh_bytes_t tag = S_tobytes(DP(u)->tag);
	if(tag.len > 0) {
		knh_putc(ctx, w, '[');
		knh_write(ctx, w, tag);
		knh_putc(ctx, w, ']');
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Semantics_initFloatRange(CTX ctx, knh_Semantics_t *u, knh_float_t min, knh_float_t max, knh_float_t step)
{
	DP(u)->fmin = min;
	DP(u)->fmax = max;
	DP(u)->fstep = step;
	DP(u)->imax = (knh_int_t)max;
	DP(u)->imin = (knh_int_t)min;

	if(min >= K_FLOAT_ZERO) {
		DP(u)->ficmp = (knh_Ficmp)knh_ficmp__unsigned;
		if(min == 0) {
			DP(u)->fichk = (knh_Fichk)knh_fichk__umax;
		}
		else {
			DP(u)->fichk = (knh_Fichk)knh_fichk__urange;
		}
		DP(u)->ffchk = knh_ffchk__range;
	}
	else {
		if(min != K_FLOAT_MIN || max != K_FLOAT_MAX) {
			DP(u)->fichk = knh_fichk__range;
			DP(u)->ffchk = knh_ffchk__range;
		}
	}

	if(step != K_FLOAT_ZERO) {
		DP(u)->ffcmp = knh_ffcmp__step;
	}
}

/* ------------------------------------------------------------------------ */

void knh_write_floatx(CTX ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_float_t v)
{
	char *FMT = K_FLOAT_FMT;
#if !defined(K_USING_NOFLOT)
	knh_float_t step = DP(u)->fstep;
	if(0.1 <= step && step < 1.0) {
		FMT = K_FLOAT_FMT1;
	}
	else if(0.01 <= step && step < 0.1) {
		FMT = K_FLOAT_FMT2;
	}
	else if(0.001 <= step && step < 0.01) {
		FMT = K_FLOAT_FMT3;
	}
	else if(0.0001 <= step && step < 0.001) {
		FMT = K_FLOAT_FMT4;
	}
#endif
	knh_write_ffmt(ctx, w, FMT, v);
	knh_bytes_t tag = S_tobytes(DP(u)->tag);
	if(tag.len > 0) {
		knh_putc(ctx, w, '[');
		knh_write(ctx, w, tag);
		knh_putc(ctx, w, ']');
	}
}

/* ------------------------------------------------------------------------ */

static TYPEMAP knh_IntX_FloatX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_float_t v = (knh_float_t)sfp[0].ivalue;
	KNH_MAPPED_Float(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */

static TYPEMAP knh_FloatX_IntX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_int_t v = (knh_int_t)sfp[0].fvalue;
	KNH_MAPPED_Int(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */

static
int Semantics_isVocab(knh_Semantics_t *u)
{
	return IS_DictIdx(DP(u)->vocabDictIdx);
}

/* ------------------------------------------------------------------------ */

static
knh_String_t *knh_Semantics_getVocabAt(CTX ctx, knh_Semantics_t *u, size_t n)
{
	return knh_DictIdx_get__fast(DP(u)->vocabDictIdx, n - DP(u)->imin);
}

/* ------------------------------------------------------------------------ */

static
knh_int_t knh_Semantics_getVocabIdx(CTX ctx, knh_Semantics_t *u, knh_String_t *s)
{
	return knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, S_tobytes(s)) + DP(u)->imin;
}

/* ------------------------------------------------------------------------ */

static TYPEMAP knh_IntX_Vocab(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	KNH_ASSERT(IS_Semantics(u));
	RETURN_(knh_Semantics_getVocabAt(ctx, u, sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */

static TYPEMAP knh_Vocab_IntX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	KNH_ASSERT(IS_Semantics(u));
	KNH_MAPPED_Int(ctx, sfp, knh_Semantics_getVocabIdx(ctx, u, sfp[0].s));
}

/* ------------------------------------------------------------------------ */

void knh_Semantics_reuse(CTX ctx, knh_Semantics_t *u, knh_class_t cid)
{
	knh_class_t bcid = ClassTBL(cid)->bcid;
	if(bcid == CLASS_Int) {
		knh_int_t v = 0;
		if(!DP(u)->fichk(u, v)) {
			v = DP(u)->imin;
		}
		KNH_ASSERT(DP(u)->ivalue == NULL);
		KNH_INITv(DP(u)->ivalue, new_Int(ctx, cid, v));
		if(DP(u)->fvalue != NULL) {
			knh_TypeMap_t *mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, DP(u)->ucid, cid, knh_FloatX_IntX, (Object*)u);
			knh_addTypeMap(ctx, mpr);
			mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, cid, DP(u)->ucid, knh_IntX_FloatX, (Object*)u);
			knh_addTypeMap(ctx, mpr);
		}
		if(DP(u)->svalue != NULL) {
			if(Semantics_isVocab(u)) {
				knh_TypeMap_t *mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, cid, DP(u)->ucid, knh_IntX_Vocab, (Object*)u);
				knh_addTypeMap(ctx, mpr);
				mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, DP(u)->ucid, cid, knh_Vocab_IntX, (Object*)u);
				knh_addTypeMap(ctx, mpr);
			}
		}
	}
	else if(bcid == CLASS_Float) {
		knh_float_t v = 0;
		if(!DP(u)->ffchk(u, v)) {
			v = DP(u)->fmin;
		}
		KNH_ASSERT(DP(u)->fvalue == NULL);
		KNH_INITv(DP(u)->fvalue, new_Float(ctx, cid, v));
		if(DP(u)->ivalue != NULL) {
			knh_TypeMap_t *mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, cid, DP(u)->ucid, knh_FloatX_IntX, (Object*)u);
			knh_addTypeMap(ctx, mpr);
			mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, DP(u)->ucid, cid, knh_IntX_FloatX, (Object*)u);
			knh_addTypeMap(ctx, mpr);
		}
	}
	else {
		KNH_ASSERT(bcid == CLASS_String);
		TODO();

	}
}

/* ------------------------------------------------------------------------ */

knh_Semantics_t* new_Enum(CTX ctx, char *tag, knh_bytes_t urn, knh_int_t min, knh_int_t max)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_Int;
	KNH_SETv(ctx, DP(u)->urn, new_S(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, new_T(tag));
	}
	knh_Semantics_initIntRange(ctx, u, min, max);
	if(!DP(u)->fichk(u, 0)) {
		KNH_INITv(DP(u)->ivalue, new_Int(ctx, cid, min));
	}
	else {
		KNH_INITv(DP(u)->ivalue, new_Int(ctx, cid, 0));
	}
	knh_addSpecializedType(ctx, cid, CLASS_Int, u);
	return u;
}

/* ------------------------------------------------------------------------ */


knh_Semantics_t* new_Unit(CTX ctx, char *tag, knh_bytes_t urn, knh_float_t min, knh_float_t max, knh_float_t step)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_Float;
	KNH_SETv(ctx, DP(u)->urn, new_S(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, new_T(tag));

	}
	knh_Semantics_initFloatRange(ctx, u, min, max, step);
#if !defined(K_USING_NOFLOT)
	if(!DP(u)->ffchk(u, 0.0)) {
		KNH_INITv(DP(u)->fvalue, new_Float(ctx, cid, min));
	}
	else {
		KNH_INITv(DP(u)->fvalue, new_Float(ctx, cid, 0.0));
	}
#endif
	knh_addSpecializedType(ctx, cid, CLASS_Float, u);
	return u;
}

/* ------------------------------------------------------------------------ */
/* [String] */

static
knh_String_t *knh_fsnew__dict(CTX ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL, int *foundError)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	knh_index_t n = knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, t);
	if(n == -1) {
		knh_bytes_t tag;
		if(knh_bytes_splitTag(t, &tag, &t)) {
			n = knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, t);
		}
	}
	if(n == -1) {
		*foundError = 1;
		return DP(u)->svalue;
	}
	return knh_DictIdx_get__fast(DP(u)->vocabDictIdx, n);
}

/* ------------------------------------------------------------------------ */

static
int knh_fscmp__dict(knh_Semantics_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_DictIdx_index(NULL, DP(u)->vocabDictIdx, v1) - knh_DictIdx_index(NULL, DP(u)->vocabDictIdx, v2);
}

/* ------------------------------------------------------------------------ */

static
knh_DictIdx_t* new_DictIdx__Array(CTX ctx, knh_Array_t *a)
{
	knh_DictIdx_t *o = (knh_DictIdx_t*)new_H(ctx, FLAG_DictIdx, CLASS_DictIdx, CLASS_DictIdx);
	KNH_INITv(o->terms, a);
	KNH_INITv(o->termsDictSet, new_DictSet(ctx, knh_Array_size(a)));
	o->offset = 0;
	{
		knh_uintptr_t i = 0;
		for(i = 0; i < knh_Array_size(a); i++) {
			knh_String_t *s = (knh_String_t*)knh_Array_n(a, i);
			knh_DictSet_append(ctx, o->termsDictSet, s, i+1);
		}
	}
	//knh_DictSet_toIgnoreCase(o->termsDictSet);
	return o;
}

/* ------------------------------------------------------------------------ */

static
TYPEMAP knh_Ftypemap_vocabidx(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, O_cid(sfp[0].o));
	knh_int_t n = knh_Semantics_getVocabIdx(ctx, u, sfp[0].s);
	DBG_P("n = %d", (int)n);
	KNH_MAPPED_Int(ctx, sfp, n);
}

/* ------------------------------------------------------------------------ */

static
TYPEMAP knh_Ftypemap_vocab(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	size_t n = (size_t)(sfp[0].ivalue - DP(u)->imin);
	DBG_P("n = %zd", n);
	knh_Array_t *a = (DP(u)->vocabDictIdx)->terms;
	Object *s = KNH_NULL;
	if(n < knh_Array_size(a)) {
		s = knh_Array_n(a, n);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */

knh_Semantics_t* new_Vocab(CTX ctx, char *tag, knh_bytes_t urn, int base, char **terms)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_String;
	KNH_SETv(ctx, DP(u)->urn, new_S(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, new_T(tag));
	}
	DP(u)->fsnew = knh_fsnew__dict;
	DP(u)->fscmp = knh_fscmp__dict;
	{
		knh_Array_t *a = new_Array0(ctx, 0);
		while(*terms != NULL) {
			knh_String_t *s = new_T(*terms);
			knh_Array_add_(ctx, a, UPCAST(s));
			s->h.cid = cid;
			terms++;
		}
		KNH_SETv(ctx, DP(u)->vocabDictIdx, new_DictIdx__Array(ctx, a));
		knh_Semantics_initIntRange(ctx, u, (knh_int_t)base, (knh_int_t)(base + knh_Array_size(a) - 1));
		KNH_ASSERT(knh_Array_size(a) > 0);
		KNH_INITv(DP(u)->svalue, knh_Array_n(a, 0));
	}
	knh_addSpecializedType(ctx, cid, CLASS_String, u);
	{
		knh_TypeMap_t *mpr = new_TypeMap(ctx, FLAG_TypeMap_Total|FLAG_TypeMap_Const|FLAG_TypeMap_Final, cid, CLASS_Int, knh_Ftypemap_vocabidx, (Object*)u);
		knh_addTypeMap(ctx, mpr);
		mpr = new_TypeMap(ctx, FLAG_TypeMap_Const|FLAG_TypeMap_Final, CLASS_Int, cid, knh_Ftypemap_vocab, (Object*)u);
		knh_addTypeMap(ctx, mpr);
	}
	return u;
}

///* ------------------------------------------------------------------------ */
//
//static
//TYPEMAP knh_TypeMap__fdict(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	String *s = sfp[0].s;
//	Semantics *u = knh_getSemantics(ctx, O_cid(sfp[0].o)].cspec;
//	KNH_ASSERT(IS_Semantics(u));
//	KNH_ASSERT(IS_DictIdx(DP(u)->vocabDictIdx));
//
//	Semantics *u2 = knh_getSemantics(ctx, DP(sfp[1].mpr)->tcid].cspec;
//	KNH_ASSERT(IS_Semantics(u2));
//	KNH_ASSERT(IS_DictIdx(DP(u2)->vocabDictIdx));
//
//	size_t n = (size_t)knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, S_tobytes(s));
//	KNH_ASSERT(n < knh_DictIdx_size(DP(u2)->vocabDictIdx));
//	RETURN_(knh_DictIdx_get__fast(DP(u2)->vocabDictIdx, n));
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_addVocabularyTypeMap(CTX ctx, knh_class_t scid, char *text)
//{
//	DBG_ASSERT_cid(scid);
//	knh_class_t tcid = knh_findcid(ctx, B(text));
//
//	if(tcid != CLASS_unknown || ctx->share->ClassTBL[tcid].bcid != tcid) {
////		Semantics *u = knh_getSemantics(ctx, scid].cspec;
////		if(!IS_Semantics(u) || !IS_DictIdx(DP(u)->vocabDictIdx)) {
////			TODO();
////			return;
////		}
//		Semantics *u2 = knh_getSemantics(ctx, tcid].cspec;
//		if(!IS_Semantics(u2) || !IS_DictIdx(DP(u2)->vocabDictIdx)) {
//			TODO();
//			return;
//		}
////		if(knh_DictIdx_size(DP(u)->vocabDictIdx) != knh_DictIdx_size(DP(u2)->vocabDictIdx)) {
////			TODO();
////			return;
////		}
//		TypeMap *mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, scid, tcid,
//				knh_TypeMap_fvocab, (Object*)KNH_NULL);
//		if(ctx->share->ClassTBL[scid].cmap == NULL) {
//			knh_ClassTBL_t *TC = (knh_ClassTBL_t*)(&ctx->share->ClassTBL[scid]);
//			KNH_INITv(TC->cmap, new_ClassMap0(ctx, 4));
//		}
//		knh_ClassMap_add(ctx, ctx->share->ClassTBL[scid].cmap, mpr);
//
//		mpr = new_TypeMap(ctx, FLAG_TypeMap_Affine, tcid, scid,
//				knh_TypeMap_fvocab, (Object*)KNH_NULL);
//		if(ctx->share->ClassTBL[tcid].cmap == NULL) {
//			knh_ClassTBL_t *TC = (knh_ClassTBL_t*)(&ctx->share->ClassTBL[tcid]);
//			KNH_INITv(TC->cmap, new_ClassMap0(ctx, 4));
//		}
//		knh_ClassMap_add(ctx, ctx->share->ClassTBL[tcid].cmap, mpr);
//	}
//}

/* ------------------------------------------------------------------------ */
/* [urnalias] */

knh_bytes_t knh_getURNAlias(CTX ctx, knh_bytes_t aurn)
{
	knh_String_t *s = (knh_String_t*)knh_DictMap_get(ctx,  DP(ctx->sys)->URNAliasDictMap, aurn);
	if(IS_NOTNULL(s)) {
		return S_tobytes(s);
	}
	return aurn;
}

///* ------------------------------------------------------------------------ */
//
//void knh_setAliasURN(CTX ctx, String *alias, String *urn)
//{
//	if(CTX_isVerbose(ctx)) {
//		String *s =(String*)knh_DictMap_get__b(ctx,  DP(ctx->sys)->URNAliasDictMap, S_tobytes(urn));
//		if(IS_NOTNULL(s)) {
//			KNH_SYSLOG(ctx, LOG_WARNING, "Overriding %s %s", __tochar(alias), __tochar(s));
//		}
//	}
//	knh_DictMap_set(ctx, DP(ctx->sys)->URNAliasDictMap, alias, UPCAST(urn));
//}

/* ------------------------------------------------------------------------ */

void knh_loadScriptURNAliasData(CTX ctx, knh_StringData_t *data)
{
	knh_DictMap_t *map = DP(ctx->sys)->URNAliasDictMap;
	knh_StringData_t *d = data;
	while(d->name != NULL) {
		knh_String_t *s =(knh_String_t*)knh_DictMap_get(ctx,  map, B(d->name));
		if(IS_NOTNULL(s) && !S_equals(s, B(d->value))) {
			KNH_SYSLOG(ctx, LOG_WARNING, _("overriding alias %s %s as %s"), d->name, S_tochar(s), d->value);
		}
		d++;
	}
	d = data;
	while(d->name != NULL) {
		knh_String_t *n = new_T(d->name);
		knh_DictMap_append(ctx, map, n, UPCAST(new_T(d->value)));
		d++;
	}
}

/* ------------------------------------------------------------------------ */

void knh_loadScriptSemanticsFuncData(CTX ctx, knh_NamedPointerData_t *data)
{
	knh_DictSet_t *ds = DP(ctx->sys)->SpecFuncDictSet;
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	while(data->name != NULL) {
		DBG_P("adding.. '%s'", data->name);
		knh_String_t *n = new_T(data->name);
		knh_DictSet_set(ctx, ds, n, (knh_uintptr_t)data->ptr);
		data++;
	}
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */

static
knh_Semantics_t *new_SemanticsNULL(CTX ctx, knh_bytes_t urn)
{
	knh_Semantics_t *u = NULL;
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t loc = 0;
	knh_bytes_t p = urn;
	while(loc != -1) {
		knh_fspec func = (knh_fspec)knh_DictSet_get(DP(ctx->sys)->SpecFuncDictSet, p);
		if(func != NULL) {
			u = func(ctx, urn);
			if(u != NULL) {
				KNH_SETv(ctx, DP(u)->urn, new_S(ctx, urn, NULL));
			}
			goto L_UNLOCK;
		}
		loc = knh_bytes_rindex(p, '/');
		if(loc != -1) {
			p = knh_bytes_first(p, loc);
		}
	}
L_UNLOCK:;
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return u;
}

/* ------------------------------------------------------------------------ */
/* [SPEC] */

knh_Semantics_t *knh_getSemantics(CTX ctx, knh_class_t cid)
{
	knh_Semantics_t *u = (knh_Semantics_t*)ClassTBL(cid)->cspec;
	KNH_ASSERT(IS_Semantics(u));
	return u;
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTBL_fdefault__ISPEC(CTX ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UPCAST(DP(u)->ivalue);
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTBL_fdefault__FSPEC(CTX ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UPCAST(DP(u)->fvalue);
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTBL_fdefault__SSPEC(CTX ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UPCAST(DP(u)->svalue);
}

/* ------------------------------------------------------------------------ */

static
knh_class_t knh_addSpecializedType(CTX ctx, knh_class_t cid, knh_class_t supcid, knh_Semantics_t *u)
{
	knh_class_t bcid = knh_class_bcid(supcid);
	char bufcn[CLASS__AME_BUFSIZ];
	if(cid == CLASS_newid) {
		cid = new_ClassId(ctx);
	}
	knh_snprintf(bufcn, sizeof(bufcn), KNH_SEMANTICS_FMT, CLASS__(bcid), S_tochar(DP(u)->urn));
	knh_setClassName(ctx, cid, new_S(ctx, B(bufcn), NULL));
	{
		knh_ClassTBL_t *t = varClassTBL(cid);
		if((DP(u)->tag)->size > 0) {
			knh_snprintf(bufcn, sizeof(bufcn), "%s:%s", CLASS__(bcid), S_tochar(DP(u)->tag));
			KNH_SETv(ctx, t->sname, new_S(ctx, B(bufcn), NULL));
		}
		DBG_P("added %d, '%s'", cid, S_tochar(t->lname));

		t->bcid   = bcid;
		t->supcid = bcid;
		t->cflag  = ClassTBL(bcid)->cflag | FLAG_Class_Final;
		t->oflag  = ClassTBL(bcid)->oflag;
		t->offset = ClassTBL(bcid)->offset;

		t->cspi  = ClassTBL(bcid)->cspi;
		t->size = ClassTBL(bcid)->size;
		t->bsize  = ClassTBL(bcid)->bsize;

		DBG_ASSERT(t->fields == NULL);
		KNH_INITv(t->methods, ClassTBL(supcid).methods);

		if(t->cmap == NULL) {
			KNH_INITv(t->typemaps, K_EMPTYARRAY);
		}
		else {
			KNH_ASSERT(IS_ClassMap(t->cmap));
		}

		DBG_ASSERT(t->cspec == NULL);
		KNH_INITv(t->cspec, u);

		if(DP(u)->ucid != cid) {
			knh_Semantics_reuse(ctx, u, cid);
		}
		if(bcid == CLASS_Int) {
			KNH_ASSERT(DP(u)->ivalue != NULL);
			t->fdefnull = knh_ClassTBL_fdefault__ISPEC;
		}
		else if(bcid == CLASS_Float) {
			KNH_ASSERT(DP(u)->fvalue != NULL);
			t->fdefnull = knh_ClassTBL_fdefault__FSPEC;
		}
		else {
			KNH_ASSERT(bcid == CLASS_String);
			KNH_ASSERT(DP(u)->svalue != NULL);
			t->fdefnull = knh_ClassTBL_fdefault__SSPEC;
		}
	}
	return cid;
}

/* ------------------------------------------------------------------------ */

static
knh_Semantics_t* knh_findSemanticsNULL(CTX ctx, knh_bytes_t lname)
{
	knh_index_t loc = knh_bytes_index(lname, '{');
	if(loc != -1) {
		char *postfix = (char*)lname.buf + loc;
		char cname[CLASS__AME_BUFSIZ];
		knh_snprintf(cname, sizeof(cname), "Int%s", postfix);
		knh_class_t cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
		knh_snprintf(cname, sizeof(cname), "Float%s", postfix);
		cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
		knh_snprintf(cname, sizeof(cname), "String%s", postfix);
		cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_findcidx(CTX ctx, knh_bytes_t lname)
{
	knh_Semantics_t *u = 	knh_findSemanticsNULL(ctx, lname);
	if(u != NULL) {
		knh_index_t loc = knh_bytes_index(lname, '{');
		knh_class_t bcid = knh_getcid(ctx, knh_bytes_first(lname, loc));
		return knh_addSpecializedType(ctx, CLASS_newid, bcid, u);
	}
	else {
		knh_index_t loc = knh_bytes_index(lname, '{');
		knh_bytes_t urn = knh_bytes_last(lname, loc+1); urn.len -= 1;
		knh_class_t bcid = knh_getcid(ctx, knh_bytes_first(lname, loc));
		DBG_P("cid=%d,%s", bcid, CLASS__(bcid));
		u = new_SemanticsNULL(ctx, urn);
		if(u != NULL) {
			knh_class_t ucid = DP(u)->ucid;
			knh_class_t ubcid = DP(u)->ubcid;
			DBG_P("cid=%d,%s", ubcid, CLASS__(ubcid));
			//knh_addSpecializedType(ctx, ucid, ubcid, u);
			if(ubcid != bcid) {
				return knh_addSpecializedType(ctx, CLASS_newid, bcid, u);
			}
			return ucid;
		}
		return bcid;
	}
}

/* ------------------------------------------------------------------------ */

#endif/*K_USING_SEMANTICS*/

#ifdef __cplusplus
}
#endif
