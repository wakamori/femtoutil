/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved..
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us.
 *
 * (1) GNU General Public License 3.0 (with K_UNDER_GPL)
 * (2) Konoha Non-Disclosure License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORSÄ
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

#define USE_bytes   1
#define USE_cwb     1

#include"commons.h"

#define TT_SIZE TT_LOR   /* |a| */

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [term] */

static knh_Token_t* new_Token(CTX ctx, knh_term_t tt)
{
	knh_Token_t *tk = new_(Token);
	SP(tk)->tt = tt;
	tk->uline = ctx->gma->uline;
	return tk;
}

knh_Stmt_t* new_Stmt2(CTX ctx, knh_term_t stt, ...)
{
	knh_Stmt_t *stmt = new_(Stmt);
	knh_Term_t *tm = NULL;
	va_list ap;
	DBG_ASSERT(stt < TT_PRAGMA);
	stmt->stt = stt;
	stmt->uline = ctx->gma->uline;
	va_start(ap , stt);
	while((tm = (knh_Term_t*)va_arg(ap, knh_Term_t*)) != NULL) {
		DBG_ASSERT(IS_Token(tm) || IS_Stmt(tm));
		knh_Stmt_add_(ctx, stmt, tm, NULL);
	}
	va_end(ap);
	return stmt;
}

void knh_Stmt_swap(CTX ctx, knh_Stmt_t *stmt, size_t n, size_t m)
{
	DBG_ASSERT(n < DP(stmt)->size);
	DBG_ASSERT(m < DP(stmt)->size);
	knh_Term_t *tm = tmNN(stmt, n);
	tmNN(stmt, n) = tmNN(stmt, m);
	tmNN(stmt, m) = tm;
}

void knh_Stmt_trimToSize(CTX ctx, knh_Stmt_t *stmt, size_t n)
{
	size_t i;
	for(i = n; i < DP(stmt)->size; i++) {
		KNH_FINALv(ctx, tmNN(stmt, i));
	}
	DBG_ASSERT(n <= DP(stmt)->size);
	DP(stmt)->size = n;
}

knh_Token_t *knh_Stmt_done(CTX ctx, knh_Stmt_t *stmt)
{
	knh_StmtEX_t *b = DP((knh_Stmt_t*)stmt);
	stmt->stt = STT_DONE;
	if(stmt->terms != NULL) {
		size_t i;
		for(i = 0; i < b->size; i++) {
			KNH_FINALv(ctx, stmt->terms[i]);
		}
		KNH_FREE(ctx, stmt->terms, sizeof(knh_Term_t*) * b->capacity);
		stmt->terms = NULL;
		b->capacity = 0;
		b->size = 0;
	}
	KNH_SETv(ctx, b->metaDictCaseMap, KNH_NULL);
	return (knh_Token_t*)(stmt);
}

void knh_Stmt_toERR(CTX ctx, knh_Stmt_t *stmt, knh_Token_t *tkERR)
{
	DBG_ASSERT(IS_Stmt(stmt));
	if(STT_(stmt) == STT_ERR) return;
	DBG_ASSERT(TT_(tkERR) == TT_ERR);
	DBG_ASSERT(IS_String((tkERR)->text));
	stmt->uline = tkERR->uline;
	if(DP(stmt)->size == 0) {
		TT_(tkERR) = TT_ASIS;
		knh_Stmt_add(ctx, stmt, tkERR);
		TT_(tkERR) = TT_ERR;
	}
	else {
		KNH_SETv(ctx, tmNN(stmt, 0), tkERR);
		knh_Stmt_trimToSize(ctx, stmt, 1);
	}
	STT_(stmt) = STT_ERR;
}

static void Stmt_toSyntaxError(CTX ctx, knh_Stmt_t *stmt, knh_Token_t *tk K_TRACEARGV)
{
	knh_Stmt_toERR(ctx, stmt, ERROR_Token(ctx, tk K_TRACEDATA));
}

/* ------------------------------------------------------------------------ */

typedef struct {
	struct knh_Token_t** ts;
	int meta;
	int c;
	int e;
} tkitr_t;

#define ITR_next(itr)        (itr)->c += 1;
#define ITR_hasNext(itr)     ((itr)->c < (itr)->e)
#define ITR_size(itr)        ((itr)->e - (itr)->c)
#define ITR_uline(itr)       if((itr)->c < (itr)->e) ctx->gma->uline = ((itr)->ts[(itr)->c])->uline

static tkitr_t* ITR_new(knh_Token_t *tk, tkitr_t *buf)
{
	buf->meta = -1;
	buf->c = 0;
	if(IS_Array((tk)->data)) {
		buf->ts = ((tk)->list)->tokens;
		buf->e = knh_Array_size((tk)->list);
	}
	else if(IS_Token((tk)->data)) {
		buf->ts = &((tk)->token);
		buf->e = 1;
	}
	else {
		buf->ts = NULL;
		buf->e = 0;
	}
	return buf;
}

static knh_term_t ITR_tt(tkitr_t *itr)
{
	DBG_ASSERT(itr->c < itr->e);
	return TT_(itr->ts[itr->c]);
}

#define ITR_is(itr, tt) ITR_isN(itr, 0, tt)

static int ITR_isN(tkitr_t *itr, int shift, knh_term_t tt)
{
	int c = itr->c + shift;
	return (c < itr->e && TT_(itr->ts[c]) == tt);
}

#define ITR_tk(itr)     (itr->ts[itr->c])

static knh_Token_t *ITR_nextTK(tkitr_t *itr)
{
	DBG_ASSERT(itr->c < itr->e);
	(itr)->c += 1;
	return (itr)->ts[(itr)->c - 1];
}

static void ITR_chop(tkitr_t *itr, knh_term_t tt)
{
	int e = itr->e - 1;
	if(itr->c <= e && TT_(itr->ts[e]) == tt) {
		itr->e = e;
	}
}

static int ITR_count(tkitr_t *tc, knh_term_t tt)
{
	knh_Token_t **ts = tc->ts;
	int i, cnt = 0;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_(ts[i]) == tt) cnt++;
	}
	return cnt;
}

/* ------------------------------------------------------------------------ */
/* [tokenizer] */

static void Token_add(CTX ctx, knh_Token_t *tkB, knh_Token_t *tk);

static knh_term_t TT_ch(int ch)
{
	switch(ch) {
		case '{': case '}': return TT_BRACE;
		case '(': case ')': return TT_PARENTHESIS;
		case '[': case ']': return TT_BRANCET;
#ifdef TT_SIZE
		case '|': return TT_SIZE;
#endif
		case ',': return TT_COMMA;
		case '"': return TT_STR;
		case '\'': return TT_TSTR;
		case '`': return TT_ESTR;
		case '/': return TT_REGEX;
	}
	DBG_ABORT("unknown ch=%c", ch);
	return TT_ERR;
}

static int isTYPE(knh_Token_t *tk)
{
	knh_term_t tt = TT_(tk);
	return (tt == TT_PTYPE || tt == TT_UNAME || (TT_TYPEOF <= tt && tt <= TT_DYN));
}

static knh_Token_t *new_TokenCID(CTX ctx, knh_class_t cid)
{
	knh_Token_t *tk = new_(Token);
	TT_(tk) = TT_UNAME;
	tk->uline = ctx->gma->uline;
	(tk)->cid = cid;
	KNH_SETv(ctx, (tk)->text, ClassTBL(cid)->sname);
	return tk;
}

static knh_Token_t *new_TokenPTYPE(CTX ctx, knh_class_t cid, knh_Token_t *tk)
{
	knh_Token_t *tkT = new_Token(ctx, TT_PTYPE);
	knh_Token_t *tkC = new_TokenCID(ctx, cid);
	Token_add(ctx, tkT, tkC);
	if(cid == CLASS_Map) {
		Token_add(ctx, tkT, new_TokenCID(ctx, CLASS_String));
	}
	Token_add(ctx, tkT, tk);
	return tkT;
}

static int ITR_findPTYPE(tkitr_t *itr)
{
	int c, e = 0, f = 0;
	for(c = itr->e-2; 0 <= c; c--) {
		knh_Token_t *tk = itr->ts[c];
		if(!isTYPE(tk)) break;
		if(e == 1) {
			itr->c = c; return 1;
		}
		c--; tk = itr->ts[c];
		if(c < 0) return 0;
		if(TT_(tk) == TT_COMMA) continue;
		if(TT_(tk) == TT_LT) { e = 1; continue; }
		if(f == 0 && TT_(tk) == TT_DARROW) {
			f = 1; continue;
		}
		break;
	}
	return 0;
}

static void Token_setTYPEOFEXPR(CTX ctx, knh_Token_t *tk);

static void Token_add(CTX ctx, knh_Token_t *tkB, knh_Token_t *tk)
{
	int prev_idx = 0;
	knh_Token_t *tkPREV;
	knh_Array_t *a;
	DBG_ASSERT(tkB->uline != 0 && tk->uline != 0);
	if(IS_NULL((tkB)->data)) {
		KNH_SETv(ctx, (tkB)->data, tk);
		return;
	}
	if(IS_Token((tkB)->data)) {
		a = new_Array0(ctx, 0);
		tkPREV = (tkB)->token;
		knh_Array_add(ctx, a, (tkB)->data);
		KNH_SETv(ctx, (tkB)->data, a);
	}
	else {
		DBG_ASSERT(IS_Array((tkB)->data));
		a = (tkB)->list;
		DBG_ASSERT(knh_Array_size(a) > 0);
		prev_idx = knh_Array_size(a)-1;
		tkPREV = a->tokens[prev_idx];
	}
	knh_Array_add(ctx, a, tk);

	if(TT_(tk) == TT_SEMICOLON) {
		Token_setBOL(tk, 1);
		return;
	}
	if((tk->uline > tkPREV->uline) && !(Token_isPLUSLINE(tkB))) {
		Token_setBOL(tk, 1);
	}

	// 1. translation
	if(TT_(tk) == TT_DECLLET) {
		TT_(tk) = TT_LET;
		return;
	}

	if(prev_idx > 0 && TT_(tk) == TT_CODE && TT_(tkPREV) == TT_DARROW) {
		knh_Token_t *tkPREV2 = a->tokens[prev_idx - 1];
		if(TT_(tkPREV2) == TT_PARENTHESIS) {  // (n) = > {} ==> function(n) {}
			TT_(tkPREV) = TT_FUNCTION;
			knh_Array_swap(ctx, a, prev_idx-1, prev_idx);
			return;
		}
	}

	if(TT_(tk) == TT_PARENTHESIS) {
		if(TT_(tkPREV) == TT_DOT) {
			KNH_SETv(ctx, (tkPREV)->text, TS_EMPTY);
			Token_setDOT(tkPREV, 1);
			TT_(tkPREV) = (TT_(tkPREV) == TT_UNAME) ? TT_UFUNCNAME : TT_FUNCNAME;
		}
		else if(TT_(tkPREV) == TT_NAME) {
			TT_(tkPREV) = TT_FUNCNAME;
		}
		else if(TT_(tkPREV) == TT_UNAME) {
			TT_(tkPREV) = TT_UFUNCNAME;
		}
	}

	L_JOIN:;
	if(TT_isSTR(TT_(tk))) {
		if(TT_isSTR(TT_(tkPREV)) || TT_(tkPREV) == TT_URN) {
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_Bytes_write(ctx, cwb->ba, S_tobytes((tkPREV)->text));
			if(tk->uline > tkPREV->uline && TT_(tkPREV) != TT_URN) {
				knh_Bytes_putc(ctx, cwb->ba, '\n');
			}
			knh_Bytes_write(ctx, cwb->ba, S_tobytes((tk)->text));
			KNH_SETv(ctx, (tkPREV)->data, knh_cwb_newString(ctx, cwb));
			//if(TT_(tk) == TT_ESTR) TT_(tkPREV) = TT_ESTR;
			goto L_JOIN1;
		}
	}
	if(TT_(tk) == TT_EXPT && TT_(tkPREV) == TT_UNAME) {
		Token_setExceptionType(tkPREV, 1);
		goto L_JOIN1;
	}
	if(TT_(tk) == TT_PARENTHESIS && TT_(tkPREV) == TT_TYPEOF) { // typeof
		KNH_SETv(ctx, tkPREV->data, tk);
		Token_setTYPEOFEXPR(ctx, tkPREV);
		goto L_JOIN1;
	}
	if(TT_(tkPREV) == TT_BYTE) {
		if(TT_(tk) == TT_BRANCET && IS_NULL((tk)->data)) { // byte[]
			KNH_SETv(ctx, a->list[prev_idx], new_TokenCID(ctx, CLASS_Bytes));
			goto L_JOIN1;
		}
	}
	if(isTYPE(tkPREV)) {
		if(TT_(tk) == TT_BRANCET && IS_NULL((tk)->data)) { // String[]
			KNH_SETv(ctx, a->list[prev_idx], new_TokenPTYPE(ctx, CLASS_Array, tkPREV));
			goto L_JOIN1;
		}
		if(TT_(tk) == TT_ITR) {  // String..
			KNH_SETv(ctx, a->list[prev_idx], new_TokenPTYPE(ctx, CLASS_Iterator, tkPREV));
			goto L_JOIN1;
		}
		if(TT_(tk) == TT_DMUL) { // String** => Thunk<String>
			KNH_SETv(ctx, a->list[prev_idx], new_TokenPTYPE(ctx, CLASS_Thunk, tkPREV));
			goto L_JOIN1;
		}
#ifdef CLASS_Immutable
		if(TT_(tk) == TT_TSTR && S_size(tk->text) == 0) { // String'' => Immutable<String>
			KNH_SETv(ctx, a->list[prev_idx], new_TokenPTYPE(ctx, CLASS_Immutable, tkPREV));
			goto L_JOIN1;
		}
#endif
		if(TT_(tk) == TT_GT || TT_(tk) == TT_RSFT || TT_(tk) == TT_RSEND) { // String> or String>>
			tkitr_t itrbuf, *itr = ITR_new(tkB, &itrbuf);
			if(ITR_findPTYPE(itr)) {
				knh_Token_t *tkT = new_Token(ctx, TT_PTYPE);
				prev_idx = itr->c;
				while(ITR_hasNext(itr)) {
					knh_Token_t *tkPT = ITR_nextTK(itr);
					Token_add(ctx, tkT, tkPT);
					if(ITR_is(itr, TT_DARROW)) {
						Token_add(ctx, tkT, ITR_tk(itr));
					}
					ITR_next(itr);
				}
				KNH_SETv(ctx, a->list[prev_idx], tkT);
				if(TT_(tk) == TT_RSEND) {
					TT_(tk) = TT_RSFT;
					tkPREV = tkT;
					KNH_SETv(ctx, a->list[prev_idx+1], tk);
					knh_Array_trimSize(ctx, a, prev_idx+2);
					goto L_JOIN;
				}
				if(TT_(tk) == TT_RSFT) {
					TT_(tk) = TT_GT;
					tkPREV = tkT;
					KNH_SETv(ctx, a->list[prev_idx+1], tk);
					knh_Array_trimSize(ctx, a, prev_idx+2);
					goto L_JOIN;
				}
				goto L_JOIN1;
			}
		}
	}
	return;

	L_JOIN1:
	knh_Array_trimSize(ctx, a, prev_idx+1);
}

static knh_Token_t *Token_lastChildNULL(knh_Token_t *tk)
{
	if(IS_Array((tk)->data)) {
		size_t n = knh_Array_size((tk)->list);
		DBG_ASSERT(n>0);
		return (tk)->list->tokens[n-1];
	}
	else if(IS_Token((tk)->data)) {
		return (tk)->token;
	}
	return NULL;
}

