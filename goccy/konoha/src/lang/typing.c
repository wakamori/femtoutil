/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us
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

#define USE_STEXT 1
#define USE_bytes_equals
#define USE_bytes_endsWith    1
#define USE_bytes_index       1
#define USE_bytes_first       1
#define USE_bytes_last        1
#define USE_bytes_parseint    1
#define USE_bytes_parsefloat  1
#define USE_cwb_open          1
#define USE_cwb_size          1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

static knh_Token_t *Stmt_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt);
static knh_Token_t *CALL_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt);

#define STT_DECLFIELD  STT_DECL
#define STT_DECLSCRIPT STT_DECL

#define BEGIN_BLOCK(X) \
	size_t X = DP(ctx->gma)->espidx;\
	DBG_ASSERT(DP(ctx->gma)->espidx + DP(ctx->gma)->ebpidx == DP(ctx->gma)->gsize);\

#define END_BLOCK(X) \
	if((size_t)DP(ctx->gma)->espidx > X) {\
		/*DBG_P("ESP %s start=%d, end=%d, stmt->espidx=%d", TT__(STT_(stmt)), X, DP(ctx->gma)->espidx, DP(stmt)->espidx);*/ \
		/*DP(stmt)->espidx = DP(ctx->gma)->espidx;*/ \
		Gamma_clear(ctx, (DP(ctx->gma)->gsize - (DP(ctx->gma)->espidx - (knh_short_t)X)), stmt);\
	}\

#define CONSTPOOL(v)   knh_getConstPools(ctx, v)

#define _FREADONLY   FLAG_Field_ReadOnly
#define _FGETTER     FLAG_Field_Getter
#define _FSETTER     FLAG_Field_Setter
#define _FKEY        FLAG_Field_Key
#define _FREG        (FLAG_Field_Key|FLAG_Field_ReadOnly)
#define _FCHKOUT     FLAG_Field_Hidden

#define IS_SCRIPTLEVEL(ctx)       IS_NULL(DP(ctx->gma)->mtd)

#define Gamma_type(ctx, type)  knh_type_tocid(ctx, type, DP(ctx->gma)->this_cid)

static inline void Stmt_toSTT(knh_Stmt_t *stmt, knh_term_t stt)
{
	DBG_ASSERT(stt < TT_PRAGMA);
	STT_(stmt) = stt;
}

static inline knh_Token_t *Stmt_typed(CTX ctx, knh_Stmt_t *stmt, knh_type_t type)
{
	SP(stmt)->type = type;
	return (knh_Token_t*)stmt;
}

/* ----------------------------------------------------------------------- */
/* [Token] */

static knh_Token_t* Token_toCONST(CTX ctx, knh_Token_t *tk)
{
	DBG_ASSERT(IS_Token(tk));
	TT_(tk) = TT_CONST;
	SP(tk)->type = O_cid((tk)->data);
	return tk;
}

#define Token_setCONST(ctx, tk, d)  Token_setCONST_(ctx, tk, UPCAST(d))
static knh_Token_t* Token_setCONST_(CTX ctx, knh_Token_t *tk, dynamic *data)
{
	KNH_SETv(ctx, (tk)->data, data);
	return Token_toCONST(ctx, tk);
}

static knh_Token_t* new_TokenCONST_(CTX ctx, dynamic *data)
{
	knh_Token_t *tk = new_(Token);
	Token_setCONST_(ctx, tk, data);
	return tk;
}

knh_Token_t* knh_Token_toTYPED(CTX ctx, knh_Token_t *tk, knh_term_t tt, knh_type_t type, knh_short_t nn)
{
	TK_typing(tk, tt, type, nn);
	if(tt == TT_FIELD) {Gamma_foundFIELD(ctx->gma, 1); }
	return tk;
}

knh_Token_t* new_TokenTYPED(CTX ctx, knh_term_t tt, knh_type_t type, knh_short_t nn)
{
	knh_Token_t *tk = new_(Token);
	knh_Token_toTYPED(ctx, tk, tt, type, nn);
	return tk;
}

knh_bool_t StmtMETA_is_(CTX ctx, knh_Stmt_t *stmt, knh_bytes_t name)
{
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		Object *v = knh_DictMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, name);
		if(v != NULL) return 1;
	}
	return 0;
}

#define Stmt_insert(ctx, stmt, n, tm)   Stmt_insert_(ctx, stmt, n, (knh_Term_t*)(tm))

static void Stmt_insert_(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_Term_t *tm)
{
	size_t i = DP(stmt)->size;
	DBG_ASSERT(n <= DP(stmt)->size);
	knh_Stmt_add_(ctx, stmt, tm, NULL); // DP(o)->size++;
	for(; n < i ; i--) {
		knh_Stmt_swap(ctx, stmt, i - 1, i);
	}
}

/* ------------------------------------------------------------------------ */

#define Token_fn(ctx, tk) FN_UNMASK(Token_fnq(ctx, tk))

static knh_fieldn_t Token_fnq(CTX ctx, knh_Token_t *tk)
{
	knh_fieldn_t fn = FN_NONAME;
	if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME) {
		fn = knh_getfnq(ctx, TK_tobytes(tk), FN_NEWID);
	}
	return fn;
}

static knh_methodn_t Token_mn(CTX ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_FUNCNAME || TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME || TT_(tk) == TT_UFUNCNAME) {
		TT_(tk) = TT_MN;
		(tk)->mn = knh_getmn(ctx, TK_tobytes(tk), MN_NEWID);
	}
	if(TT_(tk) == TT_NEW) {
		TT_(tk) = TT_MN;
		(tk)->mn = knh_getmn(ctx, TK_tobytes(tk), MN_NEWID);
	}
	DBG_ASSERT(TT_(tk) == TT_MN);
	if(Token_isISBOOL(tk)) {
		(tk)->mn = MN_toISBOOL(MN_toFN((tk)->mn));
		Token_setISBOOL(tk, 0);
	}
	else if(Token_isGetter(tk)) {
		(tk)->mn = MN_toGETTER(MN_toFN((tk)->mn));
		Token_setGetter(tk, 0);
	}
	else if(Token_isSetter(tk)) {
		(tk)->mn = MN_toSETTER(MN_toFN((tk)->mn));
		Token_setSetter(tk, 0);
	}
	return (tk)->mn;
}

static knh_class_t TokenPTYPE_cid(CTX ctx, knh_Token_t *tk, knh_class_t bcid)
{
	knh_class_t cid;
	BEGIN_LOCAL(ctx, lsfp, 1);
	LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, pa, new_ParamArray(ctx));
	knh_ParamArray_t *bpa = ClassTBL(bcid)->cparam;
	size_t i;
	int psize = knh_Array_size(tk->list) - 1;
	for(i = 1; i < knh_Array_size((tk)->list); i++) {
		knh_Token_t *tkT = (tk)->list->tokens[i];
		if(TT_(tkT) == TT_DARROW) { i++; break; }
		knh_param_t p = {knh_Token_cid(ctx, tkT, CLASS_Tdynamic), FN_NONAME};
		if(p.type == TYPE_void) continue;
		if(psize == 1 && (bcid == CLASS_Map || bcid == CLASS_MapIm || bcid == CLASS_Tuple)) {
			knh_param_t p1 = {CLASS_String, FN_k}; // Map<T> => Map<String, T>
			knh_ParamArray_add(ctx, pa, p1);       // Tuple<T> => Tuple<String, T>
		}
		knh_ParamArray_add(ctx, pa, p);
	}
	if(bcid == CLASS_Func) { /* Func<T => T> */
		if(i < knh_Array_size((tk)->list)) {
			knh_Token_t *tkT = tk->list->tokens[i];
			knh_param_t p = {knh_Token_cid(ctx, tkT, CLASS_Tdynamic), FN_NONAME};
			if(p.type != TYPE_void) {
				knh_ParamArray_radd(ctx, pa, p);
			}
		}
		if(pa->psize == 0 && pa->rsize == 0) {
			cid = CLASS_Func;
			goto L_END;
		}
	}
	if(bcid == CLASS_Func || bcid == CLASS_Tuple) {
		cid = knh_class_Generics(ctx, bcid, pa);
		goto L_END;
	}
	if((pa)->psize != (bpa)->psize || (pa)->rsize != (bpa)->rsize) {
		WARN_WrongTypeParam(ctx, bcid);
		cid = bcid;
		goto L_END;
	}
	for(i = 0; i < (size_t)((pa)->psize + (pa)->rsize); i++) {
		knh_param_t* p = knh_ParamArray_get(pa, i);
		knh_param_t* bp = knh_ParamArray_get(bpa, i);
		p->fn = bp->fn;
		if(bp->type == TYPE_dyn) continue;
		if(p->type == bp->type) continue;
		{
			knh_class_t tcid = knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid);
			if(!class_isa(tcid, bp->type)) {
				WARN_WrongTypeParam(ctx, bcid);
				cid = bcid;
				goto L_END;
			}
		}
	}
	cid = knh_class_Generics(ctx, bcid, pa);
	L_END:;
	END_LOCAL_(ctx, lsfp);
	return cid;
}

knh_class_t knh_Token_cid(CTX ctx, knh_Token_t *tk, knh_type_t reqt)
{
	knh_class_t cid = CLASS_unknown;
	switch(TT_(tk)) {
		case TT_CID : {
			DBG_ASSERT((tk)->cid != CLASS_unknown);
			return (tk)->cid;
		}
		case TT_VAR : case TT_ASIS: {
			if(reqt != TYPE_var) cid = reqt;
			break;
		}
		case TT_VOID: cid = CLASS_Tvoid; break;
		case TT_DYN: cid = CLASS_Tdynamic; break;
		case TT_BYTE: {
			WARN_MuchBetter(ctx, "int");
			cid = CLASS_Int;
		}
		case TT_UNAME: case TT_UFUNCNAME: {
			knh_NameSpace_t *ns = K_GMANS;
			if(Token_isExceptionType(tk)) {
				(tk)->cid = CLASS_Exception;
				return CLASS_Exception;
			}
			cid = knh_NameSpace_getcid(ctx, ns, TK_tobytes(tk));
			break;
		}
		case TT_TYPEOF: {
			if(IS_Stmt((tk)->data)) {
				knh_Stmt_t *stmt = (tk)->stmt;
				knh_Token_t *tkRES = Stmt_typing(ctx, stmt, reqt);
				if(TT_(tkRES) != TT_ERR) {
					KNH_SETv(ctx, (tk)->data, tkRES);  // TO AVOID GC
					cid = CLASS_t(tkRES->type);
					DBG_P("typeof cid=%s", CLASS__(cid));
				}
			}
			break;
		}
		case TT_PTYPE: {
			DBG_ASSERT(IS_Array((tk)->list));
			knh_Token_t *tkC = tk->list->tokens[0];
			knh_class_t bcid = knh_Token_cid(ctx, tkC, CLASS_unknown);
			if(bcid == CLASS_unknown || (bcid != CLASS_Tuple && ClassTBL(bcid)->cparam == NULL)) {
				WARN_WrongTypeParam(ctx, bcid);  /* @CODE: String<T> // no such generics */
				cid = bcid;
			}
			else {
				cid = TokenPTYPE_cid(ctx, tk, bcid);
			}
		}
		default : {

		}
	}
	if(cid == CLASS_unknown && reqt != CLASS_unknown) {
		cid = reqt;
		if(reqt != TYPE_var) {
			WarningUnknownClass(ctx, tk, cid);
		}
	}
	if(TT_(tk) != TT_CID && cid != CLASS_unknown) {
		TK_typing(tk, TT_CID, TYPE_Class, cid);
	}
	return cid;
}

static knh_Token_t *TTYPE_typing(CTX ctx, knh_Token_t *tk, knh_type_t reqt)
{
	knh_class_t cid = knh_Token_cid(ctx, tk, reqt);
	if(cid == CLASS_unknown) {
		return ERROR_Undefined(ctx, "name", CLASS_unknown, tk);
	}
	return tk;
}

/* ------------------------------------------------------------------------ */
/* Gamma */

static void *Gamma_loadFunc(CTX ctx, char *funcname, int isREQUIRED)
{
	knh_NameSpace_t *ns = K_GMANS;
	if(ns->dlhdr != NULL) {
		void *f = knh_dlsym(ctx, LOG_DEBUG, ns->dlhdr, (const char*)funcname);
		if(f != NULL) return f;
		if (isREQUIRED) {
			WARN_NotFound(ctx, _("foreign function"), funcname);
		}
	}
	return NULL;
}

static knh_Fmethod Gamma_loadMethodFunc(CTX ctx, knh_class_t cid, knh_methodn_t mn, int isNATIVE)
{
	DBG_ASSERT_cid(cid);
	char buf[80];
	const char *cname = S_tochar(ClassTBL(cid)->sname);
	if(MN_isFMT(mn)) {
		knh_snprintf(buf, sizeof(buf), "%s__%s", cname, FN__(MN_toFN(mn)));
	}
	else if(MN_isGETTER(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_get%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else if(MN_isSETTER(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_set%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else if(MN_isISBOOL(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_is%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else {
		knh_snprintf(buf, sizeof(buf), "%s_%s", cname, FN__(mn));
	}
	return (knh_Fmethod)Gamma_loadFunc(ctx, buf, isNATIVE);
}

static void Gamma_clear(CTX ctx, size_t offset, knh_Stmt_t *stmt)
{
	size_t i;
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	if(stmt != NULL) {
		while(DP(stmt)->nextNULL != NULL) {
			stmt = DP(stmt)->nextNULL;
		}
	}
	for(i = offset; i < DP(ctx->gma)->gsize; i++) {
		if(gf[i].ucnt == 0 && IS_Token(gf[i].tkIDX)) {
			WarningUnusedName(ctx, gf[i].tkIDX, gf[i].fn);
		}
//		if(stmt != NULL && FLAG_is(gf[i].flag, _FCHKOUT)) {
//			knh_Stmt_t *stmt2 = new_Stmt2(ctx, STT_CHKOUT, gf[i].tkIDX, NULL);
//			gf[i].tkIDX->type = gf[i].type;
//			KNH_INITv(DP(stmt)->nextNULL, stmt2);
//			stmt = stmt2;
//		}
		gf[i].flag  = 0;
		gf[i].type  = TYPE_void;
		gf[i].fn    = FN_NONAME;
		KNH_SETv(ctx, gf[i].value, KNH_NULL);
		KNH_SETv(ctx, gf[i].tkIDX, KNH_NULL);
	}
	DP(ctx->gma)->gsize = offset;
	if((long)offset < (long)DP(ctx->gma)->ebpidx) {
		DP(ctx->gma)->espidx = 0;
		DP(ctx->gma)->ebpidx = offset;
	}
	else {
		DP(ctx->gma)->espidx = offset - DP(ctx->gma)->ebpidx;
		DBG_ASSERT(DP(ctx->gma)->espidx + DP(ctx->gma)->ebpidx == DP(ctx->gma)->gsize);
	}
}

static knh_gmafields_t* Gamma_expand(CTX ctx, knh_Gamma_t *gma, size_t minimum)
{
	knh_GammaEX_t *b = DP(gma);
	size_t i, s = b->gcapacity;
	knh_gmafields_t *gf = NULL;
	b->gcapacity = b->gcapacity * 2;
	if(b->gcapacity < minimum) b->gcapacity = minimum;
	b->gf = (knh_gmafields_t*)KNH_REALLOC(ctx, "gmafield", b->gf, s, b->gcapacity, sizeof(knh_gmafields_t));
	gf = b->gf;
	//DBG_P("Gamma_expand: b->gcapacity=%d=>%d", s, b->gcapacity);
	for(i = s; i < b->gcapacity; i++) {
		gf[i].flag  = 0;
		gf[i].ucnt  = 0;
		gf[i].type  = TYPE_void;
		gf[i].fn    = FN_NONAME;
		DBG_ASSERT(gf[i].value == NULL);
		KNH_INITv(gf[i].value, KNH_NULL);
		KNH_INITv(gf[i].tkIDX, KNH_NULL);
	}
	return b->gf;
}

static knh_Gamma_t *Gamma_clone(CTX ctx)
{
	size_t i;
	knh_Gamma_t *oldgma = ctx->gma;
	knh_Gamma_t *newgma = new_(Gamma);
	knh_gmafields_t *gf = Gamma_expand(ctx, newgma, DP(oldgma)->gsize + 1);
	knh_gmafields_t *ogf = DP(oldgma)->gf;
	for(i = 0; i < DP(oldgma)->gsize; i++) {
		gf[i].flag = ogf[i].flag;
		gf[i].fn   = ogf[i].fn;
		gf[i].type = ogf[i].type;
		gf[i].ucnt  = ogf[i].ucnt;
		DBG_P("clone.SET %p gf[%d].value=%s(%p)", gf, i, O__(gf[i].value), gf[i].value);
		KNH_SETv(ctx, gf[i].value, ogf[i].value);
		KNH_SETv(ctx, gf[i].tkIDX, ogf[i].tkIDX);
	}
	DP(newgma)->flag    = DP(oldgma)->flag;
	DP(newgma)->cflag   = DP(oldgma)->cflag;
	DP(newgma)->gsize   = DP(oldgma)->gsize;
	DP(newgma)->espidx  = DP(oldgma)->espidx;
	DP(newgma)->ebpidx  = DP(oldgma)->ebpidx;
	DP(newgma)->scridx  = DP(oldgma)->scridx;
	DP(newgma)->psize   = DP(oldgma)->psize;
	KNH_SETv(ctx, DP(newgma)->mtd, DP(oldgma)->mtd);
	KNH_SETv(ctx, DP(newgma)->stmt, DP(oldgma)->stmt);
	KNH_SETv(ctx, newgma->scr, oldgma->scr);
	DP(newgma)->this_cid   = DP(oldgma)->this_cid;
	KNH_SETv(ctx, DP(newgma)->errmsgs, DP(oldgma)->errmsgs);
	return newgma;
}

#define GF_FIELD     (1)
#define GF_UNIQUE    (1<<1)
#define GF_FUNCVAR   (1<<2)
#define GF_USEDCOUNT (1<<3)

static knh_Token_t *Gamma_add(CTX ctx, knh_flag_t flag, knh_Token_t *tkT, knh_Token_t *tkN, knh_Token_t *tkV, knh_flag_t op)
{
	knh_index_t idx = 0;
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	knh_fieldn_t fn = Token_fn(ctx, tkN);
	knh_type_t type = (tkT)->cid;
	if(FLAG_is(op, GF_UNIQUE)) {
		for(idx = 0; idx < DP(ctx->gma)->gsize; idx++) {
			if(gf[idx].fn == fn) {
				if(gf[idx].type == type) return gf[idx].tkIDX;
				return ErrorDifferentlyDeclaredType(ctx, fn, type);
			}
		}
	}
	idx = DP(ctx->gma)->gsize;
	if(!(idx < DP(ctx->gma)->gcapacity)) {
		gf = Gamma_expand(ctx, ctx->gma, /*minimum*/4);
	}
	if(FLAG_is(op, GF_FUNCVAR)) {
		knh_Token_toTYPED(ctx, tkN, TT_FUNCVAR, type, idx);
		STRICT_(Token_setReadOnly(tkN, 1));
		DP(ctx->gma)->ebpidx += 1;
	}
	else if(FLAG_is(op, GF_FIELD)) {
		DP(ctx->gma)->espidx += 1;
	}
	else {
		knh_Token_toTYPED(ctx, tkN, TT_LOCAL, type, idx - DP(ctx->gma)->ebpidx);
		Token_setReadOnly(tkN, FLAG_is(flag, _FREADONLY));
		DP(ctx->gma)->espidx += 1;
	}
	gf[idx].flag  = flag;
	gf[idx].type  = type;
	gf[idx].fn    = fn;
	gf[idx].ucnt  = FLAG_is(op, GF_USEDCOUNT) ? 1 : 0;
	if(tkV != NULL && TT_(tkV) == TT_CONST) {
		//DBG_P("Gamma_add %p gf[%d].value=%s(%p)", gf, idx, O__(gf[idx].value), gf[idx].value);
		KNH_SETv(ctx, gf[idx].value, (tkV)->data);
	}
	KNH_SETv(ctx, gf[idx].tkIDX, tkN);
	DP(ctx->gma)->gsize += 1;
	DBG_ASSERT(DP(ctx->gma)->gsize = DP(ctx->gma)->ebpidx + DP(ctx->gma)->espidx);
	return tkN;
}

static knh_Token_t *Gamma_addLOCAL(CTX ctx, knh_flag_t flag, knh_type_t type, knh_fieldn_t fn, int ucnt)
{
	knh_index_t idx = DP(ctx->gma)->gsize;
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	knh_Token_t *tkIDX = new_TokenTYPED(ctx, TT_LOCAL, type, idx - DP(ctx->gma)->ebpidx);
	if(!(idx < DP(ctx->gma)->gcapacity)) {
		gf = Gamma_expand(ctx, ctx->gma, /*minimum*/4);
	}
	gf[idx].flag  = flag;
	gf[idx].type  = type;
	gf[idx].fn    = fn;
	gf[idx].ucnt  = ucnt;
	KNH_SETv(ctx, gf[idx].tkIDX, tkIDX);
	DP(ctx->gma)->espidx += 1;
	DP(ctx->gma)->gsize += 1;
	DBG_ASSERT(DP(ctx->gma)->gsize = DP(ctx->gma)->ebpidx + DP(ctx->gma)->espidx);
	return tkIDX;
}

static knh_Token_t *Gamma_addFUNCVAR(CTX ctx, knh_flag_t flag, knh_type_t type, knh_Token_t *tkN, int ucnt)
{
	knh_index_t idx = DP(ctx->gma)->ebpidx;
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	if(!(DP(ctx->gma)->gsize < DP(ctx->gma)->gcapacity)) {
		gf = Gamma_expand(ctx, ctx->gma, /*minimum*/4);
	}
	if(idx < DP(ctx->gma)->gsize) {
		knh_index_t n = DP(ctx->gma)->gsize;
		knh_gmafields_t gftmp = gf[n];
		for(; idx < n; n--) {
			gf[n] = gf[n-1];
		}
		//DBG_ASSERT(n == idx);
		gf[idx] = gftmp;
	}
	gf[idx].flag  = flag;
	gf[idx].type  = type;
	if(tkN == NULL) {
		gf[idx].fn    = FN_;
		tkN = new_TokenTYPED(ctx, TT_FUNCVAR, type, idx);
	}
	else {
		gf[idx].fn    = Token_fn(ctx, tkN);
		knh_Token_toTYPED(ctx, tkN, TT_FUNCVAR, type, idx);
	}
	KNH_SETv(ctx, gf[idx].tkIDX, tkN);
	gf[idx].ucnt  = 1;
	DP(ctx->gma)->ebpidx += 1;
	DP(ctx->gma)->gsize  += 1;
	DBG_ASSERT(DP(ctx->gma)->gsize = DP(ctx->gma)->ebpidx + DP(ctx->gma)->espidx);
	return tkN;
}

static knh_Token_t* Gamma_rindexFNQ(CTX ctx, knh_Gamma_t *gma, knh_fieldn_t fnq, int ucnt)
{
	knh_index_t idx;
	knh_fieldn_t fn = FN_UNMASK(fnq);
	knh_gmafields_t *gf = DP(gma)->gf;
	for(idx = DP(gma)->gsize - 1; idx >= 0; idx--) {
		if(gf[idx].fn == fn) {
			gf[idx].ucnt += ucnt;
			gf[idx].tkIDX->type = gf[idx].type;
			return gf[idx].tkIDX;
		}
	}
	return NULL;
}

static knh_fields_t* class_rindexFNQ(CTX ctx, knh_class_t cid, knh_fieldn_t fnq, knh_index_t *n)
{
	knh_fieldn_t fn = FN_UNMASK(fnq);
	const knh_ClassTBL_t *t = ClassTBL(cid);
	knh_index_t idx;
	for(idx = (knh_index_t)t->fsize - 1; idx >= 0 ; idx--) {
		knh_fields_t *cf = t->fields + idx;
		if(cf->fn == fn) {
			*n = idx;
			return cf;
		}
	}
	return NULL;
}

#define IS_SYSVAL(t,v)  (knh_bytes_strcasecmp(t, STEXT(v)) == 0)

#define _FINDLOCAL   (1<<1)
#define _FINDFIELD   (1<<2)
#define _FINDSCRIPT  (1<<3)
#define _USEDCOUNT   (1<<4)
#define _FINDFUNC    (1<<5)
#define _TOERROR     (1<<6)

static knh_class_t class_FuncType(CTX ctx, knh_class_t this_cid, knh_Method_t *mtd);
static knh_Func_t * new_StaticFunc(CTX ctx, knh_class_t bcid, knh_Method_t *mtd);

static knh_Token_t *TNAME_typing(CTX ctx, knh_Token_t *tk, knh_type_t reqt, knh_flag_t op)
{
	knh_fieldn_t fnq = Token_fnq(ctx, tk);
	DBG_ASSERT(fnq != FN_NONAME);
	if(FN_isU1(fnq) || FN_isSUPER(fnq)) goto L_FIELD;  /* _name */
	if(FN_isU2(fnq)) {
		if(DP(ctx->gma)->this_cid == O_cid(K_GMASCR)) goto L_FIELD;
		goto L_SCRIPT; /* __name */
	}
	if(FLAG_is(op, _FINDLOCAL)){
		knh_Token_t *tkIDX = Gamma_rindexFNQ(ctx, ctx->gma, fnq, 1);
		if(tkIDX != NULL) {
			knh_Token_toTYPED(ctx, tk, tkIDX->tt, tkIDX->type, (tkIDX)->index);
			Token_setReadOnly(tk, Token_isReadOnly(tkIDX));
			return tk;
		}
		if(DP(ctx->gma)->parentNULL != NULL) {
			tkIDX = Gamma_rindexFNQ(ctx, DP(ctx->gma)->parentNULL, fnq, 1);
			if(tkIDX != NULL) {
				knh_Token_toTYPED(ctx, tk, TT_XLOCAL, tkIDX->type, (tkIDX)->index);
				Token_setReadOnly(tk, Token_isReadOnly(tkIDX));
				Gamma_foundXLOCAL(ctx->gma, 1);
				return tk;
			}
		}
	}
	L_FIELD:;
	if(FLAG_is(op, _FINDFIELD)) {
		knh_index_t idx = -1;
		knh_fields_t *cf = class_rindexFNQ(ctx, DP(ctx->gma)->this_cid, fnq, &idx);
		if(cf != NULL) {
			knh_type_t type = Gamma_type(ctx, cf->type);
			knh_Token_toTYPED(ctx, tk, TT_FIELD, type, idx);
			Token_setReadOnly(tk, FLAG_is(cf->flag, _FREADONLY));
			Gamma_foundFIELD(ctx->gma, 1);
			return tk;
		}
	}
	L_SCRIPT:;
	if(FLAG_is(op, _FINDSCRIPT)) {
		knh_class_t scrcid = O_cid(K_GMASCR);
		if(DP(ctx->gma)->this_cid != scrcid) {
			knh_index_t idx = -1;
			knh_fields_t *cf = class_rindexFNQ(ctx, scrcid, fnq, &idx);
			if(cf != NULL) {
				knh_type_t type = Gamma_type(ctx, cf->type);
				knh_Token_toTYPED(ctx, tk, TT_SCRFIELD, type, idx);
				if(DP(ctx->gma)->scridx == -1) {
					knh_Token_t *tkSCR = Gamma_addFUNCVAR(ctx, 0, scrcid, new_(Token), 1);
					DP(ctx->gma)->scridx = (tkSCR)->index;
				}
				return tk;
			}
		}
	}
	if(FLAG_is(op, _FINDFUNC)) { // TODO_AC
		knh_Method_t *mtd = NULL;
		knh_NameSpace_t *ns = K_GMANS;
		knh_class_t this_cid = DP(ctx->gma)->this_cid;
		knh_class_t mtd_cid = knh_NameSpace_getFuncClass(ctx, ns, fnq);
		if(mtd_cid != CLASS_unknown) {
			mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, fnq);
			if(!Method_isStatic(mtd)) mtd = NULL;
		}
		if(mtd == NULL) {
			mtd = knh_NameSpace_getMethodNULL(ctx, this_cid, fnq);
			if(mtd != NULL && !Method_isStatic(mtd)) {
				mtd = NULL;
			}
		}
		if(mtd == NULL) {
			mtd_cid = O_cid(K_GMASCR);
			mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, fnq);
			if(mtd != NULL && !Method_isStatic(mtd)) {
				mtd = NULL;
			}
		}
		if(mtd != NULL) {
			this_cid = class_FuncType(ctx, this_cid, mtd);
			return Token_setCONST(ctx, tk, new_StaticFunc(ctx, this_cid, mtd));
		}
	}
	if(FLAG_is(op, _TOERROR)) {
		knh_bytes_t t = TK_tobytes(tk);
		if(IS_SYSVAL(t, "__line__")) {
			return Token_setCONST(ctx, tk, new_Int(ctx, CLASS_Int, ULINE_line(tk->uline)));
		}
		else if(IS_SYSVAL(t, "__file__")) {
			return Token_setCONST(ctx, tk, knh_getURN(ctx, ULINE_uri(tk->uline)));
		}
		else if(IS_SYSVAL(t, "__method__") || IS_SYSVAL(t, "__func__")) {
			return Token_setCONST(ctx, tk, DP(ctx->gma)->mtd);
		}
		else if(IS_SYSVAL(t, "__namespace__") || IS_SYSVAL(t, "__ns__")) {
			knh_NameSpace_t *ns = K_GMANS;
			return Token_setCONST(ctx, tk, ns);
		}
		else {
			return ERROR_Undefined(ctx, "variable", CLASS_unknown, tk);
		}
	}
	return NULL;
}

/* UNAME */

static knh_Token_t* Token_toSYSVAL(CTX ctx, knh_Token_t *tk)
{
	knh_bytes_t t = TK_tobytes(tk);
	if(IS_SYSVAL(t, "CTX")) {
		TK_typing(tk, TT_SYSVAL, TYPE_Context, K_SYSVAL_CTX);
	}
	else if(IS_SYSVAL(t, "IN")) {
		TK_typing(tk, TT_SYSVAL, TYPE_InputStream, K_SYSVAL_CTXIN);
	}
	else if(IS_SYSVAL(t, "OUT")) {
		if(Gamma_isCompilingFmt(ctx)) {
			TK_typing(tk, TT_LOCAL, TYPE_OutputStream, 0);
		}
		else {
			TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, K_SYSVAL_CTXOUT);
		}
	}
	else if(IS_SYSVAL(t, "ERR")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, K_SYSVAL_CTXERR);
	}
	else if(IS_SYSVAL(t, "STDIN")) {
		TK_typing(tk, TT_SYSVAL, TYPE_InputStream, K_SYSVAL_STDIN);
	}
	else if(IS_SYSVAL(t, "STDOUT")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, K_SYSVAL_STDOUT);
	}
	else if(IS_SYSVAL(t, "STDERR")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, K_SYSVAL_STDERR);
	}
	else if(IS_SYSVAL(t, "OS")) {
		TK_typing(tk, TT_SYSVAL, TYPE_System, K_SYSVAL_OS);
	}
//	else if(IS_SYSVAL(t, "__line__")) {
//		Token_setCONST(ctx, tk, new_Int(ctx, CLASS_Int, ULINE_line(tk->uline)));
//	}
//	else if(IS_SYSVAL(t, "__file__")) {
//		Token_setCONST(ctx, tk, knh_getURN(ctx, ULINE_uri(tk->uline)));
//	}
//	else if(IS_SYSVAL(t, "__method__") || IS_SYSVAL(t, "__function__")) {
//		Token_setCONST(ctx, tk, knh_Object_getkey(ctx, UPCAST(DP(ctx->gma)->mtd)));
//	}
//	else if(IS_SYSVAL(t, "__namespace__") || IS_SYSVAL(t, "__ns__")) {
//		knh_NameSpace_t *ns = K_GMANS;
//		Token_setCONST(ctx, tk, ns);
//	}
	else if(IS_SYSVAL(t, "EOL")) {
		Token_setCONST(ctx, tk, TS_EOL);
	}
	else if(IS_SYSVAL(t, "BEGIN")) {
		Token_setCONST(ctx, tk, TS_BEGIN);
	}
	else if(IS_SYSVAL(t, "END")) {
		Token_setCONST(ctx, tk, TS_END);
	}
	else {
		return ERROR_Undefined(ctx, "name", CLASS_unknown, tk);
	}
	return tk;
}

