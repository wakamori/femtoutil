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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_first       1
#define USE_bytes_index       1
#define USE_bytes_last        1
#define USE_bytes_startsWith  1
#define USE_bytes_endsWith    1

#define USE_cwb_open      1
#define USE_cwb_tobytes   1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [properties] */

static void knh_cwb_nzenvkey(CTX ctx, knh_cwb_t *cwb, knh_bytes_t t)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		knh_Bytes_putc(ctx, (cwb->ba), toupper(t.utext[i]));
	}
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_getPropertyNULL(CTX ctx, knh_bytes_t key)
{
	if(knh_bytes_startsWith(key, STEXT("env."))) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_cwb_nzenvkey(ctx, cwb, knh_bytes_last(key, 4));
		char *v = knh_getenv(knh_cwb_tochar(ctx, cwb));
		knh_cwb_close(cwb);
		if(v == NULL) return NULL;
		return new_S(ctx, B(v));
	}
	return (knh_String_t*)knh_DictMap_getNULL(ctx,  DP(ctx->sys)->props, key);
}

/* ------------------------------------------------------------------------ */

void knh_setProperty(CTX ctx, knh_String_t *key, dynamic *value)
{
	DictMap_set_(ctx, DP(ctx->sys)->props, key, value);
}

/* ------------------------------------------------------------------------ */

KNHAPI2(void) knh_setPropertyText(CTX ctx, char *key, char *value)
{
	knh_String_t *k = new_T(key);
	knh_String_t *v = new_T(value);
	DictMap_set_(ctx, DP(ctx->sys)->props, k, UPCAST(v));
}

/* ------------------------------------------------------------------------ */
/* [Constant Value] */

Object *knh_getClassConstNULL(CTX ctx, knh_class_t cid, knh_bytes_t name)
{
	DBG_ASSERT_cid(cid);
	if(ClassTBL(cid)->constDictCaseMapNULL == NULL) return NULL;
	knh_DictMap_t *cmap = ClassTBL(cid)->constDictCaseMapNULL;
	Object *value = NULL;
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	int res = knh_DictMap_index(cmap, name);
	if(res != -1) {
		value = knh_DictMap_valueAt(cmap, res);
	}
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return value;
}

/* ------------------------------------------------------------------------ */

int knh_addClassConst(CTX ctx, knh_class_t cid, knh_String_t* name, Object *value)
{
	int ret;
	knh_DictMap_t *cmap = ClassTBL(cid)->constDictCaseMapNULL;
	DBG_ASSERT_cid(cid);
	if(cmap == NULL) {
		knh_ClassTBL_t *t = varClassTBL(cid);
		cmap = new_DictMap0(ctx, 0, 1/*isCaseMap*/, "ClassTBL.constDictMap");
		KNH_INITv(t->constDictCaseMapNULL, cmap);
	}
	DBG_ASSERT(IS_Map(cmap));
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	int idx = knh_DictMap_index(cmap, S_tobytes(name));
	if(idx != -1) {
		ret =  0;
		goto L_UNLOCK;
	}
	knh_DictMap_set(ctx, cmap, name, value);
	ret = 1;

	L_UNLOCK:
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return ret;
}

/* ------------------------------------------------------------------------ */
/* [tfieldn, tmethodn] */

knh_fieldn_t knh_addname(CTX ctx, knh_String_t *s, knh_Fdictset f)
{
	knh_SystemEX_t *b = DP(ctx->sys);
	size_t n = knh_DictSet_size(b->nameDictCaseSet);
	if(n == b->namecapacity) {
		b->namecapacity = k_grow(n);
		b->nameinfo = (knh_nameinfo_t*)KNH_REALLOC(ctx, "nameinfo", b->nameinfo, n, b->namecapacity, sizeof(knh_nameinfo_t));
	}
	DBG_ASSERT(n < b->namecapacity);
	KNH_INITv(b->nameinfo[n].name, s);
	if(unlikely(!(n+1 < K_FLAG_MN_SETTER))) {  /* Integer overflowed */
		KNH_PANIC(ctx, "too many names, last nameid(fn)=%d < %d", (int)(n+1), (int)K_FLAG_MN_SETTER);
	}
	f(ctx, b->nameDictCaseSet, s, n + 1);
	return (knh_fieldn_t)(n);
}