static int Token_startsWithExpr(CTX ctx, knh_Token_t *tkB)
{
	knh_Token_t *tk = Token_lastChildNULL(tkB);
	if(tk != NULL) {
		knh_term_t tt = TT_(tk);
		if(tt == TT_COMMA || tt == TT_SEMICOLON || tt == TT_COLON) return 1;
		if(tt == TT_CASE || tt == TT_RETURN || tt == TT_YIELD
			|| tt == TT_PRINT || tt == TT_ASSERT) return 1;
		if(tt == TT_PREV || tt == TT_NEXT) return 0;  // a++ + 1
		if(TT_LET <= tt && tt <= TT_TSUB) return 1;
		return 0;
	}
	return 1;
}

static knh_String_t *new_StringSYMBOL(CTX ctx, knh_bytes_t t)
{
	knh_DictMap_t *symbolDictMap = ctx->symbolDictMap;
	knh_index_t idx = knh_DictMap_index(symbolDictMap, t);
	if(idx == -1) {
		knh_String_t *s = new_S(ctx, t);
		knh_DictMap_set(ctx, symbolDictMap, s, s);
		return s;
	}
	else {
		return knh_DictMap_keyAt(symbolDictMap, idx);
	}
}

static knh_String_t* NameSpace_getAliasNULL(CTX ctx, knh_NameSpace_t* ns, knh_bytes_t t)
{
	knh_String_t *s = NULL;
	L_TAIL:;
	if(ctx->wshare->sysAliasDictMapNULL != NULL) {
		s = (knh_String_t*)knh_DictMap_getNULL(ctx, ctx->wshare->sysAliasDictMapNULL, t);
	}
	if(s == NULL) {
		if(ns->parentNULL != NULL) {
			ns = ns->parentNULL;
			goto L_TAIL;
		}
		if(ns != ctx->share->rootns) {
			ns = ctx->share->rootns;
			goto L_TAIL;
		}
	}
	return s;
}

static void Token_setNAME(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(t.utext[0] == '.') {
		Token_setDOT(tk, 1);
	}
	if(t.utext[0] == '@') {  /* alias */
		t.utext = t.utext + 1; t.len = t.len - 1;
		TT_(tk) = TT_METAN;
		KNH_SETv(ctx, (tk)->data, new_StringSYMBOL(ctx, t));
	}
	else if(isupper(t.utext[0]) || (t.utext[0] == '.' && isupper(t.utext[1]))) {
		TT_(tk) = TT_UNAME;
		if(t.utext[0] == '.') {
			t.utext = t.utext + 1; t.len = t.len - 1;
		}
		KNH_SETv(ctx, (tk)->data, new_StringSYMBOL(ctx, t));
	}
	else {
		size_t i, u = 0;
		knh_cwb_t cwbbuf2, *cwb2 = knh_cwb_open(ctx, &cwbbuf2);
		TT_(tk) = TT_NAME;
		for(i = cwb->pos; i < cwb->pos + t.len; i++) {
			const char *p = BA_tochar(cwb->ba);
			int ch = p[i];
			if(ch == '.') continue;
			if(ch == '_') {
				if(!(knh_cwb_size(cwb2) < 2)) { /* _name, __name */
					u = 1;
					continue;
				}
			}
			if(u == 1) {
				ch = toupper(ch);
				u = 0;
			}
			knh_Bytes_putc(ctx, cwb2->ba, ch);
		}
		KNH_SETv(ctx, (tk)->data, new_StringSYMBOL(ctx, knh_cwb_tobytes(cwb2)));
	}
}

/* ------------------------------------------------------------------------ */
/* [tokenizer] */

#define K_POOLSIZE 64
#define K_POOLHALFSIZE (K_POOLSIZE/2)

static size_t phit = 0;
static size_t pmiss = 0;
static size_t presize = 0;

Object* knh_getConstPools(CTX ctx, void *p)
{
	knh_Object_t *v = UPCAST(p), *result = NULL;
	knh_Array_t *a = ctx->constPools;
	long i;
	for(i = knh_Array_size(a) - 1; i >= 0; i--) {
		knh_Object_t *o = a->list[i];
		if(O_cid(o) == O_cid(v) && knh_Object_compareTo(o, v) == 0) {
			knh_Array_add(ctx, a, o); result = o;
			KNH_SETv(ctx, a->list[i], v); // TO AVOID RCGC
			phit++;
			goto L_POOLMGM;
		}
	}
	knh_Array_add(ctx, a, v); result = v;
	pmiss++;
	L_POOLMGM:;
	if(a->size  == K_POOLSIZE) {
		for(i = 0; i < K_POOLHALFSIZE; i++) {
			KNH_FINALv(ctx, a->list[i]);
		}
		knh_memcpy(&(a->list[0]), &(a->list[K_POOLHALFSIZE]), sizeof(Object*) * K_POOLHALFSIZE);
		knh_bzero(&(a->list[K_POOLHALFSIZE]), sizeof(Object*) * K_POOLHALFSIZE);
		a->size -= K_POOLHALFSIZE;
		presize++;
	}
	//DBG_P("ConstPool hit=%ld, miss=%ld resize=%ld", phit, pmiss, presize);
	return result;
}

static void Token_setTEXT(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(TT_(tk) == TT_UNTYPED) {
		knh_String_t *text = NameSpace_getAliasNULL(ctx, K_GMANS, t);
		if(text != NULL) {
			t = S_tobytes(text);
			KNH_SETv(ctx, (tk)->data, text);
			knh_cwb_clear(cwb, 0);
			knh_Bytes_write(ctx, cwb->ba, t);  // alias
		}
		knh_DictSet_t *tokenDictSet = DP(ctx->sys)->tokenDictSet;
		knh_index_t idx = knh_DictSet_index(tokenDictSet, t);
		if(idx != -1) {
			knh_term_t tt = (knh_term_t)knh_DictSet_valueAt(tokenDictSet, idx);
			TT_(tk) = tt;
			if(IS_NULL((tk)->data)) {
				KNH_SETv(ctx, (tk)->data, knh_DictSet_keyAt(tokenDictSet, idx));
			}
		}
		else {
			Token_setNAME(ctx, tk, cwb);
		}
	}
	else if(TT_(tk) == TT_METAN) {
		KNH_SETv(ctx, (tk)->data, new_StringSYMBOL(ctx, t));
	}
	else {
		knh_Array_t *a = ctx->constPools;
		knh_String_t *s = NULL;
		if(a != NULL) {
			long i;
			for(i = knh_Array_size(a) - 1; i >= 0; i--) {
				knh_Object_t *o = a->list[i];
				if(O_cid(o) == CLASS_String) {
					if(S_equals((knh_String_t*)o, t)) {
						knh_cwb_clear(cwb, 0);
						if(isdigit(t.utext[0]) && TT_(tk) == TT_NUM) {
							KNH_SETv(ctx, (tk)->data, o);
						}
						s = (knh_String_t*)o;
						break;
					}
				}
			}
		}
		if(s == NULL) {
			s = knh_cwb_newString(ctx, cwb);
		}
		KNH_SETv(ctx, (tk)->data, knh_getConstPools(ctx, s));
	}
}

static void Token_addBuf(CTX ctx, knh_Token_t *tkB, knh_cwb_t *cwb, knh_term_t tt, int ch)
{
	if(knh_cwb_size(cwb) != 0) {
		knh_Token_t *newtk = new_Token(ctx, tt);
		Token_setTEXT(ctx, newtk, cwb);
		Token_add(ctx, tkB, newtk);
		knh_cwb_clear(cwb, 0);
	}
	else if(tt == TT_CODE || TT_isSTR(tt) || tt == TT_REGEX) {
		knh_Token_t *tk2 = new_Token(ctx, tt);
		KNH_SETv(ctx, (tk2)->data, TS_EMPTY);
		Token_add(ctx, tkB, tk2);
	}
}

static void InputStream_skipLINE(CTX ctx, knh_InputStream_t *in)
{
	int ch;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\n') {
			break;
		}
	}
}

static void InputStream_skipBLOCKCOMMENT(CTX ctx, knh_InputStream_t *in, knh_Bytes_t *ba)
{
	int ch, prev = '*', level = 1;
	if(ba != NULL) {
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(prev == '*' && ch == '/') {
				level--;
				if(level == 0) break;
			}else if(prev == '/' && ch == '*') {
				level++;
			}
			knh_Bytes_putc(ctx, ba, ch);
			prev = ch;
		}
	}
	else {
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(prev == '*' && ch == '/') {
				level--;
				if(level == 0) break;
			}else if(prev == '/' && ch == '*') {
				level++;
			}
			prev = ch;
		}
	}
}

static knh_bool_t Bytes_isTripleQuote(knh_Bytes_t *ba, int quote)
{
	if(BA_size(ba) > 2 &&
		ba->bu.utext[BA_size(ba)-1] == quote
		&& ba->bu.utext[BA_size(ba)-2] == quote) return 1;
	return 0;
}

static void Bytes_addESC(CTX ctx, knh_Bytes_t *ba, knh_InputStream_t *in)
{
	int ch = knh_InputStream_getc(ctx, in);
	if(ch == 'n') ch = '\n';
	else if(ch == 't') ch = '\t';
	else if(ch == 'r') ch = '\r';
	knh_Bytes_putc(ctx, ba, ch);
}

static void Bytes_addQUOTE(CTX ctx, knh_Bytes_t *ba, knh_InputStream_t *in, int quote, int skip, int isRAW, int isTQUOTE)
{
	if(isRAW == 1) {
		int prev = quote;
		int ch = knh_InputStream_getc(ctx, in);
		if(ch != EOF && ch != skip) goto L_INLOOP_RAW;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			L_INLOOP_RAW:;
			if(ch == quote && prev != '\\') {
				return;
			}
			knh_Bytes_putc(ctx, ba, ch);
			if(prev == '\\' && ch == '\\') ch = 0;
			prev = ch;
		}
	}
	else {
		int ch = knh_InputStream_getc(ctx, in);
		if(ch != skip) goto L_INLOOP;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			L_INLOOP:;
			if(ch == quote) {
				if(isTQUOTE == 1) {
					if(Bytes_isTripleQuote(ba, quote)) {
						knh_Bytes_unputc(ba, 2);
						return;
					}
				}
				else {
					return;
				}
			}
			else if(ch == '\\') {
				Bytes_addESC(ctx, ba, in);
				continue;
			}
			knh_Bytes_putc(ctx, ba, ch);
		}
	}
}

static int Token_addQUOTE(CTX ctx, knh_Token_t *tkB, knh_cwb_t *cwb, knh_InputStream_t *in, int quote, int isRAW)
{
	int ch;
	if(quote != '/') {
		Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, quote);
	}
	ch = knh_InputStream_getc(ctx, in);
	if(quote == '#') {
		if(ch == '#') {
			Bytes_addQUOTE(ctx, cwb->ba, in, '\n', ' '/*skip*/, 1/*isRAW*/, 0/*isTQUOTE*/);
			Token_addBuf(ctx, tkB, cwb, TT_ESTR, '\n');
			return '\n';
		}
		else if(ch == '!') {
			InputStream_skipLINE(ctx, in);
			return knh_InputStream_getc(ctx, in);
		}
	}
	if(ch != quote) {
		if(ch == '\\' && !isRAW) {
			Bytes_addESC(ctx, cwb->ba, in);
		}
		else {
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		Bytes_addQUOTE(ctx, cwb->ba, in, quote, -2, isRAW, 0/*isTQUOTE*/);
		ch = knh_InputStream_getc(ctx, in);
		Token_addBuf(ctx, tkB, cwb, TT_ch(quote), ch);
		return ch;
	}
	ch = knh_InputStream_getc(ctx, in);
	if(quote != ch) { /* "" */
		Token_addBuf(ctx, tkB, cwb, TT_ch(quote), ch);
	}
	else { /* triple quote */
		knh_term_t tt = TT_ch(quote);
		if(tt == TT_TSTR) tt = TT_STR;
		Bytes_addQUOTE(ctx, cwb->ba, in, quote, '\n'/*skip*/, isRAW, 1/*isTQUOTE*/);
		ch = knh_InputStream_getc(ctx, in);
		Token_addBuf(ctx, tkB, cwb, tt, ch);
		Token_setPLUSLINE(tkB, 1); /* ... hoge """ <<< EOL; */
	}
	return ch;
}

#define ISB1_(t, c)  (t.utext[0] == c)
#define ISB2_(t, c, c2)  (t.utext[0] == c && t.utext[1] == c2)

static int bytes_isOPR(knh_bytes_t t, int ch)
{
	if(t.len == 1) {
		switch(ch) {
		case '%':
			if(ISB1_(t, '%')) return 1; /* %% */
			return 0;
		case '&':
			if(ISB1_(t, '&')) return 1; /* && */
			return 0;
		case '+':
			if(ISB1_(t, '+')) return 1; /* ++ */
			if(ISB1_(t, '=')) return 1; /* =+ */
			return 0;
		case '-':
			if(ISB1_(t, '-')) return 1; /* -- */
			return 0;
		case '.':
			if(ISB1_(t, '.')) return 1;  /* .. */
			return 0;
		case '*':
			if(ISB1_(t, '*')) return 1; /* ** */
			return 0;
		case '<':
			if(ISB1_(t, '<')) return 1;  /* << */
			return 0;
		case '=':
			if(ISB1_(t, '+')) return 1;   /* += */
			if(ISB1_(t, '-')) return 1;   /* -= */
			if(ISB1_(t, '/')) return 1;   /* /= */
			if(ISB1_(t, '%')) return 1;   /* %= */
			if(ISB1_(t, '*')) return 1;   /* *= */
			if(ISB1_(t, '&')) return 1;   /* &= */
			if(ISB1_(t, '|')) return 1;   /* |= */
			if(ISB1_(t, '=')) return 1;   /* == */
			if(ISB1_(t, '!')) return 1;   /* != */
			if(ISB1_(t, '<')) return 1;   /* >= */
			if(ISB1_(t, '>')) return 1;   /* >= */
			if(ISB1_(t, ':')) return 1;   /* >= */
			return 0;
		case '>':
			if(ISB1_(t, '<')) return 1;  /* <> */
			if(ISB1_(t, '-')) return 1;  /* -> */
			if(ISB1_(t, '=')) return 1;  /* => */
			if(ISB1_(t, '>')) return 1;  /* >> */
			return 0;
		case '?':
			if(ISB1_(t, '?')) return 1;  /* ?? */
			return 0;
		case '!':
			if(ISB1_(t, '!')) return 1;  /* !! */
			return 0;
		case '|':
			if(ISB1_(t, '|')) return 1;  /* || */
			return 0;
		case '~':
			if(ISB1_(t, '=')) return 1;  /* ~= */
			return 0;
		case ':':
			if(ISB1_(t, '<')) return 1;  /* <: */
			return 0;
		}
	}
	else if(t.len == 2) {
		switch(ch) {
		case '.':
			if(ISB2_(t, '.', '.')) return 1; /* ... */
			return 0;
		case '<':
			if(ISB2_(t, '<', '<')) return 1;  /* <<< */
			return 0;
		case '=':
			if(ISB2_(t, '<', '<')) return 1;  /* <<= */
			if(ISB2_(t, '>', '>')) return 1;  /* >>= */
			if(ISB2_(t, '?', '?')) return 1;   /* ??= */
			if(ISB2_(t, '=', '=')) return 1;   /* === */
			return 0;
		case '>':
			if(ISB2_(t, '>', '>')) return 1;  /* >>> */
			if(ISB2_(t, '=', '=')) return 1;  /* ==> */
			if(ISB2_(t, '-', '-')) return 1;  /* --> */
			return 0;
		}
	}
	return 0;
}