static knh_Token_t* TUNAME_typing(CTX ctx, knh_Token_t *tk)
{
	knh_class_t cid = knh_Token_cid(ctx, tk, CLASS_unknown);
	if(cid != CLASS_unknown) {
		return knh_Token_toTYPED(ctx, tk, TT_CID, TYPE_Class, cid);
	}
	else {
		knh_NameSpace_t *ns = K_GMANS;
		Object *value = knh_NameSpace_getConstNULL(ctx, ns, TK_tobytes(tk));
		if(value != NULL) {
			return Token_setCONST(ctx, tk, value);
		}
	}
	return Token_toSYSVAL(ctx, tk);
}

static knh_Token_t* TPROPN_typing(CTX ctx, knh_Token_t *tk, knh_type_t reqt)
{
	knh_bytes_t t = S_tobytes((tk)->text);
	if(B_endsWith(t, "*")) { /* name.* */
		knh_Stmt_t *stmt =
			new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_listProperties), new_TokenCONST(ctx, ctx->sys), tk, NULL);
		Token_toCONST(ctx, tk);
		return CALL_typing(ctx, stmt, reqt);
	}
	else {
		size_t i;
		knh_Object_t *v = (knh_Object_t*)knh_getPropertyNULL(ctx, t);
		if(v != NULL) {
			SP(tk)->type = O_cid(v);
		}
		else {
			if(IS_Tvany(reqt)) {
				reqt = TYPE_String;
				InfoType(ctx, "$", t, TYPE_String);
			}
			tk->type = reqt;
			v = KNH_NULVAL(CLASS_t(reqt));
			knh_setProperty(ctx, tk->text, v);
		}
		for(i = 0; i < t.len; i++) {
			if(islower(t.buf[i])) return TM(tk);
		}
		return Token_setCONST(ctx, tk, v);
	}
}

/* ------------------------------------------------------------------------ */
/* [NUM] */

#ifdef K_USING_SEMANTICS
static knh_class_t knh_Token_tagcNUM(CTX ctx, knh_Token_t *tk, knh_class_t reqc, knh_NameSpace_t *ns)
{
	knh_bytes_t t = TK_tobytes(ctx, tk), tag = STEXT("");
	size_t i = 1;
	int ishex = 0;
	if(t.utext[0] == '0' && (t.utext[1] == 'x' || t.utext[1] == 'b')) {
		i = 2;
		ishex = 1;
		knh_style(ctx, 1);
	}
	for(; i < t.len; i++) {
		if(isdigit(t.utext[i]) || t.utext[i] == '_' || t.utext[i] == '.') continue;
		if(t.utext[i] == '[') {
			int loc;
			tag.buf = t.buf + i + 1;
			tag.len = t.len - (i + 1);
			loc = knh_bytes_index(tag, ']');
			if(loc > 0) {
				tag = knh_bytes_first(tag, loc);
			}
			break;
		}
		else if(t.utext[i] == ':') {
			tag.buf = t.buf + i + 1;
			tag.len = t.len - (i + 1);
			break;
		}
		else {
			if((t.utext[i] == 'E' || t.utext[i] == 'e')) {
				if(isdigit(t.utext[i+1]) || t.utext[i+1] == '-' || t.utext[i+1] == '+') {
					i++;
					continue;
				}
			}
			tag.buf = t.buf + i;
			tag.len = t.len - (i);
			break;
		}
	}
	if(tag.len == 0 || ishex) {
		return reqc;
	}
	else {
		knh_class_t tagc = knh_NameSpace_tagcid(ctx, ns, reqc, tag);
		if(tagc == CLASS_unknown) {
			knh_Gamma_perror(ctx, tk, KC_DWARN, _("unknown class tag: %L"), tk);
			return reqc;
		}
		knh_style(ctx, 1);
		return tagc;
	}
}
#endif

/* ------------------------------------------------------------------------ */

static knh_class_t bytes_guessNUMcid(CTX ctx, knh_bytes_t t)
{
	size_t i;
	if(t.utext[0] == 0 && (t.utext[1] == 'x' || t.utext[1]=='b')) {
		return CLASS_Int;
	}
	for(i = 1; i < t.len; i++) {
		if(t.utext[i] == '_') {
#ifdef CLASS_Decimal
			return CLASS_Decimal;
#endif
		}
		else if(t.utext[i] == '.') {
			return CLASS_Float;
		}
		if(!isdigit(t.utext[i])) break;
	}
	return CLASS_Int;
}

static knh_Token_t* NUM_typing(CTX ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t t = TK_tobytes(tk);
	knh_class_t breqc = knh_class_bcid(reqt);
	if(reqt == CLASS_Boolean) {
		if(t.utext[0] == '0') {
			WARN_MuchBetter(ctx, "false");
			return Token_setCONST(ctx, tk, KNH_FALSE);
		}
		else {
			WARN_MuchBetter(ctx, "true");
			return Token_setCONST(ctx, tk, KNH_TRUE);
		}
	}
	if(breqc == CLASS_Int || breqc == CLASS_Float) {
	}
	else if(IS_Tvany(reqt) || breqc == CLASS_Object || breqc == CLASS_Number) {
		breqc = bytes_guessNUMcid(ctx, t);
		reqt = breqc;
	}
	else {
		return ErrorNumericType(ctx, reqt);
	}
	if(breqc == CLASS_Float) {
		knh_float_t n = K_FLOAT_ZERO;
		if(!knh_bytes_parsefloat(t, &n)) {
			WarningOverflow(ctx, "float", t);
		}
#if defined(K_USING_SEMANTICS)
		knh_class_t tagc = knh_Token_tagcNUM(ctx, tk, reqc, ns);
		knh_Semantics_t *u = knh_getSemantics(ctx, tagc);
		if(!DP(u)->ffchk(u, n)) {
			knh_Gamma_perror(ctx, KC_ERRATA, _("%C: out of range: %B ==> %O"), tagc, t, DP(u)->fvalue);
			return Token_setCONST(ctx, tk, UPCAST(DP(u)->fvalue));
		}
		else {
			return Token_setCONST(ctx, tk, UPCAST(new_FloatX(ctx, tagc, n)));
		}
#else
		return Token_setCONST(ctx, tk, CONSTPOOL(new_Float(ctx, CLASS_Float, n)));
#endif/*K_USING_SEMANTICS*/
	}
	else { /* if(req_bcid == CLASS_Int) */
		knh_int_t n = 0;
		if(!knh_bytes_parseint(t, &n)) {
			WarningOverflow(ctx, "integer", t);
		}
#if defined(K_USING_SEMANTICS)
		knh_class_t tagc = knh_Token_tagcNUM(ctx, tk, reqc, ns);
		knh_Semantics_t *u = knh_getSemantics(ctx, tagc);
		if(!DP(u)->fichk(u, n)) {
			knh_Gamma_perror(ctx, KC_ERRATA, _("%C: out of range: %B ==> %O"), tagc, t, DP(u)->ivalue);
			return Token_setCONST(ctx, tk, UPCAST(DP(u)->ivalue));
		}
		else {
			return Token_setCONST(ctx, tk, UPCAST(new_IntX(ctx, tagc, n)));
		}
#else
		return Token_setCONST(ctx, tk, CONSTPOOL(new_Int(ctx, CLASS_Int, n)));
#endif/*K_USING_SEMANTICS*/
	}
}

static knh_Token_t* TSTR_typing(CTX ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t t = TK_tobytes(tk);
	if(CLASS_t(reqt) != CLASS_String && knh_bytes_mlen(t) == 1) {
		/* 'A' ==> int if not String */
		knh_bytes_t sub = knh_bytes_mofflen(t, 0, 1);
		return Token_setCONST(ctx, tk, new_Int(ctx, CLASS_Int, knh_uchar_toucs4(&sub.utext[0])));
	}
	return Token_toCONST(ctx, tk);
}

static knh_bool_t bytes_isLONGFMT(knh_bytes_t t)
{
	size_t i = 0, size = t.len - 1;
	if(t.len < 1) return 0;
	L_AGAIN:;
	for(;i < size; i++) {
		if(t.utext[i] == '%') {
			int ch = t.utext[i+1];
			i++;
			if(isdigit(ch) || ch == ' ' || ch == '.' || ch == '+' || ch == '-' || ch == '#') {
				goto L_CFMT;
			}
			if(isalpha(ch)) goto L_KFMT;
		}
		if(t.utext[i] == '$' && isalpha(t.utext[i+1])) return 1;
	}
	return 0;
	L_CFMT:;
	for(; i < size; i++) {
		int ch = t.utext[i];
		if(isalpha(ch) || t.utext[i+1] == '{') return 1;
		if(!isdigit(ch) && ch != '.') goto L_AGAIN;
	}
	return 0;
	L_KFMT:;
	for(; i < size; i++) {
		int ch = t.utext[i];
		if(ch == '{') return 1;
		if(!isalnum(ch) && ch != ':') goto L_AGAIN;
	}
	return 0;
}

static knh_class_t knh_bytes_CFMT(knh_bytes_t t)
{
	if(t.utext[0] == '%' && (isdigit(t.utext[1]) || t.utext[1] == ' ' || t.utext[1] == '.')) {
		int ch = t.utext[t.len - 1];
		switch(ch) {
		case 'd': case 'u': case 'x': case 'c': return CLASS_Int;
		case 'e': case 'f': return CLASS_Float;
		case 's': return CLASS_String;
		}
	}
	return CLASS_unknown;
}

static knh_methodn_t bytes_parsemn(CTX ctx, knh_bytes_t t)
{
	if(t.utext[0] == '%' && t.utext[1] != '%') {
		size_t i;
		for(i = 1; i < t.len; i++) {
			int ch = t.utext[i];
			if(isalnum(ch) || ch == ':' || ch == ' ') continue;
			if(ch == '.' && !isalpha(t.utext[i-1])) continue;
			break;
		}
		if(i == t.len) {
			return knh_getmn(ctx, t, MN_NEWID);
		}
	}
	return MN__s;
}

static void Token_toCALLMTD(CTX ctx, knh_Token_t *tk, knh_methodn_t mn, knh_Method_t *mtd)
{
	TT_(tk) = TT_MN;
	(tk)->mn = mn;
	SP(tk)->type = TYPE_Method;
	if(mtd != NULL) {
		KNH_SETv(ctx, (tk)->data, mtd);
	}
}

static knh_Token_t *EXPR_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt);
static knh_Token_t* CALLPARAMs_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, knh_class_t mtd_cid, knh_Method_t *mtd);

static knh_Token_t *W1_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkFMT = tkNN(stmt, 0);
	knh_bytes_t fmt = S_tobytes((tkFMT)->text);
	knh_class_t cid = knh_bytes_CFMT(fmt);
	DBG_ASSERT(DP(stmt)->size == 3);
	if(cid != CLASS_unknown) {  // "%4d"(1), not "%d"(1)
		knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, cid, MN_format);
		DBG_ASSERT(mtd != NULL);
		Token_setCONST(ctx, tkNN(stmt, 1), (tkFMT)->data);
		TYPING(ctx, stmt, 2, cid, _ICAST);
		knh_Stmt_swap(ctx, stmt, 1, 2);
		Token_toCALLMTD(ctx, tkFMT, MN_format, mtd);
		STT_(stmt) = STT_CALL;
		return CALLPARAMs_typing(ctx, stmt, TYPE_String, cid, mtd);
	}
	if(DP(stmt)->size > 3) {
		WarningTooManyParameters(ctx);
		knh_Stmt_trimToSize(ctx, stmt, 3);
	}
	if(DP(stmt)->size == 3) { //"%bit" (a)
		STT_(stmt) = STT_W1;
		TYPING(ctx, stmt, 2, TYPE_var, 0);
		knh_methodn_t mn = bytes_parsemn(ctx, fmt);
		knh_Method_t *mtdf = knh_NameSpace_getFmtNULL(ctx, K_GMANS, Tn_cid(stmt, 2), mn);
		if(mtdf != NULL) {
			KNH_SETv(ctx, (tkFMT)->data, mtdf);
		}
		else {
			WarningUndefinedOfClass(ctx, "formatter", Tn_cid(stmt, 2), "", S_tochar((tkFMT)->text));
		}
		DBG_ASSERT(TT_(tmNN(stmt, 1)) == TT_ASIS);
	}
	return Stmt_typed(ctx, stmt, TYPE_String);
}

static knh_Stmt_t* new_StmtW1(CTX ctx, knh_Token_t *tkFMT, knh_Token_t *tkW, knh_Token_t *tkEXPR)
{
	if(tkFMT == NULL) tkFMT = new_TokenCONST(ctx, TS_EMPTY);
	knh_Stmt_t *stmtW = new_Stmt2(ctx, STT_W1, tkFMT, tkW, tkEXPR, NULL);
	knh_Token_t *tkRES = W1_typing(ctx, stmtW);
	if(TT_(tkRES) == STT_CALL) {
		return new_StmtW1(ctx, NULL, tkW, tkRES);  //"%d{expr}"
	}
	else if(TT_(tkRES) == TT_ERR) {
		KNH_SETv(ctx, stmtNN(stmtW, 0), tkRES);
		knh_Stmt_done(ctx, stmtW);
	}
	//DBG_P("STT=%s", TT__(stmtW->stt));
	return stmtW;
}

#define APPEND_TAIL(stmtHEAD, stmtTAIL, stmtW) {\
		if(stmtHEAD == NULL) {stmtHEAD = stmtW;}\
		else {KNH_INITv(DP(stmtTAIL)->nextNULL, stmtW);}\
		stmtTAIL = stmtW;\
	}\

static knh_Token_t *FMTCALL_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkFMT = tkNN(stmt, 0);
	knh_bytes_t t = S_tobytes((tkFMT)->text);
	knh_Stmt_t *stmtHEAD = NULL, *stmtTAIL = NULL, *stmtW;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_uline_t uline = tkFMT->uline;
	size_t i = 0, s = 0;
	while(i < t.len) {
		for(;i < t.len; i++) {
			int ch = t.utext[i];
			if(ch == '\n') uline++;
			if(ch == '%') {
				i++;
				if(t.text[i] == '%') {
					knh_Bytes_putc(ctx, cwb->ba, '%');
					continue;
				}
				break;
			}
			if(ch == '$' && t.text[i+1] == '$') {
				i+=2;
				break;
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		if(knh_cwb_size(cwb) > 0) {
			stmtW = new_StmtW1(ctx, NULL, tkNN(stmt,1), new_TokenCONST(ctx, knh_cwb_newString(ctx, cwb)));
			APPEND_TAIL(stmtHEAD, stmtTAIL, stmtW);
		}
		if(!(i < t.len)) break;
		s = i - 1;
		DBG_P("FMT t[%d]=%c", s, t.utext[s]);
		if(t.utext[s] == '$') {
			for(;i < t.len; i++) {
				int ch = t.utext[i];
				if(!isalnum(ch)) break;
			}
			DBG_P("nm last t[%d]=%c", i-1, t.utext[-1]);
			knh_bytes_t name = {{t.text + s + 1}, i - (s + 1)};
			knh_Token_t *tkN = new_(Token);
			tkN->tt = isupper(t.text[0]) ? TT_UNAME : TT_NAME;
			tkN->uline = uline;
			KNH_SETv(ctx, tkN->data, new_S(ctx, name));
			stmtW = new_StmtW1(ctx, new_TokenCONST(ctx, new_S(ctx, STEXT("%s"))), tkNN(stmt, 1), tkN);
			APPEND_TAIL(stmtHEAD, stmtTAIL, stmtW);
		}
		else if(t.utext[s] == '%') {
			for(;i < t.len; i++) {
				int ch = t.utext[i];
				DBG_P("t[%d]=%c", i, ch);
				if(ch == '{') {
					knh_bytes_t mt = {{t.text + s}, i - s};
					knh_bytes_t expr = {{t.text + (i+1)}, t.len - (i+1)};
					knh_index_t loc = knh_bytes_index(expr, '}');
					if(loc == -1) {
						WarningIllegalFormatting(ctx, expr.text - 1);
						DBG_P("unclosed }");
						goto L_ERROR;
					}
					if(expr.text[0] == '#' && isdigit(expr.text[1])) {
						long n = strtol(expr.text+1, NULL, 10);
						if(n < 0 || DP(stmt)->size - 2 <= n) {
							WarningIllegalFormatting(ctx, mt.text);
							DBG_P("out of number: %d !< %d", n, DP(stmt)->size - 2);
							goto L_ERROR;
						}
						TYPING(ctx, stmt, 2 + n, TYPE_var, 0);
						stmtW = new_StmtW1(ctx, new_TokenCONST(ctx, new_S(ctx, mt)), tkNN(stmt, 1), new_TokenTYPED(ctx, TT_NUM, Tn_type(stmt, n+2), n));
						APPEND_TAIL(stmtHEAD, stmtTAIL, stmtW);
					}
					else {
						stmtW = new_StmtW1(ctx, new_TokenCONST(ctx, new_S(ctx, mt)),
								tkNN(stmt, 1), (knh_Token_t*)knh_bytes_parseStmt(ctx, expr, uline));
						APPEND_TAIL(stmtHEAD, stmtTAIL, stmtW);
					}
					i += (loc + 2);
					break;
				}
				else if(!isalnum(ch) && ch != '.') {
					WarningIllegalFormatting(ctx, t.text + i);
					DBG_P("illegal char: %c", ch);
					goto L_ERROR;
				}
			}
		}
	}/*while*/
	STT_(stmt) = STT_FMTCALL;
	for(i = 2; i < DP(stmt)->size; i++) {
		if(tmNN(stmt, i)->type == TYPE_var) {  // unused
			KNH_SETv(ctx, tkNN(stmt, i), new_(Token)); // ASIS
		}
	}
	DBG_ASSERT(stmtHEAD != NULL);
	KNH_SETv(ctx, stmtNN(stmt, 0), stmtHEAD);
	knh_cwb_close(cwb);
	return Stmt_typed(ctx, stmt, TYPE_String);

	L_ERROR:
	knh_cwb_close(cwb);
	return new_TokenCONST(ctx, TS_EMPTY);
}

static knh_Token_t *FMTOP_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkFMT = tkNN(stmt, 0);
	knh_bytes_t t = S_tobytes((tkFMT)->text);
	if(bytes_isLONGFMT(t)) {
		return FMTCALL_typing(ctx, stmt);
	}
	else if(t.text[0] == '%' && t.text[1] != '%') {
		return W1_typing(ctx, stmt);
	}
	else {
		return Token_toCONST(ctx, tkFMT);
	}
	return Stmt_typed(ctx, stmt, TYPE_String);
}