static knh_fieldn_t knh_getname(CTX ctx, knh_bytes_t n, knh_fieldn_t def)
{
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t idx = knh_DictSet_index(DP(ctx->sys)->nameDictCaseSet, n);
	if(idx == -1) {
		if(def == FN_NEWID) {
			idx = knh_addname(ctx, new_S(ctx, n), knh_DictSet_set);
		}
		else {
			idx = def - MN_OPSIZE;
		}
	}
	else {
		idx = knh_DictSet_valueAt(DP(ctx->sys)->nameDictCaseSet, idx) - 1;
	}
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return (knh_fieldn_t)idx + MN_OPSIZE;
}

knh_nameinfo_t *knh_getnameinfo(CTX ctx, knh_fieldn_t fn)
{
	size_t n = (FN_UNMASK(fn) - MN_OPSIZE);
	DBG_(
		size_t size = knh_DictSet_size(DP(ctx->sys)->nameDictCaseSet);
		DBG_ASSERT(n < size);
	);
	return DP(ctx->sys)->nameinfo + n;
}

/* ------------------------------------------------------------------------ */
/* [fn] */

KNHAPI2(knh_String_t*) knh_getFieldName(CTX ctx, knh_fieldn_t fn)
{
	return knh_getnameinfo(ctx, fn)->name;
}

/* ------------------------------------------------------------------------ */

knh_fieldn_t knh_getfnq(CTX ctx, knh_bytes_t tname, knh_fieldn_t def)
{
	knh_fieldn_t mask = 0;
	knh_index_t idx = knh_bytes_index(tname, ':');
	if(idx > 0) {
		tname = knh_bytes_first(tname, idx);
	}
	else if(knh_bytes_startsWith(tname, STEXT("super."))) {
		mask = (def == FN_NONAME) ? 0 : K_FLAG_FN_SUPER;
		tname = knh_bytes_last(tname, 6);
	}
	else if(!knh_bytes_endsWith(tname, STEXT("__"))) {
		if(tname.utext[0] == '_' && def != FN_NONAME) {
			mask = K_FLAG_FN_U1;
			tname = knh_bytes_last(tname, 1);
		}
		if(tname.utext[0] == '_' && def != FN_NONAME) {
			mask = K_FLAG_FN_U2;
			tname = knh_bytes_last(tname, 1);
		}
		while(tname.utext[0] == '_') {
			tname = knh_bytes_last(tname, 1);
		}
	}
	return knh_getname(ctx, tname, def) | mask;
}

/* ------------------------------------------------------------------------ */
/* [methodn] */

static knh_bytes_t knh_bytes_skipFMTOPT(knh_bytes_t t)
{
	size_t i;
	for(i = 1; i < t.len; i++) {
		if(isalnum(t.utext[i])) break;
	}
	t.utext = t.utext + i;
	t.len = t.len - i;
	return t;
}

knh_methodn_t knh_getmn(CTX ctx, knh_bytes_t tname, knh_methodn_t def)
{
	knh_fieldn_t mask = 0;
	if(tname.utext[0] == '%') {
		tname = knh_bytes_skipFMTOPT(tname);
		if(def != MN_NONAME) mask |= K_FLAG_MN_FMT;
	}
	else if(tname.utext[0] == 'i' && tname.utext[1] == 's') { /* is => get */
		tname = knh_bytes_last(tname, 2);
		if(def != MN_NONAME) mask |= K_FLAG_MN_ISBOOL;
	}
	else if(tname.utext[0] == 'g' && tname.utext[1] == 'e' && tname.utext[2] == 't') {
		tname = knh_bytes_last(tname, 3);
		if(def != MN_NONAME) mask |= K_FLAG_MN_GETTER;
	}
	else if(tname.utext[0] == 's' && tname.utext[1] == 'e' && tname.utext[2] == 't') {
		tname = knh_bytes_last(tname, 3);
		if(def != MN_NONAME) mask |= K_FLAG_MN_SETTER;
	}
	return knh_getname(ctx, tname, def) | mask;
}

/* ------------------------------------------------------------------------ */