static int Token_addOPR(CTX ctx, knh_Token_t *tkB, knh_cwb_t *cwb, knh_InputStream_t *in, int ch)
{
	if(ch == '/') {
		ch = knh_InputStream_getc(ctx, in);
		if(ch == '*') {
			InputStream_skipBLOCKCOMMENT(ctx, in, NULL);
			return knh_InputStream_getc(ctx, in);
		}else if(ch == '/') {
			InputStream_skipLINE(ctx, in);
			ctx->gma->uline = in->uline;
			return knh_InputStream_getc(ctx, in);
		}
		if(Token_startsWithExpr(ctx, tkB)) {
			knh_Bytes_putc(ctx, cwb->ba, ch);
			return Token_addQUOTE(ctx, tkB, cwb, in, '/', 1);
		}
		knh_Bytes_putc(ctx, cwb->ba, '/');
		goto L_INLOOP;
	}
	knh_Bytes_putc(ctx, cwb->ba, ch);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_INLOOP:;
		knh_bytes_t top = knh_cwb_tobytes(cwb);
		if(bytes_isOPR(top, ch)) {
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		else {
			int isHEAD = Token_startsWithExpr(ctx, tkB);
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			if(isHEAD) {
				knh_Token_t *ctk = Token_lastChildNULL(tkB);
				DBG_ASSERT(ctk != NULL);
				if(TT_(ctk) == TT_SUB) TT_(ctk) = TT_TSUB;
				if(TT_(ctk) == TT_ADD) TT_(ctk) = TT_TADD;
				if(TT_(ctk) == TT_LAND) TT_(ctk) = TT_TAND;
			}
			break;
		}
	}
	return ch;
}

static int Token_addMETAN(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch;
	Token_addBuf(ctx, tk, cwb, TT_UNTYPED, '@');
	ch = knh_InputStream_getc(ctx, in);
	if(ch == '"' || ch == '\'' || ch == '`') {
		ch = Token_addQUOTE(ctx, tk, cwb, in, ch, /*RAW*/1);
	}
	do {
		if(ch == ';') {
			WarningNoEffect(ctx);
			continue;
		}
		if(!isalnum(ch) && ch != '_' && ch != '.') {
			break;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	while((ch = knh_InputStream_getc(ctx, in)) != EOF);
	Token_addBuf(ctx, tk, cwb, TT_METAN, ch);
	return ch;
}

static int Token_addPROPN(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch;
	Token_addBuf(ctx, tk, cwb, TT_UNTYPED, '$');
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(!isalnum(ch) && ch != '_' && ch != '.' && ch != '*') {
			break;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	Token_addBuf(ctx, tk, cwb, TT_PROPN, ch);
	return ch;
}

static int Token_addURN(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch = knh_InputStream_getc(ctx, in);
	if(ch == '+') {  /* hoge:+ */
		ch = knh_InputStream_getc(ctx, in);
		Token_addBuf(ctx, tk, cwb, TT_TSCHEME, ch);
		return ch;
	}
	if(ch == '=') {   /* a:=1*/
		Token_addBuf(ctx, tk, cwb, TT_UNTYPED, ':');
		knh_Bytes_putc(ctx, cwb->ba,':');
		knh_Bytes_putc(ctx, cwb->ba,'=');
		ch = knh_InputStream_getc(ctx, in);
		Token_addBuf(ctx, tk, cwb, TT_UNTYPED, ch);
		return ch;
	}
	if(isspace(ch) || ch == ';' || ch == '"' || ch == '\'') {
		Token_addBuf(ctx, tk, cwb, TT_UNTYPED, ':');
		knh_Bytes_putc(ctx, cwb->ba, ':');
		Token_addBuf(ctx, tk, cwb, TT_UNTYPED, ch);
		return ch;
	}
	knh_Bytes_putc(ctx, cwb->ba, ':');
	knh_Bytes_putc(ctx, cwb->ba, ch);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		switch(ch) {
			case '\n': case '\r': case ' ':  case '\t': case ';': case ',':
			case '\'': case '"': case '`':
			case ')': case '}': case ']':
			case '(': case '{': case '[':
				goto L_NEWURN;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	L_NEWURN: {
		knh_bytes_t t = knh_cwb_tobytes(cwb);
		knh_term_t tt = TT_URN;
#ifdef K_USING_SEMANTICS
		if(knh_bytes_startsWith(t, STEXT("int:")) || knh_bytes_startsWith(t, STEXT("float:"))) {
			t.ubuf[0] = toupper(t.utext[0]);
		}
#endif
		if(knh_bytes_startsWith(t, STEXT("new:"))) {
			tt = TT_NEW;
		}
		Token_addBuf(ctx, tk, cwb, (isupper(t.utext[0])) ? TT_UNAME : TT_URN, ch);
	}
	return ch;
}

static int Token_addNUM(CTX ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in, int ch)
{
	int prev = 0, dot = 0;
	L_ADD:;
	knh_Bytes_putc(ctx, cwb->ba, ch);
	prev = ch;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(isalnum(ch)) goto L_ADD;
		if(ch == '_') continue; // nothing
		if(prev == '.' && ch == '.') {  /* 1.. => 1 .. */
			knh_Bytes_unputc(cwb->ba, 1);
			Token_addBuf(ctx, tk, cwb, TT_NUM, ch);
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_putc(ctx, cwb->ba, '.');
			ch = knh_InputStream_getc(ctx, in);
			Token_addBuf(ctx, tk, cwb, TT_UNTYPED, ch);
			return ch;
		}
		if(ch == '.') {
			dot++;
			if(dot == 1) goto L_ADD;
			continue;
		}
		if((ch == '-' || ch == '+') && (prev == 'e' || prev =='E')) {
			goto L_ADD;
		}
		break;
	}
	Token_addBuf(ctx, tk, cwb, TT_NUM, ch);
	return ch;
}

static void Token_addBLOCKERR(CTX ctx, knh_Token_t *tkB, knh_InputStream_t *in, int ch)
{
	const char *block = "indent";
	if(ch == ')') block = ")";
	if(ch == ']') block = "]";
	tkB->uline = in->uline;
	(ctx->gma)->uline = in->uline;
	Token_add(ctx, tkB, ERROR_Block(ctx, block));
}

static void Token_addBLOCK(CTX ctx, knh_Token_t *tkB, knh_cwb_t *cwb, knh_InputStream_t *in, int block_indent)
{
	int c, ch, prev = '{', level = 1;
	Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, '{');
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch != '\t' && ch != ' ') goto L_STARTLINE;
	}
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_STARTLINE:;
		if(ch == '}') {
			level--;
			if(level == 0) {
				Token_addBuf(ctx, tkB, cwb, TT_CODE, ch);
				return;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
		if(ch == '\n') {
			prev = ch;
			break;
		}
		if(ch == '{') {
			level++;
		}
		else if(ch == '"' || ch == '`' || ch == '\'') {
			Bytes_addQUOTE(ctx, cwb->ba, in, ch, -2/*skip*/, 1/*isRAW*/, 0);
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		else if(prev == '/' && ch == '/') {
			knh_Bytes_unputc(cwb->ba, 2);
			InputStream_skipLINE(ctx, in);
			knh_Bytes_putc(ctx, cwb->ba, '\n');
			break;
		}
		else if(prev == '/' && ch == '*') {
			InputStream_skipBLOCKCOMMENT(ctx, in, cwb->ba);
			knh_Bytes_putc(ctx, cwb->ba, '/');
		}
		prev = ch;
	}
	c = 0;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\t') { c += 3; continue; }
		if(ch == ' ') { c += 1; continue; }
		if(ch == '\n') {c = 0; continue; }
//		if(c > 0 && block_indent == c && ch == '}') {
//			Token_addBuf(ctx, tkB, cwb, TT_CODE, ch);
//			return;
//		}
		break;
	}
	if(block_indent <= c) {
		goto L_STARTLINE;
	}
	knh_cwb_clear(cwb, 0);
	DBG_P("block_indent=%d, c=%d, last=%d", block_indent, c, ch);
	Token_addBLOCKERR(ctx, tkB, in, 0);
}

static void InputStream_parseToken(CTX ctx, knh_InputStream_t *in, knh_Token_t *tkB)
{
	int ch;
	int block_indent = 0, block_line = 0;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	DBG_ASSERT(in->uline != 0);
	Token_setPLUSLINE(tkB, 0);
	L_NEWLINE:;
	{
		int c = 0;
		ctx->gma->uline = in->uline;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == '\t') { c += 3; }
			else if(ch == ' ') { c += 1; }
			else if(ch == '\n') goto L_NEWLINE;
			else {
				if(block_line == 0) {
					block_indent = c;
				}
				else {
					if(block_indent < c) {
						DBG_P("indent %d < %d", block_indent, c);
						Token_setPLUSLINE(tkB, 1);
					}
					else if(c < block_indent) {
						Token_addBLOCKERR(ctx, tkB, in, 0);
						return;
					}
				}
				block_line += 1;
				goto L_AGAIN;
			}
		}
	}
	L_NEWTOKEN:;
	ctx->gma->uline = in->uline;
	knh_cwb_clear(cwb, 0);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_AGAIN:;
		switch(ch) {
		case '\n':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			Token_setPLUSLINE(tkB, 0);
			goto L_NEWLINE;

		case '\\':
			InputStream_skipLINE(ctx, in);
			Token_setPLUSLINE(tkB, 1);
			goto L_NEWLINE;

		case ' ': case '\t': case '\v': case '\r':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			goto L_NEWTOKEN;

		case ';': case ',':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			knh_Bytes_putc(ctx, cwb->ba, ch);
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			goto L_NEWTOKEN;

		case '{':
			Token_setPLUSLINE(tkB, 1);
			Token_addBLOCK(ctx, tkB, cwb, in, block_indent);
			goto L_NEWTOKEN;

		case '[': case '(':
			Token_setPLUSLINE(tkB, 1);
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			{
				knh_Token_t *ctk = new_Token(ctx, TT_ch(ch));
				ctk->h.meta = tkB;
				tkB = ctk;
			}
			goto L_NEWTOKEN;

		case ')': case ']':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			if(TT_ch(ch) == TT_(tkB)) {
				knh_Token_t *ctk = tkB;
				tkB = (knh_Token_t*)(ctk)->h.meta;
				(ctk)->h.meta = NULL;
				Token_add(ctx, tkB, ctk);
				goto L_NEWTOKEN;
			}
		case '}':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			Token_addBLOCKERR(ctx, tkB, in, ch);
			return;
			/*goto L_NEWTOKEN;*/

		/* token */
		case '\'': {  // @CODE prime'
			knh_bytes_t t = knh_cwb_tobytes(cwb);
			if(t.len > 0 && (islower(t.text[t.len-1]) || t.text[t.len-1] == '\'')) {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				break;
			}
		}
		case '"': case '`' :
		case '#':
			ch = Token_addQUOTE(ctx, tkB, cwb, in, ch, 0/*isRAW*/);
			goto L_AGAIN;
		case '$':
			ch = Token_addPROPN(ctx, tkB, cwb, in);
			goto L_AGAIN;
		case '@':
			ch = Token_addMETAN(ctx, tkB, cwb, in);
			goto L_AGAIN;
		case '|':
#ifdef TT_SIZE
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			if(TT_ch(ch) == TT_(tkB)) {
				knh_Token_t *ctk = tkB;
				tkB = (knh_Token_t*)(ctk)->h.meta;
				(ctk)->h.meta = NULL;
				Token_add(ctx, tkB, ctk);
				TT_(ctk) = TT_PARENTHESIS;
				knh_Bytes_write(ctx, cwb->ba, STEXT(".size"));
				Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ' ');
				goto L_NEWTOKEN;
			}
			if(Token_startsWithExpr(ctx, tkB)) {
				knh_Token_t *ctk = new_Token(ctx, TT_ch(ch));
				ctk->h.meta = tkB;
				tkB = ctk;
				goto L_NEWTOKEN;
			}
#endif
		case '?': {
			knh_bytes_t t = knh_cwb_tobytes(cwb);
			if(ISB(t, "in") || ISB(t, "isa") || ISB(t, "is")) {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ' ');
				break;
			}
		}
		case '+': case '-': case '*': case '/': case '%':
		case '=': case '&':
		case '<': case '>': case '^': case '!': case '~':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, ch);
			ch = Token_addOPR(ctx, tkB, cwb, in, ch);
			goto L_AGAIN;

		case '.':
			Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, '.');
			ch = knh_InputStream_getc(ctx, in);
			if(ch == '.') {
				knh_Bytes_putc(ctx, cwb->ba, '.');
				ch = Token_addOPR(ctx, tkB, cwb, in, ch);
			}
			else if(isdigit(ch)) {
				knh_Bytes_putc(ctx, cwb->ba, '0');
				knh_Bytes_putc(ctx, cwb->ba, '.');
				ch = Token_addNUM(ctx, tkB, cwb, in, ch);
			}
			else {
				knh_Bytes_putc(ctx, cwb->ba, '.');
			}
			goto L_AGAIN;

		case ':':
			ch = Token_addURN(ctx, tkB, cwb, in);
			goto L_AGAIN;

		case EOF :
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if(knh_cwb_size(cwb) == 0) {
				ch = Token_addNUM(ctx, tkB, cwb, in, ch);
				goto L_AGAIN;
			}

		default:
			if(ch > 127) {
				WarningUnexpectedCharacter(ctx);
				ch = ' ';
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		} /* switch */
	}/*while*/
	Token_addBuf(ctx, tkB, cwb, TT_UNTYPED, EOF);
}

/* ------------------------------------------------------------------------ */

static void Token_toBRACE(CTX ctx, knh_Token_t *tk, int isEXPANDING)
{
	if(S_size(tk->text) > 0) {
		//fprintf(stderr, "'''%s'''\n", S_tochar(tk->text));
		BEGIN_LOCAL(ctx, lsfp, 1);
		LOCAL_NEW(ctx, lsfp, 0, knh_InputStream_t*, in, new_StringInputStream(ctx, (tk)->text));
		KNH_SETv(ctx, (tk)->data, KNH_NULL);
		TT_(tk) = TT_BRACE;
		in->uline = tk->uline;
		InputStream_parseToken(ctx, in, tk);
		DBG_(
		if(knh_isSystemVerbose() && ULINE_uri(in->uline) == URI_EVAL) {
			knh_write_Object(ctx, KNH_STDOUT, UPCAST(tk), FMT_dump);
		});
		END_LOCAL_(ctx, lsfp);
	}
	else {
		KNH_SETv(ctx, (tk)->data, KNH_NULL);
		TT_(tk) = TT_BRACE;
	}
}

static void Stmt_expand(CTX ctx, knh_Stmt_t *stmt)
{
	size_t newcapacity = DP(stmt)->capacity * 2;
	if(newcapacity == 0) {
		newcapacity = (K_FASTMALLOC_SIZE / sizeof(knh_Object_t*));
	}
	stmt->terms = (knh_Term_t**)KNH_REALLOC(ctx, NULL, stmt->terms, DP(stmt)->capacity, newcapacity, sizeof(knh_Term_t*));
	DP(stmt)->capacity = newcapacity;
}