static knh_Token_t* ESTR_typing(CTX ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t t = S_tobytes((tk)->text);
	if(bytes_isLONGFMT(t)) {
		knh_Stmt_t *stmt = new_Stmt2(ctx, STT_FUNCCALL, tk, new_(Token)/*ASIS*/, NULL);
		return FMTOP_typing(ctx, stmt, reqt);
	}
	else {
		return Token_toCONST(ctx, tk);
	}
}

static knh_Token_t* PATH_typing(CTX ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_NameSpace_t *ns = K_GMANS;
	knh_bytes_t path = S_tobytes((tk)->text);
	const knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, ns, path);
	if(dspi == NULL) {
		return ERROR_Undefined(ctx, "scheme", CLASS_unknown, tk);
	}
	if(TT_(tk) == TT_TSCHEME) {
		return Token_toCONST(ctx, tk);
	}
	if(reqt == TYPE_dyn || reqt == TYPE_var) reqt = dspi->cid;
	DBG_P("reqt=%s", TYPE__(reqt));
	if(reqt == TYPE_Boolean || reqt == TYPE_void) {
		knh_Object_t *tf = dspi->exists(ctx, ns, path, dspi->thunk) ? KNH_TRUE : KNH_FALSE;
		return Token_setCONST(ctx, tk, tf);
	}
	else if(reqt == TYPE_String) {
		return Token_toCONST(ctx, tk);
	}
	else if(!dspi->hasType(ctx, CLASS_t(reqt), dspi->thunk)) {
		return TERROR_Token(ctx, tk, dspi->cid, reqt);
	}
	else {
		knh_Object_t *o = dspi->newObjectNULL(ctx, ns, reqt, (tk)->text, dspi->thunk);
		if(o == NULL) o = KNH_NULVAL(reqt);
		return Token_setCONST(ctx, tk, o);
	}
}

static knh_Token_t* TSCHEME_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkSCM = tkNN(stmt, 2);
	knh_bytes_t path = S_tobytes((tkSCM)->text);
	knh_NameSpace_t *ns = K_GMANS;
	DBG_ASSERT(TT_(tkSCM) == TT_TSCHEME);
	TYPING(ctx, stmt, 1, TYPE_String, 0);
	const knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, ns, path);
	if(dspi == NULL) {
		return ERROR_Undefined(ctx, "scheme", CLASS_unknown, tkSCM);
	}
	if(reqt == TYPE_dyn || reqt == TYPE_var) reqt = dspi->cid;
	if(reqt == TYPE_Boolean || reqt == TYPE_void || reqt == TYPE_String ||
			dspi->hasType(ctx, CLASS_t(reqt), dspi->thunk)) {
		DBG_ASSERT(DP(stmt)->size == 3);
		STT_(stmt) = STT_CALL;
		Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_path, knh_NameSpace_getMethodNULL(ctx, CLASS_String, MN_path));
		TT_(tkSCM) = TT_CONST;
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, K_GMANS));
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, new_Type(ctx, reqt)));
		return Stmt_typed(ctx, stmt, reqt);
	}
	else {
		return TERROR_Token(ctx, tkSCM, dspi->cid, reqt);
	}
}

/* ------------------------------------------------------------------------ */
/* [Token] */

static knh_Token_t *Token_typing(CTX ctx, knh_Token_t *tk, knh_type_t reqt)
{
	if(Token_isTyped(tk)) return tk;
	if(reqt == TYPE_TEXT && IS_String(tk->text)) {
		return Token_toCONST(ctx, tk);
	}
	if(reqt == TYPE_var || reqt == TYPE_void) reqt = TYPE_dyn;
	switch(TT_(tk)) {
	case TT_ASIS:  tk->type = reqt; return tk;
	case TT_NULL:  return knh_Token_toTYPED(ctx, tk, TT_NULL/*DEFVAL*/, reqt, CLASS_t(reqt));
	case TT_TRUE:  return Token_setCONST(ctx, tk, KNH_TRUE);
	case TT_FALSE: return Token_setCONST(ctx, tk, KNH_FALSE);
	case TT_NAME:  return TNAME_typing(ctx, tk, reqt, _FINDLOCAL|_FINDFIELD|_FINDSCRIPT|_FINDFUNC|_TOERROR);
	case TT_UNAME: return TUNAME_typing(ctx, tk);
	case TT_TYPEOF:
	case TT_PTYPE:  return TTYPE_typing(ctx, tk, reqt);
	case TT_PROPN: return TPROPN_typing(ctx, tk, reqt);
	case TT_REGEX: case TT_STR: return Token_toCONST(ctx, tk);
	case TT_TSTR: return TSTR_typing(ctx, tk, reqt);
	case TT_ESTR: return ESTR_typing(ctx, tk, reqt);
	case TT_NUM: return NUM_typing(ctx, tk, reqt);
	case TT_URN: case TT_TSCHEME: return PATH_typing(ctx, tk, reqt);
	case TT_ERR: return tk;
	case TT_MN:
	default:
		return ERROR_Token(ctx, tk K_TRACEPOINT);
	}
}

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/* STMT */

#define _VFINDTHIS          (1<<1)
#define _VFINDSCRIPT        (1<<2)

static knh_type_t Gamma_lookupVariableType(CTX ctx, knh_Token_t *tkN, knh_flag_t op)
{
	//if(TK_tobytes(tkN).len > 0) {
		knh_fieldn_t fn = Token_fn(ctx, tkN);
		if(FLAG_is(op, _VFINDTHIS)) {
			knh_index_t idx;
			knh_fields_t *cf = class_rindexFNQ(ctx, DP(ctx->gma)->this_cid, fn, &idx);
			if(cf != NULL) return cf->type;
			knh_Array_t *a = ClassTBL(DP(ctx->gma)->this_cid)->methods;
			long i;
			for(i = knh_Array_size(a) - 1; i >=0; i--) {
				knh_Method_t *mtd = a->methods[i];
				if(Method_isKonohaCode(mtd)) {
					size_t j;
					knh_ParamArray_t *pa = DP(mtd)->mp;
					for(j = 0; j < pa->psize; j++) {
						knh_param_t *p = knh_ParamArray_get(pa, j);
						if(p->fn == fn) return p->type;
					}
				}
			}
		}
		if(FLAG_is(op, _VFINDSCRIPT) && DP(ctx->gma)->this_cid != O_cid(K_GMASCR)) {
			knh_index_t idx;
			knh_fields_t *cf = class_rindexFNQ(ctx, DP(ctx->gma)->this_cid, fn, &idx);
			if(cf != NULL) return cf->type;
		}
	//}
	return TYPE_var;  // Not found
}

static knh_flag_t DECL_flag(CTX ctx, knh_Stmt_t *o, knh_flag_t flag)
{
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		if(knh_DictMap_getNULL(ctx, DP(o)->metaDictCaseMap, STEXT("Private"))) {
			flag = 0;
		}
		ADD_FLAG(flag, "Getter", _FGETTER);
		ADD_FLAG(flag, "Setter", _FSETTER);
		ADD_FLAG(flag, "Volatile", FLAG_Field_Volatile);
		ADD_FLAG(flag, "ReadOnly", _FREADONLY);
		ADD_FLAG(flag, "Key", _FKEY);
		if((flag & _FREADONLY) == _FREADONLY) {
			flag = ((~_FSETTER) & flag);
		}
	}
	return flag;
}

static knh_Token_t *DECL_typing(CTX ctx, knh_Stmt_t *stmt) /* LOCAL*/
{
	knh_flag_t flag  = DECL_flag(ctx, stmt, 0);
	knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmt, 0), TYPE_var);
	knh_Token_t *tkN = tkNN(stmt, 1);
	if((tkT)->cid == TYPE_var) {
		if(TT_(tkNN(stmt, 2)) == TT_ASIS) {
			return ErrorStaticType(ctx, TK__(tkN));
		}
		else {
			knh_type_t iftype;
			TYPING(ctx, stmt, 2, TYPE_dyn, _NOTCHECK);
			iftype = Tn_type(stmt, 2);
			knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, iftype);
			InfoType(ctx, "", TK_tobytes(tkN), iftype);
		}
	}
	else {
		if(TT_(tkNN(stmt, 2)) == TT_ASIS) {
			knh_Token_toTYPED(ctx, tkNN(stmt, 2), TT_NULL, tkT->type, tkT->type);
		}
		else {
			TYPING(ctx, stmt, 2, (tkT)->cid, 0);
		}
	}
	{
		knh_Token_t *tkRES = Gamma_add(ctx, flag, tkT, tkN, NULL, 0);
		if(TT_(tkRES) != TT_ERR) {
			Stmt_toSTT(stmt, STT_LET);
			return Stmt_typed(ctx, stmt, TYPE_void);
		}
		return tkRES;
	}
}

void knh_class_addField(CTX ctx, knh_class_t bcid, knh_flag_t flag, knh_type_t type, knh_fieldn_t fn);
static void ObjectField_add(CTX ctx, knh_ObjectField_t *of, size_t i, knh_type_t type, knh_Object_t *v);
#define k_goodbsize(n)   (k_goodsize((n) * sizeof(void*)) / sizeof(void*))

static knh_index_t Script_addField(CTX ctx, knh_Script_t *scr, knh_flag_t flag, knh_type_t type, knh_fieldn_t fn)
{
	const knh_ClassTBL_t *t = O_cTBL(scr);
	size_t fsize = t->fsize, fcapacity = t->fcapacity;
	DBG_ASSERT(scr == (knh_Script_t*)t->defnull);
	knh_class_addField(ctx, t->cid, flag, type, fn);
	if(fcapacity < t->fcapacity) {
		DBG_P("fsize=%d=>%d, fcapacity=%d=>%d", fsize, t->fsize, fcapacity, t->fcapacity);
		knh_ObjectField_expand(ctx, (knh_ObjectField_t*)t->defnull, fcapacity, t->fcapacity);
	}
	ObjectField_add(ctx, (knh_ObjectField_t*)t->defnull, fsize, type, KNH_NULL);
	return (knh_index_t)fsize;
}

static knh_Token_t *DECLSCRIPT_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_flag_t flag  = DECL_flag(ctx, stmt, _FGETTER | _FSETTER);
	knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmt, 0), TYPE_var);
	knh_Token_t *tkN = tkNN(stmt, 1);
	if((tkT)->cid == TYPE_var) {
		if(TT_(tkNN(stmt, 2)) == TT_ASIS) {
			return ErrorStaticType(ctx, TK__(tkN));
		}
		else {
			knh_type_t iftype;
			TYPING(ctx, stmt, 2, TYPE_var, _NOTCHECK);
			iftype = Tn_type(stmt, 2);
			knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, iftype);
			InfoType(ctx, "", TK_tobytes(tkN), iftype);
		}
	}
	else {
		TYPING(ctx, stmt, 2, (tkT)->cid, 0);
	}
	{
		knh_Script_t *scr = K_GMASCR;
		knh_fieldn_t fn = Token_fn(ctx, tkN);
		const knh_ClassTBL_t *t = O_cTBL(scr);
		knh_index_t idx = -1;
		for(idx = (knh_index_t)t->fsize - 1; idx >= 0 ; idx--) {
			if(t->fields[idx].fn == fn) {
				if(t->fields[idx].type != tkT->cid) {
					return ErrorDifferentlyDeclaredType(ctx, fn, tkT->cid);
				}
				break;
			}
		}
		if(idx == -1) {
			idx = Script_addField(ctx, scr, flag, (tkT)->cid, fn);
		}
		knh_Token_toTYPED(ctx, tkN, TT_FIELD, (tkT)->cid, idx);
		if(TT_(tkNN(stmt, 2)) == TT_ASIS) {
			knh_Token_toTYPED(ctx, tkNN(stmt, 2), TT_NULL, (tkT)->cid, (tkT)->cid);
		}
		Stmt_toSTT(stmt, STT_LET);
		Stmt_typed(ctx, stmt, TYPE_void);
		return TM(stmt);
	}
}

#define MN_isNEW(mn)  MN_isNEW_(ctx, mn)

static int MN_isNEW_(CTX ctx, knh_methodn_t mn)
{
	if(mn == MN_new) return 1;
	if(!MN_isFMT(mn) && !MN_isGETTER(mn) && !MN_isSETTER(mn)) {
		const char *n = MN__(mn);
		if(n[0] == 'n' && n[1] == 'e' && n[2] == 'w' && n[3] == ':') {
			return 1;
		}
	}
	return 0;
}

static knh_Token_t *OPR_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt);

static knh_Token_t *SETPROPN_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkN = tkNN(stmt, 1);
	knh_String_t *v = knh_getPropertyNULL(ctx, S_tobytes(tkN->text));
	knh_type_t type;
	if(v != NULL) {
		type = O_cid(v);
		TYPING(ctx, stmt, 2, type, 0);
	}
	else {
		TYPING(ctx, stmt, 2, TYPE_dyn, 0);
		type = Tn_type(stmt, 2);
		InfoType(ctx, "$", S_tobytes(tkN->text), type);
	}
	Stmt_toSTT(stmt, STT_CALL);
	KNH_SETv(ctx, tkNN(stmt, 0), new_TokenMN(ctx, MN_setProperty));
	Stmt_insert(ctx, stmt, 1, new_TokenTYPED(ctx, TT_CID, CLASS_Class, CLASS_System));
	TT_(tkN) = TT_STR;  // reset
	return CALL_typing(ctx, stmt, type);
}

static knh_Token_t *LET_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkN = tkNN(stmt, 1);
	Token_setReadOnly(tkN, 0);
	if(reqt == TYPE_Boolean) {
		WARN_MuchBetter(ctx, "==");
		STT_(stmt) = STT_OPR;
		KNH_SETv(ctx, tkNN(stmt, 0), new_TokenMN(ctx, MN_opEQ));
		return OPR_typing(ctx, stmt, reqt);
	}
	if(TT_(tkN) == TT_NAME) {
		knh_Token_t *tkRES = TNAME_typing(ctx, tkN, reqt, _FINDLOCAL | _FINDFIELD | _FINDSCRIPT);
		DBG_P("%p %p", tkN, tkRES);
		if(tkRES == NULL) { /* not found */
			TYPING(ctx, stmt, 2, reqt, _NOTCHECK);
			reqt = Tn_type(stmt, 2);
			InfoType(ctx, "", TK_tobytes(tkN), reqt);
			if(IS_SCRIPTLEVEL(ctx)) {
				knh_fieldn_t fn = Token_fn(ctx, tkN);
				knh_index_t idx = Script_addField(ctx, K_GMASCR, 0, reqt, fn);
				knh_Token_toTYPED(ctx, tkN, TT_FIELD, reqt, idx);
			}
			else {
				tkRES = Gamma_addFUNCVAR(ctx, 0, reqt, tkN, 0);
			}
		}
		else {
			if(Token_isReadOnly(tkRES)) {
				knh_Method_t *mtd = DP(ctx->gma)->mtd;
				if(!(TT_(tkRES) == TT_FIELD && MN_isNEW((mtd)->mn))) {
					return ERROR_Denied(ctx, "read only", tkN);
				}
			}
			TYPING(ctx, stmt, 2, tkRES->type, 0);
			knh_Token_toTYPED(ctx, tkN, tkRES->tt, tkRES->type, (tkRES)->index);
		}
		return Stmt_typed(ctx, stmt, tkN->type);
	}
	if(TT_(tkN) == TT_PROPN) {
		return SETPROPN_typing(ctx, stmt, reqt);
	}
	if(TT_(tkN) == TT_UNAME) {
		return ERROR_OnlyTopLevel(ctx, "constant definition");
	}
	return ErrorTokenCannotAssign(ctx, tkN);
}

static inline void Stmt_setESPIDX(CTX ctx, knh_Stmt_t *stmt)
{
	DP(stmt)->espidx = DP(ctx->gma)->espidx;
}

static knh_type_t StmtTUPLE_typing(CTX ctx, knh_Stmt_t *stmt, size_t n, size_t i)
{
	knh_class_t cid = Tn_cid(stmt, n);
	const knh_ClassTBL_t *t = ClassTBL(cid);
	if(t->bcid == CLASS_Tuple && t->cparam != NULL && i < t->cparam->psize ) {
		return knh_ParamArray_get(t->cparam, i)->type;
	}
	return TYPE_void;
}