const char* knh_getmnname(CTX ctx, knh_methodn_t mn)
{
	mn = MN_toFN(mn);
	if(mn < MN_OPSIZE) {
		return knh_getopname(mn);
	}
	return S_tochar(knh_getFieldName(ctx, mn));
}

/* ------------------------------------------------------------------------ */
/* [uri] */

knh_uri_t knh_getURI(CTX ctx, knh_bytes_t t)
{
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t idx = knh_DictSet_index(DP(ctx->sys)->urnDictSet, t);
	if(idx == -1) {
		knh_String_t *s = new_S(ctx, t);
		idx = knh_Array_size(DP(ctx->sys)->urns);
		knh_DictSet_set(ctx, DP(ctx->sys)->urnDictSet, s, idx);
		knh_Array_add(ctx, DP(ctx->sys)->urns, s);
		KNH_INFO(ctx, "NEW_URI URI=%d, URN='%B'", idx, S_tobytes(s));
	}
	else {
		idx = knh_DictSet_valueAt(DP(ctx->sys)->urnDictSet, idx);
	}
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return (knh_uri_t)idx;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_getURN(CTX ctx, knh_uri_t uri)
{
	size_t n = URI_UNMASK(uri);
	knh_Array_t *a = DP(ctx->sys)->urns;
	if(n < knh_Array_size(a)) {
		return (knh_String_t*)(a)->list[n];
	}
	else {
		DBG_ASSERT(uri == URI_unknown);
		return TS_EMPTY;
	}
}

/* ------------------------------------------------------------------------ */
/* [Driver] */

void knh_NameSpace_addDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_DSPI_t* p)
{
	const char *name = (scheme == NULL) ? p->name : scheme;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write(ctx, cwb->w, B(name));
	knh_putc(ctx, cwb->w, ':');
	knh_write_ifmt(ctx, cwb->w, K_INT_FMT, p->type);
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_DictSet_set(ctx, DP(ctx->sys)->dspiDictSet, knh_cwb_newString(ctx, cwb), (knh_uintptr_t)p);
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */

const knh_DSPI_t *knh_NameSpace_getDSPINULL(CTX ctx, knh_NameSpace_t *ns, int type, knh_bytes_t path)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_index_t idx = knh_bytes_index(path, ':');
	if(idx == -1) {
		knh_write(ctx, cwb->w, path);
	}
	else {
		knh_write(ctx, cwb->w, knh_bytes_first(path, idx));
	}
	knh_putc(ctx, cwb->w, ':');
	knh_write_ifmt(ctx, cwb->w, K_INT_FMT, type);
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	const knh_DSPI_t *p = (const knh_DSPI_t*)knh_DictSet_get(ctx, DP(ctx->sys)->dspiDictSet, knh_cwb_tobytes(cwb));
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	knh_cwb_close(cwb);
	return p;
}

/* ------------------------------------------------------------------------ */

const knh_PathDSPI_t *knh_NameSpace_getPathDSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	return (const knh_PathDSPI_t *)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_PATH, path);
}

/* ------------------------------------------------------------------------ */

const knh_ConvDSPI_t *knh_NameSpace_getConvTODSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	return (const knh_ConvDSPI_t *)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_CONVTO, path);
}

/* ------------------------------------------------------------------------ */

const knh_ConvDSPI_t *knh_NameSpace_getConvFROMDSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	return (const knh_ConvDSPI_t *)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_CONVFROM, path);
}

/* ------------------------------------------------------------------------ */

void knh_exit(CTX ctx, int status)
{
	exit(status);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#else/*K_INCLUDE_BUILTINAPI*/

#if defined(K_USING_POSIX)
#include <dirent.h>
#endif

/* ------------------------------------------------------------------------ */
//## @Static method InputStream System.getIn();

static METHOD System_getIn(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->in);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream System.getOut();

static METHOD System_getOut(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->out);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream System.getErr();

static METHOD System_getErr(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->err);
}

/* ------------------------------------------------------------------------ */
//## @Static method dynamic System.getProperty(String key);

static METHOD System_getProperty(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Object_t *v = (knh_Object_t*)knh_getPropertyNULL(ctx, S_tobytes(sfp[1].s));
	if(v == NULL) {
		v = KNH_NULVAL(CLASS_String);
		knh_setProperty(ctx, sfp[1].s, v);
	}
	RETURNa_(v);
}