knh_Term_t *knh_Stmt_add_(CTX ctx, knh_Stmt_t *stmt, ...)
{
	va_list ap;
	knh_Term_t *tm = NULL;
	size_t size = DP(stmt)->size;
	va_start(ap, stmt);
	while((tm = (knh_Term_t*)va_arg(ap, knh_Term_t*)) != NULL) {
		DBG_ASSERT(IS_Token(tm) || IS_Stmt(tm));
		if(!(DP(stmt)->size < DP(stmt)->capacity)) {
			Stmt_expand(ctx, stmt);
		}
		DBG_ASSERT(tmNN(stmt, size) == NULL);
		KNH_INITv(tmNN(stmt, size), tm);
		size++;
		DP(stmt)->size = size;
		if(STT_(stmt) == STT_ERR) continue;
		if(TT_(tm) == TT_ERR) {
			knh_Stmt_toERR(ctx, stmt, (knh_Token_t*)tm);
		}
		if(TT_(tm) == STT_ERR) {
			knh_Stmt_t *stmt2 = (knh_Stmt_t*)tm;
			knh_Token_t *tk = tkNN(stmt2, 0);
			DBG_ASSERT(TT_(tk) == TT_ERR);
			knh_Stmt_toERR(ctx, stmt, tk);
		}
	}
	va_end(ap);
	return (knh_Term_t*)(stmt);
}

knh_Token_t *new_TokenMN(CTX ctx, knh_methodn_t mn)
{
	knh_Token_t *tk = new_(Token);
	tk->uline = ctx->gma->uline;
	TT_(tk) = TT_MN;
	(tk)->mn = mn;
	if(MN_isGETTER(mn)) {Token_setGetter(tk, 1);};
	if(MN_isSETTER(mn)) {Token_setSetter(tk, 1);}
	return tk;
}

/* ------------------------------------------------------------------------ */

#define ADD(stmt, FUNC) \
	if(STT_(stmt) != STT_ERR) {\
		FUNC;\
	}\

static void _EXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr);
static knh_Stmt_t *new_StmtSTMT1(CTX ctx, tkitr_t *itr);

static int ITR_indexTT(tkitr_t *itr, knh_term_t tt, int def)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) return i;
	}
	return def;
}

static void ITR_replace(tkitr_t *itr, knh_term_t tt, knh_term_t tt1)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) TT_(itr->ts[i]) = tt1;
	}
}

static tkitr_t *ITR_copy(tkitr_t *itr, tkitr_t *buf, int shift)
{
	*buf = *itr;
	buf->c = itr->c + shift;
	return buf;
}

static void _SEMICOLON(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_SEMICOLON)) {
		ITR_next(itr);
	}
	else if(STT_(stmt) != STT_ERR) {
		BadPracticeSemicolon(ctx);
	}
}

static void ITR_skipSEMICOLON(tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		if(TT_(ITR_tk(itr)) != TT_SEMICOLON) break;
		ITR_next(itr);
	}
}

static void ITR_nextStmt(tkitr_t *itr)
{
	int i;
	knh_Token_t **ts = itr->ts;
	for(i = itr->c; i < itr->e; i++) {
		if(Token_isBOL(ts[i])) {
			itr->c = i;
			ITR_skipSEMICOLON(itr);
			return;
		}
	}
	itr->c = itr->e;
}

static tkitr_t *ITR_stmt(CTX ctx, tkitr_t *itr, int pos, tkitr_t *buf, int isNeedSemicolon)
{
	int i;
	*buf = *itr;
	for(i = itr->c + 1 + pos; i < itr->e; i++) {
		if(Token_isBOL(itr->ts[i])) {
			if(isNeedSemicolon && TT_(itr->ts[i]) != TT_SEMICOLON) {
				BadPracticeSemicolon(ctx);
			}
			buf->e = i;
			itr->c = i;
			ITR_skipSEMICOLON(itr);
			return buf;
		}
	}
	buf->e = itr->e;
	itr->c = itr->e;
	if(isNeedSemicolon && itr->e -1 >= 0) {
		BadPracticeSemicolon(ctx);
	}
	return buf;
}

static tkitr_t *ITR_first(tkitr_t *titr, int idx, tkitr_t *buf, int shift)
{
	DBG_ASSERT(idx != -1 && idx <= titr->e);
	int i;
	*buf = *titr;
	buf->meta = -1;
	for(i = titr->c; i < idx; i++) {
		knh_term_t tt = TT_(titr->ts[i]);
		if(tt == TT_METAN) {
			if(buf->meta == -1) buf->meta = i;
			titr->c = i + 1;
		}
		else {
			break;
		}
	}
	buf->e = idx;
	titr->c = idx + shift;
	titr->meta = -1;
	return buf;
}

static void ITR_skipUNTIL(tkitr_t *itr, knh_term_t tt)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) {
			itr->e = i;
			return;
		}
	}
}

static void TT_skipMETA(CTX ctx, tkitr_t *itr)
{
	itr->meta = -1;
	while(ITR_hasNext(itr)) {
		knh_term_t tt = ITR_tt(itr);
		ctx->gma->uline = ITR_tk(itr)->uline;
		if(ITR_isN(itr, +1, TT_COLON) && (tt == TT_NAME || tt == TT_UNAME)) {
			if(itr->meta == -1) itr->meta = itr->c;
			ITR_next(itr);
		}
		else if(tt == TT_METAN) {
			if(ITR_isN(itr, +1, TT_STR) || ITR_isN(itr, +1, TT_TSTR) || ITR_isN(itr, +1, TT_ESTR)) {
				TT_(itr->ts[itr->c]) = TT_DOCU;
				break;
			}
			if(itr->meta == -1) itr->meta = itr->c;
		}
		else {
			break;
		}
		ITR_next(itr);
	}
}

#define ITR_ignore(ctx, itr, n)

/* ------------------------------------------------------------------------ */

#define _ERROR(ctx, stmt, itr, whatis)    _DBGERROR(ctx, stmt, itr, whatis K_TRACEPOINT); return;

static void _DBGERROR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, const char *whatis K_TRACEARGV)
{
	if(STT_(stmt) != STT_ERR) {
		knh_Token_t* tkERR;
		if(whatis != NULL && ITR_hasNext(itr)) {
			tkERR = ITR_tk(itr);
			if(TT_(tkERR) != TT_ERR) {
				tkERR = SyntaxErrorTokenIsNot(ctx, ITR_tk(itr), whatis);
			}
		}
		else {
			tkERR = ERROR_Stmt(ctx, stmt, _file, _line, _func);
		}
		knh_Stmt_toERR(ctx, stmt, tkERR);
	}
	ITR_nextStmt(itr);
}
static void _ASIS(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Token(ctx, TT_ASIS));
}

static void _VAR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Token(ctx, TT_VAR));
}
static void _DONE(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Stmt2(ctx, STT_DONE, NULL));
}

typedef int (*knh_FisToken)(knh_Token_t *tk);

static int ITR_isT(tkitr_t *itr, knh_FisToken f)
{
	return (ITR_hasNext(itr) && f(ITR_tk(itr)));
}

static void Stmt_tadd(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, knh_FisToken f, const char *whatis)
{
	if(STT_(stmt) == STT_ERR) return;
	if(ITR_hasNext(itr) && f(ITR_tk(itr))) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
	else {
		_ERROR(ctx, stmt, itr, whatis);
	}
}

static void Stmt_taddASIS(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, knh_FisToken f)
{
	if(STT_(stmt) == STT_ERR) return;
	if(ITR_hasNext(itr)) {
		knh_Token_t *tk = ITR_tk(itr);
		if(TT_(tk) != TT_SEMICOLON && f(tk)) {
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
			return;
		}
	}
	_ASIS(ctx, stmt, itr);
}

static int isVARN(knh_Token_t* tk)
{
	return (TT_(tk) == TT_NAME && !Token_isDOT(tk));
}

static void _PNAME(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_String_t *s = NULL;
	if(itr->c + 1 == itr->e) {
		knh_Token_t *tk = itr->ts[itr->c];
		if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME) {
			s = (tk)->text;
		}
	}
	if(s != NULL) {
		knh_Token_t *tkN = new_Token(ctx, TT_CONST);
		KNH_SETv(ctx, (tkN)->data, s);
		SP(tkN)->type = TYPE_String;
		Token_setPNAME(tkN, 1);
		knh_Stmt_add(ctx, stmt, tkN);
	}
}

static void _EXPRs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_SEMICOLON)) return;
	do {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t ebuf, *eitr = ITR_first(itr, idx, &ebuf, +1);
		if(ITR_size(eitr) != 0) {
			if(STT_(stmt) == STT_PRINT) { /* @TEST print a */
				_PNAME(ctx, stmt, eitr);
			}
			_EXPR(ctx, stmt, eitr);
		}
	}
	while(ITR_hasNext(itr));
}

static knh_index_t ITR_indexTOUNTIL(tkitr_t *itr)
{
	knh_index_t i;
	for(i = itr->c; i < itr->e; i++) {
		knh_term_t tt = TT_(itr->ts[i]);
		if(tt == TT_TO || tt == TT_UNTIL) return i;
		if(tt == TT_ITR || tt == TT_COLON) { /* a[1..n] as a [1 until n] */
			TT_(itr->ts[i]) = TT_UNTIL;
			return i;
		}

	}
	return -1;
}

static void _ASISEXPRs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	do {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t ebuf, *eitr = ITR_first(itr, idx, &ebuf, +1);
		if(ITR_size(eitr) == 0) {
			_ASIS(ctx, stmt, eitr);
		}
		else {
			_EXPR(ctx, stmt, eitr);
		}
	}
	while(ITR_hasNext(itr));
	(itr)->c = (itr)->e - 1;
	if(ITR_is(itr, TT_COMMA)) {
		_ASIS(ctx, stmt, itr);
	}
}

static int ITR_indexKEY(tkitr_t *itr, int shift)
{
	knh_Token_t **ts = itr->ts;
	int i;
	for(i = itr->c + shift; i < itr->e - 2; i++) { /* BEGIN */
		if(TT_(ts[i+1]) == TT_COLON && IS_bString((ts[i])->text)) {
			return i;
		}
	}
	return itr->e;
}

static int ITR_isImmutable(tkitr_t *titr)
{
	if(titr->e > 0) {
		if(TT_(titr->ts[titr->e - 1]) == TT_DOTS) {
			titr->e -= 1;
			return 1;
		}
		return 0;
	}
	return 1;
}

static void _ARRAY(CTX ctx, knh_Stmt_t *stmt, knh_methodn_t mn, knh_class_t cid, tkitr_t *itr)
{
	knh_Token_t *tkC = new_TokenCID(ctx, cid);
	DBG_ASSERT(STT_(stmt) == STT_NEW);
	knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, mn));
	knh_Stmt_add(ctx, stmt, tkC);
	Token_setImmutable(tkC, ITR_isImmutable(itr));
	_EXPRs(ctx, stmt, itr);
}

static knh_Stmt_t *Stmt_addNewStmt(CTX ctx, knh_Stmt_t *stmt, knh_term_t stt)
{
	knh_Stmt_t *newstmt = new_Stmt2(ctx, stt, NULL);
	knh_Stmt_add(ctx, stmt, newstmt);
	return newstmt;
}

static void _KEYVALUEs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int p = ITR_indexKEY(itr, +0), p_next;
	while(p < itr->e) {
		tkitr_t ebuf, *eitr = ITR_copy(itr, &ebuf, 0);
		eitr->c = p;
		p_next = ITR_indexKEY(eitr, +2);
		eitr->e = p_next;
		ITR_chop(eitr, TT_SEMICOLON);
		ITR_chop(eitr, TT_COMMA);
		if(ITR_size(eitr) > 2) {
			knh_Token_t *tk = ITR_nextTK(eitr);
			TT_(tk) = TT_STR;
			knh_Stmt_add(ctx, stmt, tk);
			DBG_ASSERT(ITR_is(eitr, TT_COLON));
			ITR_next(eitr);
			if(!ITR_hasNext(eitr)) {
				knh_Stmt_add(ctx, stmt, new_Token(ctx, TT_NULL));
				return;
			}
			if(ITR_count(eitr, TT_COMMA) == 0) {
				_EXPR(ctx, stmt, eitr);
			}
			else {
				knh_Stmt_t *stmtARRAY = Stmt_addNewStmt(ctx, stmt, STT_NEW);
				_ARRAY(ctx, stmtARRAY, MN_newLIST, CLASS_Array, eitr);
			}
		}
		p = p_next;
	}
}

static void _DICT(CTX ctx, knh_Stmt_t *stmt, knh_Token_t *tkC, knh_Token_t *tkB)
{
	DBG_ASSERT(STT_(stmt) == STT_NEW);
	knh_Stmt_add(ctx, stmt, tkC);
	if(TT_(tkB) == TT_CODE) {
		Token_toBRACE(ctx, tkB, 1/*isEXPANDING*/);
	}
	DBG_ASSERT(TT_(tkB) == TT_BRACE);
	{
		tkitr_t pbuf, *pitr = ITR_new(tkB, &pbuf);
		Token_setImmutable(tkC, ITR_isImmutable(pitr));
		_KEYVALUEs(ctx, stmt, pitr);
	}
}

static void Token_setTYPEOFEXPR(CTX ctx, knh_Token_t *tk)
{
	DBG_ASSERT(TT_(tk) == TT_TYPEOF);
	tkitr_t tbuf, *titr = ITR_new(tk->token, &tbuf);
	if(ITR_hasNext(titr)) {
		knh_Stmt_t *stmt = new_Stmt2(ctx, STT_CALL1, NULL);
		_EXPR(ctx, stmt, titr);
		KNH_SETv(ctx, (tk)->data, stmt);
	}
	else {
		TT_(tk) = TT_VOID;
		KNH_SETv(ctx, (tk)->data, ClassTBL(CLASS_Tvoid)->sname);
	}
}

static knh_Stmt_t *new_StmtREUSE(CTX ctx, knh_Stmt_t *stmt, knh_term_t stt)
{
	if(STT_(stmt) == STT_CALL1 || (STT_(stmt) == STT_CALL && DP(stmt)->size == 0)) {
		STT_(stmt) = stt;
		return stmt;
	}
	else {
		return Stmt_addNewStmt(ctx, stmt, stt);
	}
}

/* ------------------------------------------------------------------------ */
/* EXPR */

knh_short_t TT_to(knh_term_t tt);

static int ITR_indexLET(tkitr_t *itr)
{
	knh_Token_t **ts = itr->ts; int i;
	for(i = itr->c; i < itr->e; i++) {
		knh_term_t tt = TT_(ts[i]);
		if(tt == TT_LET || tt == TT_DECLLET) return i;
		if(TT_DECLLET < tt && tt < TT_LET) {
			TT_(ts[i]) = TT_to(tt);
			return i;
		}
	}
	return -1;
}

static int isLVALUE(knh_Token_t *tk)
{
	knh_term_t tt = TT_(tk);
	if(tt == TT_NAME || tt == TT_UNAME || tt == TT_PROPN) {
		return 1;
	}
	return 0;
}