static knh_Token_t *LETM_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t tuplecid = CLASS_Tuple;
	size_t i, msize = DP(stmt)->size - 1;
	for(i = 0; i < msize; i++) {
		knh_Token_t *tkN = tkNN(stmt, i);
		if(TT_(tkN) == TT_NAME) {
			Token_setReadOnly(tkN, 0);
			knh_Token_t *tkRES = TNAME_typing(ctx, tkN, TYPE_dyn, _FINDLOCAL | _FINDFIELD | _FINDSCRIPT);
			if(tkRES == NULL) { /* not found */
				knh_type_t type = Tn_type(stmt, msize);
				if(type == TYPE_var) {
					TYPING(ctx, stmt, msize, TYPE_dyn, 0);
				}
				type = StmtTUPLE_typing(ctx, stmt, msize, i);
				if(type == TYPE_void) {
					return ErrorStaticType(ctx, TK__(tkN));
				}
				if(IS_SCRIPTLEVEL(ctx)) {
					knh_fieldn_t fn = Token_fn(ctx, tkN);
					knh_index_t idx = Script_addField(ctx, K_GMASCR, 0, type, fn);
					knh_Token_toTYPED(ctx, tkN, TT_FIELD, type, idx);
				}
				else {
					tkRES = Gamma_addFUNCVAR(ctx, 0, type, tkN, 0);
					KNH_SETv(ctx, tmNN(stmt, i), tkRES);
				}
			}
			else {
				if(Token_isReadOnly(tkRES)) {
					knh_Method_t *mtd = DP(ctx->gma)->mtd;
					if(!(TT_(tkRES) == TT_FIELD && MN_isNEW((mtd)->mn))) {
						return ERROR_Denied(ctx, "read only", tkN);
					}
				}
			}
		}
		else {
			return ERROR_text(ctx, "assignment" K_TRACEPOINT);
		}
	}
	tuplecid = Tn_cid(stmt, msize);
	if(tuplecid == CLASS_Tvar) {
		BEGIN_LOCAL(ctx, lsfp, 1);
		LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, pa, new_ParamArray(ctx));
		for(i = 0; i < msize; i++) {
			knh_param_t p = {Tn_type(stmt, i), FN_NONAME};
			knh_ParamArray_add(ctx, pa, p);
		}
		tuplecid = knh_class_Generics(ctx, CLASS_Tuple, pa);
		TYPING(ctx, stmt, msize, tuplecid, 0);
		END_LOCAL_(ctx, lsfp);
	}
	else {
		for(i = 0; i < msize; i++) {
			knh_type_t reqt = Tn_type(stmt, i);
			knh_type_t type = StmtTUPLE_typing(ctx, stmt, msize, i);
			if(reqt == TYPE_dyn || reqt == type) continue;
			KNH_TODO("type check");
		}
	}
	{
		BEGIN_BLOCK(espidx);
		knh_Token_t *tkASIS = new_(Token);
		knh_Token_t *tkIDX = Gamma_addLOCAL(ctx, 0, tuplecid, FN_, 1);
		knh_Stmt_t *stmtLET = new_Stmt2(ctx, STT_LET, tkASIS, tkIDX, tmNN(stmt, msize), NULL);
		knh_Stmt_t *stmtTAIL = stmtLET;
		size_t ti = 0;
		Stmt_setESPIDX(ctx, stmtTAIL);
		stmtTAIL->type = TYPE_void;
		for(i = 0; i < msize; i++) {
			knh_Token_t *tkN = tkNN(stmt, i);
			knh_Stmt_t *stmtLET2 = NULL;
			knh_Token_t *tkTPL = new_TokenTYPED(ctx, TT_FIELD, tkN->type, ti);
			KNH_SETv(ctx, tkTPL->data, tkIDX);
			if(TT_(tkN) == TT_LOCAL || TT_(tkN) == TT_FUNCVAR || TT_(tkN) == TT_FIELD || TT_(tkN) == TT_SCRFIELD) {
				stmtLET2 = new_Stmt2(ctx, STT_LET, tkASIS, tkN, tkTPL, NULL);
			}
			DBG_ASSERT(stmtLET2 != NULL);
			KNH_INITv(DP(stmtTAIL)->nextNULL, stmtLET2);
			stmtTAIL = stmtLET2;
			Stmt_setESPIDX(ctx, stmt);
			stmtTAIL->type = TYPE_void;
			ti++;
			DBLNDATA_(if(IS_Tunbox(tkN->type)) ti++;)
		}
		Stmt_toSTT(stmt, STT_BLOCK);
		knh_Stmt_trimToSize(ctx, stmt, 1);
		KNH_SETv(ctx, stmtNN(stmt, 0), stmtLET);
		END_BLOCK(espidx);
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t *SWAP_typing(CTX ctx, knh_Stmt_t *stmt)
{
	size_t i, msize = DP(stmt)->size / 2;
	for(i = 0; i < msize; i++) {
		knh_Token_t *tkN = tkNN(stmt, i);
		if(TT_(tkN) == TT_NAME) {
			Token_setReadOnly(tkN, 0);
			knh_Token_t *tkRES = TNAME_typing(ctx, tkN, TYPE_dyn, _FINDLOCAL | _FINDFIELD | _FINDSCRIPT);
			if(tkRES == NULL) { /* not found */
				TYPING(ctx, stmt, msize+i, TYPE_dyn, 0);
				knh_type_t type = Tn_type(stmt, msize+i);
				if(IS_SCRIPTLEVEL(ctx)) {
					knh_fieldn_t fn = Token_fn(ctx, tkN);
					knh_index_t idx = Script_addField(ctx, K_GMASCR, 0, type, fn);
					knh_Token_toTYPED(ctx, tkN, TT_FIELD, type, idx);
				}
				else {
					tkRES = Gamma_addFUNCVAR(ctx, 0, type, tkN, 0);
					KNH_SETv(ctx, tmNN(stmt, i), tkRES);
				}
			}
			else {
				if(Token_isReadOnly(tkRES)) {
					knh_Method_t *mtd = DP(ctx->gma)->mtd;
					if(!(TT_(tkRES) == TT_FIELD && MN_isNEW((mtd)->mn))) {
						return ERROR_Denied(ctx, "read only", tkN);
					}
				}
				TYPING(ctx, stmt, msize+i, tkRES->type, 0);
			}
		}
		else {
			return ERROR_text(ctx, "assignment" K_TRACEPOINT);
		}
	}
	{
		BEGIN_BLOCK(espidx);
		knh_Token_t *tkASIS = new_(Token);
		knh_Stmt_t *stmtLET = NULL, *stmtLET2 = NULL;
		knh_Stmt_t *stmtTAIL = NULL, *stmtTAIL2 = NULL;
		for(i = 0; i < msize; i++) {
			knh_Token_t *tkL = tkNN(stmt, i);
			knh_Token_t *tkIDX = Gamma_addLOCAL(ctx, 0, tkL->type, FN_, 1);
			knh_Stmt_t *stmtR = new_Stmt2(ctx, STT_LET, tkASIS, tkIDX, tmNN(stmt, msize+i), NULL);
			knh_Stmt_t *stmtL = new_Stmt2(ctx, STT_LET, tkASIS, tkL, tkIDX, NULL);
			if(stmtLET == NULL) {
				stmtLET = stmtR; stmtLET2 = stmtL;
			}
			else {
				KNH_INITv(DP(stmtTAIL)->nextNULL, stmtR);
				KNH_INITv(DP(stmtTAIL2)->nextNULL, stmtL);
			}
			stmtTAIL = stmtR; stmtTAIL2 = stmtL;
			stmtTAIL->type = stmtTAIL2->type = TYPE_void;
		}
		Stmt_toSTT(stmt, STT_BLOCK);
		knh_Stmt_trimToSize(ctx, stmt, 1);
		KNH_SETv(ctx, stmtNN(stmt, 0), stmtLET);
		KNH_INITv(DP(stmtTAIL)->nextNULL, stmtLET2);
		while(stmtLET != NULL) {
			Stmt_setESPIDX(ctx, stmtLET);
			stmtLET = DP(stmtLET)->nextNULL;
		}
		END_BLOCK(espidx);
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* CALL1_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		knh_Stmt_t *stmtPOST = DP(stmt)->stmtPOST;
		knh_Token_t *tkRES = Stmt_typing(ctx, DP(stmt)->stmtPOST, reqt);
		if(TT_(tkRES) == TT_ERR) return tkRES;
		DBG_ASSERT((void*)(tkRES) == (void*)stmtPOST);
		Stmt_setESPIDX(ctx, stmtPOST);
		if(reqt == TYPE_STMT) {
			stmtPOST->type = TYPE_void;
			STT_(stmt) = STT_BLOCK;
			KNH_SETv(ctx, stmtNN(stmt, 0), stmtPOST);
			return Stmt_typed(ctx, stmt, TYPE_void);
		}
	}
	else {
		if(reqt == TYPE_STMT) {
			WarningNoEffect(ctx);
			return knh_Stmt_done(ctx, stmt);
		}
	}
	TYPING(ctx, stmt, 0, reqt, 0);
	return Stmt_typed(ctx, stmt, Tn_type(stmt, 0));
}

/* ------------------------------------------------------------------------ */

static inline void boxSFP(CTX ctx, knh_sfp_t *sfp, knh_type_t type)
{
	knh_class_t cid = CLASS_t(type);
	if(IS_Tunbox(cid)) {
		KNH_SETv(ctx, sfp[0].o, new_Boxing(ctx, sfp, ClassTBL(cid)));
	}
}

static inline void unboxSFP(CTX ctx, knh_sfp_t *sfp)
{
	sfp[0].ndata = (sfp[0].i)->n.ivalue;
}

static knh_Token_t* CALL_toCONST(CTX ctx, knh_Stmt_t *stmt, knh_Method_t *mtd)
{
	BEGIN_LOCAL(ctx, lsfp, DP(stmt)->size + K_CALLDELTA);
#ifdef K_USING_DEBUG
	if(Method_isConst(mtd) || Stmt_isCONST(stmt)) {
#else
	if(Method_isConst(mtd) || Stmt_isCONST(stmt) || IS_SCRIPTLEVEL(ctx)) {
#endif
		size_t i = 1, size = DP(stmt)->size;
		long rtnidx = 0, thisidx = rtnidx + K_CALLDELTA;
		knh_Token_t *rvalue;
		if(MN_isNEW((mtd)->mn)) {
			knh_class_t cid = CLASS_t(stmt->type);
			KNH_SETv(ctx, lsfp[thisidx].o, new_Object_init2(ctx, ClassTBL(cid)));
			i = 2;
		}
		for(; i < size; i++) {
			knh_Token_t *tk = tkNN(stmt, i);
			if(TT_(tk) != TT_CONST && TT_(tk) != TT_NULL) goto L_NONSTATIC;
			KNH_SETv(ctx, lsfp[thisidx+(i-1)].o, (tk)->data);
			unboxSFP(ctx, &lsfp[thisidx+(i-1)]);
		}
		KNH_SCALL(ctx, lsfp, rtnidx, mtd, (size - 2));
		boxSFP(ctx, &lsfp[rtnidx], stmt->type);
		DBG_P("STMT = %s TURNED INTO CONST", TT__(STT_(stmt)));
		rvalue = ((DP(mtd)->mp)->rsize == 0) ? knh_Stmt_done(ctx, stmt) : Token_setCONST(ctx, tkNN(stmt, 0), lsfp[0].o);
		END_LOCAL(ctx, lsfp, rvalue);
		return rvalue;
	}
	L_NONSTATIC:;
	END_LOCAL_(ctx, lsfp);
	return TM(stmt);
}

static void METHOD_asm(CTX ctx, knh_Stmt_t *stmt);

static knh_Token_t *new_TokenDEFAULT(CTX ctx, knh_class_t cid, knh_type_t reqt)
{
	switch(cid) {
	case CLASS_Class:
		reqt = (reqt == TYPE_var) ? TYPE_dyn : reqt;
		return new_TokenCONST(ctx, new_Type(ctx, reqt));
	case CLASS_NameSpace: return new_TokenCONST(ctx, K_GMANS);
	default:
		return new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, cid, cid);
	}
}

static knh_Token_t* CALLPARAMs_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, knh_class_t mtd_cid, knh_Method_t *mtd)
{
	size_t i, size = DP(stmt)->size;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(pa), mtd_cid);
	//DBG_P("mtd_cid=%s rtype=%s", CLASS__(mtd_cid), TYPE__(rtype));
	if(rtype == TYPE_var && DP(ctx->gma)->mtd == mtd) {
		return ERROR_Unsupported(ctx, "type inference of recursive calls", CLASS_unknown, NULL);
	}
	Stmt_typed(ctx, stmt, rtype);
	for(i = 0; i < pa->psize; i++) {
		size_t n = i + 2;
		knh_type_t param_reqt = knh_ParamArray_getptype(pa, i);
		param_reqt = knh_type_tocid(ctx, param_reqt, mtd_cid);
		if(n < size) {
			TYPING(ctx, stmt, n, param_reqt, 0);
		}
		else {
			if(!(n < size) && !ParamArray_isVARGs(pa)) {
				if(param_reqt == TYPE_var) {
					return ERROR_RequiredParameter(ctx);
				}
				else {
					knh_Stmt_add(ctx, stmt, new_TokenDEFAULT(ctx, CLASS_t(param_reqt), TYPE_Class));
				}
			}
		}
	}
	if(ParamArray_isVARGs(pa)) {
		knh_type_t param_reqt = knh_ParamArray_getptype(pa, i);
		param_reqt = knh_type_tocid(ctx, param_reqt, mtd_cid);
		for(i = pa->psize; i + 2 < size; i++) {
			TYPING(ctx, stmt, i+2, param_reqt, 0);
		}
	}
	else if(i + 2 < size) {
		WarningTooManyParameters(ctx/*, mtd_cid, (mtd)->mn*/);
		knh_Stmt_trimToSize(ctx, stmt, i+2);
	}
	if(Method_isDynamic(mtd)) {
		knh_Gamma_t *lgma = DP(mtd)->gma;
		BEGIN_LOCAL(ctx, lsfp, 2);
		KNH_WARN(ctx, "dynamic compiled method");
		LOCAL_NEW(ctx, lsfp, 0, knh_Gamma_t*, gma, ctx->gma);
		LOCAL_NEW(ctx, lsfp, 1, knh_Stmt_t*, stmtM, DP(lgma)->stmt);
		Method_setDynamic(mtd, 0);
		for(i = 0; i < pa->psize; i++) {
			knh_param_t *p = knh_ParamArray_get(pa, i);
			if(p->type == TYPE_var) {
				p->type = tmNN(stmt, i+2)->type;
				InfoType(ctx, FN__(p->fn), STEXT(""), p->type);
			}
		}
		if(IS_Stmt(stmtM) && STT_(stmtM) == STT_METHOD) {
			KNH_SETv(ctx, ((knh_context_t*)ctx)->gma, lgma);
			METHOD_asm(ctx, stmtM);
			KNH_SETv(ctx, ((knh_context_t*)ctx)->gma, gma);
		}
		rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(pa), mtd_cid);
		DBG_ASSERT(rtype != TYPE_var);
		Stmt_typed(ctx, stmt, rtype);
		END_LOCAL_(ctx, lsfp); // NEED TO CHECK
		return knh_Stmt_done(ctx, stmt);
	}
	return CALL_toCONST(ctx, stmt, mtd);
}

static knh_Token_t* CALL_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkM = tkNN(stmt, 0);
	knh_Token_t *tkO = tkNN(stmt, 1);
	int maybeCLASSCONST = (TT_(tkM) == TT_UNAME && TT_(tkO) == TT_UNAME && DP(stmt)->size == 2) ? 1 : 0;
	knh_methodn_t mn = Token_mn(ctx, tkM);
	knh_Method_t *mtd = NULL;
	knh_class_t mtd_cid = CLASS_Object;
	DBG_ASSERT(TT_(tkO) != TT_ASIS);
	if(MN_isNEW(mn)) { /* reported by Maeda */
		return ERROR_Unsupported(ctx, "calling new as method", CLASS_unknown, NULL);
	}
	TYPING(ctx, stmt, 1, TYPE_dyn, 0);
	if(Tn_isCID(stmt, 1)) {
		knh_Token_toTYPED(ctx, tkO, TT_NULL/*DEFVAL*/, (tkO)->cid, (tkO)->cid);
	}
	mtd_cid = Tn_cid(stmt, 1);
	mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
	if(mtd != NULL) {
		Token_toCALLMTD(ctx, tkM, mn, mtd);
		return CALLPARAMs_typing(ctx, stmt, reqt, mtd_cid, mtd);
	}
	else {
		size_t i;
		const char *name = "method";
		if(maybeCLASSCONST) {
			knh_Object_t *v = knh_getClassConstNULL(ctx, mtd_cid, S_tobytes((tkM)->text));
			if(v != NULL) {
				return Token_setCONST(ctx, tkM, v);
			}
			name = "const"; TT_(tkM) = TT_UNAME; tkM->flag0 = 0;
		}
		for(i = 2; i < DP(stmt)->size; i++) {
			TYPING(ctx, stmt, i, TYPE_dyn, 0);
		}
		if(Stmt_isCLASSCONSTDEF(stmt)) {
			knh_Object_t *v = knh_getClassConstNULL(ctx, mtd_cid, S_tobytes((tkM)->text));
			if(v != NULL) {
				WARN_Ignored(ctx, "redefinition", mtd_cid, S_tochar((tkM)->text));
				return knh_Stmt_done(ctx, stmt);
			}
			Token_setCONST(ctx, tkO, new_Type(ctx, mtd_cid));
			Token_setCONST(ctx, tkM, tkM->data);
			Stmt_insert(ctx, stmt, 0, new_TokenMN(ctx, MN_setConst));
			knh_Stmt_swap(ctx, stmt, 1, 2);
			mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_Class, MN_setConst);
			Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_setConst, mtd);
			Stmt_typed(ctx, stmt, TYPE_void);
			return CALL_toCONST(ctx, stmt, mtd);
		}
		if(mtd_cid != TYPE_dyn) {
			return ERROR_Undefined(ctx, name, mtd_cid, tkM);
		}
		Token_toCALLMTD(ctx, tkM, mn, mtd);
		return Stmt_typed(ctx, stmt, TYPE_dyn);
	}
}

/* ------------------------------------------------------------------------ */
/* [built-in function] */

//static knh_Token_t* FLIKELY_typing(CTX ctx, knh_Stmt_t *stmt)
//{
//	if(Tn_typing(ctx, stmt, 2, TYPE_Boolean, _NOTYPEMAP)) {
//		return tkNN(stmt, 2);
//	}
//	return NULL;
//}

//static knh_Term_t* knh_StmtDOMAIN_typing(CTX ctx, knh_Stmt_t *stmt)
//{
//	if(Tn_typing(ctx, stmt, 2, CLASS_Tdynamic, TCHECK_)) {
//		knh_class_t cid;
//		if(Tn_isCID(stmt, 2)) {
//			cid = (tkNN(stmt, 2))->cid;
//		}
//		else {
//			cid = Tn_cid(stmt, 2);
//		}
//		DBG_ASSERT_cid(cid);
//		Token_setCONST(ctx, tkNN(stmt, 1), UPCAST(new_Type(ctx, cid)));
//		DP(stmt)->size = 2;
//		knh_Token_toMTD(ctx, tkNN(stmt, 0), MN_domain, knh_NameSpace_getMethodNULL(ctx, CLASS_Class, MN_domain));
//		return knh_Stmt_typed(ctx, stmt, TYPE_cid(knh_class_Generics(ctx, CLASS_Array, cid, CLASS_Tdynamic)));
//	}
//	TODO();
//	return NULL;
//}

static knh_Token_t* defined_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tk = tkNN(stmt, 0);
	knh_Token_t *tkRES = Tn_typing(ctx, stmt, 2, TYPE_dyn, _NOTWARN | _NOTCHECK);
	if(TT_(tkRES) != TT_ERR) {
		return Token_setCONST(ctx, tk, KNH_TRUE);
	}
	else {
		return Token_setCONST(ctx, tk, KNH_FALSE);
	}
}

static knh_Token_t* copy_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkRES = Tn_typing(ctx, stmt, 2, TYPE_dyn, 0);
	if(TT_(tkRES) == TT_ERR) {
		return tkRES;
	}
	else {
		knh_class_t cid = Tn_cid(stmt, 2);
		if(knh_class_canObjectCopy(ctx, cid)) {
			knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_Object, MN_copy);
			KNH_SETv(ctx, tmNN(stmt, 1), tmNN(stmt, 2));
			Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_copy, mtd);
			knh_Stmt_trimToSize(ctx, stmt, 2);
			return Stmt_typed(ctx, stmt, cid);
		}
		else {
			WarningUnnecessaryOperation(ctx, "copy");
			return tkNN(stmt, 2);
		}
	}
}

static knh_Token_t* this_typing(CTX ctx, knh_Stmt_t *stmt, knh_methodn_t mn)
{
	knh_class_t mtd_cid = DP(ctx->gma)->this_cid;
	knh_Token_t *tkMTD = tkNN(stmt, 0); /* change */
	knh_Method_t *mtd;
	if(mn == MN_super) {
		DBG_ASSERT_cid(mtd_cid);
		if(ClassTBL(mtd_cid)->supcid == CLASS_Object) {
			//knh_Gamma_perror(ctx, KC_ERR, _("not extended: %C"), mtd_cid);
			KNH_TODO("not extend?");
			return NULL;
		}
		mtd_cid = ClassTBL(mtd_cid)->supcid;
	}
	mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, MN_new);
	if(mtd == NULL || (mtd)->cid != mtd_cid) {
		return ErrorUnsupportedConstructor(ctx, mtd_cid);
	}
	knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_FUNCVAR, TYPE_cid(mtd_cid), 0); /* this */
	Token_toCALLMTD(ctx, tkMTD, MN_new, mtd);
	return TM(stmt);
}

static knh_class_t class_FuncType(CTX ctx, knh_class_t this_cid, knh_Method_t *mtd)
{
	knh_class_t cid;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	if(knh_ParamArray_hasTypeVar(pa)) {
		BEGIN_LOCAL(ctx, lsfp, 1);
		knh_ParamArray_t *npa = new_ParamArray(ctx);
		KNH_SETv(ctx, lsfp[0].o, npa);
		knh_ParamArray_tocid(ctx, pa, this_cid, npa);
		cid = knh_class_Generics(ctx, CLASS_Func, npa);
		END_LOCAL_(ctx, lsfp);
	}
	else {
		cid = knh_class_Generics(ctx, CLASS_Func, pa);
	}
	return cid;
}

static knh_Func_t * new_StaticFunc(CTX ctx, knh_class_t bcid, knh_Method_t *mtd)
{
	knh_Func_t *fo = new_H(Func);
	O_cTBL(fo) = ClassTBL(bcid);
	KNH_INITv(fo->mtd, mtd);
	fo->baseNULL = NULL;
	fo->xsfp = NULL;
	fo->xsize = 0;
	return fo;
}

static knh_Token_t* delegate_typing(CTX ctx, knh_Stmt_t *stmt)
{
	if(DP(stmt)->size == 4) {
		knh_Token_t *tkMN = tkNN(stmt, 3);
		TYPING(ctx, stmt, 2, TYPE_dyn, _NOTCHECK);
		if(Tn_isCID(stmt, 2)) { /* delegate(Class, f) */
			knh_Token_t *tkC = tkNN(stmt, 2);
			knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, (tkC)->cid, Token_mn(ctx, tkMN));
			if(mtd == NULL) {
				return ErrorUndefinedMethod(ctx, tkMN);
			}
			if(!Method_isStatic(mtd)) {
				return ErrorNotStaticMethod(ctx, mtd);
			}
			{
				knh_class_t cid = class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
				return Token_setCONST(ctx, tkMN, new_StaticFunc(ctx, cid, mtd));
			}
		}
		else {
			knh_class_t cid = Tn_cid(stmt, 2);
			knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, cid, Token_mn(ctx, tkMN));
			if(mtd == NULL) {
				return ErrorUndefinedMethod(ctx, tkMN);
			}
			cid = class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
			if(Method_isStatic(mtd)) {
				cid = class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
				return Token_setCONST(ctx, tkMN, new_StaticFunc(ctx, cid, mtd));
			}
			knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_CID, TYPE_Class, cid);
			Token_setCONST(ctx, tkNN(stmt, 3), mtd);
			STT_(stmt) = STT_NEW;
			mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_Func, MN_new);
			DBG_ASSERT(mtd != NULL);
			Token_toCALLMTD(ctx, tkNN(stmt, 0), (mtd)->mn, mtd);
			return Stmt_typed(ctx, stmt, cid);
		}
	}
	else {
		return ERROR_text(ctx, "delegate" K_TRACEPOINT);
	}
}

static knh_Token_t* FUNCCALLPARAMs_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	knh_class_t cid = Tn_cid(stmt, 0);
	DBG_ASSERT(IS_Tfunc(cid));
	knh_ParamArray_t *pa = ClassTBL(cid)->cparam;
	knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, cid, MN_invoke);
	KNH_ASSERT(mtd != NULL);
		/* 0 1 2 3 4 .. 5 */
	knh_Stmt_swap(ctx, stmt, 0, 1);
	DBG_ASSERT(TT_(tkNN(stmt, 0)) == TT_ASIS);
	Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_invoke, mtd);
	for(i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		knh_type_t type = Gamma_type(ctx, p->type);
		if(2 + i < DP(stmt)->size) {
			TYPING(ctx, stmt, 2 + i, type, 0);
		}
		else {
			knh_Stmt_add(ctx, stmt, new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, type, CLASS_t(type)));
		}
	}
	if(pa->psize + 2 < DP(stmt)->size) {
		DBG_P("STT=%s", TT__(stmt->stt));
		WarningTooManyParameters(ctx);
		knh_Stmt_trimToSize(ctx, stmt, 2+ pa->psize);
	}
	STT_(stmt) = STT_FUNCCALL;
	reqt = Gamma_type(ctx, knh_ParamArray_rtype(pa));
	return Stmt_typed(ctx, stmt, reqt);
}

static knh_Token_t* FUNCDYNCALL_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	knh_class_t cid = CLASS_Func;
	BEGIN_LOCAL(ctx, lsfp, 1);
	LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, pa, new_ParamArray(ctx));
	for(i = 2; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_var, 0);
		knh_param_t p = {Tn_type(stmt, i), FN_};
		knh_ParamArray_add(ctx, pa, p);
	}
	if(reqt != TYPE_void) {
		knh_param_t p = {(reqt == TYPE_var) ? TYPE_dyn : reqt, FN_};
		knh_ParamArray_radd(ctx, pa, p);
	}
	cid = knh_class_Generics(ctx, CLASS_Func, pa);
	DBG_ASSERT(cid != CLASS_unknown);
	if(IS_String(tkNN(stmt,0)->text)) {
		InfoType(ctx, "function ", S_tobytes(tkNN(stmt,0)->text), cid);
	}
	else {
		InfoType(ctx, "function ", STEXT(""), cid);
	}
	tkNN(stmt, 0)->type = cid;
	Stmt_setDYNCALL(stmt, 1);
	{
		knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, cid, MN_invoke);
		KNH_ASSERT(mtd != NULL);
		knh_Stmt_swap(ctx, stmt, 0, 1);
		DBG_ASSERT(TT_(tkNN(stmt, 0)) == TT_ASIS);
		Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_invoke, mtd);
	}
	END_LOCAL_(ctx, lsfp);
	STT_(stmt) = STT_FUNCCALL;
	reqt = Gamma_type(ctx, knh_ParamArray_rtype(pa));
	return Stmt_typed(ctx, stmt, reqt);
}

static knh_Token_t* func_typingNULL(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkF = tkNN(stmt, 0);
	knh_methodn_t mn = Token_mn(ctx, tkF);
	knh_Token_t *tkIDX = NULL;
	{ /* 1. lookup builtin function */
		switch(mn) {
//				case MN_likely:
//				case MN_unlikely: return FLIKELY_typing(ctx, stmt);
//				case MN_format:  return knh_StmtFFORMAT_typing(ctx, stmt);
//				case MN_domain:  return knh_StmtDOMAIN_typing(ctx, stmt);
			case MN_copy: return copy_typing(ctx, stmt);
			case MN_defined: return defined_typing(ctx, stmt);
			case MN_this:
			case MN_super: return this_typing(ctx, stmt, mn);
			case MN_delegate: return delegate_typing(ctx, stmt);
		}
	}
	{ /* 2. searching local variable of Func */
		knh_fieldn_t fn = Token_fn(ctx, tkF);
		tkIDX = Gamma_rindexFNQ(ctx, ctx->gma, fn, 0);
		if(tkIDX != NULL && IS_Tfunc(tkIDX->type)) {
			Gamma_rindexFNQ(ctx, ctx->gma, fn, 1);  // use count
			KNH_SETv(ctx, tkNN(stmt, 0), tkIDX);
			return FUNCCALLPARAMs_typing(ctx, stmt, reqt);
		}
	}
	knh_class_t mtd_cid = knh_NameSpace_getFuncClass(ctx, K_GMANS, mn);
	knh_Method_t *mtd = NULL;
	/* 3. static function in namespace */
	if(mtd_cid != CLASS_unknown) {
		mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
		if(mtd != NULL) {
			knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_NULL, mtd_cid, mtd_cid);
			goto L_CALLPARAMs;
		}
	}

	mtd_cid = DP(ctx->gma)->this_cid;
	mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
	if(mtd != NULL) {
		Gamma_foundFIELD(ctx->gma, 1);
		knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_FUNCVAR, mtd_cid, 0);
		goto L_CALLPARAMs;
	}
	if(mtd_cid != O_cid(K_GMASCR)) {
		mtd_cid = O_cid(K_GMASCR);
		mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
		if(mtd != NULL) {
			knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_NULL/*DEFVAL*/, mtd_cid, mtd_cid);
			goto L_CALLPARAMs;
		}
	}
	mtd_cid = CLASS_System;
	mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
	DBG_P("********* mtd=%p", mtd);
	if(mtd != NULL) {
		knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_NULL/*DEFVAL*/, mtd_cid, mtd_cid);
		goto L_CALLPARAMs;
	}
	return NULL; // continue to variable ErrorUndefinedName(ctx, tkF);

	L_CALLPARAMs:;
	DBG_ASSERT(mtd != NULL);
	STT_(stmt) = STT_CALL;
	Token_toCALLMTD(ctx, tkF, mn, mtd);
	return CALLPARAMs_typing(ctx, stmt, reqt, mtd_cid, mtd);
}

static knh_Token_t* FUNCCALL_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkF = tkNN(stmt, 0);
	DBG_ASSERT(TT_(tkNN(stmt, 1)) == TT_ASIS);
	if(TT_(tkF) == TT_FUNCNAME || TT_(tkF) == TT_UFUNCNAME) {
		TT_(tkF) = TT_NAME;
	}
	if(TT_(tkF) == TT_NAME || TT_(tkF) == TT_UNAME) {
		tkF = func_typingNULL(ctx, stmt, reqt);
		if(tkF != NULL) {
			return tkF;
		}
		tkF = tkNN(stmt, 0);
		TT_(tkF) = TT_NAME;
		DBG_P("STT_(stmt)=%s, TT_(tkF)=%s", TT__(stmt->stt), TT__(tkF->tt));
	}
	TYPING(ctx, stmt, 0, TYPE_var, 0);
	knh_type_t type = Tn_type(stmt, 0);
	if(IS_Tfunc(type)) {
		return FUNCCALLPARAMs_typing(ctx, stmt, reqt);
	}
	if(type == TYPE_dyn) {
		return FUNCDYNCALL_typing(ctx, stmt, reqt);
	}
	if(type == TYPE_String && TT_(tkNN(stmt, 0)) == TT_CONST) {
		return FMTOP_typing(ctx, stmt, reqt);
	}
	return TypeErrorNotFuncion(ctx, type);
}