/* ------------------------------------------------------------------------ */
//## @Static method dynamic System.setProperty(String key, dynamic value);

static METHOD System_setProperty(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_setProperty(ctx, sfp[1].s, sfp[2].o);
	RETURNa_(sfp[2].o);
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_bytes_matchWildCard(knh_bytes_t t, knh_bytes_t p)
{
	if(p.utext[0] == '*') {
		p.utext = p.utext + 1;
		p.len = p.len - 1;
		return knh_bytes_endsWith(t, p);
	}
	else if(p.utext[p.len-1] == '*') {
		p.len -= 1;
		return knh_bytes_startsWith(t, p);
	}
	else {
		knh_index_t idx = knh_bytes_index(p, '*');
		if(idx == -1) {
			return knh_bytes_startsWith(t, p);
		}
		else {
			return knh_bytes_startsWith(t, knh_bytes_first(p, idx)) &&
				knh_bytes_endsWith(t, knh_bytes_last(p, idx+1));
		}
	}
}

/* ------------------------------------------------------------------------ */
//## @Hidden method String[] System.listProperties(String key);

static METHOD System_listProperties(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = new_Array(ctx, CLASS_String, 0);
	knh_bytes_t prefix = IS_NULL(sfp[1].s) ? STEXT("") : S_tobytes(sfp[1].s);
	knh_DictMap_t *map = DP(ctx->sys)->props;
	size_t i;
	for(i = 0; i < knh_DictMap_size(map); i++) {
		knh_String_t *key = knh_DictMap_keyAt(map, i);
		if(knh_bytes_matchWildCard(S_tobytes(key), prefix)) {
			knh_Array_add(ctx, a, key);
		}
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## @Static method void System.gc();

static METHOD System_gc(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_System_gc(ctx);
}

///* ------------------------------------------------------------------------ */
////## @Static @Hidden method void System.push(Object value, ...);
//
//static METHOD System_push(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	long i, ac = knh_stack_argc(ctx, (sfp+1));
//	for(i = 0; i < ac; i++) {
//		KNH_SETv(ctx, sfp[rix+i].o, sfp[i+1].o);
//		sfp[rix+i].ndata = sfp[i+1].ndata;
//	}
//}

///* ------------------------------------------------------------------------ */
////## @Hidden method void System.test(Boolean result, String msg);
//
//static METHOD System_test(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	char *result = (sfp[1].bvalue) ? "PASS" : "FAILED";
//	knh_intptr_t line = (knh_intptr_t)sfp[0].ivalue;
//	knh_printf(ctx, KNH_STDERR, "[%s:%d]", result, line);
//	if(IS_bString(sfp[2].s)) {
//		knh_putc(ctx, KNH_STDERR, ' ');
//		knh_print(ctx, KNH_STDERR, S_tobytes(sfp[2].s));
//	}
//	knh_write_EOL(ctx, KNH_STDERR);
//}

/* ------------------------------------------------------------------------ */
//## method Int System.getTime();

static METHOD System_getTime(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(knh_getTimeMilliSecond());
}

///* ------------------------------------------------------------------------ */
////## @Static @Unsafe method void System.exit(Int status);
//
//static METHOD System_exit(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//#if defined(K_USING_STDC_)
//	int status = IS_NULL(sfp[1].o) ? 0 : Int_to(size_t, sfp[1]);
//	KNH_SECURE(ctx, sfp);
//	KNH_SYSLOG(ctx, LOG_NOTICE, "EXIT", "exiting by a user");
//	exit(status);
//#endif
//	RETURNvoid_();
//}

///* ------------------------------------------------------------------------ */
////## method String[] System.listDir(String path);
//
//static METHOD System_listDir(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_Array_t *a = new_Array(ctx, CLASS_String, 0);
//	knh_bytes_t t = (IS_NULL(sfp[1].s)) ? STEXT(".") : S_tobytes(sfp[1].s);
//	knh_cwb_write(ctx, cwb, t);
//	knh_cwb_ospath(ctx, cwb);
//	KNH_SETv(ctx, sfp[2].o, a);
//#if defined(K_USING_POSIX_)
//	{
//		char *dirname = knh_cwb_tochar(ctx, cwb);
//		DIR *dirptr = opendir(dirname);
//		KNH_PERROR_IF(ctx, sfp, (dirptr == NULL), "opendir");
//		if (dirptr != NULL) {
//			struct dirent *direntp;
//			while ((direntp = readdir(dirptr)) != NULL) {
//				char *p = direntp->d_name;
//				if(p[0] == '.' && (p[1] == 0 || p[1] == '.')) continue;
//				knh_Array_add(ctx, a, new_S(ctx, B(p)));
//			}
//			closedir(dirptr);
//		}
//	}
//#else
//	KNH_TODO("opendir for this platform");
//#endif
//	knh_cwb_close(cwb);
//	RETURN_(a);
//}

/* ------------------------------------------------------------------------ */
//## @Static method InputStream Context.setIn(InputStream? in);

static METHOD CTX_setIn(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KNH_SETv(ctx, ((knh_context_t*)ctx)->in, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream Context.setOut(OutputStream? out);

static METHOD CTX_setOut(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KNH_SETv(ctx, ((knh_context_t*)ctx)->out, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream Context.setErr(OutputStream? out);

static METHOD CTX_setErr(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KNH_SETv(ctx, ((knh_context_t*)ctx)->err, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## method Boolean Exception.opOF(String event);

static METHOD Exception_opOF(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int isa = 0;
	knh_String_t *event = sfp[1].s;
	if(knh_bytes_strcasecmp(S_tobytes(event), S_tobytes(DP(sfp[0].e)->event)) != 0) {
		knh_ebi_t eid = knh_geteid(ctx, S_tobytes(event), EBI_unknown);
		if(eid != EBI_unknown) {
			isa = expt_isa(ctx, DP(sfp[0].e)->eid, eid);
		}
	}
	else {
		isa = 1;
	}
	RETURNb_(isa);
}

///* ------------------------------------------------------------------------ */
////## @Hidden @Const method dynamic NameSpace.setConst(String name, Object value);
//
//static METHOD NameSpace_setConst(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_NameSpace_t *ns = sfp[0].ns;
//	if(DP(ns)->constDictCaseMapNULL == NULL) {
//		KNH_INITv(DP(ns)->constDictCaseMapNULL, new_DictMap0(ctx, 0, 1/*isCaseMap*/, "NameSpace.lconstDictMap"));
//	}
//	DictMap_set_(ctx, DP(ns)->constDictCaseMapNULL, sfp[1].s, sfp[2].o);
//	RETURNa_(sfp[2].o);
//}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method dynamic Class.setConst(String name, Object value);

static METHOD Class_setConst(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_addClassConst(ctx, (sfp[0].c)->cid, sfp[1].s, sfp[2].o);
	RETURNa_(sfp[2].o);
}

/* ------------------------------------------------------------------------ */
//## @Static @Audit method String System.exec(String cmd, Class reqt);

static METHOD System_exec(CTX ctx, knh_sfp_t *sfp _RIX)
{
#ifdef K_DEOS_TRACE
	char cmd[1024];
	knh_snprintf(cmd, sizeof(cmd), "%s=%s %s", K_DEOS_TRACE, ctx->trace, S_tochar(sfp[1].s));
#else
	const char *cmd = S_tochar(sfp[1].s);
#endif
	KNH_SECINFO(ctx, "fork command='%s'", cmd);
#ifdef K_USING_POSIX_
	FILE *fp = popen((const char*)cmd, "r+");
	if(fp != NULL) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		char buf[K_PAGESIZE];
		while(1) {
			size_t size = fread(buf, 1, sizeof(buf), fp);
			if(size > 0) {
				knh_bytes_t t = {{buf}, size};
				knh_Bytes_write(ctx, cwb->ba, t);
			}
			else {
				break;
			}
		};
		pclose(fp);
		RETURN_(knh_cwb_newString(ctx, cwb));
	}
	else {
		KNH_WARN(ctx, "command failed: %s", cmd);
	}
#endif
	RETURN_(KNH_NULVAL(CLASS_String));
}

/* ------------------------------------------------------------------------ */

#endif/*K_INCLUDE_BUILTINAPI*/