static void _EXPRLET(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, knh_index_t idx)
{
	knh_Token_t *tkCUR = itr->ts[idx];
	tkitr_t lbuf, *litr = ITR_first(itr, idx, &lbuf, +1);
	if(TT_(tkCUR) != TT_LET) { /* i += 1 ==> i = i + 1 */
		itr->c = litr->c;
	}
	if(ITR_size(litr) == 1) {
		if(ITR_isT(litr, isLVALUE)) {
			stmt = new_StmtREUSE(ctx, stmt, STT_LET);
			_VAR(ctx, stmt, litr);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(litr));
			_EXPR(ctx, stmt, itr); return;
		}
	}
	else {
		if(STT_(stmt) != STT_CALL1) {
			knh_Stmt_t *stmt1 = new_Stmt2(ctx, STT_CALL1, NULL);
			knh_Stmt_add(ctx, stmt, stmt1);
			stmt = stmt1;
		}
		_EXPR(ctx, stmt, litr);
		if(STT_(stmt) == STT_CALL) {
			knh_Token_t *tkS = tkNN(stmt, 0);
			knh_Token_t *tkM = new_Token(ctx, TT_(tkS));
			(tkM)->flag0 = (tkS)->flag0;
			KNH_SETv(ctx, (tkM)->data, (tkS)->data);
			(tkM)->mn = (tkS)->mn;
			KNH_SETv(ctx, tkNN(stmt, 0), tkM);
			if(Token_isGetter(tkM)) {
				Token_setGetter(tkM, 0);
				Token_setSetter(tkM, 1);
				if(isupper(S_tochar(tkM->text)[0])) {
					Stmt_setCLASSCONSTDEF(stmt, 1);
				}
				_EXPR(ctx, stmt, itr);
				return;
			}
		}
	}
	_ERROR(ctx, stmt, itr, NULL);
}

static int ITR_isDOTNAME(tkitr_t *itr, int shift)
{
	int c = itr->c + shift;
	if(c < itr->e) {
		knh_Token_t *tk = itr->ts[c];
		if(!Token_isDOT(tk)) return 0;
		if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME ||
			TT_(tk) == TT_FUNCNAME || TT_(tk) == TT_UFUNCNAME) return 1;
	}
	return 0;
}

static int ITR_isCAST(tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS) && !ITR_isDOTNAME(itr, 1)) {
		tkitr_t cbuf, *citr = ITR_new(ITR_tk(itr), &cbuf);
		if(ITR_is(citr, TT_TO)) {
			ITR_next(citr)
		}
		if(ITR_isT(citr, isTYPE) && ITR_size(citr) == 1) {
			return 1;
		}
	}
	else if(ITR_is(itr, TT_FROM) && itr->c + 1 < itr->e) {
		return 1;
	}
	else if(ITR_is(itr, TT_TSCHEME) && itr->c + 1 < itr->e) {
		return 1;
	}
	return 0;
}

static void _EXPRCAST(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtTCST = new_StmtREUSE(ctx, stmt, STT_TCAST);
	tkitr_t cbuf, *citr = ITR_new(ITR_nextTK(itr), &cbuf);
	if(ITR_is(citr, TT_TO)) {
		TODO();
		ITR_next(citr)
	}
	if(ITR_isT(citr, isTYPE)) {
//			if(ITR_is(citr, TT_PTYPE)) {
//				Token_toStmtTYPEOF(ctx, ITR_tk(citr));
//			}
		knh_Stmt_add(ctx, stmtTCST, ITR_nextTK(citr));
	}
	_EXPR(ctx, stmtTCST, itr); return;
}


int TT_priority(knh_term_t tt);

static int ITR_indexOPR(CTX ctx, tkitr_t *itr)
{
	int i, oppri = 98, idx = -1;
	knh_term_t tt_found = TT_EOT;
	knh_Token_t **ts = itr->ts;
	{
		int isTEXTPARAM = 0;
		for(i = itr->c; i < itr->e; i++) {
			if(TT_(ts[i]) == TT_NAME || TT_(ts[i]) == TT_UNAME) {
				isTEXTPARAM = 1;
				continue;
			}
			if(isTEXTPARAM == 1 && TT_isSTR(TT_(ts[i]))) {
				return -1;
			}
			break;
		}
	}
	for(i = itr->c; i < itr->e; i++) {
		int p = TT_priority(TT_(ts[i]));
		if(p == 0) {
			KNH_SETv(ctx, ts[i], ERROR_Token(ctx, ts[i] K_TRACEPOINT));
			itr->e = i;
			return i;
		}
		if(p <= oppri) { /* p <= oppri is binary*/
			oppri = p;
			tt_found = TT_(ts[i]);
		}
	}
	if(tt_found != TT_EOT) {
		for(i = itr->c; i < itr->e; i++) {
			if(TT_(ts[i]) == tt_found) {
				idx = i; /* last idx */
				if(!TT_isBINARY(tt_found)) break; /* fisrt idx */
			}
		}
	}
//	if(idx != -1) {
//		DBG_P("idx=%d, TT=%s", idx, TT__(TT_(ts[idx])));
//	}
	return idx;
}

static void ITR_toLET(CTX ctx, tkitr_t *itr, knh_Token_t *tk)
{
	int i;
	knh_Token_t *tkC = NULL;
	KNH_SETv(ctx, (tk)->data, new_Array0(ctx, 0));
	for(i = itr->c; i < itr->e; i++) {
		tkC = new_Token(ctx, TT_(itr->ts[i]));
		KNH_SETv(ctx, (tkC)->data, (itr->ts[i])->data);
		(tkC)->index = (itr->ts[i])->index;
		knh_Array_add(ctx, (tk)->list, tkC);
	}
	knh_Array_add(ctx, (tk)->list, new_Token(ctx, TT_LET));
	for(i = itr->c; i < itr->e; i++) {
		tkC = new_Token(ctx, TT_(itr->ts[i]));
		KNH_SETv(ctx, (tkC)->data, (itr->ts[i])->data);
		(tkC)->index = (itr->ts[i])->index;
		knh_Array_add(ctx, (tk)->list, tkC);
	}
	tkC = new_Token(ctx, (TT_(tk)==TT_NEXT) ? TT_ADD : TT_SUB);
	knh_Array_add(ctx, (tk)->list, tkC);
	tkC = new_Token(ctx, TT_NUM);
	KNH_SETv(ctx, (tkC)->data, TS_ONE);
	knh_Array_add(ctx, (tk)->list, tkC);
}

static void _EXPROP(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, int idx, int isCAST)
{
	knh_term_t stt = STT_OPR;
	knh_Token_t *tkOP = itr->ts[idx];
	knh_term_t tt = TT_(tkOP);
	if(ITR_size(itr) == 1) {
		Stmt_toSyntaxError(ctx, stmt, tkOP K_TRACEPOINT);
		return;
	}
	switch(tt){
	case TT_EXISTS: /* @CODE: exists expr */
	case TT_LNOT:   /* @CODE: ~expr */
	case TT_NOT:    /* @CODE: not expr */
	case TT_TSUB:   /* @CODE: -expr */
	case TT_TADD:   /* @CODE: +expr */
	case TT_TAND:   /* @CODE: &expr */ {
		if(isCAST) {
			_EXPRCAST(ctx, stmt, itr);
		}
		else if(itr->c == idx) {
			knh_Stmt_t *stmtOPR = new_StmtREUSE(ctx, stmt, stt);
			knh_Stmt_add(ctx, stmtOPR, tkOP);
			ITR_next(itr);
			_EXPR(ctx, stmtOPR, itr);
		}
		else {
			Stmt_toSyntaxError(ctx, stmt, tkOP K_TRACEPOINT);
		}
		return;
	}
	case TT_NEXT: /* @CODE: expr++, ++expr */
	case TT_PREV: /* @CODE: expr--, --expr */ {
		tkitr_t lbuf, *litr;
		if(ITR_is(itr, tt)) { /* @CODE: ++expr */
			itr->c += 1;
			ITR_toLET(ctx, itr, tkOP);
			litr = ITR_new(tkOP, &lbuf);
			_EXPR(ctx, stmt, litr);
		}
		else { /* @CODE expr++ */
			int c = itr->c, e = itr->e -1;
			itr->e -= 1;
			if(STT_(stmt) != STT_CALL1) {
				stmt = Stmt_addNewStmt(ctx, stmt, STT_CALL1);
			}
			_EXPR(ctx, stmt, itr);
			if(STT_(stmt) != STT_ERR) {
				knh_Stmt_t *stmtPOST = new_Stmt2(ctx, STT_CALL1, NULL);
				KNH_SETv(ctx, DP(stmt)->stmtPOST, stmtPOST);
				itr->c = c; itr->e = e;
				ITR_toLET(ctx, itr, tkOP);
				litr = ITR_new(tkOP, &lbuf);
				_EXPR(ctx, stmtPOST, litr);
			}
		}
		return;
	}
	case TT_QTN: {/* @CODE: expr ? expr : expr */
		int idx2 = ITR_indexTT(itr, TT_COLON, -1);
		if(idx2 != -1) {
			stmt = new_StmtREUSE(ctx, stmt, STT_TRI);
			TT_(tkOP) = TT_COMMA;
			TT_(itr->ts[idx2]) = TT_COMMA;
			_EXPRs(ctx, stmt, itr);
		}
		else {
			knh_Stmt_toERR(ctx, stmt, ERROR_Required(ctx, tkOP, _("trinary"), ":"));
		}
		return;
	}
	case TT_AND: stt = STT_AND; goto L_OPR;
	case TT_OR:  stt = STT_OR; goto L_OPR;
	case TT_ALT: stt = STT_ALT; goto L_OPR;
	default: {
		L_OPR:;
		knh_Stmt_t *stmtOPR = new_StmtREUSE(ctx, stmt, stt);
		if(stt == STT_OPR) {
			knh_Stmt_add(ctx, stmtOPR, tkOP);
		}
		if(TT_isBINARY(tt)) {
			TT_(tkOP) = TT_COMMA;
		}
		else {
			ITR_replace(itr, tt, TT_COMMA);
		}
		DBG_ASSERT(!ITR_is(itr, TT_COMMA));
		_EXPRs(ctx, stmtOPR, itr);
		TT_(tkOP) = tt;
		return;
		}
	}
}

static void _REGEX(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr, knh_Token_t *tk)
{
	knh_Regex_t *re = new_H(Regex);
	const char *opt = "";
	int cflags = 0;
	KNH_INITv(re->pattern, (tk)->text);
	re->spi = knh_getRegexSPI();
	if(re->spi == knh_getStrRegexSPI()) {
		WarningNotInitialized(ctx, tk, "regex");
	}
	re->reg = re->spi->regmalloc(ctx, (tk)->text);
	if(ITR_is(itr, TT_NAME)) {
		opt = S_tochar((ITR_nextTK(itr))->text);
		DBG_P("opt=%s", opt);
	}
	cflags = re->spi->parse_cflags(ctx, opt);
	if(re->spi->regcomp(ctx, re->reg, S_tochar(re->pattern), cflags) != 0) {
		knh_Stmt_toERR(ctx, stmt, ErrorRegexCompilation(ctx, tk, re->spi->name, S_tochar((tk)->text)));
	}
	re->eflags = re->spi->parse_eflags(ctx, opt);
	KNH_SETv(ctx, (tk)->data, re);
	knh_Stmt_add(ctx, stmt, tk);
	ITR_ignore(ctx, itr, +0);
}

static void _CODEDOC(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(STT_(stmt) == STT_ERR) return;
	knh_Token_t *tkDOC = new_Token(ctx, TT_DOC);
	if(ITR_is(itr, TT_CODE)) {
		KNH_SETv(ctx, (tkDOC)->data, (ITR_tk(itr))->text);
	}
	else {
		KNH_SETv(ctx, (tkDOC)->data, TS_EMPTY);
	}
	knh_Stmt_add(ctx, stmt, tkDOC);
}

static void _RETURNEXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtRTN = new_Stmt2(ctx, STT_RETURN, NULL);
	knh_Stmt_add(ctx, stmt, stmtRTN);
	if(ITR_hasNext(itr)) {
		_EXPR(ctx, stmtRTN, itr);
	}
}

static void _PARAM(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr);
static void _PARAMs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr);
static void _STMT1(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr);

static void _EXPR1(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkCUR = ITR_nextTK(itr);
	switch(TT_(tkCUR)) {
		case TT_NAME:    /* @CODE: name */
		case TT_UNAME:   /* @CODE: NAME */
			DBG_ASSERT(!ITR_hasNext(itr));  // to avoid name. hoge
		case TT_NULL:    /* @CODE: null */
		case TT_TRUE:    /* @CODE: true */
		case TT_FALSE:   /* @CODE: false */
		case TT_PTYPE:    /* @CODE: T<T> */
		case TT_PROPN:   /* @CODE: $NAME */
		case TT_TYPEOF:  /* @CODE: typeof(expr) */
		case TT_STR:     /* @CODE: "hoge" */
		case TT_TSTR:    /* @CODE: 'hoge' */
		case TT_ESTR:    /* @CODE: `hoge` */
		case TT_NUM:     /* @CODE: 123m */
		case TT_URN:     /* @CODE: URL */
			knh_Stmt_add(ctx, stmt, tkCUR);
			break;
		case TT_BYTE:    /* @CODE: byte */
			TT_(tkCUR) = TT_NAME;
			knh_Stmt_add(ctx, stmt, tkCUR);
			break;
		case TT_REGEX:
			_REGEX(ctx, stmt, itr, tkCUR);
			break;
		case TT_PARENTHESIS: /* @CODE: () */ {
			tkitr_t pbuf, *pitr = ITR_new(tkCUR, &pbuf);
			int c = ITR_indexTT(pitr, TT_DARROW, -1);
			if(c != -1) {
				STT_(stmt) = STT_FUNCTION;
				_PARAMs(ctx, stmt, pitr);
				_CODEDOC(ctx, stmt, pitr);
				pitr->c = c + 1;
				_RETURNEXPR(ctx, stmt, pitr);
				break;
			}
			c = ITR_count(pitr, TT_COMMA);
			if(c == 0) {
				if(ITR_hasNext(pitr)) {   /* @CODE: (expr) => expr */
					_EXPR(ctx, stmt, pitr);
				}
				else { /* @CODE: () => null */
					TT_(tkCUR) = TT_NULL;
					knh_Stmt_add(ctx, stmt, tkCUR);
				}
			}
			else {  /* @CODE: (1, 2) */
				stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
				_ARRAY(ctx, stmt, MN_newTUPLE, CLASS_Tuple, pitr);
			}
			break;
		}
		case TT_BRANCET: {  /* @CODE: [] */
			tkitr_t pbuf, *pitr = ITR_new(tkCUR, &pbuf);
			knh_class_t cid = CLASS_Array;
			knh_index_t idx = ITR_indexTT(pitr, TT_TO, -1);
			if(idx != -1) { /* [1 to 2] => [1, 2] */
				cid = CLASS_Range;
				TT_(pitr->ts[idx]) = TT_COMMA;
			}
			stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
			_ARRAY(ctx, stmt, MN_newLIST, cid, pitr);
			break;
		}
		case TT_CODE:
		case TT_BRACE: {
			stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
			knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, MN_newMAP));
			_DICT(ctx, stmt, new_TokenCID(ctx, CLASS_Map), tkCUR);
			break;
		}
		case TT_FUNCTION: { /* function () */
			STT_(stmt) = STT_FUNCTION;
			if(ITR_is(itr, TT_FUNCNAME) || ITR_is(itr, TT_UFUNCNAME)) {
				knh_Token_t *tkN = ITR_nextTK(itr);
				WARN_Ignored(ctx, _("function name"), CLASS_unknown, S_tochar(tkN->text));
			}
			if(ITR_is(itr, TT_PARENTHESIS) && ITR_isN(itr, +1, TT_CODE)) {
				tkCUR = new_Token(ctx, TT_DOC);
				KNH_SETv(ctx, (tkCUR)->data, (ITR_tk(itr))->text);
				_PARAM(ctx, stmt, itr);
				knh_Stmt_add(ctx, stmt, tkCUR);
				_STMT1(ctx, stmt, itr);
			}
			else {
				tkCUR = ITR_tk(itr); goto L_ERROR;
			}
			break;
		}
		case TT_ERR:
			knh_Stmt_toERR(ctx, stmt, tkCUR);
			break;
		default: {
			L_ERROR:;
			Stmt_toSyntaxError(ctx, stmt, tkCUR K_TRACEPOINT);
		}
	}
}