/* ------------------------------------------------------------------------ */
/* [NEW] */

static knh_Token_t* typingFIELD(CTX ctx, knh_class_t cid, knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tkK = tkNN(stmt, n); DBG_ASSERT(IS_String(tkK->text));
	knh_fieldn_t fn = knh_getfnq(ctx, S_tobytes(tkK->text), FN_NEWID);
	const knh_ClassTBL_t *ct = ClassTBL(cid);
	size_t i;
	for(i = 0; i < ct->fsize; i++) {
		if(ct->fields[i].fn == fn) {
			TYPING(ctx, stmt, n+1, ct->fields[i].type, 0);
			Token_setCONST(ctx, tkK, new_Int(ctx, CLASS_Int, i));
			return tkK; // OK
		}
	}
	{
		WARN_UndefinedName(ctx, tkK);
		TYPING(ctx, stmt, n+1, TYPE_dyn, 0);
	}
	return TYPE_void;
}

static knh_Token_t* NEWPARAMs_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t mtd_cid, knh_methodn_t mn, int needsTypingPARAMs)
{
	knh_Token_t *tkMTD = tkNN(stmt, 0);
	knh_Token_t *tkC = tkNN(stmt, 1);
	knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
	knh_Token_t *tkRES = (knh_Token_t*)stmt;
	if(mtd == NULL || ClassTBL((mtd)->cid)->bcid != ClassTBL(mtd_cid)->bcid) {
		return ERROR_Undefined(ctx, _("constructor"), mtd_cid, tkMTD);
	}
	Token_toCALLMTD(ctx, tkMTD, mn, mtd);
	//DBG_P("mtd_cid=%s, reqt=%s", CLASS__(mtd_cid), CLASS__(reqt));
	knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
	if(needsTypingPARAMs) {
		tkRES = CALLPARAMs_typing(ctx, stmt, mtd_cid, mtd_cid, mtd);
	}
	tkRES->type = mtd_cid;
	return tkRES;
}

#define IS_NEWLIST(cid)  (cid == CLASS_Array || cid == CLASS_ArrayIm || cid == CLASS_Range)
#define IS_NEWMAP(cid)  (cid == CLASS_Map || cid == CLASS_MapIm)

static knh_Token_t* NEW_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkMTD = tkNN(stmt, 0);
	knh_Token_t *tkC = tkNN(stmt, 1);
	knh_methodn_t mn = Token_mn(ctx, tkMTD);
	knh_class_t mtd_cid = CLASS_unknown;
	if(reqt == TYPE_var || reqt == TYPE_void) reqt = TYPE_dyn;
	if(TT_(tkC) == TT_ASIS) { /* new () */
		if(reqt == TYPE_dyn) {
			return ERROR_Needs(ctx, "class");
		}
		mtd_cid = CLASS_t(reqt);
	}
	else {
		mtd_cid = knh_Token_cid(ctx, tkC, CLASS_unknown);
	}
	if(mtd_cid == CLASS_unknown) { /* new UnknownClass(...) */
		if(reqt == TYPE_dyn) return ERROR_UndefinedName(ctx, tkC);
		mtd_cid = CLASS_t(reqt);
	}
	if(mn == MN_newARRAY) {  /* new C [10, 10] */
		size_t i;
		for(i = 2; i < DP(stmt)->size; i++) {
			TYPING(ctx, stmt, i, TYPE_Int, 0);
		}
		return NEWPARAMs_typing(ctx, stmt, mtd_cid, mn, 0/*needsTypingPARAMs*/);
	}
	if(mn == MN_newLIST) {  /* [1, 2, .. ] */
		size_t i;
		DBG_ASSERT(IS_NEWLIST(mtd_cid));
		knh_class_t bcid = knh_class_bcid(reqt);
		if(!IS_NEWLIST(bcid) && DP(stmt)->size > 2) {
			knh_class_t p1;
			TYPING(ctx, stmt, 2, TYPE_dyn, 0);
			p1 = Tn_cid(stmt, 2);
			for(i = 3; i < DP(stmt)->size; i++) {
				TYPING(ctx, stmt, i, TYPE_dyn, 0);
				if(p1 != TYPE_dyn && p1 != Tn_cid(stmt, i)) p1 = TYPE_dyn;
			}
			if(p1 != TYPE_dyn) {
				mtd_cid = knh_class_P1(ctx, mtd_cid, p1);
			}
		}
		else {
			knh_class_t p1 = knh_class_p1(reqt);
			for(i = 2; i < DP(stmt)->size; i++) {
				TYPING(ctx, stmt, i, p1, 0);
			}
			mtd_cid = (reqt != TYPE_dyn) ? reqt : mtd_cid;
		}
		return NEWPARAMs_typing(ctx, stmt, mtd_cid, mn, 0/*needsTypingPARAMs*/);
	}

	if(mn == MN_newMAP) {  /* {hoge: 1, hogo: 2} */
		size_t i;
		knh_class_t bcid = knh_class_bcid(reqt);
		DBG_P("mtd_cid=%s, reqt=%s, bcid=%s", CLASS__(mtd_cid), CLASS__(reqt), CLASS__(bcid));
		for(i = 2; i < DP(stmt)->size; i+=2) {
			TYPING(ctx, stmt, i, TYPE_TEXT, 0);  // key
		}
		if(knh_class_bcid(mtd_cid) == CLASS_Object) {
			for(i = 2; i < DP(stmt)->size; i+=2) {
				knh_Token_t *tkRES = typingFIELD(ctx, mtd_cid, stmt, i+1);
				if(TT_(tkRES) == TT_ERR) return tkRES;
			}
		}
		else if(!IS_NEWMAP(bcid) && DP(stmt)->size > 2) {
			knh_class_t p1, p2;
			TYPING(ctx, stmt, 3, TYPE_dyn, 0); // value
			p1 = Tn_cid(stmt, 2); p2 = Tn_cid(stmt, 3);
			for(i = 4; i < DP(stmt)->size; i+=2) {
				TYPING(ctx, stmt, i+1, TYPE_dyn, 0);  // value
				if(p2 != TYPE_dyn && p2 != Tn_cid(stmt, i+1)) p2 = TYPE_dyn;
			}
			if(p2 != TYPE_dyn) {
				mtd_cid = knh_class_P2(ctx, mtd_cid, CLASS_String, p2);
			}
		}
		else {
			knh_class_t p2 = knh_class_p2(reqt);
			for(i = 2; i < DP(stmt)->size; i+=2) {
				TYPING(ctx, stmt, i+1, p2, 0);       // value
			}
			mtd_cid = (reqt != TYPE_dyn) ? reqt : mtd_cid;
		}
		if(IS_SCRIPTLEVEL(ctx)) {
			DBG_P("** SCRIPT_LEVEL **");
		}
		return NEWPARAMs_typing(ctx, stmt, mtd_cid, mn, 0/*needsTypingPARAMs*/);
	}

	if(mn == MN_newTUPLE) {  /* (1, 2) */
		size_t i;
		BEGIN_LOCAL(ctx, lsfp, 1);
		LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, pa, new_ParamArray(ctx));
		for(i = 2; i < DP(stmt)->size; i++) {
			TYPING(ctx, stmt, i, TYPE_dyn, 0);
			knh_param_t p = {Tn_type(stmt, i), FN_NONAME};
			knh_ParamArray_add(ctx, pa, p);
		}
		mtd_cid = knh_class_Generics(ctx, CLASS_Tuple, pa);
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
		END_LOCAL_(ctx, lsfp);
		return NEWPARAMs_typing(ctx, stmt, mtd_cid, mn, 0/*needsTypingPARAMs*/);
	}

	if(mtd_cid == CLASS_Exception) {
		knh_ebi_t eid = knh_geteid(ctx, S_tobytes((tkC)->text), EBI_unknown);
		if(eid == EBI_unknown) WarningUndefined(ctx, "event", "", S_tochar((tkC)->text));
		Token_toCONST(ctx, tkC);
		tkC = new_TokenTYPED(ctx, TT_CID, CLASS_Class, mtd_cid);
		Stmt_insert(ctx, stmt, 1, tkC);
		DBG_ASSERT(TT_(tmNN(stmt, 2)) == TT_CONST);
		DBG_ASSERT((tmNN(stmt, 2))->type == CLASS_String);
	}
	return NEWPARAMs_typing(ctx, stmt, mtd_cid, mn, 1/*needsTypingPARAMs*/);
}

/* ------------------------------------------------------------------------ */
/* [OP] */

static knh_class_t OPADD_bcid(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid1 = Tn_cid(stmt, 1);
	knh_class_t cid2 = Tn_cid(stmt, 2);
	knh_class_t bcid1 = knh_class_bcid(cid1);
	knh_class_t bcid2 = knh_class_bcid(cid2);
	if(cid1 == CLASS_Tdynamic || cid2 == CLASS_Tdynamic) return CLASS_Tdynamic;
	if(cid1 == cid2) return cid1;
	if(bcid1 == bcid2) return bcid1;
	if(bcid1 == CLASS_Float && bcid2 == CLASS_Int) return cid1;
	if(bcid2 == CLASS_Float && bcid1 == CLASS_Int) return cid2;
	if(bcid1 == CLASS_String || bcid2 == CLASS_String) return CLASS_String;
	return cid1;
}

static knh_class_t OPEQ_bcid(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid1 = Tn_cid(stmt, 1);
	knh_class_t cid2 = Tn_cid(stmt, 2);
	knh_class_t bcid1 = knh_class_bcid(cid1);
	knh_class_t bcid2 = knh_class_bcid(cid2);
	if(cid1 == cid2) return cid1;
	if(cid1 == CLASS_Tdynamic || cid2 == CLASS_Tdynamic) return CLASS_Tdynamic;
	if(bcid1 == cid2) return bcid1;
	if(bcid2 == cid1) return bcid2;
	if(bcid1 == CLASS_Float && bcid2 == CLASS_Int) return cid1;
	if(bcid2 == CLASS_Float && bcid1 == CLASS_Int) return cid2;
	return CLASS_unknown;
}

knh_methodn_t TT_toMN(knh_term_t tt);

static knh_Token_t* OPR_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i, opsize = DP(stmt)->size - 1;
	knh_Token_t *tkOP = tkNN(stmt, 0);
	knh_methodn_t mn;
	knh_class_t mtd_cid = CLASS_Tdynamic;
	if(TT_(tkOP) == TT_MN) {
		mn = (tkOP)->mn;
		mtd_cid = Tn_cid(stmt, 1);
	}
	else {
		mn = TT_toMN(TT_(tkOP));
		if(TT_isBINARY(TT_(tkOP)) && opsize != 2) {
			return ERROR_MustBe(ctx, _("binary operator"), knh_getopname(mn));
		}
		if(TT_(tkOP) == TT_EXISTS) mtd_cid = CLASS_String;
		for(i = 1; i < opsize + 1; i++) {
			TYPING(ctx, stmt, i, mtd_cid, 0);
		}
	}
	if(mn == MN_opEQ || mn == MN_opNOTEQ) {
		knh_term_t tt = TT_TERMs(stmt, 1);
		if(tt == TT_NULL || tt == TT_TRUE || tt == TT_FALSE || tt == TT_CONST) {
			knh_Stmt_swap(ctx, stmt, 1, 2);
		}
		if(Tn_isNULL(stmt, 2)) { /* o == null, o != null */
			knh_class_t cid = Tn_cid(stmt, 1);
			mn = (mn == MN_opEQ) ? MN_isNull : MN_isNotNull;
			if(IS_Tunbox(cid)) {
				WarningNullable(ctx, cid);
			}
			mtd_cid = CLASS_Object;
			knh_Stmt_trimToSize(ctx, stmt, 2);
			goto L_LOOKUPMETHOD;
		}
	}
	for(i = 1; i < opsize + 1; i++) {
		if(TT_(tmNN(stmt, i)) == TT_NULL) {
			return ErrorUndefinedBehavior(ctx, "null");
		}
	}

	switch(mn) {
	case MN_opADD: /* a + b*/
	case MN_opSUB: /* a - b */
	case MN_opMUL: /* a * b */
	case MN_opDIV: /* a / b */
	case MN_opMOD: /* a % b */
	{
		mtd_cid = OPADD_bcid(ctx, stmt);
		if(mtd_cid == CLASS_String && mn == MN_opADD
				&& (!Tn_isCONST(stmt, 1) || !Tn_isCONST(stmt, 2))) {
			knh_Token_t *tk = new_(Token);
			TT_(tk) = TT_ASIS;
			Stmt_insert(ctx, stmt, 1, tk);
			STT_(stmt) = STT_SEND;
			return Stmt_typed(ctx, stmt, TYPE_String);
		}
		goto L_LOOKUPMETHOD;
	}
	case MN_opPLUS:
		return tkNN(stmt, 1);

	case MN_opEQ: case MN_opNOTEQ:
	{
		if(Tn_isTRUE(stmt, 1) || Tn_isFALSE(stmt, 1)) {
			knh_Stmt_swap(ctx, stmt, 1, 2);
		}
		if((Tn_isTRUE(stmt, 2) && (mn == MN_opEQ)) /* b == true */
		|| (Tn_isFALSE(stmt, 2) && (mn == MN_opNOTEQ))) {  /* b != false */
			TYPING(ctx, stmt, 1, TYPE_Boolean, 0);
			return tkNN(stmt, 1);
		}
		if((Tn_isTRUE(stmt, 2) && (mn == MN_opNOTEQ)) /* b != true */
		|| (Tn_isFALSE(stmt, 2) && (mn == MN_opEQ))) {  /* b == false */
			TYPING(ctx, stmt, 1, TYPE_Boolean, 0);
			mn = MN_opNOT;
			mtd_cid = CLASS_Boolean;
			knh_Stmt_trimToSize(ctx, stmt, 2);
			goto L_LOOKUPMETHOD;
		}
		mtd_cid = OPEQ_bcid(ctx, stmt);
		if(mtd_cid == CLASS_unknown) {
			return ErrorComparedDiffrentType(ctx, Tn_type(stmt, 1), Tn_type(stmt, 2));
		}
		goto L_LOOKUPMETHOD;
	}
	case MN_opGT: case MN_opGTE: case MN_opLT: case MN_opLTE:
	{
		mtd_cid = OPEQ_bcid(ctx, stmt);
		if(mtd_cid == CLASS_unknown) {
			return ErrorComparedDiffrentType(ctx, Tn_type(stmt, 1), Tn_type(stmt, 2));
		}
		goto L_LOOKUPMETHOD;
	}
	case MN_opHAS:
	{
		knh_Stmt_swap(ctx, stmt, 1, 2);
		mtd_cid = Tn_cid(stmt, 1);
		goto L_LOOKUPMETHOD;
	}

	case MN_opOF:
	{
		knh_Token_t *tkC = tkNN(stmt, 2);
		knh_class_t cid = CLASS_t((tkC)->type);
		mtd_cid = Tn_cid(stmt, 1);
		if(TT_(tkC) == TT_CID) {
			if(mtd_cid == CLASS_Exception) {
				mtd_cid = CLASS_Object;
			}
			cid = (tkC)->cid;
		}
		if(mtd_cid == CLASS_Exception){
			goto L_LOOKUPMETHOD;
		}
//		This must be checked later
//		if(mtd_cid == cid || cid == CLASS_Tdynamic) {
//			return Token_setCONST(ctx, tkC, KNH_TRUE);
//		}
//		if(!class_isa(mtd_cid, cid)) {
//			return Token_setCONST(ctx, tkC, KNH_FALSE);
//		}
		goto L_LOOKUPMETHOD;
	}

	case MN_opEXISTS:
	{
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, K_GMANS));
		goto L_LOOKUPMETHOD;
	}

	default:
		mtd_cid = Tn_cid(stmt, 1);
		break;
	}

	L_LOOKUPMETHOD:;
	DBG_ASSERT_cid(mtd_cid);
	{
		knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
		if(mtd == NULL) {
			return ERROR_Unsupported(ctx, "operator", mtd_cid,
				mn == MN_NONAME ? S_tochar(tkOP->text) : knh_getopname(mn));
		}
		Token_toCALLMTD(ctx, tkOP, mn, mtd);
		TYPING(ctx, stmt, 1, mtd_cid, 0);
		return CALLPARAMs_typing(ctx, stmt, reqt, mtd_cid, mtd);
	}
}

/// x <<< y
static knh_Token_t* SEND_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 1; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_dyn, 0);
	}
	knh_class_t mtd_cid = Tn_cid(stmt, 1);
	if(mtd_cid == CLASS_OutputStream) {
		return Stmt_typed(ctx, stmt, TYPE_void);
	}
	else {
		knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, MN_send);
		if(mtd == NULL) {
			return ERROR_Unsupported(ctx, "operator", mtd_cid, "<<<");
		}
		STT_(stmt) = STT_CALL;
		Token_toCALLMTD(ctx, tkNN(stmt, 0), MN_send, mtd);
		return CALLPARAMs_typing(ctx, stmt, reqt, mtd_cid, mtd);
	}
}

static knh_Token_t* ACALL_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	return ERROR_Unsupported(ctx, "actor", CLASS_unknown, NULL);
}

/* ------------------------------------------------------------------------ */
/* [TYPEMAP] */

static void Token_toTYPEMAPMPR(CTX ctx, knh_Token_t *tk, knh_class_t cid, knh_TypeMap_t *trlNULL)
{
	(tk)->cid = cid;
	if(trlNULL != NULL) {
		KNH_SETv(ctx, (tk)->data, trlNULL);
	}
}

static knh_Token_t* TCAST_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkC = tkNN(stmt, 0);
	knh_class_t scid, tcid;
	knh_Token_t *tkRES = TTYPE_typing(ctx, tkC, CLASS_unknown);
	if(TT_(tkRES) == TT_ERR) return tkRES;
	tcid = (tkC)->cid;
	if(tcid == CLASS_Tdynamic) {   /* (dynamic)expr */
		tcid = CLASS_t(reqt);
		if(tcid == CLASS_Tdynamic) {
			TYPING(ctx, stmt, 1, TYPE_dyn, _NOTCHECK);
			return tkNN(stmt, 1);
		}
	}
	//DBG_P("TCID(reqt)= %s", CLASS__(tcid));
	TYPING(ctx, stmt, 1, tcid, _NOTCHECK | _NOTYPEMAP);
	scid = Tn_cid(stmt, 1);
	if(scid == CLASS_Tdynamic) {     /* (T)anyexpr */
		Token_toTYPEMAPMPR(ctx, tkC, tcid, NULL);
		return Stmt_typed(ctx, stmt, tcid);
	}
	if(class_isa(scid, tcid)) {
		if(!Tn_isCONST(stmt, 1) && tcid == scid) {
			WarningUpcast(ctx, tcid, scid);
		}
		return tkNN(stmt, 1);
	}

	if(class_isa(tcid, scid)) {  /* downcast */
		Stmt_setDOWNCAST(stmt, 1);
		Token_toTYPEMAPMPR(ctx, tkC, tcid, NULL);
		WarningDowncast(ctx, tcid, scid);
		return Stmt_typed(ctx, stmt, tcid);
	}

	{
		knh_TypeMap_t *trl = knh_getTypeMapNULL(ctx, scid, tcid);
		if(trl == NULL) {
			if(scid != CLASS_Tdynamic) {
				return ErrorNoSuchTransCast(ctx, scid, tcid);
			}
			Token_toTYPEMAPMPR(ctx, tkC, tcid, NULL);
			return Stmt_typed(ctx, stmt, tcid);
		}
		if(TypeMap_isConst(trl) && Tn_isCONST(stmt, 1)) {
			BEGIN_LOCAL(ctx, lsfp, 1);
			knh_Token_t *tk1 = tkNN(stmt, 1);
			KNH_SETv(ctx, lsfp[0].o, (tk1)->data);
			KNH_SCAST(ctx, lsfp, 0, trl);
			boxSFP(ctx, &lsfp[0], SP(trl)->tcid);
			DBG_P("TYPEMAP TO CONST .. %s ==> %s, %s", CLASS__(SP(trl)->scid), CLASS__(SP(trl)->tcid), O__(lsfp[0].o));
			Token_setCONST(ctx, tk1, lsfp[0].o);
			END_LOCAL_(ctx, lsfp);  // NEED TO CHECK
			return TM(tk1);
		}
		else {
			Token_toTYPEMAPMPR(ctx, tkC, tcid, trl);
			return Stmt_typed(ctx, stmt, tcid);
		}
	}
}

/* [MT,AND,OR,] */
static knh_Token_t* AND_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Boolean, _NOTYPEMAP);
		if(Tn_isFALSE(stmt, i)) {
			return new_TermCONST(ctx, KNH_FALSE);
		}
	}
	return Stmt_typed(ctx, stmt, TYPE_Boolean);
}

static knh_Token_t* OR_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Boolean, _NOTYPEMAP);
		if(Tn_isTRUE(stmt, i)) {
			return new_TermCONST(ctx, KNH_TRUE);
		}
	}
	return Stmt_typed(ctx, stmt, TYPE_Boolean);
}

static knh_Token_t* ALT_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	DBG_ASSERT(DP(stmt)->size > 1);
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, reqt, 0);
	}
	return Stmt_typed(ctx, stmt, reqt);
}

static knh_Token_t* TRI_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTYPEMAP);
	TYPING(ctx, stmt, 1, reqt, 0);
	if(reqt == TYPE_var) {
		reqt = Tn_type(stmt, 1);
	}
	TYPING(ctx, stmt, 2, reqt, 0);
	if(Tn_isTRUE(stmt, 0)) return tkNN(stmt, 1);
	if(Tn_isFALSE(stmt, 0)) return tkNN(stmt, 2);
	return Stmt_typed(ctx, stmt, reqt);
}

/* ------------------------------------------------------------------------ */

#define CASE_EXPR(XX, ...) case STT_##XX : { \
		return XX##_typing(ctx, ## __VA_ARGS__); \
	}\

static knh_Token_t* FUNCTION_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt);

static knh_Token_t* EXPR_typing(CTX ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	switch(STT_(stmt)) {
		CASE_EXPR(CALL1, stmt, reqt);
		CASE_EXPR(LET, stmt, reqt);
		CASE_EXPR(FUNCCALL, stmt, reqt);
		CASE_EXPR(CALL, stmt, reqt);
		CASE_EXPR(NEW, stmt, reqt);
		CASE_EXPR(OPR, stmt, reqt);
		CASE_EXPR(ACALL, stmt, reqt);
		CASE_EXPR(SEND, stmt, reqt);
		CASE_EXPR(TCAST, stmt, reqt);
		CASE_EXPR(TSCHEME, stmt, reqt);
		CASE_EXPR(AND, stmt, reqt);
		CASE_EXPR(OR, stmt, reqt);
		CASE_EXPR(ALT, stmt, reqt);
		CASE_EXPR(TRI, stmt, reqt);
		CASE_EXPR(FUNCTION, stmt, reqt);
	default:
		return ERROR_Unsupported(ctx, "expression", CLASS_unknown, cSTT_((stmt)));
	}
}

/* ------------------------------------------------------------------------ */

static knh_Token_t* ErrorTypeError(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	knh_Token_t *tkERR = NULL;
	knh_type_t type = TYPE_void;
	if(n < DP(stmt)->size) {
		type = Tn_type(stmt, n);
	}
	else {
		DBG_P("!! n < DP(stmt)->size: n=%d, DP(stmt)->size=%d", n, DP(stmt)->size);
	}
	switch(SP(stmt)->stt) {
	case STT_NEW:
	case STT_FUNCCALL:
	case STT_CALL:
	case STT_OPR:
	{
		knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
		tkERR = TypeErrorCallParam(ctx, n-1, mtd, reqt, type);
		break;
	}
	case STT_LET:
	case STT_DECL:
		tkERR = TERROR_Token(ctx, tkNN(stmt, 1), type, reqt);
		break;
	default :
		tkERR = TypeErrorStmtNN(ctx, stmt, n, reqt, type);
		break;
	}
	DBG_ASSERT(tkERR == NULL); // always check if K_USING_DEBUG_
	return tkERR;
}

static knh_Term_t *new_TermTCAST(CTX ctx, knh_class_t reqt, knh_TypeMap_t *trlNULL, knh_Token_t *tk2)
{
	if(TT_(tk2) == TT_CONST && trlNULL != NULL && TypeMap_isConst(trlNULL)) {
		BEGIN_LOCAL(ctx, lsfp, 1);
		KNH_SETv(ctx, lsfp[0].o, (tk2)->data);
		KNH_SCAST(ctx, lsfp, 0, trlNULL);
		boxSFP(ctx, lsfp, SP(trlNULL)->tcid);
		Token_setCONST(ctx, tk2, lsfp[0].o);
		END_LOCAL(ctx, lsfp, tk2);
		return (knh_Term_t*)(tk2);
	}
	else {
		knh_Stmt_t *cstmt = new_Stmt2(ctx, STT_TCAST, NULL);
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_CID, TYPE_Class, CLASS_Tdynamic);
		Token_toTYPEMAPMPR(ctx, tk, reqt, trlNULL);
		knh_Stmt_add(ctx, cstmt, tk);
		knh_Stmt_add(ctx, cstmt, tk2);
		return (knh_Term_t*)Stmt_typed(ctx, cstmt, reqt);
	}
}

#define ERROR_IF(c)     if(c)  goto L_ERROR;
#define PERROR_IF(c)    if(c)  goto L_PERROR;

knh_Token_t* Tn_typing(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, knh_flag_t opflag)
{
	knh_flag_t flag = DP(ctx->gma)->flag;
	knh_Token_t *tkRES = NULL;
	DBG_ASSERT(STT_(stmt) != STT_ERR);
	if(FLAG_is(opflag, _NOTWARN)) {
		Gamma_setQuiet(ctx->gma, 1);
	}
	PERROR_IF(!(n < DP(stmt)->size));
	/* TYPING */{
		knh_Token_t *tk = tkNN(stmt, n);
		tkRES = IS_Token(tk)
			? Token_typing(ctx, tkNN(stmt, n), reqt) : Stmt_typing(ctx, stmtNN(stmt, n), reqt);
		if(TT_(tkRES) == TT_ERR) goto L_PERROR;
		if(tk != tkRES) {
			KNH_SETv(ctx, tmNN(stmt, n), tkRES);
		}
	}
	if(FLAG_is(opflag, _NOTCHECK)) {
		goto L_RETURN;
	}
	if(reqt == TYPE_var)  {
		if(Tn_type(stmt, n) == TYPE_var) {
			reqt = TYPE_Object; // actual instance
			goto L_PERROR;
		}
		goto L_RETURN;
	}
	if(reqt == TYPE_TEXT) {
		if(TT_(tkNN(stmt, n)) == TT_CONST && Tn_cid(stmt, n) == CLASS_String) {
			goto L_RETURN;
		}
		goto L_PERROR;
	}
	if(FLAG_is(opflag, _BCHECK)) {
		knh_class_t cid = Tn_cid(stmt, n);
		if(knh_class_bcid(cid) == reqt) goto L_RETURN;
		tkRES = NULL;
	}
	else { 	/*TYPECHECK*/
		knh_type_t vart = Tn_type(stmt, n);
		if(vart == TYPE_var) {
			tkRES = ErrorStaticType(ctx, TT__(TT_(tmNN(stmt, n))));
		}
		else {
			if(vart != TYPE_void && class_isa(vart, reqt)) goto L_RETURN;
			tkRES = NULL;
		}
	}
	if(FLAG_is(opflag, _NOTYPEMAP)) {
		goto L_RETURN;
	}
	else {
		knh_type_t vart = Tn_type(stmt, n);
		if(vart == TYPE_dyn) {
			KNH_SETv(ctx, tmNN(stmt, n), new_TermTCAST(ctx, reqt, NULL, tkNN(stmt, n)));
			goto L_RETURN;
		}
		else {
			knh_TypeMap_t *tmr = knh_getTypeMapNULL(ctx, vart, reqt);
			if((vart == TYPE_Int && reqt == TYPE_Float) || (vart == TYPE_Float && vart == TYPE_Int)) {
				KNH_SETv(ctx, tmNN(stmt, n), new_TermTCAST(ctx, reqt, tmr, tkNN(stmt, n)));
				goto L_RETURN;
			}
			if(reqt == TYPE_Regex && vart == TYPE_String) {
				KNH_SETv(ctx, tmNN(stmt, n), new_TermTCAST(ctx, reqt, tmr, tkNN(stmt, n)));
				goto L_RETURN;
			}
			if((tmr != NULL && TypeMap_isSemantic(tmr)) || FLAG_is(opflag, _ICAST)) {
				KNH_SETv(ctx, tmNN(stmt, n), new_TermTCAST(ctx, reqt, tmr, tkNN(stmt, n)));
				goto L_RETURN;
			}
		}
		DBG_P("stt=%s n=%d, reqt=%s, vart=%s", TT__(SP(stmt)->stt), (int)n, TYPE__(reqt), TYPE__(vart));
	}

	L_PERROR:;
	if(tkRES == NULL) {
		DP(ctx->gma)->flag |= flag;
		return ErrorTypeError(ctx, stmt, n, reqt);
	}

	L_RETURN:;
	DP(ctx->gma)->flag |= flag;
	if(tkRES == NULL) {
		tkRES = tkNN(stmt, n);
	}
	if(FLAG_is(opflag, _CONSTASIS)) {
		knh_term_t tt = TT_(tkRES);
		if(tt != TT_ASIS && tt != TT_CONST && tt != TT_NULL) {
			tkRES = ERROR_MustBe(ctx, "a constant value", TT__(tkNN(stmt, n)->tt));
		}
	}
	else if(FLAG_is(opflag, _CONSTONLY)) {
		knh_term_t tt = TT_(tkRES);
		if(tt != TT_CONST && tt != TT_NULL) {
			tkRES = ERROR_MustBe(ctx, "a constant value", TT__(tkNN(stmt, n)->tt));
		}
	}
	return tkRES;
}

static knh_type_t Gamma_getReturnType(CTX ctx)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	return Gamma_type(ctx, knh_ParamArray_rtype(DP(mtd)->mp));
}

static void Gamma_inferReturnType(CTX ctx, knh_type_t rtype)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	if(ParamArray_isRVAR(pa)) {
		DBG_ASSERT(pa->rsize == 0);
		if(rtype != TYPE_void) {
			knh_param_t p = {rtype, FN_};
			knh_ParamArray_radd(ctx, pa, p);
		}
		InfoType(ctx, "return value", STEXT(""), rtype);
		ParamArray_setRVAR(pa, 0);
	}
	//??? rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(pa), (mtd)->cid);
}

static knh_Token_t* RETURN_typing(CTX ctx, knh_Stmt_t *stmt)
{
	size_t size = DP(stmt)->size;
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_class_t this_cid = DP(ctx->gma)->this_cid;
	knh_type_t rtype = Gamma_type(ctx, knh_ParamArray_rtype(pa));
	if(size == 0 && MN_isNEW((mtd)->mn)) {
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_FUNCVAR, this_cid, 0);
		knh_Stmt_add(ctx, stmt, tk); size = 1;
	}
	if(size > 1) {
		WARN_Unsupported(ctx, "multi value returning");
		knh_Stmt_trimToSize(ctx, stmt, pa->rsize);
	}
	if(ParamArray_isRVAR(pa)) {
		DBG_ASSERT(pa->rsize == 0);
		if(size > 0) {
			TYPING(ctx, stmt, 0, TYPE_var, 0);
			Gamma_inferReturnType(ctx, Tn_type(stmt, 0));
		}
		else {
			Gamma_inferReturnType(ctx, TYPE_void);
		}
	}
	if(size == 0) {
		if(rtype != TYPE_void) {
			knh_Token_t *tk = new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, rtype, CLASS_t(rtype));
			knh_Stmt_add(ctx, stmt, tk);
			WARN_UseDefaultValue(ctx, "return", rtype);
		}
	}
	else if(Stmt_isImplicit(stmt)) { /*size > 0 */
		DBG_ASSERT(rtype == TYPE_dyn || rtype == TYPE_var);
		TYPING(ctx, stmt, 0, TYPE_var, _NOTCHECK);
		if(Tn_type(stmt, 0) != TYPE_void) {
			Stmt_setImplicit(stmt, 0);
		}
	}
	else { /* size > 0 */
		if(rtype == TYPE_void) {
			WARN_Ignored(ctx, "return value", CLASS_unknown, NULL);
			knh_Stmt_trimToSize(ctx, stmt, pa->rsize);
		}
		else {
			TYPING(ctx, stmt, 0, rtype, 0);
			if(STT_(stmtNN(stmt, 0)) == STT_CALL) {
				knh_Token_t *tkF = tkNN(stmtNN(stmt, 0), 0);
				if(DP(ctx->gma)->mtd == (tkF)->mtd) {
					Stmt_setTAILRECURSION(stmtNN(stmt, 0), 1);
				}
			}
		}
	}
	Stmt_setSTOPITR(stmt, 1);
	return Stmt_typed(ctx, stmt, rtype);
}

static knh_Token_t* YIELD_typing(CTX ctx, knh_Stmt_t *stmt)
{
	return ERROR_Unsupported(ctx, "statement", CLASS_unknown, "yield");
}

/* ------------------------------------------------------------------------ */

static knh_Stmt_t* StmtITR_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int *hasRETURN);

#define TYPING_STMTs(ctx, stmt, n, reqt, hasR) {\
		knh_Stmt_t *stmtRES_ = StmtITR_typing(ctx, stmtNN(stmt, n), reqt, hasR);\
		KNH_SETv(ctx, stmtNN(stmt, n), stmtRES_);\
	}\

static knh_Token_t* Stmt_toBLOCK(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int hasReturn)
{
	DBG_ASSERT(DP(stmt)->size > 0);
	STT_(stmt) = STT_BLOCK;
	KNH_SETv(ctx, tmNN(stmt, 0), tmNN(stmt, n));
	knh_Stmt_trimToSize(ctx, stmt, 1);
	Stmt_setSTOPITR(stmt, hasReturn);
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* IF_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	int hasReturnT = 0, hasReturnF = 0;
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTYPEMAP);
	if(Tn_isTRUE(stmt, 0)) {
		knh_Stmt_done(ctx, stmtNN(stmt, 2));
		TYPING_STMTs(ctx, stmt, 1, reqt, &hasReturnT);
		return Stmt_toBLOCK(ctx, stmt, 1, reqt, hasReturnT);
	}
	else if(Tn_isFALSE(stmt, 0)) {
		knh_Stmt_done(ctx, stmtNN(stmt, 1));
		TYPING_STMTs(ctx, stmt, 2, reqt, &hasReturnF);
		return Stmt_toBLOCK(ctx, stmt, 2, reqt, hasReturnF);
	}
	else {
		TYPING_STMTs(ctx, stmt, 1, reqt, &hasReturnT);
		TYPING_STMTs(ctx, stmt, 2, reqt, &hasReturnF);
		DBG_P("hasReturnT=%d, hasReturnF=%d", hasReturnT, hasReturnF);
		Stmt_setSTOPITR(stmt, (hasReturnT && hasReturnF));
	}
	return Stmt_typed(ctx, stmt, reqt);
}

static knh_Token_t *Tn_it(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t type)
{
	knh_Token_t *tkIT = tkNN(stmt, n);
	if(TT_(tkIT) != TT_LOCAL) {
		tkIT = Gamma_addLOCAL(ctx, 0, type, FN_, 1/*ucnt*/);
		KNH_SETv(ctx, tkNN(stmt, n), tkIT);
	}
	return tkIT;
}

static knh_Stmt_t* new_StmtCASE(CTX ctx, knh_Token_t *tkIT, knh_Token_t *tkC)
{
	knh_type_t switch_type = (tkIT)->type;
	knh_type_t case_type = (tkC)->type;
	knh_methodn_t mn = MN_opEQ;
	DBG_P("switch_type=%s, case_type=%s", TYPE__(switch_type), TYPE__(case_type));
	if(case_type == CLASS_Class) {  // case Object:
		mn = MN_opOF;
	}
	if(switch_type == CLASS_String && case_type == CLASS_Regex) {  // case /s/:
		mn = MN_opHAS;
	}
	return new_Stmt2(ctx, STT_OPR, new_TokenMN(ctx, mn), tkIT, tkC, NULL);
}

static knh_Token_t* SWITCH_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	int hasReturn = 1, hasReturnCASE = 0;
	TYPING(ctx, stmt, 0, TYPE_dyn, _NOTCHECK);
	if(TT_(tkNN(stmt, 0)) == TT_LOCAL) {
		KNH_SETv(ctx, tkNN(stmt, 2), tkNN(stmt, 0));
	}
	{
		BEGIN_BLOCK(esp);
		knh_type_t type = Tn_type(stmt, 0);
		knh_Token_t *tkIT = Tn_it(ctx, stmt, 2/*IT*/, type);
		int c = 0;
		knh_Stmt_t *stmtCASE, *stmtDEFAULT = NULL;
		stmtCASE = stmtNN(stmt, 1);
		while(stmtCASE != NULL) {
			if(STT_(stmtCASE) == STT_CASE) {
				Stmt_setESPIDX(ctx, stmtCASE);
				if(Tn_isASIS(stmtCASE, 0)) {
					if(stmtDEFAULT != NULL) {
						WarningDuplicatedDefault(ctx);
						knh_Stmt_done(ctx, stmtCASE);
						goto L_NEXT;
					}
					stmtDEFAULT = stmtCASE;
					goto L_STMT;
				}
				else if(!Tn_typing(ctx, stmtCASE, 0, type, _NOTCHECK)) {
					knh_Stmt_done(ctx, stmtCASE);
					goto L_NEXT;
				}
				if(Tn_isCONST(stmtCASE, 0)) {
					knh_Stmt_t *stmtOP = new_StmtCASE(ctx, tkIT, tkNN(stmtCASE, 0));
					KNH_SETv(ctx, tmNN(stmtCASE, 0), stmtOP);
					if(!Tn_typing(ctx, stmtCASE, 0, TYPE_Boolean, _NOTYPEMAP)) {
						knh_Stmt_done(ctx, stmtCASE);
						goto L_NEXT;
					}
				}
				else {
					WarningNotConstant(ctx);
					knh_Stmt_done(ctx, stmtCASE);
					goto L_NEXT;
				}
//				if(Tn_isCONST(stmtCASE, 0)){
//				}
				L_STMT:;
				TYPING_STMTs(ctx, stmtCASE, 1, reqt, &hasReturnCASE);
				hasReturn = (hasReturn && hasReturnCASE);
				c++;
			}
			L_NEXT:;
			stmtCASE = DP(stmtCASE)->nextNULL;
		}
		if(c == 0) {
			return knh_Stmt_done(ctx, stmt);
		}
		Stmt_setSTOPITR(stmt, hasReturn);
		END_BLOCK(esp);
	}
	return TM(stmt);
}

static knh_Token_t* WHILE_typing(CTX ctx, knh_Stmt_t *stmt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTYPEMAP);
	if(Tn_isFALSE(stmt, 0)) {
		return knh_Stmt_done(ctx, stmt);
	}
	else {
		int hasReturn = 0; // dummy
		TYPING_STMTs(ctx, stmt, 1, TYPE_STMT, &hasReturn);
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* DO_typing(CTX ctx, knh_Stmt_t *stmt)
{
	int hasReturn = 0; // dummy
	TYPING_STMTs(ctx, stmt, 0, TYPE_STMT, &hasReturn);
	TYPING(ctx, stmt, 1, TYPE_Boolean, _NOTYPEMAP);
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* FOR_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkRES;
	BEGIN_BLOCK(esp);
	tkRES = Stmt_typing(ctx, stmtNN(stmt, 0), TYPE_void);
	if(tkRES != NULL) {
		TYPING(ctx, stmt, 1, TYPE_Boolean, _NOTYPEMAP);
		if(Tn_isFALSE(stmt, 1)) {
			return tkNN(stmt, 0);
		}
		else {
			int hasReturn = 0; // dummy
			TYPING_STMTs(ctx, stmt, 2, TYPE_STMT, &hasReturn);
			TYPING_STMTs(ctx, stmt, 3, TYPE_STMT, &hasReturn);
		}
		tkRES = Stmt_typed(ctx, stmt, TYPE_void);
	}
	END_BLOCK(esp);
	return tkRES;
}

static knh_class_t FOREACH1_inferITR(CTX ctx, knh_Stmt_t *stmt, size_t n)
{
	knh_class_t cid = Tn_cid(stmt, n);
	if(knh_class_bcid(cid) != CLASS_Iterator) {
		knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, cid, MN_opITR);
		if(mtd != NULL) {
			knh_Stmt_t *stmtC = new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_opITR), tmNN(stmt, n), NULL);
			KNH_SETv(ctx, tmNN(stmt, n), CALL_typing(ctx, stmtC, TYPE_var));
			cid = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
		}
		else {
			KNH_TODO("NO opITR")
			return TYPE_Iterator;
		}
	}
	return cid;
}

static knh_Token_t* FOREACH1_typing(CTX ctx, knh_Stmt_t *stmt)
{
	BEGIN_BLOCK(esp);
	knh_Stmt_t *stmtDECL = stmtNN(stmt, 0);
	knh_class_t itrcid = CLASS_unknown;
	//DBG_P("tkT=%s", TT__(tkNN(stmtDECL, 0)->tt));
	knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtDECL, 0), TYPE_var);
	knh_Token_t *tkN = tkNN(stmtDECL, 1);
	knh_fieldn_t fn = Token_fn(ctx, tkN);
	//DBG_P("tkT->cid=%s", CLASS__(tkT->cid));
	if((tkT)->cid == TYPE_var) {
		knh_Token_t *tkN2 = TNAME_typing(ctx, tkN, TYPE_dyn, _FINDLOCAL | _FINDFIELD | _FINDSCRIPT | _USEDCOUNT);
		if(tkN2 == NULL) {
			knh_type_t iftype;
			TYPING(ctx, stmt, 1, TYPE_Iterator, _NOTCHECK);
			itrcid = FOREACH1_inferITR(ctx, stmt, 1);
			iftype = knh_class_p1(itrcid);
			KNH_SETv(ctx, tkNN(stmt, 0), Gamma_addLOCAL(ctx, 0, iftype, fn, 1/*ucnt*/));
			InfoType(ctx, "", TK_tobytes(tkN), iftype);
		}
		else {
			if(TT_(tkN2) != TT_LOCAL || TT_(tkN2) != TT_FUNCVAR) {
				KNH_SETv(ctx, tkNN(stmt, 0), Gamma_addLOCAL(ctx, 0, tkN2->type, fn, 1/*ucnt*/));
			}
			else {
				KNH_SETv(ctx, tkNN(stmt, 0), tkN);
			}
			itrcid = knh_class_P1(ctx, CLASS_Iterator, CLASS_t(tkN2->type));
		}
		TYPING(ctx, stmt, 1, itrcid, _ICAST);
	}
	else {
		KNH_SETv(ctx, tkNN(stmt, 0), Gamma_addLOCAL(ctx, 0, (tkT)->cid, fn, 1/*ucnt*/));
		itrcid = knh_class_P1(ctx, CLASS_Iterator, (tkT)->cid);
		TYPING(ctx, stmt, 1, itrcid, _ICAST);
	}
	Tn_it(ctx, stmt, 3, itrcid);
	int hasReturn = 0; // dummy
	TYPING_STMTs(ctx, stmt, 2, TYPE_STMT, &hasReturn);
	END_BLOCK(esp);
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* FOREACH_typing(CTX ctx, knh_Stmt_t *stmt)
{
	if(isSINGLEFOREACH(stmtNN(stmt, 0))) {
		return FOREACH1_typing(ctx, stmt);
	}
	else {
		KNH_TODO("multiple foreach");
	}
	return NULL;
}

static knh_Token_t* TRY_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	int hasReturn = 0;
	BEGIN_BLOCK(esp);
	knh_Stmt_t *stmtCATCH = stmtNN(stmt, 1);
	Tn_it(ctx, stmt, 3/*HDR*/, TYPE_ExceptionHandler);
	TYPING_STMTs(ctx, stmt, 0/*try*/, TYPE_STMT, &hasReturn);
	while(stmtCATCH != NULL) {
		if(STT_(stmtCATCH) == STT_CATCH) {
			BEGIN_BLOCK(esp2);
			knh_fieldn_t fn = Token_fn(ctx, tkNN(stmtCATCH, 1));
			knh_Token_t *tkIDX = Gamma_addLOCAL(ctx, 0, TYPE_Exception, fn, 1/*ucnt*/);
			KNH_SETv(ctx, tkNN(stmt, 1), tkIDX);
			Stmt_setESPIDX(ctx, stmtCATCH);
			TYPING_STMTs(ctx, stmtCATCH, 2, TYPE_STMT, &hasReturn);
			END_BLOCK(esp2);
		}
		stmtCATCH = DP(stmtCATCH)->nextNULL;
	}
	hasReturn = 0;
	TYPING_STMTs(ctx, stmt, 2/*finally*/, reqt, &hasReturn);
	Stmt_setSTOPITR(stmt, hasReturn);
	END_BLOCK(esp);
	return TM(stmt);
}

static knh_Token_t* THROW_typing(CTX ctx, knh_Stmt_t *stmt)
{
	TYPING(ctx, stmt, 0, TYPE_Exception, 0);
	Stmt_setSTOPITR(stmt, 1);
	return TM(stmt);
}

static knh_Token_t* PRINT_typing(CTX ctx, knh_Stmt_t *stmt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_dyn, _NOTCHECK);
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t *ASSURE_typing(CTX ctx, knh_Stmt_t *stmt)
{
	int hasReturn = 0; // dummy
	BEGIN_BLOCK(esp);
	TYPING(ctx, stmt, 0, TYPE_String, 0);
	{
		knh_Token_t *tkC = new_TokenTYPED(ctx, TT_CID, CLASS_Class, CLASS_Assurance);
		knh_Token_t *tkCALL = new_(Token);
		knh_Method_t* mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_Assurance, MN_new);
		Token_toCALLMTD(ctx, tkCALL, MN_new, mtd);
		knh_Stmt_t *stmtCALL = new_Stmt2(ctx, STT_NEW, tkCALL, tkC, tmNN(stmt, 0), NULL);
		KNH_SETv(ctx, tmNN(stmt, 0), stmtCALL);
		TYPING(ctx, stmt, 0, CLASS_Assurance, 0); // To make constant
	}
	Tn_it(ctx, stmt, 2/*VAL*/, Tn_type(stmt, 0));
	TYPING_STMTs(ctx, stmt, 1/*{}*/, TYPE_STMT, &hasReturn);
	END_BLOCK(esp);
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Token_t* ASSERT_typing(CTX ctx, knh_Stmt_t *stmt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTYPEMAP);
	if(Tn_isTRUE(stmt, 0)) {
		return knh_Stmt_done(ctx, stmt);
	}
	if(Tn_isFALSE(stmt, 0)) {
		WarningAlwaysFalseAssertion(ctx);
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

/* ------------------------------------------------------------------------ */
/* [METHOD] */

static knh_flag_t METHOD_flag(CTX ctx, knh_Stmt_t *o, knh_class_t cid)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		ADD_FLAG(flag, "Virtual", FLAG_Method_Virtual);
		ADD_FLAG(flag, "Private", FLAG_Method_Private);
		ADD_FLAG(flag, "Const", FLAG_Method_Const);
		ADD_FLAG(flag, "Static", FLAG_Method_Static);
		ADD_FLAG(flag, "Immutable", FLAG_Method_Static);
		if(class_isSingleton(cid)) flag |= FLAG_Method_Static;
		if(class_isImmutable(cid)) flag |= FLAG_Method_Immutable;
//		if(class_isDebug(cid)) flag |= FLAG_Method_Debug;
		ADD_FLAG(flag, "Debug", FLAG_Method_Debug);
	}
	return flag;
}

static knh_class_t METHOD_cid(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkC = tkNN(stmt, 1); DBG_ASSERT(IS_Token(tkC));
	knh_class_t this_cid = DP(ctx->gma)->this_cid;
	if(TT_(tkC) == TT_ASIS) {
		return this_cid;
	}
	else {
		knh_class_t cid = knh_Token_cid(ctx, tkC, DP(ctx->gma)->this_cid);
		if(this_cid != O_cid(K_GMASCR) && this_cid != cid) {
			cid = this_cid;
			WarningDifferentMethodClass(ctx, TK_tobytes(tkC), cid);
		}
		return cid;
	}
}

static knh_methodn_t METHOD_name(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkN = tkNN(stmt, 2); DBG_ASSERT(IS_Token(tkN));
	return (TT_(tkN) == TT_ASIS) ? MN_new : Token_mn(ctx, tkN);
}

#define Stmt_isAbstractMethod(stmt)   (DP(stmt)->size == 4)

static knh_Token_t* knh_StmtMTD_typing(CTX ctx, knh_Stmt_t *stmt, knh_Method_t *mtd, knh_class_t mtd_cid)
{
	DP(mtd)->uri = ULINE_uri(stmt->uline);
	Token_setCONST(ctx, tkNN(stmt, 2/*method*/), mtd);
	{
		knh_Fmethod func = NULL;
		func = Gamma_loadMethodFunc(ctx, mtd_cid, (mtd)->mn, knh_StmtMETA_is(ctx, stmt, "Native"));
		if(func != NULL) {
			knh_Method_setFunc(ctx, mtd, func);
			return knh_Stmt_done(ctx, stmt);
		}
	}
	if(Stmt_isAbstractMethod(stmt)) {
		return knh_Stmt_done(ctx, stmt);
	}
	return TM(stmt);
}