static knh_Stmt_t *Stmt_addFUNC(CTX ctx, knh_Stmt_t *stmt, knh_Token_t *tkF)
{
	size_t size = DP(stmt)->size;
	if(TT_(tkF) == TT_NAME || TT_(tkF) == TT_UNAME) {
		Token_setGetter(tkF, 1);
	}
	if(size == 0) {
		DBG_ASSERT(STT_(stmt) == STT_CALL);
		knh_Stmt_add(ctx, stmt, tkF);
	}
	else if(DP(stmt)->size == 1) {
		DBG_ASSERT(STT_(stmt) == STT_CALL);
		knh_Stmt_add(ctx, stmt, tkF);
		knh_Stmt_swap(ctx, stmt, 0, 1);
	}
	else {
		knh_Stmt_t *stmtIN = new_Stmt2(ctx, STT_(stmt), NULL);
		size_t i;
		for(i = 0; i < DP(stmt)->size; i++) {
			knh_Stmt_add(ctx, stmtIN, tmNN(stmt, i), NULL);
		}
		knh_Stmt_done(ctx, stmt);
		STT_(stmt) = STT_CALL;
		knh_Stmt_add(ctx, stmt, tkF, stmtIN);
	}
	if(TT_(tkF) == TT_ASIS) {
		STT_(stmt) = STT_FUNCCALL;
		knh_Stmt_swap(ctx, stmt, 0, 1);  // (_ f) = > (f _)
	}
	DBG_ASSERT(DP(stmt)->size == 2);
	return stmt;
}

#define Stmt_addFUNCMN(ctx, stmt, mn) Stmt_addFUNC(ctx, stmt, new_TokenMN(ctx, mn))

static void _CALLPARAM(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		while(ITR_isDOTNAME(itr, 0)) {
			stmt = Stmt_addFUNC(ctx, stmt, ITR_nextTK(itr));
		}
		if(ITR_is(itr, TT_BRANCET)) {   /* @CODE: expr[...] */
			tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
			int idx = ITR_indexTOUNTIL(pitr);
			if(idx == -1) {
				knh_methodn_t mn = MN_get;
				switch(ITR_count(pitr, TT_COMMA)) {
					case 1: mn = MN_get2; break;  /* @CODE expr[x,y] */
					case 2: mn = MN_get3; break;  /* @CODE expr[x,y,z] */
					case 3: mn = MN_get4; break;  /* @CODE expr[x,y,z,w] */
				}
				stmt = Stmt_addFUNCMN(ctx, stmt, mn);
				_EXPRs(ctx, stmt, pitr);
			}
			else {
				knh_methodn_t mn = (TT_(pitr->ts[idx]) == TT_TO) ? MN_opTO : MN_opUNTIL;
				stmt = Stmt_addFUNCMN(ctx, stmt, mn);
				TT_((pitr)->ts[idx]) = TT_COMMA;  // replace a, b
				_ASISEXPRs(ctx, stmt, pitr);
			}
			continue;
		}
		if(DP(stmt)->size != 2) {
			stmt = Stmt_addFUNC(ctx, stmt, new_(Token) /*ASIS*/);
		}
		if(ITR_is(itr, TT_PARENTHESIS)) {
			tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
			_EXPRs(ctx, stmt, pitr);
		}
		else if(ITR_hasNext(itr)) { /* CODE: f n + 1 =>  f (n+1) */
			Token_setGetter(tkNN(stmt, 0), 0);  // name.f a
			_EXPR(ctx, stmt, itr);
			return;
		}
	}
}

static void _EXPRCALL(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_term_t tt = ITR_tt(itr);
	knh_Token_t *tkCUR = ITR_nextTK(itr);
	int isCALL = 0;
	DBG_ASSERT(ITR_hasNext(itr));
	stmt = new_StmtREUSE(ctx, stmt, STT_CALL);
	switch(tt) {
		case TT_STR: case TT_TSTR: // "%s"(a)
		case TT_FUNCNAME: // f()
		case TT_UFUNCNAME: // F()
		case TT_UNAME:
		case TT_NAME:  // a .f .c
			knh_Stmt_add(ctx, stmt, tkCUR);
			isCALL = 1;
			break;
		case TT_REGEX: {
			_REGEX(ctx, stmt, itr, tkCUR);
			break;
		}
		case TT_NEW: {
			STT_(stmt) = STT_NEW;
			isCALL = 1;
			knh_Stmt_add(ctx, stmt, tkCUR);
			if(ITR_is(itr, TT_UFUNCNAME) /* new T() */ /* thanks, ide */
				|| (ITR_isT(itr, isTYPE) && ITR_isN(itr, +1, TT_PARENTHESIS)) /* new T<T>() */) {
				knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
				break;
			}
			if(ITR_is(itr, TT_PARENTHESIS)) { /* new () */
				_ASIS(ctx, stmt, itr);
				break;
			}
			if(ITR_is(itr, TT_BYTE) && ITR_isN(itr, +1, TT_BRANCET)) {
				TT_(tkCUR) = TT_MN; /* new byte[10] */
				(tkCUR)->mn = MN_newARRAY;
				knh_Stmt_add(ctx, stmt, new_TokenCID(ctx, CLASS_Bytes));
				TT_(itr->ts[itr->c]) = TT_PARENTHESIS;
				break;
			}
			if(ITR_isT(itr, isTYPE) && ITR_isN(itr, +1, TT_BRANCET)) {
				knh_Token_t *tkC = ITR_nextTK(itr);
				TT_(tkCUR) = TT_MN; /* new C[10] */
				(tkCUR)->mn = MN_newARRAY;
				knh_Stmt_add(ctx, stmt, new_TokenPTYPE(ctx, CLASS_Array, tkC));
				TT_(itr->ts[itr->c]) = TT_PARENTHESIS;
				break;
			}
			if(ITR_is(itr, TT_BRANCET)) { /* new [] */
				TT_(tkCUR) = TT_MN; /* new [10] */
				(tkCUR)->mn = MN_newARRAY;
				_ASIS(ctx, stmt, itr);
				TT_(itr->ts[itr->c]) = TT_PARENTHESIS;
				break;
			}
			if(ITR_isT(itr, isTYPE) && ITR_isN(itr, +1, TT_CODE)) {
				knh_Token_t *tkC = ITR_nextTK(itr);
				TT_(tkCUR) = TT_MN; /* new C {} */
				(tkCUR)->mn = MN_newMAP;
				_DICT(ctx, stmt, tkC, ITR_nextTK(itr));
				break;
			}
			if(ITR_is(itr, TT_CODE)) {
				(tkCUR)->mn = MN_newMAP;  /* new {} */
				_DICT(ctx, stmt, new_Token(ctx, TT_ASIS), ITR_nextTK(itr));
				break;
			}
			knh_Stmt_toERR(ctx, stmt, ERROR_Token(ctx, tkCUR K_TRACEPOINT));
			return;
		}/*TT_NEW*/
		default: {
			itr->c -= 1;
			_EXPR1(ctx, stmt, itr);
		}
	}
	if(isCALL || ITR_isDOTNAME(itr, 0) || ITR_is(itr, TT_PARENTHESIS) || ITR_is(itr, TT_BRACE)) {
		_CALLPARAM(ctx, stmt, itr);
	}
	else {
		STT_(stmt) = STT_CALL1;
		if(ITR_hasNext(itr)) WarningMisplaced(ctx);
	}
}

static void _EXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	size_t n = ITR_size(itr);
	if(n == 1) {
		_EXPR1(ctx, stmt, itr);
	}
	else if(n > 1) {
		int idx = ITR_indexLET(itr);
		if(idx != -1) { /* a = b */
			_EXPRLET(ctx, stmt, itr, idx);
			return;
		}
		int isCAST = ITR_isCAST(itr);
		idx = ITR_indexOPR(ctx, itr);
		if(idx != -1) {
			_EXPROP(ctx, stmt, itr, idx, isCAST);
		}
		else if(isCAST) {
			_EXPRCAST(ctx, stmt, itr);
		}
		else if(ITR_is(itr, TT_TSCHEME) && itr->c + 1 < itr->e) {
			knh_Token_t *tkCUR = ITR_nextTK(itr);
			knh_Stmt_t *stmTYPEMAP = new_StmtREUSE(ctx, stmt, STT_TSCHEME);
			_ASIS(ctx, stmTYPEMAP, itr);
			_EXPR(ctx, stmTYPEMAP, itr);
			knh_Stmt_add(ctx, stmTYPEMAP, tkCUR);
		}
		else {
			_EXPRCALL(ctx, stmt, itr);
		}
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
}

typedef void (*knh_Fstmt)(CTX, knh_Stmt_t *stmt, tkitr_t *itr);

static void _PEXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_EXPR(ctx, stmt, pitr);
	}
	else {
		_ERROR(ctx, stmt, itr, "()");
	}
}

static void _ONEEXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int e = itr->e;
	itr->e = itr->c + 1;
	_EXPR(ctx, stmt, itr);
	itr->e = e;
}

/* ------------------------------------------------------------------------ */

static void _DECL(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	DBG_ASSERT(ITR_isT(itr, isTYPE));
	knh_Token_t *tkT = ITR_nextTK(itr);
	tkitr_t declbuf, *declitr = ITR_stmt(ctx, itr, +0, &declbuf, 0/*need;*/);
	while(ITR_hasNext(declitr)) {
		int idx = ITR_indexTT(declitr, TT_COMMA, declitr->e);
		tkitr_t abuf, *aitr = ITR_first(declitr, idx, &abuf, +1);
		if(isVARN(ITR_tk(aitr))) {
			knh_Stmt_add(ctx, stmt, tkT);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(aitr));
		}
		else {
			_ERROR(ctx, stmt, declitr, _("variable"));
		}
		if(ITR_is(aitr, TT_LET)) {
			ITR_next(aitr);
			_EXPR(ctx, stmt, aitr);
		}
		else {
			_ASIS(ctx, stmt, aitr);
		}
		if(ITR_hasNext(declitr)) {
			knh_Stmt_t *stmtDECL = new_Stmt2(ctx, STT_DECL, NULL);
			KNH_INITv(DP(stmt)->nextNULL, stmtDECL);
			stmt = stmtDECL;
		}
	}
}

static void _PARAMs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtP = new_Stmt2(ctx, STT_DECL, NULL);
	int e = itr->e, ridx = ITR_indexTT(itr, TT_DARROW, -1);
	if(ridx != -1) {
		itr->e = ridx;
	}
	knh_Stmt_add(ctx, stmt, stmtP);
	while(ITR_hasNext(itr)) {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t abuf, *aitr = ITR_first(itr, idx, &abuf, +1);
		if(ITR_is(aitr, TT_DOTS)) {
			StmtMETHOD_setVARGs(stmtP, 1);
			break;
		}
		if(ITR_is(aitr, TT_VOID)) {
			break;
		}
		if(ITR_isT(aitr, isTYPE)) {
			knh_Stmt_add(ctx, stmtP, ITR_nextTK(aitr));
		}
		if(DP(stmtP)->size % 3 != 1) {
			_VAR(ctx, stmtP, aitr);
		}
		if(ITR_isT(aitr, isVARN)) {
			knh_Token_t *tkN = ITR_nextTK(aitr);
			knh_Stmt_add(ctx, stmtP, tkN);
		}
		else {
			_ERROR(ctx, stmt, aitr, _("variable"));
		}
		if(ITR_is(aitr, TT_LET)) {
			ITR_next(aitr);
			_EXPR(ctx, stmtP, aitr);
		}
		else {
			_ASIS(ctx, stmtP, aitr);
		}
	}
	if(ridx != -1) {
		itr->e = e;
	}
}

static void _PARAM(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		int funcidx = ITR_indexTT(pitr, TT_DARROW, -1);
		_PARAMs(ctx, stmt, pitr);
		if(funcidx != -1) {
			_CODEDOC(ctx, stmt, itr);
			pitr->c = funcidx + 1;
			_RETURNEXPR(ctx, stmt, pitr);
		}
	}
	else {
		_ERROR(ctx, stmt, itr, _("argument definition"));
	}
}

static knh_bool_t Stmt_checkDepth(knh_Stmt_t *stmt, int depth, int max)
{
	if(depth > max) return 1;
	else {
		size_t i;
		for(i = 0; i < DP(stmt)->size; i++) {
			knh_Stmt_t *stmt2 = stmtNN(stmt, i);
			if(IS_Stmt(stmt2)) {
				if(Stmt_checkDepth(stmt2, depth+1, max)) return 1;
			}
		}
	}
	return 0;
}

static knh_Stmt_t *Stmt_norm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_Stmt_t *stmtITR = stmt;
	knh_Stmt_t *stmtPREV = NULL, *stmtLAST = NULL;
	while(stmtITR != NULL) {
		if(Stmt_checkDepth(stmtITR, 0, 64)) {
			knh_Stmt_toERR(ctx, stmtITR, NULL); // TODO
		}
		if(STT_(stmtITR) == STT_RETURN || STT_(stmtITR) == STT_THROW) {
			if(DP(stmtITR)->nextNULL != NULL) {
				KNH_FINALv(ctx, DP(stmtITR)->nextNULL);
			}
			stmtLAST = NULL;
			break;
		}
		stmtPREV = stmtLAST;
		stmtLAST = stmtITR;
		stmtITR = DP(stmtITR)->nextNULL;
	}
	if(stmtLAST != NULL) {
		ctx->gma->uline = stmtLAST->uline;
		if(STT_(stmtLAST) == STT_CALL1 && !IS_Stmt(DP(stmtLAST)->stmtPOST)) {
			STT_(stmtLAST) = STT_RETURN;
		}
//		else if(stmt_isExpr(STT_(stmtLAST)) && STT_(stmtLAST) != STT_LET) {
//			knh_Stmt_t *stmtRETURN = new_Stmt2(ctx, STT_RETURN, stmtLAST, NULL);
//			if(stmtPREV != NULL) {
//				KNH_SETv(ctx, DP(stmtPREV)->nextNULL, stmtRETURN);
//			}
//			else {
//				stmt = stmtRETURN;
//			}
//		}
	}
	return stmt;
}

static void _STMTs(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtHEAD = NULL, *stmtTAIL = NULL;
	while(ITR_hasNext(itr)) {
		knh_Stmt_t *stmtCUR = new_StmtSTMT1(ctx, itr);
		if(stmtHEAD == NULL) {
			stmtHEAD = stmtCUR;
		}
		else {
			KNH_INITv(DP(stmtTAIL)->nextNULL, stmtCUR);
		}
		stmtTAIL = stmtCUR;
		while(DP(stmtTAIL)->nextNULL != NULL) stmtTAIL = DP(stmtTAIL)->nextNULL;
	}
	if(stmtHEAD == NULL) {
		stmtHEAD = new_Stmt2(ctx, STT_DONE, NULL);
	}
	knh_Stmt_add(ctx, stmt, Stmt_norm(ctx, stmtHEAD));
}

static void _STMT1(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_CODE)) {
		Token_toBRACE(ctx, ITR_tk(itr), 1/*isEXPANDING*/);
	}
	if(ITR_is(itr, TT_BRACE)) {
		tkitr_t stmtbuf, *stmtitr = ITR_new(ITR_nextTK(itr), &stmtbuf);
		_STMTs(ctx, stmt, stmtitr);
	}
	else {
		knh_Stmt_add(ctx, stmt, Stmt_norm(ctx, new_StmtSTMT1(ctx, itr)));
	}
}