static knh_Stmt_t *knh_Stmt_clone(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Stmt_t *newstmt = new_Stmt2(ctx, STT_(stmt), NULL);
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Stmt_add(ctx, newstmt, tmNN(stmt, i), NULL);
	}
	KNH_SETv(ctx, DP(newstmt)->metaDictCaseMap, DP(stmt)->metaDictCaseMap);
	return newstmt;
}

static void* knh_lookupLibraryFunc(CTX ctx, knh_bytes_t libfunc)
{
	void *cfunc = NULL;
//	knh_index_t loc = knh_bytes_index(libfunc, ':');
//	if(loc != -1) {
//		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_bytes_t libname = knh_bytes_first(libfunc, loc);
//		knh_bytes_t funcname = knh_bytes_last(libfunc, loc+1);
//		knh_Bytes_write(ctx, cwb->ba, libname);
//		void *p = knh_path_dlopen(ctx, LOG_DEBUG, cwb);
//		if(p != NULL) {
//			cfunc = knh_dlsym(ctx, LOG_DEBUG, p, funcname.text);
//		}
//		knh_cwb_close(cwb);
//	}
	fprintf(stderr, "Hey!, Shinpei, If you want to use this, email me!\n");
	return cfunc;
}

static void* knh_fficonv(CTX ctx, knh_type_t type, knh_bytes_t t)
{
	return NULL;
}

static void set_ffireturn(CTX ctx, knh_ffiparam_t *p, knh_Method_t *mtd, knh_Token_t *tkF)
{
	p->type = 	knh_ParamArray_rtype(DP(mtd)->mp);
	p->sfpidx = -1;
	if(tkF != NULL) {
		p->conv_func = knh_fficonv(ctx, p->type, S_tobytes(tkF->text));
	}
}

static knh_bool_t set_ffiparam(CTX ctx, knh_ffiparam_t *p, knh_Method_t *mtd, knh_Token_t *tkN)
{
	knh_Token_t *tkF = NULL;
	if(TT_(tkN) == STT_FUNCCALL) {
		knh_Stmt_t *stmtF = (knh_Stmt_t*)tkN;
		tkN = tkNN(stmtF, 2);
		tkF = tkNN(stmtF, 0);
	}
	if(TT_(tkN) == TT_NAME) {
		knh_fieldn_t fn = Token_fn(ctx, tkN);
		if(fn == FN_this) {
			p->sfpidx = 0;
			p->type = mtd->cid;
			if(tkF != NULL) {
				p->conv_func = knh_fficonv(ctx, p->type, S_tobytes(tkF->text));
			}
		}
		else {
			size_t i;
			knh_ParamArray_t *pa = DP(mtd)->mp;
			for(i = 0; i < pa->psize; i++) {
				knh_param_t *mp = knh_ParamArray_get(pa, i);
				if(mp->fn == fn) {
					p->sfpidx = 1 + i;
					p->type = mp->type;
					if(tkF != NULL) {
						p->conv_func = knh_fficonv(ctx, p->type, S_tobytes(tkF->text));
					}
					return 1;
				}
			}
		}
	}
	return 0;
}

static void Method_linkFFI(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmt)
{
	int argc = 0;
	knh_ffiparam_t params[16] = {};
	knh_Token_t *tkF = NULL;
	if(STT_(stmt) != STT_FUNCCALL) goto L_ERROR;
	tkF = tkNN(stmt, 0);
	DBG_P("TT=%s, '%s'", TT__(tkF->tt), S_tochar(tkF->text));
	if(TT_(tkF) != TT_URN) {
		set_ffireturn(ctx, params, mtd, tkF);
		if(DP(stmt)->size != 3) {
			goto L_ERROR;
		}
		argc = 1;
		stmt = stmtNN(stmt, 2);
		tkF = tkNN(stmt, 0);
	}
	if(TT_(tkF) == TT_URN) {
		size_t i;
		void *cfunc = knh_lookupLibraryFunc(ctx, S_tobytes(tkF->text));
		if(cfunc == NULL) {
			goto L_ERROR;
		}
		if(argc == 0) {
			set_ffireturn(ctx, params + argc, mtd, NULL);
			argc = 1;
		}
		for(i = 2; (i < DP(stmt)->size && argc < 16); i++) {
			if(set_ffiparam(ctx, params + argc, mtd, tkNN(stmt, i))) {
				argc++;
			}
			else {
				goto L_ERROR;
			}
		}
		{
			knh_Fmethod fmethod = knh_makeFmethod(ctx, cfunc, argc, params);
			if(fmethod != NULL) {
				knh_Method_setFunc(ctx, mtd, fmethod);
				return;
			}
		}
	}
	L_ERROR:;
	DBG_P("ERROR");
}

static knh_Token_t* METHOD_typing(CTX ctx, knh_Stmt_t *stmtM, knh_type_t reqt)
{
	knh_class_t mtd_cid = METHOD_cid(ctx, stmtM);
	knh_flag_t flag   = METHOD_flag(ctx, stmtM, mtd_cid);
	knh_methodn_t mn = METHOD_name(ctx, stmtM);
	knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, mtd_cid, mn);
	knh_ParamArray_t *mp = NULL;
	knh_Stmt_t *stmtP = stmtNN(stmtM, 3/*PARAMs*/);
	int isDynamic = 0, allowDynamic = 1;
	if(StmtMETHOD_isFFI(stmtM) || knh_StmtMETA_is(ctx, stmtM, "Native")) {
		allowDynamic = 0;
	}
	if(mtd != NULL && (mtd)->mn < MN_OPSIZE) { /* op */
		WarningDeprecated(ctx, "operator overriding");
		mtd = NULL;
	}
	/* New method, and constructors are always new */
	if(mtd != NULL) {
		DBG_P("OVERRIDING %s.%s", CLASS__(mtd->cid), MN__(mtd->mn));
		if(mtd->cid != mtd_cid && Method_isPrivate(mtd)) {
			mtd = NULL;
		}
		if(mtd->cid != mtd_cid && mn == MN_new) {
			mtd = NULL;
		}
	}
	if(mtd != NULL) {  // overriding method
		size_t i;
		mp = DP(mtd)->mp;
		if((mtd)->cid != mtd_cid) { /* Overriding */
			if(Method_isFinal(mtd)) {
				return ErrorFinalMethod(ctx, (mtd)->cid, mn);
			}
		}
		if(DP(stmtP)->size / 3 != mp->psize) {
			return ErrorDifferentlyDefinedMethod(ctx, mtd_cid, mn);
		}
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			size_t n = i / 3;
			knh_param_t *p = knh_ParamArray_get(mp, n);
			knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, i+0), p->type);
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_Token_t *tkV = tkNN(stmtP, i+2);
			knh_type_t type = (tkT)->cid;
			if(type == TYPE_var) {  // inference
				if(TT_(tkV) == TT_ASIS) {
					type = Gamma_lookupVariableType(ctx, tkN, _VFINDTHIS | _VFINDSCRIPT);
				}
				else {
					TYPING(ctx, stmtP, i+2, TYPE_dyn, _NOTCHECK | _CONSTONLY);
					tkV = tkNN(stmtP, i+2);
					type = tkV->type;
				}
				if(type == TYPE_var) {
					if(allowDynamic == 0) {
						return ErrorStaticType(ctx, S_tochar(tkN->text));
					}
					isDynamic = 1;
				}
				else {
					knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, type);
					InfoType(ctx, "", TK_tobytes(tkN), type);
				}
			}
			else if(type != p->type) {
				return ErrorDifferentlyDefinedMethod(ctx, mtd_cid, mn);
			}
			else {
				TYPING(ctx, stmtP, i+2, type, _CONSTASIS);
				tkV = tkNN(stmtP, i+2);
			}
		}
		/* check return value */
		if(mp->rsize == 0) {
			knh_Token_t *tkR = TTYPE_typing(ctx, tkNN(stmtM, 0), TYPE_void);
			knh_type_t rtype = (tkR)->cid;
			if(rtype != TYPE_void) {
				return ErrorDifferentlyDefinedMethod(ctx, mtd_cid, mn);
			}
		}
		else {
			knh_param_t *p = knh_ParamArray_rget(mp, 0);
			knh_Token_t *tkR = TTYPE_typing(ctx, tkNN(stmtM, 0), p->type);
			knh_type_t rtype = (tkR)->cid;
			if(rtype != p->type) {
				return ErrorDifferentlyDefinedMethod(ctx, mtd_cid, mn);
			}
		}
		if((mtd)->cid != mtd_cid) { /* Override */
			mtd = new_Method(ctx, flag, mtd_cid, mn, NULL);
			knh_NameSpace_addMethod(ctx, mtd_cid, mtd);
			KNH_SETv(ctx, DP(mtd)->mp, mp);
		}
	}
	else {  // newly defined method
		size_t i;
		mtd = new_Method(ctx, flag, mtd_cid, mn, NULL);
		knh_NameSpace_addMethod(ctx, mtd_cid, mtd);
		mp = new_ParamArray(ctx);
		KNH_SETv(ctx, DP(mtd)->mp, mp);
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, i+0), TYPE_var);
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_Token_t *tkV = tkNN(stmtP, i+2);
			knh_type_t type = (tkT)->cid;
			if(type == TYPE_var) {  // inference
				if(TT_(tkV) == TT_ASIS) {
					type = Gamma_lookupVariableType(ctx, tkN, _VFINDTHIS | _VFINDSCRIPT);
				}
				else {
					TYPING(ctx, stmtP, i+2, TYPE_dyn, _NOTCHECK | _CONSTONLY);
					tkV = tkNN(stmtP, i+2);
					type = tkV->type;
				}
				if(type == TYPE_var) {
					if(allowDynamic == 0) {
						return ErrorStaticType(ctx, S_tochar(tkN->text));
					}
					isDynamic = 1;
				}
				else {
					knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, type);
					InfoType(ctx, "", TK_tobytes(tkN), type);
				}
			}
			else {
				TYPING(ctx, stmtP, i+2, type, _CONSTASIS);
				tkV = tkNN(stmtP, i+2);
			}
			{
				knh_param_t p = {type, Token_fn(ctx, tkN)};
				knh_ParamArray_add(ctx, mp, p);
			}
		}
		{
			knh_Token_t *tkR = TTYPE_typing(ctx, tkNN(stmtM, 0), TYPE_var);
			knh_type_t rtype = (tkR)->cid;
			if(rtype == TYPE_var) {
				ParamArray_setRVAR(mp, 1);
			}
			else if(rtype != TYPE_void) {
				knh_param_t p = {rtype, 0};
				knh_ParamArray_radd(ctx, mp, p);
			}
		}
	}
	DBG_({
		size_t i;
		DBG_P("mp->psize=%d, rsize=%d", mp->psize, mp->rsize);
		for(i = 0; i < mp->psize; i++) {
			knh_param_t *p = knh_ParamArray_get(mp, i);
			DBG_P("param type=%s fn=%s", TYPE__(p->type), FN__(p->fn));
		}
		for(i = 0; i < mp->rsize; i++) {
			knh_param_t *p = knh_ParamArray_rget(mp, i);
			DBG_P("return type=%s, idx=%d", TYPE__(p->type), p->fn);
		}
	});
	if(isDynamic == 1) {
		DBG_P("************************* dynamic ****************************** refc=%d", ctx->gma->h.refc);
		Method_setDynamic(mtd, 1);
		if(!Stmt_isAbstractMethod(stmtM)) {
			knh_Gamma_t *newgma = Gamma_clone(ctx);
			KNH_SETv(ctx, DP(newgma)->stmt, knh_Stmt_clone(ctx, stmtM));
			if(Method_isObjectCode(mtd)) {
				KNH_SETv(ctx, DP(mtd)->gma, newgma);
			}
			else {
				KNH_INITv( DP(mtd)->gma, newgma);
				Method_setObjectCode(mtd, 1);
			}
		}
	}
	if(StmtMETHOD_isFFI(stmtM)) {
		knh_Stmt_t *stmtD = stmtNN(stmtM, 4/*using DSL*/);
		Method_linkFFI(ctx, mtd, stmtD);
		return knh_Stmt_done(ctx, stmtM);
	}
	return knh_StmtMTD_typing(ctx, stmtM, mtd, mtd_cid);
}

static knh_Token_t* FUNCTION_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	//knh_Token_t *tkF = tkNN(stmt, 0);
	knh_Stmt_t *stmtP = stmtNN(stmt, 0);
	knh_Method_t *mtd = new_Method(ctx, 0, DP(ctx->gma)->this_cid, MN_, NULL);
	knh_ParamArray_t *mp = new_ParamArray(ctx);
	KNH_SETv(ctx, DP(mtd)->mp, mp);
	//KNH_SETv(ctx, (tkF)->data, mtd);
	DBG_ASSERT(STT_(stmtP) == STT_DECL);
	if(DP(ctx->gma)->parentNULL != NULL) {
		return ERROR_Unsupported(ctx, "nested function", CLASS_unknown, NULL);
	}
	if(IS_Tfunc(reqt)) {
		knh_ParamArray_t *cpm = new_ParamArray(ctx);
		if(DP(stmtP)->size / 3 != cpm->psize) {
			return ErrorDifferentlyDefinedMethod(ctx, (mtd)->cid, (mtd)->mn);
		}
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			size_t n = i / 3;
			knh_param_t *p = knh_ParamArray_get(cpm, n);
			knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, i+0), p->type);
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_Token_t *tkV = tkNN(stmtP, i+2);
			knh_type_t type = (tkT)->cid;
			if(type == TYPE_var) {  // inference
				type = p->type;
				knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, type);
				InfoType(ctx, "", TK_tobytes(tkN), type);
			}
			if(type != p->type) {
				return ErrorDifferentlyDefinedMethod(ctx, (mtd)->cid, (mtd)->mn);
			}
			else {
				TYPING(ctx, stmtP, i+2, type, _CONSTASIS);
				tkV = tkNN(stmtP, i+2);
			}
			{
				knh_param_t p = {type, Token_fn(ctx, tkN)};
				knh_ParamArray_add(ctx, mp, p);
			}
		}
		if(cpm->rsize > 0) {
			knh_param_t *p = knh_ParamArray_rget(cpm, 0);
			if(p->type != TYPE_void) {
				knh_param_t r = {p->type, FN_};
				knh_ParamArray_radd(ctx, mp, r);
			}
		}
	}
	else {
		knh_ParamArray_t *mp = new_ParamArray(ctx);
		KNH_SETv(ctx, DP(mtd)->mp, mp);
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, i+0), TYPE_var);
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_Token_t *tkV = tkNN(stmtP, i+2);
			knh_type_t type = (tkT)->cid;
			if(type == TYPE_var) {  // inference
				if(TT_(tkV) == TT_ASIS) {
					type = Gamma_lookupVariableType(ctx, tkN, _VFINDTHIS | _VFINDSCRIPT);
				}
				else {
					TYPING(ctx, stmtP, i+2, TYPE_dyn, _NOTCHECK | _CONSTONLY);
					tkV = tkNN(stmtP, i+2);
					type = tkV->type;
				}
				if(type == TYPE_var) {
					KNH_TODO("var in function"); //isDynamic = 1;
				}
				else {
					knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, type);
					InfoType(ctx, "", TK_tobytes(tkN), type);
				}
			}
			else {
				TYPING(ctx, stmtP, i+2, type, _CONSTASIS);
				tkV = tkNN(stmtP, i+2);
			}
			{
				knh_param_t p = {type, Token_fn(ctx, tkN)};
				knh_ParamArray_add(ctx, mp, p);
			}
		}
		ParamArray_setRVAR(mp, 1);
	}
	{
		knh_Token_t *tkM = new_TokenCONST(ctx, mtd);
		knh_Gamma_t *lgma = Gamma_clone(ctx);  DP(lgma)->flag = 0;
		KNH_INITv(DP(lgma)->parentNULL, ctx->gma);
		KNH_SETv(ctx, ((knh_context_t*)ctx)->gma, lgma);
		knh_Method_asm(ctx, mtd, stmtP, TYPE_void, stmtNN(stmt, 2), knh_Method_typing);
		KNH_SETv(ctx, tkNN(stmt, 0), tkM);
		reqt = class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
		((knh_context_t*)ctx)->gma = DP(lgma)->parentNULL;
		if(Gamma_hasXLOCAL(lgma)) {
			knh_Token_t *tkIDX = Gamma_addFUNCVAR(ctx, _FCHKOUT, reqt, NULL, 1);
			KNH_SETv(ctx, tkNN(stmt, 1), tkIDX);
		}
		else {
			if(Gamma_hasFIELD(lgma)) {
				STT_(stmt) = STT_NEW;
				KNH_SETv(ctx, tkNN(stmt, 1), new_TokenTYPED(ctx, TT_CID, TYPE_Class, reqt));
				KNH_SETv(ctx, tkNN(stmt, 2), new_TokenTYPED(ctx, TT_FUNCVAR, DP(ctx->gma)->this_cid, 0));
				knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, mtd));
				mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_Func, MN_new);
				DBG_ASSERT(mtd != NULL);
				Token_toCALLMTD(ctx, tkNN(stmt, 0), (mtd)->mn, mtd);
			}
			else {
				KNH_FINALv(ctx, lgma);
				return new_TokenCONST(ctx, new_StaticFunc(ctx, reqt, mtd));
			}
		}
		KNH_FINALv(ctx, lgma);
		return Stmt_typed(ctx, stmt, reqt);
	}
}

/* ------------------------------------------------------------------------ */

static knh_class_t knh_StmtPARAMs_getFmtClass(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid = CLASS_unknown;
	DBG_ASSERT(STT_(stmt) == STT_DECL);
	if(DP(stmt)->size == 3) {
		knh_Token_t *tkT = tkNN(stmt, 0);
		cid = knh_Token_cid(ctx, tkT, CLASS_Tdynamic);
		return cid;
	}
	KNH_TODO("formatter method takes one parameter");
	//knh_Gamma_perror(ctx, KC_ERR, _("formatter method takes one parameter"));
	return cid;
}

static knh_Token_t* FORMAT_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_class_t cid = knh_StmtPARAMs_getFmtClass(ctx, stmtNN(stmt, 3));
	knh_methodn_t mn = MN_toFMT(Token_mn(ctx, tkNN(stmt, 2)));
	if(cid != CLASS_unknown) {
		knh_Method_t *mtd = knh_NameSpace_getFmtNULL(ctx, K_GMANS, cid, mn);
		if(mtd == NULL) {
			mtd = new_Method(ctx, 0, cid, mn, NULL);
			KNH_SETv(ctx, DP(mtd)->mp, KNH_TNULL(ParamArray));
			knh_NameSpace_addFmt(ctx, K_GMANS, mtd);
		}
		return knh_StmtMTD_typing(ctx, stmt, mtd, cid);
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

#define CASE_STMT(XX, ...) case STT_##XX : { \
		tkRES = XX##_typing(ctx, ## __VA_ARGS__); \
		break;\
	}\

static int Gamma_initClassTBLField(CTX ctx, knh_class_t cid)
{
	const knh_ClassTBL_t *t = ClassTBL(cid);
	if(t->bcid == CLASS_Object && t->subclass == 0) {
		size_t i;
		knh_gmafields_t *gf = DP(ctx->gma)->gf;
		DP(ctx->gma)->flag = 0;
		DP(ctx->gma)->this_cid = cid;
		if(!(t->fsize < DP(ctx->gma)->gcapacity)) {
			gf = Gamma_expand(ctx, ctx->gma, t->fsize);
		}
		Gamma_clear(ctx, 0, NULL);
		for(i = 0; i < t->fsize; i++) {
			gf[i].flag = t->fields[i].flag;
			gf[i].type = t->fields[i].type;
			gf[i].fn = t->fields[i].fn;
			gf[i].ucnt = 1;
		}
		DP(ctx->gma)->espidx = t->fsize;
		DP(ctx->gma)->gsize = t->fsize;
		return 1;
	}
	return 0;
}

static knh_Token_t *DECLFIELD_typing(CTX ctx, knh_Stmt_t *stmt, size_t i)
{
	knh_flag_t flag  = DECL_flag(ctx, stmt, _FGETTER | _FSETTER);
	knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmt, i+0), TYPE_var);
	knh_Token_t *tkN = tkNN(stmt, i+1);
	knh_Token_t *tkV = tkNN(stmt, i+2);
	if(FLAG_is(flag, _FREADONLY)) {
		flag |= _FGETTER;
	}
	else {
		flag = flag | _FGETTER | _FSETTER;
	}
	if((tkT)->cid == TYPE_var) {
		knh_type_t iftype;
		if(TT_(tkV) == TT_ASIS) {
			iftype = Gamma_lookupVariableType(ctx, tkN, _VFINDSCRIPT);
			if(iftype == TYPE_var) {
				return ErrorStaticType(ctx, TK__(tkN));
			}
		}
		else {
			TYPING(ctx, stmt, i+2, TYPE_var, _NOTCHECK | _CONSTONLY);
			tkV = tkNN(stmt, i+2);
			iftype = tkV->type;
		}
		knh_Token_toTYPED(ctx, tkT, TT_CID, CLASS_Class, iftype);
		InfoType(ctx, "", TK_tobytes(tkN), iftype);
	}
	else {
		TYPING(ctx, stmt, i+2, (tkT)->cid, _CONSTASIS);
		tkV = tkNN(stmt, i+2);
	}
	return Gamma_add(ctx, flag, tkT, tkN, tkV, GF_UNIQUE|GF_FIELD|GF_USEDCOUNT);
}

static knh_fields_t *ClassTBL_expandFields(CTX ctx, knh_ClassTBL_t *t)
{
	size_t newsize = (t->fcapacity == 0) ? sizeof(knh_Object_t) / sizeof(knh_fields_t) : t->fcapacity * 2;
	t->fields = (knh_fields_t*)KNH_REALLOC(ctx, "fields", t->fields, t->fcapacity, newsize, sizeof(knh_fields_t));
	t->fcapacity = newsize;
	return t->fields;
}

void knh_class_addField(CTX ctx, knh_class_t bcid, knh_flag_t flag, knh_type_t type, knh_fieldn_t fn)
{
	knh_ClassTBL_t *t = varClassTBL(bcid);
	knh_fields_t *cf = t->fields;
	size_t n = t->fsize;
	if(t->fcapacity == n) {
		cf = ClassTBL_expandFields(ctx, t);
	}
	cf[n].flag = flag;
	cf[n].fn   = fn;
	cf[n].type = type;
	cf[n].israw = (type == TYPE_void || IS_Tunbox(type)) ? 1 : 0;
	t->fsize += 1;
	DBLNDATA_(if(IS_Tunbox(type)) {
		n = t->fsize;
		if(t->fcapacity == n) {
			cf = ClassTBL_expandFields(ctx, t);
		}
		cf[n].fn   = FN_;
		cf[n].type = TYPE_void;
		cf[n].israw = 1;
		t->fsize += 1;
	})
}

void knh_ObjectField_expand(CTX ctx, knh_ObjectField_t *of, size_t oldsize, size_t newsize)
{
	knh_Object_t **oldf = of->fields;
	if(newsize < K_SMALLOBJECT_FIELDSIZE) {
		of->fields = &(of->smallobject);
		if(oldsize == 0) {
			of->fields[0] = of->fields[1] = of->fields[2] = NULL;
		}
	}
	else if(oldsize != 0 && oldsize < K_SMALLOBJECT_FIELDSIZE) {
		knh_Object_t** newf = (knh_Object_t**)KNH_MALLOC(ctx, newsize*sizeof(Object*));
		knh_memcpy(newf, oldf, oldsize*sizeof(Object*));
		knh_bzero(newf+oldsize, (newsize-oldsize)*sizeof(Object*));
		of->fields = newf;
	}
	else {
		of->fields = (knh_Object_t**)KNH_REALLOC(ctx, "ObjectField", oldf, oldsize, newsize, sizeof(Object*));
	}
	//DBG_P("fields=%p => %p, size=%d => %d", oldf, of->fields, oldsize, newsize);
}

static void ObjectField_add(CTX ctx, knh_ObjectField_t *of, size_t i, knh_type_t type, knh_Object_t *v)
{
	knh_class_t cid = knh_type_tocid(ctx, type, O_cid(of));
	knh_class_t bcid = knh_class_bcid(cid);
	//DBG_P("i=%d type=%s bcid=%s object_type=%s", i, CLASS__(cid), CLASS__(bcid), CLASS__(O_cid(v)));
	switch(bcid) {
		case CLASS_Boolean: {
			knh_ndata_t *nn = (knh_ndata_t*)(of->fields + i);
			nn[0] = IS_bBoolean(v) ? N_tobool(v) : 0;
			break;
		}
		case CLASS_Int: {
			knh_int_t *nn = (knh_int_t*)(of->fields + i);
			nn[0] = IS_bInt(v) ? N_toint(v) : 0;
			break;
		}
		case CLASS_Float: {
			knh_float_t *nn = (knh_float_t*)(of->fields + i);
			nn[0] = IS_bFloat(v) ? N_tofloat(v) : K_FLOAT_ZERO;
			break;
		}
		case CLASS_Tvoid:
			if(cid == CLASS_Tvoid) return ;
		default: {
			if(IS_NOTNULL(v)) {
				DBG_ASSERT(cid == O_cid(of));
				KNH_INITv(of->fields[i], v);
			}
			else {
				KNH_INITv(of->fields[i], KNH_NULVAL(cid));
			}
			DBG_ASSERT(of->fields[i] != NULL);
		}
	}
}

static void Gamma_declareClassField(CTX ctx, knh_class_t cid, size_t s)
{
	knh_ClassTBL_t *ct = varClassTBL(cid);
	size_t i, gsize = DP(ctx->gma)->gsize;
	DBG_ASSERT(O_cid(ct->defnull) == cid);
	if(s < gsize) {
		knh_gmafields_t *gf = DP(ctx->gma)->gf;
		size_t fsize = ct->fsize;
		for(i = s; i < gsize; i++) {
			//DBG_P("FIELD type=%s fn=%s", TYPE__(gf[i].type), FN__(gf[i].fn));
			knh_class_addField(ctx, cid, gf[i].flag, gf[i].type, gf[i].fn);
		}
		//DBG_P("fsize(old)=%d, t->fsize=%d, t->fcapacity=%d", fsize, ct->fsize, ct->fcapacity);
		if(fsize < ct->fsize) {
			size_t fi = 0;
			knh_ObjectField_expand(ctx, ct->protoNULL, fsize, ct->fsize);
			for(i = s; i < gsize; i++) {
				ObjectField_add(ctx, ct->protoNULL, fsize + fi, gf[i].type, gf[i].value);
				fi++;
				DBLNDATA_(if(IS_Tunbox(gf[i].type)) fi++;)
			}
			knh_ObjectField_expand(ctx, ct->defobj, fsize, ct->fsize);
			fi = 0;
			for(i = s; i < gsize; i++) {
				ObjectField_add(ctx, ct->defobj, fsize + fi, gf[i].type, gf[i].value);
				fi++;
				DBLNDATA_(if(IS_Tunbox(gf[i].type)) fi++;)
			}
		}
	}
	knh_ClassTBL_setObjectCSPI(ct);
	Gamma_clear(ctx, 0, NULL);
}

static knh_Token_t* CLASS_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkC = tkNN(stmt, 0);
	knh_class_t this_cid = (tkC)->cid;
	if(DP(stmt)->size == 5) {
		knh_Stmt_t *stmtFIELD = knh_Token_parseStmt(ctx, tkNN(stmt, 4));
		KNH_SETv(ctx, stmtNN(stmt, 4), stmtFIELD);
	}
	if(Gamma_initClassTBLField(ctx, this_cid)) {
		size_t s = DP(ctx->gma)->gsize;
		if(TT_(tkNN(stmt, 1)) != TT_ASIS) {
			knh_Stmt_t *stmtP = stmtNN(stmt, 1);
			size_t i;
			for(i = 0; i < DP(stmtP)->size; i += 3) {
				knh_Token_t *tkRES = DECLFIELD_typing(ctx, stmtP, i);
				if(TT_(tkRES) == TT_ERR) return tkRES;
			}
		}
		if(DP(stmt)->size == 5) {
			knh_Stmt_t *stmtFIELD = stmtNN(stmt, 4/*instmt*/);
			while(stmtFIELD != NULL) {
				knh_Token_t *tkRES = NULL;
				ctx->gma->uline = stmtFIELD->uline;
				switch(STT_(stmtFIELD)) {
					CASE_STMT(DECLFIELD, stmtFIELD, 0);
					CASE_STMT(LET, stmtFIELD, TYPE_void);
				}
				stmtFIELD = DP(stmtFIELD)->nextNULL;
			}
		}
		Gamma_declareClassField(ctx, this_cid, s);
	}
	else {
		return ErrorFieldAddition(ctx, this_cid);
	}
	if(TT_(tkNN(stmt, 1)) != TT_ASIS) {
		knh_Stmt_t *stmtP = stmtNN(stmt, 1);
		size_t i;
		knh_index_t idx = -1;
		knh_Stmt_t *stmtHEAD = NULL, *stmtTAIL = NULL;
		knh_Method_t *mtd = new_Method(ctx, 0, this_cid, MN_new, NULL);
		knh_ParamArray_t *pa = new_ParamArray(ctx);
		KNH_SETv(ctx, DP(mtd)->mp, pa);
		knh_ClassTBL_addMethod(ctx, ClassTBL(this_cid), mtd, 0/*isCHECK*/);
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			size_t n = (i/3) + 1;
			knh_type_t type = (tkNN(stmtP, i))->cid;
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_fieldn_t fn = Token_fn(ctx, tkN);
			knh_param_t p = {type, fn};
			knh_ParamArray_add(ctx, pa, p);
			idx = -1;
			class_rindexFNQ(ctx, this_cid, fn, &idx);
			DBG_ASSERT(idx != -1);
			knh_Token_toTYPED(ctx, tkN, TT_FUNCVAR, type, n);
			{
				knh_Token_t *tkFLD = new_TokenTYPED(ctx, TT_FIELD, type, idx);
				knh_Stmt_t *stmtLET = new_Stmt2(ctx, STT_LET, tkNN(stmtP, i), tkFLD, tkN, NULL);
				if(stmtTAIL == NULL) {
					stmtHEAD = stmtTAIL = stmtLET;
				}
				else {
					KNH_INITv(DP(stmtTAIL)->nextNULL, stmtLET);
					stmtTAIL = stmtLET;
				}
				stmtLET->type = TYPE_void;
			}
		}
		{
			knh_param_t p = {TYPE_This, FN_return};
			knh_ParamArray_radd(ctx, pa, p);
		}
		KNH_INITv(DP(stmtTAIL)->nextNULL,
			new_Stmt2(ctx, STT_RETURN, new_TokenTYPED(ctx, TT_FUNCVAR, this_cid, 0), NULL));
		KNH_SETv(ctx, stmtNN(stmt, 1), stmtHEAD);
		knh_Method_asm(ctx, mtd, NULL, TYPE_void, stmtHEAD, knh_Method_typing);
	}
	if(DP(stmt)->size == 5) {
		knh_Stmt_t *stmtFIELD = stmtNN(stmt, 4/*instmt*/);
		while(stmtFIELD != NULL) {
			knh_Token_t *tkRES = NULL;
			ctx->gma->uline = stmtFIELD->uline;
			switch(STT_(stmtFIELD)) {
				CASE_STMT(METHOD, stmtFIELD, TYPE_void);
				CASE_STMT(FORMAT, stmtFIELD, TYPE_void);
				case STT_DONE: case STT_DECL: case STT_LET: case STT_ERR: break;
				default: {
					WarningMisplaced(ctx /*TT__(STT_(stmtFIELD))*/);
				}
			}
			stmtFIELD = DP(stmtFIELD)->nextNULL;
		}
	}
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */
/* [REGISTER] */

static knh_Token_t* REGISTER_typing(CTX ctx, knh_Stmt_t *stmt)
{
	size_t i;
	knh_Stmt_t *stmtTAIL = stmt;
	if(DP(stmtTAIL)->nextNULL == NULL) {
		knh_Stmt_done(ctx, stmt);
	}
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_var, 0);
		knh_Stmt_t *stmtREG = stmtNN(stmt, i);
		if(IS_Stmt(stmtREG) && stmtREG->type != TYPE_var) {
			knh_index_t idx = DP(ctx->gma)->gsize;
			knh_gmafields_t *gf = DP(ctx->gma)->gf;
			knh_Token_t *tkIDX = new_TokenTYPED(ctx, TT_LOCAL, stmtREG->type, idx - DP(ctx->gma)->ebpidx);
			if(!(idx < DP(ctx->gma)->gcapacity)) {
				gf = Gamma_expand(ctx, ctx->gma, /*minimum*/4);
			}
			gf[idx].flag  = _FREG;
			gf[idx].type  = stmtREG->type;
			gf[idx].fn    = FN_;
			gf[idx].ucnt  = 1;
			KNH_SETv(ctx, gf[idx].tkIDX, tkIDX);
			KNH_SETv(ctx, gf[idx].value, stmtREG);
			DP(ctx->gma)->espidx += 1;
			DP(ctx->gma)->gsize += 1;
			DBG_ASSERT(DP(ctx->gma)->gsize == DP(ctx->gma)->ebpidx + DP(ctx->gma)->espidx);
			Gamma_foundREGISTER(ctx->gma, 1);
			{
				knh_Stmt_t *stmtLET = new_Stmt2(ctx, STT_LET, new_(Token), tkIDX, stmtREG, NULL);
				Stmt_setESPIDX(ctx, stmtLET);
				Stmt_typed(ctx, stmtLET, TYPE_void);
				KNH_INITv(DP(stmtLET)->nextNULL, DP(stmtTAIL)->nextNULL);
				KNH_SETv(ctx, DP(stmtTAIL)->nextNULL, stmtLET);
				stmtTAIL = stmtLET;
			}
			DBG_P("******* REGISTER %s _%d(gsize=%d) TT=%s ******", TYPE__(tkIDX->type), idx, DP(ctx->gma)->gsize, TT__(stmtREG->stt));
		}
	}
	return Stmt_typed(ctx, stmt, TYPE_void);
}

static int Token_equals(CTX ctx, knh_Token_t *tk, knh_Token_t *tk2)
{
	switch(TT_(tk)) {
		case TT_CONST: {
			return (knh_Object_compareTo((tk)->data, (tk2)->data) == 0);
		}
		default: {
			//DBG_P("TT=%s", TT__(TT_(tk2)));
			if((tk)->index != (tk2)->index) return 0;
		}
	}
	return 1;
}

static knh_bool_t Stmt_equals(CTX ctx, knh_Stmt_t *stmt, knh_Stmt_t *stmtA)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Term_t *t1 = tmNN(stmt, i);
		knh_Term_t *t2 = tmNN(stmtA, i);
		if(TT_(t1) == TT_(t2) && t1->type == t2->type) {
			if(IS_Stmt(t1)) {
				knh_Stmt_t *stmt1 = (knh_Stmt_t*)t1;
				knh_Stmt_t *stmt2 = (knh_Stmt_t*)t2;
				if(DP(stmt1)->size == DP(stmt2)->size) {
					if(!Stmt_equals(ctx, stmt1, stmt2)) return 0;
				}
			}
			else {
				if(!Token_equals(ctx, (knh_Token_t*)t1, (knh_Token_t*)t2)) return 0;
			}
		}
	}
	return 1;
}

static knh_Token_t *Gamma_findRegExpr(CTX ctx, knh_Stmt_t *stmt)
{
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	size_t i = DP(ctx->gma)->ebpidx, size = DP(ctx->gma)->gsize;
	for(; i < size; i++) {
		knh_Stmt_t *stmtREG = (knh_Stmt_t*)gf[i].value;
		if(FLAG_is(gf[i].flag, _FREG) /*&& IS_Stmt(stmtREG)*/ && STT_(stmt) == STT_(stmtREG) && stmt->type == stmtREG->type
			&& DP(stmt)->size == DP(stmtREG)->size && Stmt_equals(ctx, stmt, stmtREG)) {
			DBG_P("*** FOUND REG %s _%d TT=%s", TYPE__(stmtREG->type), (gf[i].tkIDX)->index, TT__(stmtREG->stt));
			return new_TokenTYPED(ctx, TT_LOCAL, stmtREG->type, (gf[i].tkIDX)->index);
		}
	}
	return TM(stmt);
}

static knh_Token_t* BLOCK_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	int hasReturn = 0;
	knh_Stmt_t *stmtB = stmtNN(stmt, 0);
	TYPING_STMTs(ctx, stmt, 0, reqt, &hasReturn);
	Stmt_setSTOPITR(stmtB, hasReturn);
	return (knh_Token_t*)stmtNN(stmt, 0);
}

static knh_Token_t *Stmt_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkRES = NULL;
	if(Stmt_isTyped(stmt)) return TM(stmt);
	if(reqt == TYPE_TEXT) reqt = TYPE_String;
	if(stmt_isExpr(STT_(stmt))) {
		tkRES = EXPR_typing(ctx, stmt, reqt);
		if(Gamma_hasREGISTER(ctx->gma) && IS_Stmt(tkRES)) {
			tkRES = Gamma_findRegExpr(ctx, (knh_Stmt_t*)tkRES);
		}
	}
	else {
		switch(STT_(stmt)) {
			CASE_STMT(BLOCK, stmt, reqt);
			CASE_STMT(DECL, stmt);
			CASE_STMT(LETM, stmt);
			CASE_STMT(SWAP, stmt);
			CASE_STMT(IF, stmt, reqt);
			CASE_STMT(SWITCH, stmt, reqt);
			CASE_STMT(WHILE, stmt);
			CASE_STMT(DO, stmt);
			CASE_STMT(FOR, stmt);
			CASE_STMT(FOREACH, stmt);
			CASE_STMT(TRY, stmt, reqt);
			CASE_STMT(THROW, stmt);
			CASE_STMT(RETURN, stmt);
			CASE_STMT(YIELD, stmt);
			CASE_STMT(PRINT, stmt);
			CASE_STMT(REGISTER, stmt);
			CASE_STMT(ASSURE, stmt);
			CASE_STMT(ASSERT, stmt);
			case STT_ERR:
			case STT_BREAK:
			case STT_CONTINUE:
			case STT_DONE:
				tkRES = Stmt_typed(ctx, stmt, TYPE_void);
				break;
			default:
				tkRES = ERROR_OnlyTopLevel(ctx, TT__(STT_(stmt)));
		}
	}
	return tkRES;
}

static knh_Stmt_t *StmtITR_typing(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int *hasRETURN)
{
	knh_Stmt_t *stmtITR = stmt;
	BEGIN_BLOCK(espidx);
	DBG_ASSERT(IS_Stmt(stmtITR));
	while(stmtITR != NULL) {
		knh_Token_t *tkRES;
		ctx->gma->uline = stmtITR->uline;
		tkRES = Stmt_typing(ctx, stmtITR, TYPE_STMT);
		Stmt_setESPIDX(ctx, stmtITR);
		if(TT_(tkRES) == TT_ERR) {
			knh_Stmt_toERR(ctx, stmtITR, tkRES);
			if(DP(stmtITR)->nextNULL != NULL) {
				KNH_FINALv(ctx, DP(stmtITR)->nextNULL);
			}
			stmt = stmtITR;
			goto L_RETURN;
		}
		if(IS_Token(tkRES)) {
			WarningNoEffect(ctx);
			knh_Stmt_done(ctx, stmtITR);
		}
		if(Stmt_isSTOPITR(stmtITR)) {
			if(DP(stmtITR)->nextNULL != NULL) {
				KNH_FINALv(ctx, DP(stmtITR)->nextNULL);
			}
			*hasRETURN = 1;
			break;
		}
		if(reqt == TYPE_Object && DP(stmtITR)->nextNULL == NULL) {
			knh_Stmt_t *stmtRETURN = new_Stmt2(ctx, STT_RETURN, NULL);
			KNH_INITv(DP(stmtITR)->nextNULL, stmtRETURN);
		}
		stmtITR = DP(stmtITR)->nextNULL;
	}
	L_RETURN:;
	END_BLOCK(espidx);
	return stmt;
}

static void Gamma_initThis(CTX ctx, knh_class_t cid)
{
	knh_gmafields_t *gf = DP(ctx->gma)->gf;
	Gamma_clear(ctx, 0, NULL);
	DP(ctx->gma)->psize = 0;
	DBG_ASSERT_cid(cid);
	DP(ctx->gma)->this_cid = cid;
	gf[0].flag = 0;
	gf[0].type = TYPE_cid(cid);
	gf[0].fn   = FN_this;
	gf[0].ucnt = 1;
	KNH_SETv(ctx, gf[0].tkIDX, KNH_TNULL(Token));
	DBG_ASSERT(TT_(gf[0].tkIDX) == TT_FUNCVAR);
	DBG_ASSERT((gf[0].tkIDX)->index == 0);
	DP(ctx->gma)->gsize  = 1;
	DP(ctx->gma)->ebpidx = 1;
	DP(ctx->gma)->espidx = 0;
}

#define CASE_STMT2(XX, ...) case STT_##XX : { \
		Gamma_initThis(ctx, this_cid); \
		tkRES = XX##_typing(ctx, ## __VA_ARGS__); \
		break;\
	}\

void SCRIPT_typing(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t this_cid = O_cid(K_GMASCR);
	knh_Token_t *tkRES = TM(stmt);
	if(DP(ctx->gma)->gcapacity == 0) {
		Gamma_expand(ctx, ctx->gma, 8/*init*/);
	}
	ctx->gma->uline = stmt->uline;
	KNH_SETv(ctx, DP(ctx->gma)->mtd, KNH_NULL);
	switch(STT_(stmt)) {
		CASE_STMT(CLASS, stmt, TYPE_void);
		CASE_STMT2(METHOD, stmt, TYPE_void);
		CASE_STMT(FORMAT, stmt, TYPE_void);
		CASE_STMT2(DECLSCRIPT, stmt);
		CASE_STMT2(LET, stmt, TYPE_void);
		CASE_STMT2(SWAP, stmt);
		CASE_STMT2(LETM, stmt);
	}
	if(TT_(tkRES) == TT_ERR) {
		knh_Stmt_toERR(ctx, stmt, tkRES);
	}
	if(IS_Token(tkRES)) {
		knh_Stmt_done(ctx, stmt);
	}
}

/* ------------------------------------------------------------------------ */
/* [asm] */

knh_bool_t knh_Method_typing(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t it, knh_Stmt_t *stmtB)
{
	knh_type_t reqt = TYPE_Object; // needs return
	knh_Method_toAbstract(ctx, mtd);
	KNH_SETv(ctx, DP(ctx->gma)->mtd, mtd);
	DP(ctx->gma)->flag  = 0;
	Gamma_initThis(ctx, (mtd)->cid);
	if(stmtP != NULL) {
		size_t i;
		DBG_ASSERT(STT_(stmtP) == STT_DECL);
		(ctx->gma)->uline = stmtP->uline;
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			knh_flag_t flag  = 0 ; // _FREADONLY; //PARAMs_flag(ctx, stmt);
			knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, i+0), TYPE_dyn);
			knh_Token_t *tkN = tkNN(stmtP, i+1);
			knh_Token_t *tkV = tkNN(stmtP, i+2);
			if(Token_isDOT(tkN)) break;
			tkN = Gamma_add(ctx, flag, tkT, tkN, tkV, GF_UNIQUE | GF_FUNCVAR | GF_USEDCOUNT);
			if(TT_(tkN) == TT_ERR) return 0;
		}
		DP(ctx->gma)->psize = i / 3;
		DBG_ASSERT(DP(ctx->gma)->psize + 1 == DP(ctx->gma)->ebpidx);
		DP(ctx->gma)->scridx = -1;
		if(StmtMETHOD_isVARGs(stmtP)) {
			KNH_TODO("variable arguments..");
		}
		DBG_P("PSIZE = %d", DP(ctx->gma)->psize);
		if(knh_ParamArray_rtype(DP(mtd)->mp) == TYPE_void) reqt = TYPE_STMT;
	}
	else {
		if(it != TYPE_void) {
			knh_gmafields_t *gf = DP(ctx->gma)->gf;
			gf[1].flag = 0;
			gf[1].type = it;
			gf[1].fn   = FN_it;
			gf[1].ucnt = 1;
			DBG_ASSERT(gf[1].value == KNH_NULL);
			KNH_SETv(ctx, gf[1].tkIDX, new_TokenTYPED(ctx, TT_FUNCVAR, it, 1));
			DP(ctx->gma)->psize = 1;
			DP(ctx->gma)->ebpidx = 2;
			DP(ctx->gma)->gsize  = 2;
			DP(ctx->gma)->scridx = 0;
		}
		reqt = TYPE_STMT;
	}
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
	{
		int hasReturn = 0; // dummy;
		stmtB = StmtITR_typing(ctx, stmtB, reqt, &hasReturn);
	}
	if(stmtP != NULL) {
		if(Gamma_getReturnType(ctx) == TYPE_var) {
			Gamma_inferReturnType(ctx, TYPE_void);
		}
	}
	return (STT_(stmtB) != STT_ERR);
}

knh_bool_t knh_Fmt_typing(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t it, knh_Stmt_t *stmtB)
{
	Gamma_initThis(ctx, CLASS_OutputStream);
	knh_Token_t *tkT = TTYPE_typing(ctx, tkNN(stmtP, 0), CLASS_Tdynamic);
	knh_Token_t *tkN = tkNN(stmtP, 1);
	Gamma_addLOCAL(ctx, 0, tkT->type, Token_fn(ctx, tkN), 1/*ucnt*/);
	KNH_SETv(ctx, DP(ctx->gma)->mtd, mtd);
	knh_Method_toAbstract(ctx, mtd);
	DP(ctx->gma)->flag  = 0;
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
	{
		int hasReturn = 0; // dummy;
		return (STT_(StmtITR_typing(ctx, stmtB, TYPE_STMT, &hasReturn)) != STT_ERR);
	}
}

static void Method_setSourceCode(CTX ctx, knh_Method_t *mtd, knh_String_t *source)
{
	if(Method_isObjectCode(mtd) && IS_KonohaCode(DP(mtd)->kcode)) {
		KNH_SETv(ctx, (DP(mtd)->kcode)->source, source);
	}
}

static void METHOD_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Method_t *mtd = (tkNN(stmt, 2/*method*/))->mtd;
	DBG_ASSERT(IS_Method(mtd));
	DBG_ASSERT(DP(stmt)->size == 6);
	if(TT_(tkNN(stmt, 5)) == TT_CODE) {
		KNH_SETv(ctx, stmtNN(stmt, 5), knh_Token_parseStmt(ctx, tkNN(stmt, 5)));
	}
	knh_Method_asm(ctx, mtd, stmtNN(stmt, 3/*StmtP*/), TYPE_void, stmtNN(stmt, 5), knh_Method_typing);
	Method_setSourceCode(ctx, mtd, (tkNN(stmt, 4/*source*/))->text);
}

static void FORMAT_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Method_t *mtd = (tkNN(stmt, 2/*method*/))->mtd;
	if(DP(stmt)->size == 5) {
		knh_Stmt_t *stmtOP = new_Stmt2(ctx, STT_OPR, NULL); // TODO
		knh_Stmt_add(ctx, stmt, stmtOP);
	}
	else {
		DBG_ASSERT(DP(stmt)->size == 6);
		DBG_ASSERT(TT_(tkNN(stmt, 5)) == TT_CODE);
		KNH_SETv(ctx, stmtNN(stmt, 5), knh_Token_parseStmt(ctx, tkNN(stmt, 5)));
	}
	knh_Method_asm(ctx, mtd, stmtNN(stmt, 3/*StmtP*/), TYPE_void, stmtNN(stmt,5) , knh_Fmt_typing);
	Method_setSourceCode(ctx, mtd, (tkNN(stmt, 4/*source*/))->text);
}

static void CLASS_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->gma;
	knh_class_t prev_cid = DP(kc)->this_cid;
	knh_class_t this_cid = (tkNN(stmt, 0))->cid;
	knh_Stmt_t *stmtFIELD = stmtNN(stmt, 4/*instmt*/);
	DP(kc)->this_cid = this_cid;
	while(stmtFIELD != NULL) {
		ctx->gma->uline = stmtFIELD->uline;
		if(STT_(stmtFIELD) == STT_METHOD) {
			METHOD_asm(ctx, stmtFIELD);
		}
		else if(STT_(stmtFIELD) == STT_FORMAT) {
			FORMAT_asm(ctx, stmtFIELD);
		}
		stmtFIELD = DP(stmtFIELD)->nextNULL;
	}
	DP(kc)->this_cid = prev_cid;
	knh_Stmt_done(ctx, stmt);
}

void SCRIPT_asm(CTX ctx, knh_Stmt_t *stmt)
{
	switch(STT_(stmt)) {
	case STT_CLASS: CLASS_asm(ctx, stmt); break;
	case STT_METHOD: METHOD_asm(ctx, stmt); break;
	case STT_FORMAT: FORMAT_asm(ctx, stmt); break;
	default:
		return;
	}
	knh_Stmt_done(ctx, stmt);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