static void _ELSE(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_ELSE)) {
		ITR_next(itr);
		_STMT1(ctx, stmt, itr);
	}
	else {
		knh_Stmt_add(ctx, stmt, new_Stmt2(ctx, STT_DONE, NULL));
	}
}

static void _DOWHILE(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_CODE)) {
		Token_toBRACE(ctx, ITR_tk(itr), 1/*isEXPANDING*/);
	}
	if(ITR_is(itr, TT_BRACE)) {
		tkitr_t stmtbuf, *stmtitr = ITR_new(ITR_nextTK(itr), &stmtbuf);
		_STMTs(ctx, stmt, stmtitr);
		if(ITR_is(itr, TT_WHILE)) {
			ITR_next(itr);
		}
		return;
	}
	_ERROR(ctx, stmt, itr, _("{} for while body"));
}

static void _CASESTMT(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtHEAD = NULL, *stmtLAST = NULL;
	tkitr_t stmtbuf;
	if(ITR_is(itr, TT_CODE)) {
		Token_toBRACE(ctx, ITR_tk(itr), 1/*isEXPANDING*/);
	}
	if(ITR_is(itr, TT_BRACE)) {
		itr = ITR_new(ITR_nextTK(itr), &stmtbuf);
	}
	else {
		_ERROR(ctx, stmt, itr, "{}");
		return;
	}
	while(ITR_hasNext(itr)) {
		knh_term_t tt = TT_(ITR_nextTK(itr));
		knh_Stmt_t *stmtCASE;
		if(tt == TT_CASE || tt == TT_DEFAULT) {
			int idx = ITR_indexTT(itr, TT_COLON, -1);
			if(idx == -1) {
				_ERROR(ctx, stmt, itr, ":");
				goto L_RETURN;
			}
			stmtCASE = new_Stmt2(ctx, STT_CASE, NULL);
			if(stmtHEAD == NULL) {
				stmtHEAD = stmtCASE;
			}
			else {
				KNH_INITv(DP(stmtLAST)->nextNULL, stmtCASE);
			}
			stmtLAST = stmtCASE;
			if(tt == TT_CASE) {
				int e = itr->e;
				itr->e = idx;
				_EXPR(ctx, stmtCASE, itr);
				itr->e = e;
			}
			else {
				_ASIS(ctx, stmtCASE, itr);
			}
			{
				int i, e = itr->e;
				itr->c = idx + 1;
				for(i = itr->c; i < itr->e; i++) {
					tt = TT_(itr->ts[i]);
					if(tt == TT_CASE || tt == TT_DEFAULT) {
						itr->e = i;
						break;
					}
				}
				_STMTs(ctx, stmtCASE, itr);
				itr->c = i;
				itr->e = e;
			}
		}
		else {
			_ERROR(ctx, stmt, itr, NULL);
			goto L_RETURN;
		}
	}
	if(stmtHEAD == NULL) {
		stmtHEAD = new_Stmt2(ctx, STT_DONE, NULL);
	}
	L_RETURN:;
	if(stmtHEAD != NULL) {
		knh_Stmt_add(ctx, stmt, stmtHEAD);
	}
}

static int ITR_indexINFROM(tkitr_t *itr)
{
	if(ITR_size(itr) > 2) {
		int i, c = itr->c + 1;
		if(TT_(itr->ts[c]) == TT_UNAME) c++;  // skip InputSteam in
		for(i = c; i < itr->e; i++) {
			knh_term_t tt = TT_(itr->ts[i]);
			if(tt == TT_FROM || tt == TT_COLON) {
				return i;
			}
			if(tt == TT_NAME && ISB(S_tobytes((itr->ts[i])->text), "in")) {
				return i;
			}
		}
	}
	return -1;
}

static void _EACH(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int idx = ITR_indexINFROM(itr);
	if(idx != -1) {
		tkitr_t pbuf, *pitr = ITR_first(itr, idx, &pbuf, +1);
		_PARAMs(ctx, stmt, pitr);
		_EXPR(ctx, stmt, itr);
	}
	else {
		itr->c += 1;
		_ERROR(ctx, stmt, itr, "from");
	}
}

static void _PEACH(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_EACH(ctx, stmt, pitr);
	}
	else {
		int idx = ITR_indexTT(itr, TT_CODE, -1);
		if(idx != -1) {
			tkitr_t pbuf, *pitr = ITR_first(itr, idx, &pbuf, 0);
			_EACH(ctx, stmt, pitr);
		}
		else {
			_ERROR(ctx, stmt, itr, "(... from ...)");
		}
	}
}

static void _PSTMT3(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		if(ITR_count(pitr, TT_SEMICOLON) == 2) {
			int idx = ITR_indexTT(pitr, TT_SEMICOLON, itr->e);
			tkitr_t stmtbuf, *stmtitr = ITR_first(pitr, idx, &stmtbuf, +1);
			stmtitr->e += 1; // including ";"
			knh_Stmt_add(ctx, stmt, new_StmtSTMT1(ctx, stmtitr));
			idx = ITR_indexTT(pitr, TT_SEMICOLON, itr->e);
			stmtitr = ITR_first(pitr, idx, &stmtbuf, +1);
			if(!ITR_hasNext(stmtitr)) {
				knh_Stmt_add(ctx, stmt, new_Token(ctx, TT_TRUE));
			}
			else {
				_EXPR(ctx, stmt, stmtitr);
			}
			knh_Stmt_add(ctx, stmt, new_StmtSTMT1(ctx, pitr));
			return;
		}
		else if(ITR_indexINFROM(pitr) != -1) {
			_EACH(ctx, stmt, pitr); return;
		}
	}
	_ERROR(ctx, stmt, itr, _("(...; ...; ...) in for statement"));
}

static int isLABEL(knh_Token_t *tk)
{
	knh_term_t tt = TT_(tk);
	return (tt == TT_UNAME || tt == TT_NAME);
}

static void _LABEL(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_hasNext(itr) && isLABEL(ITR_tk(itr))) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
		return;
	}
	_ASIS(ctx, stmt, itr);
}

static int isEVENT(knh_Token_t *tk)
{
	if(Token_isExceptionType(tk)) return 1;
	return 0;
}

static knh_Stmt_t *Stmt_tail(CTX ctx, knh_Stmt_t *o)
{
	knh_Stmt_t *tail = o;
	while(DP(tail)->nextNULL != NULL) {
		tail = DP(tail)->nextNULL;
	}
	return tail;
}

static knh_Stmt_t* StmtNULL_append(CTX ctx, knh_Stmt_t *o, knh_Stmt_t *stmt)
{
	if(o == NULL) {
		return stmt;
	}
	else {
		knh_Stmt_t *tail = Stmt_tail(ctx, o);
		KNH_INITv(DP(tail)->nextNULL, stmt);
		return o;
	}
}

static void _CATCH(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtHEAD = NULL;
	while(ITR_is(itr, TT_CATCH)) {
		ITR_next(itr);
		if(ITR_is(itr, TT_PARENTHESIS)) {
			tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
			knh_Stmt_t *stmtCATCH = new_Stmt2(ctx, STT_CATCH, NULL);
			Stmt_tadd(ctx, stmtCATCH, pitr, isEVENT, _("exception"));
			Stmt_tadd(ctx, stmtCATCH, pitr, isVARN, _("variable"));
			_STMT1(ctx, stmtCATCH, itr);
			stmtHEAD = StmtNULL_append(ctx, stmtHEAD, stmtCATCH);
		}
		else {
			_ERROR(ctx, stmt, itr, _("(Exception e)"));
		}
	}
	if(stmtHEAD == NULL) {
		stmtHEAD = new_Stmt2(ctx, STT_DONE, NULL);
	}
	knh_Stmt_add(ctx, stmt, stmtHEAD);
	if(ITR_is(itr, TT_FINALLY)) {
		ITR_next(itr);
		_STMT1(ctx, stmt, itr);
	}
	else {
		_DONE(ctx, stmt, itr);
	}
}

static void _PRAGMA(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
}

static void _USING(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		knh_Token_t *tk = ITR_nextTK(itr);
		if(TT_(tk) == TT_OR) _ASIS(ctx, stmt, itr);
		knh_Stmt_add(ctx, stmt, tk);
	}
	_ASIS(ctx, stmt, itr);
}

static int isCLASSAME(knh_Token_t* tk)
{
	if(TT_(tk) == TT_UNAME || TT_(tk) == TT_UFUNCNAME) {
//		knh_bytes_t t = S_tobytes((tk)->text);
//		if(knh_bytes_index(t, ':') == -1) {
//			return isupper(t.utext[0]);
//		}
		TT_(tk) = TT_UNAME;
		return 1;
	}
	return 0;
}

static void _CLASS(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{	// CNAME:0 NEW:1 EXTENDS:2 IMPLEMENTS:3 STMT:4
	Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("class name")); /*0*/
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_PARAMs(ctx, stmt, pitr);
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
	if(ITR_is(itr, TT_EXTENDS)) {
		ITR_next(itr);
		Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("class name")); /*2*/
	}
	else { /* Object */
		knh_Stmt_add(ctx, stmt, new_TokenCID(ctx, CLASS_Object));
	}
	if(ITR_is(itr, TT_IMPLEMENTS)) {
		ITR_skipUNTIL(itr, TT_CODE);
		_ASIS(ctx, stmt, itr); /* implements for future 3*/
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
	if(ITR_is(itr, TT_CODE)) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
	else {
		_SEMICOLON(ctx, stmt, itr);
	}
}

static int isMN(knh_Token_t* tk)
{
	if(TT_(tk) == TT_UFUNCNAME) TT_(tk) = TT_UFUNCNAME;
	return (TT_(tk) == TT_FUNCNAME  /* || TT_(tk) == TT_PROPN*/);
}

static void _CODE(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	size_t i, hasCODE = 0;;
	for(i = 0; i < DP(stmt)->size; i++) {
		if(TT_(tkNN(stmt, i)) == TT_DOC) {
			hasCODE = 1; break;
		}
	}
	if(ITR_is(itr, TT_CODE)) {
		if(hasCODE) {
			WARN_Ignored(ctx, "block", CLASS_unknown, S_tochar(ITR_nextTK(itr)->text));
		}
		else {
			_CODEDOC(ctx, stmt, itr);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
		}
	}
	else if(ITR_is(itr, TT_DARROW) || ITR_is(itr, TT_COLON)) {
		tkitr_t stmtbuf, *stmtitr = ITR_stmt(ctx, itr, 0, &stmtbuf, 1);
		if(hasCODE) {
			WARN_Ignored(ctx, "=>", CLASS_unknown, NULL);
		}
		else {
			_CODEDOC(ctx, stmt, stmtitr);
			stmtitr->c += 1;
			_RETURNEXPR(ctx, stmt, stmtitr);
		}
	}
	else if(ITR_is(itr, TT_USING)) {
		tkitr_t stmtbuf, *stmtitr = ITR_stmt(ctx, itr, 0, &stmtbuf, 1);
		if(hasCODE) {
			WARN_Ignored(ctx, "using", CLASS_unknown, NULL);
		}
		else {
			StmtMETHOD_setFFI(stmt, 1);
			stmtitr->c += 1;
			_EXPR(ctx, stmt, stmtitr);
		}
	}
	else {
		_SEMICOLON(ctx, stmt, itr);
	}
}

static void _METHOD(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_nextTK(itr);
	knh_Stmt_add(ctx, stmt, tkT);
	if(TT_(tkT) == TT_FUNCTION) TT_(tkT) = TT_ASIS;
	if(ITR_is(itr, TT_UNAME)) {
		Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("class name"));
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
	Stmt_taddASIS(ctx, stmt, itr, isMN);
	ADD(stmt, _PARAM(ctx, stmt, itr));
	ADD(stmt, _CODE(ctx, stmt, itr));
}

static void _CONSTRUCTOR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_tk(itr);
	TT_(tkT) = TT_UNAME;
	Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("class name"));
	_ASIS(ctx, stmt, itr);
	knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, MN_new));
	ADD(stmt, _PARAM(ctx, stmt, itr));
	ADD(stmt, _CODE(ctx, stmt, itr));
}

static void _FORMAT(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_nextTK(itr);
	knh_Stmt_add(ctx, stmt, tkT);  // void
	ITR_nextTK(itr); // skip %
	_ASIS(ctx, stmt, itr);  // class name
	knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	ADD(stmt, _PARAM(ctx, stmt, itr));
	ADD(stmt, _CODE(ctx, stmt, itr));
}

/* ------------------------------------------------------------------------ */
/* META */

static void Stmt_addMETA(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int i, e;
	knh_Token_t **ts = itr->ts;
	if(itr->meta == -1 || STT_(stmt) == STT_DONE || STT_(stmt) == STT_ERR) {
		return ;
	}
	e = (itr->meta < itr->c) ? itr->c : itr->e;
	if(IS_NULL(DP(stmt)->metaDictCaseMap)) {
		KNH_SETv(ctx, DP(stmt)->metaDictCaseMap, new_DictMap0(ctx, 2, 1/*isCaseMap*/, "Stmt.MetaDictMap"));
	}
	for(i = itr->meta; i < e; i++) {
		if(TT_(ts[i]) == TT_NAME || TT_(ts[i]) == TT_UNAME) {
			knh_DictMap_set(ctx, DP(stmt)->metaDictCaseMap, TS_ATlabel, ts[i]);
			i++;
			DBG_ASSERT(TT_(ts[i]) == TT_COLON);
		}
		else if(TT_(ts[i]) == TT_METAN) {
			knh_DictMap_set(ctx, DP(stmt)->metaDictCaseMap, (ts[i])->text, UPCAST(ts[i]));
		}
	}
	if(e == itr->e) itr->c = i;
	itr->meta = -1;
}

static knh_Stmt_t *new_StmtMETA(CTX ctx, knh_term_t stt, tkitr_t *itr, int shift, ...)
{
	knh_Fstmt func = NULL;
	va_list ap;
	knh_Stmt_t *stmt = new_Stmt2(ctx, stt, NULL);
	Stmt_addMETA(ctx, stmt, itr);
	itr->c = itr->c + shift;
	va_start(ap , shift);
	while((func = (knh_Fstmt)va_arg(ap, knh_Fstmt)) != NULL) {
		if(STT_(stmt) == STT_ERR) {
			return stmt;
		}
		func(ctx, stmt, itr);
	}
	va_end(ap);
	return stmt;
}

/* ------------------------------------------------------------------------ */
/* [STMT1] */

#define CASE_(XX, N, ...) case TT_##XX : { \
		stmt = new_StmtMETA(ctx, STT_##XX , itr, N, ## __VA_ARGS__, NULL); \
		break;\
	}\

#define CASE_L(XX, N, CHECKSEMI, ...) case TT_##XX : { \
		tkitr_t stmtbuf, *stmtitr = ITR_stmt(ctx, itr, N, &stmtbuf, CHECKSEMI);\
		stmt = new_StmtMETA(ctx, STT_##XX , stmtitr, N, ## __VA_ARGS__, NULL);\
		break;\
	}\

static int Token_isMAP(CTX ctx, knh_Token_t *tk)
{
	int isMAP = 1;
	if(TT_(tk) == TT_BRACE) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		if(titr->e > 0) {
			int i, c = 0;
			for(i = 0; i < titr->e; i++) {
				knh_Token_t *tkN = titr->ts[i];
				if(TT_(tkN) <= TT_FINALLY) {
					isMAP = 0;
					goto L_RETURN;
				}
				if(TT_(tkN) == TT_COLON) c++;
			}
			if(c > 0) isMAP = 1;
		}
	}
	DBG_P("@@@@@@@ isMAP=%d", isMAP);
	L_RETURN:;
	return isMAP;
}

static void _STMTEXPR(CTX ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_index_t idx = ITR_indexTT(itr, TT_LET, -1);
	if(idx != -1) {
		int comma = ITR_count(itr, TT_COMMA);
		if(comma > 0) {  //  check multiple assingment
			tkitr_t lbuf, *litr = ITR_first(itr, idx, &lbuf, +1);
			int lcomma = ITR_count(litr, TT_COMMA);
			if(lcomma == comma) {  // @CODE: a, b = t
				STT_(stmt) = STT_LETM;
				_EXPRs(ctx, stmt, litr);
				_EXPR(ctx, stmt, itr);
			}
			else if(lcomma * 2 == comma) {   // @CODE: a, b = b, a
				STT_(stmt) = STT_SWAP;
				_EXPRs(ctx, stmt, litr);
				_EXPRs(ctx, stmt, itr);
			}
			else {
				knh_Stmt_toERR(ctx, stmt, ERROR_text(ctx,  "mismatched assignment" K_TRACEPOINT));
			}
			return;
		}
		else if(ITR_is(itr, TT_UNAME) && itr->c+1 == idx) {
			STT_(stmt) = STT_CONST;
			ITR_replace(itr, TT_LET, TT_COMMA);
			_EXPRs(ctx, stmt, itr);
			return;
		}
		_EXPR(ctx, stmt, itr);
		return;
	}
	idx = ITR_indexTT(itr, TT_LSEND, -1);
	if(idx != -1) {
		STT_(stmt) = STT_SEND;
		knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, MN_send));
		ITR_replace(itr, TT_LSEND, TT_COMMA);
		_EXPRs(ctx, stmt, itr);
		return;
	}
	idx = ITR_indexTT(itr, TT_RSEND, -1);
	if(idx != -1) {
		int e = itr->e;
		itr->e = idx;
		_EXPR(ctx, stmt, itr);
		if(STT_(stmt) == STT_FUNCCALL) {
			itr->c = idx + 1; itr->e = e;
			KNH_SETv(ctx, stmtNN(stmt, 1), new_StmtMETA(ctx, STT_CALL1, itr, 0, _EXPR, NULL));
		}
		else {
			if(STT_(stmt) != STT_ERR) {
				knh_Stmt_toERR(ctx, stmt, ERROR_text(ctx,  "message should be sent" K_TRACEPOINT));
			}
		}
		return;
	}
	_EXPR(ctx, stmt, itr);
}

static knh_Stmt_t *new_StmtSTMT1(CTX ctx, tkitr_t *itr)
{
	knh_term_t tt;
	knh_Stmt_t *stmt = NULL;
	knh_Token_t *tkLINE = NULL;
	L_TAIL:;
	TT_skipMETA(ctx, itr);
	if(!ITR_hasNext(itr)) {
		stmt = new_Stmt2(ctx, STT_DONE, NULL);
		goto L_RETURN;
	}
	tkLINE = ITR_tk(itr);
	tt = ITR_tt(itr);
	switch(tt) {
		case TT_SEMICOLON: {
			ITR_next(itr);
			goto L_TAIL;
		}
		CASE_L(PRAGMA, +1, 1/*;*/, _PRAGMA)
		CASE_(NAMESPACE, +1, _STMT1);
		CASE_(SCRIPT, +1, _STMT1);
		CASE_L(INCLUDE, +1, 1/*;*/, _EXPRs);
		CASE_L(USING, +1, 1/*;*/, _USING);
		CASE_(CLASS, +1, _CLASS);
		CASE_L(BREAK, +1, 1/*;*/, _LABEL);
		CASE_L(CONTINUE, +1, 1/*;*/, _LABEL);
		CASE_L(RETURN, +1, 1/*;*/, _EXPRs);
		CASE_L(THROW, +1, 1/*;*/, _EXPRs);
		CASE_L(REGISTER, +1, 1/*;*/, _EXPRs);
		CASE_L(PRINT, +1, 1/*;*/, _EXPRs);
		CASE_L(YIELD, +1, 1/*;*/, _EXPRs);
		CASE_(IF, +1, _PEXPR, _STMT1, _ELSE);
		CASE_(SWITCH, +1, _PEXPR, _CASESTMT, _ASIS);  /* it */
		CASE_(WHILE, +1, _PEXPR, _STMT1);
		CASE_(DO, +1, _DOWHILE, _PEXPR, _SEMICOLON);
		CASE_(FOR, +1, _PSTMT3, _STMT1);
		CASE_(FOREACH, +1, _PEACH, _STMT1, _ASIS);  /* it */
		CASE_(TRY, +1, _STMT1, _CATCH, _ASIS);  /* it */
		CASE_(ASSURE, +1, _ONEEXPR, _STMT1, _ASIS/*it*/);
		CASE_L(ASSERT, +1, 1/*;*/, _EXPR);
		case TT_CODE:
			Token_toBRACE(ctx, ITR_tk(itr), 1/*isEXPANDING*/);
		case TT_BRACE:
			if(!Token_isMAP(ctx, ITR_tk(itr))) {
				tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
				stmt = new_StmtMETA(ctx, STT_BLOCK, pitr, 0, _STMTs, NULL);
			}
			break /*L_EXPR*/;
		case TT_VOID: {
			if(ITR_isN(itr, +1, TT_MOD) && (ITR_isN(itr, +2, TT_FUNCNAME) || ITR_isN(itr, +2, TT_UFUNCNAME))) {
				stmt = new_StmtMETA(ctx, STT_FORMAT, itr, 0, _FORMAT, NULL);
				break;
			}
		}
		case TT_VAR: case TT_PTYPE: case TT_DYN:
		case TT_TYPEOF: case TT_BYTE:
		case TT_UNAME: {
			tkitr_t mbuf, *mitr = ITR_copy(itr, &mbuf, +1);
			if(ITR_isT(mitr, isVARN) && tt != TT_VOID) {
				tkitr_t dbuf, *ditr = ITR_stmt(ctx, itr, /*pos*/0, &dbuf, 1/*needs;*/);
				stmt = new_StmtMETA(ctx, STT_DECL, ditr, 0, _DECL, NULL);
				break;
			}
			if(ITR_is(mitr, TT_UNAME)) {
				ITR_next(mitr);
				if(ITR_is(mitr, TT_UFUNCNAME)) {
					knh_Token_t *tkUF = ITR_tk(mitr); TT_(tkUF) = TT_FUNCNAME;
					WarningMethodName(ctx, S_tochar(tkUF->text));
				}
				if(ITR_is(mitr, TT_FUNCNAME)) {
					stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
					break;
				}
			}
			if(ITR_is(mitr, TT_FUNCNAME) && !Token_isDOT(ITR_tk(mitr))) {
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
				break;
			}
			if(ITR_is(mitr, TT_UFUNCNAME) && !Token_isDOT(ITR_tk(mitr))) {
				knh_Token_t *tkUF = ITR_tk(mitr); TT_(tkUF) = TT_FUNCNAME;
				WarningMethodName(ctx, S_tochar(tkUF->text));
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
				break;
			}
			break /*L_EXPR*/;
		}
		case TT_FUNCTION: {
			if(ITR_isN(itr, +1, TT_FUNCNAME) || ITR_isN(itr, +1, TT_UFUNCNAME)) {
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
				break;
			}
			break /*L_EXPR*/;
		}
		case TT_UFUNCNAME: {
			if(ITR_isN(itr, +2, TT_CODE)) {
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _CONSTRUCTOR, NULL);
				break;
			}
		}
		case TT_PROPN:
		case TT_NAME:
		case TT_FUNCNAME:
		case TT_FMT:
		case TT_PARENTHESIS:
		case TT_BRANCET:
		case TT_TRUE: case TT_FALSE: case TT_NULL:
		case TT_NOT: case TT_EXISTS: case TT_TSCHEME:
		case TT_LNOT:
		case TT_NEXT: case TT_PREV:  /* Prev */
		case TT_ITR: case TT_NEW:
		case TT_TADD: case TT_TAND: case TT_TSUB:
		case TT_NUM:
		case TT_STR: case TT_TSTR: /* case TT_FMTSTR: */
		case TT_ESTR: case TT_REGEX:
		case TT_URN:
			break; // EXPR
		case TT_ERR:  default: {
			tkitr_t sbuf, *sitr = ITR_stmt(ctx, itr, /*pos*/0, &sbuf, 0/*needs;*/);
			stmt = new_StmtMETA(ctx, STT_CALL1, sitr, 0, NULL);
			_DBGERROR(ctx, stmt, sitr, "value" K_TRACEPOINT);
	 		break;
		}
	}
	if(stmt == NULL) {
		tkitr_t sbuf, *sitr = ITR_stmt(ctx, itr, /*pos*/0, &sbuf, 0/*needs;*/);
		stmt = new_StmtMETA(ctx, STT_CALL1, sitr, 0, _STMTEXPR, NULL);
	}
	L_RETURN:;
	return stmt;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *knh_InputStream_parseStmt(CTX ctx, knh_InputStream_t *in)
{
	DBG_ASSERT(in->uline != 0);
	ctx->gma->uline = in->uline;
	knh_Stmt_t *rVALUE = new_Stmt2(ctx, STT_BLOCK, NULL);
	knh_Token_t *tk = new_Token(ctx, TT_BRACE);
	BEGIN_LOCAL(ctx, lsfp, 2);
	KNH_SETv(ctx, lsfp[0].o, rVALUE);
	KNH_SETv(ctx, lsfp[1].o, tk);
	InputStream_parseToken(ctx, in, tk);
	DBG_(if(knh_isSystemVerbose() /*&& DP(in)->uri == URI_EVAL*/) {
		knh_write_Object(ctx, KNH_STDOUT, UPCAST(tk), FMT_dump);
	})
	if(TT_(tk) != TT_ERR) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		_STMTs(ctx, rVALUE, titr);
		DBG_ASSERT(DP(rVALUE)->size == 1);
		if(IS_Stmt(stmtNN(rVALUE, 0))) {
			rVALUE = stmtNN(rVALUE, 0);
			DBG_(if(knh_isSystemVerbose() /*&& DP(in)->uri == URI_EVAL*/) {
				knh_write_Object(ctx, KNH_STDOUT, UPCAST(rVALUE), FMT_dump);
			})
			goto L_RETURN;
		}
	}
	knh_Stmt_toERR(ctx, rVALUE, tk);
	L_RETURN:;
	END_LOCAL(ctx, lsfp, rVALUE);
	return rVALUE;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *knh_Token_parseStmt(CTX ctx, knh_Token_t *tk)
{
	ctx->gma->uline = tk->uline;
	BEGIN_LOCAL(ctx, lsfp, 1);
	knh_Stmt_t *rVALUE = new_Stmt2(ctx, STT_BLOCK, NULL);
	KNH_SETv(ctx, lsfp[0].o, rVALUE);
	Token_toBRACE(ctx, tk, 1/*isEXPANDING*/);
	if(TT_(tk) != TT_ERR) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		_STMTs(ctx, rVALUE, titr);
		DBG_ASSERT(DP(rVALUE)->size == 1);
		if(STT_(rVALUE) != STT_ERR) {
			rVALUE = stmtNN(rVALUE, 0);
		}
		DBG_ASSERT(IS_Stmt(rVALUE));
	}
	else {
		knh_Stmt_toERR(ctx, rVALUE, tk);
	}
	END_LOCAL(ctx, lsfp, rVALUE);
	return rVALUE;
}

knh_Stmt_t *knh_bytes_parseStmt(CTX ctx, knh_bytes_t expr, knh_uline_t uline)
{
	ctx->gma->uline = uline;
	BEGIN_LOCAL(ctx, lsfp, 2);
	LOCAL_NEW(ctx, lsfp, 0, knh_Stmt_t*, rVALUE, new_Stmt2(ctx, STT_BLOCK, NULL));
	LOCAL_NEW(ctx, lsfp, 1, knh_Token_t*, tk, new_Token(ctx, TT_CODE));
	KNH_SETv(ctx, (tk)->data, new_S(ctx, expr));
	Token_toBRACE(ctx, tk, 1/*isEXPANDING*/);
	if(TT_(tk) != TT_ERR) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		_STMTs(ctx, rVALUE, titr);
		DBG_ASSERT(DP(rVALUE)->size > 0);
		rVALUE = stmtNN(rVALUE, 0);
	}
	else {
		knh_Stmt_toERR(ctx, rVALUE, tk);
	}
	END_LOCAL(ctx, lsfp, rVALUE);
	return rVALUE;
}

///* ------------------------------------------------------------------------ */
//
//static void Array_addTEXT(CTX ctx, knh_Array_t *a, knh_cwb_t *cwb)
//{
//	if(knh_cwb_size(cwb) != 0) {
//		knh_Array_add(ctx, a, KNH_NULL);
//		knh_Array_add(ctx, a, knh_cwb_newString(ctx, cwb));
//	}
//}
//
//static knh_bool_t Array_addEXPR(CTX ctx, knh_Array_t *a, knh_bytes_t mt, knh_bytes_t expr, knh_uline_t uline)
//{
//	knh_Array_add(ctx, a, new_S(ctx, mt));
//	if(expr.utext[0] == '#') {
//		knh_Array_add(ctx, a, new_S(ctx, expr));
//		return 1;
//	}
//	else {
//		knh_Stmt_t *stmt;
//		BEGIN_LOCAL(ctx, lsfp, 1);
//		LOCAL_NEW(ctx, lsfp, 0, knh_Token_t*, tk, new_Token(ctx, TT_CODE));
//		KNH_SETv(ctx, (tk)->data, new_S(ctx, expr));
//		tk->uline = uline;
//		stmt = knh_Token_parseStmt(ctx, tk);
//		knh_Array_add(ctx, a, stmt);
//		END_LOCAL_(ctx, lsfp);
//		return (STT_(stmt) != STT_ERR);
//	}
//}
//
//knh_bool_t knh_String_parseFMT(CTX ctx, knh_String_t *fmt, knh_Array_t *a, knh_uline_t uline)
//{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_bytes_t t = S_tobytes(fmt);
//	size_t i = 0, s = 0;
//	L_TEXT:;
//	for(;i < t.len; i++) {
//		if(t.utext[i] == '%') {
//			i++;
//			if(t.utext[i] == '%') {
//				knh_Bytes_putc(ctx, cwb->ba, '%');
//				continue;
//			}
//			goto L_FMT;
//		}
//		if(t.utext[i] == '\n') uline++;
//		knh_Bytes_putc(ctx, cwb->ba, t.utext[i]);
//	}
//	Array_addTEXT(ctx, a, cwb);
//	return 1;
//	L_FMT:;
//	s = i-1;
//	for(;i < t.len; i++) {
//		if(t.utext[i] == '\n') uline++;
//		if(t.utext[i] == '{') {
//			knh_bytes_t mt = {{t.text + s}, i - s};
//			knh_bytes_t t2 = {{t.text + (i+1)}, t.len - (i+1)};
//			knh_index_t loc = knh_bytes_index(t2, '}');
//			if(loc != -1) {
//				Array_addTEXT(ctx, a, cwb);
//				if(!Array_addEXPR(ctx, a, mt, knh_bytes_first(t2, loc), uline)) {
//					return 0;
//				}
//				knh_cwb_close(cwb);
//				i = i + 1 + loc + 1; s = i;
//				//DBG_P("START='%c'", t.utext[s]);
//				if(s < t.len) goto L_TEXT;
//				return 1;
//			}
//			break;
//		}
//	}
//	KNH_SYSLOG(ctx, LOG_WARNING, "BrokenFmt", "'%B'", t);
//	knh_cwb_close(cwb);
//	return 0;
//}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
