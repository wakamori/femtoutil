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

#define USE_RCinc 1
#define USE_bytes 1
#define USE_cwb   1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#define _RETURN(s)    status = s; goto L_RETURN;

/* ------------------------------------------------------------------------ */
/* [namespace] */

knh_NameSpace_t* new_NameSpace(CTX ctx, knh_NameSpace_t *parent)
{
	knh_NameSpace_t* ns = new_(NameSpace);
	DBG_ASSERT(IS_NameSpace(parent));
	KNH_INITv(ns->parentNULL, parent);
	KNH_SETv(ctx, DP(ns)->nsname, DP(parent)->nsname);
	KNH_SETv(ctx, ns->rpath, parent->rpath);
	DBG_P("ns=%p, rpath='%s'", ns, S_tochar(ns->rpath));
	return ns;
}

knh_class_t knh_NameSpace_getcid(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t sname)
{
	DBG_ASSERT(IS_NameSpace(ns));
	if(knh_bytes_equals(sname, STEXT("Script"))) {
		return O_cid(K_GMASCR);
	}
	L_TAIL:
	if(DP(ns)->name2cidDictSetNULL != NULL) {
		knh_uintptr_t cid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, sname);
		if(cid > 0) return (knh_class_t)(cid-1);
	}
	if(ns->parentNULL != NULL) {
		ns = ns->parentNULL;
		goto L_TAIL;
	}
	return knh_getcid(ctx, sname);
}

KNHAPI2(knh_Object_t*) new_ObjectNS(CTX ctx, knh_NameSpace_t *ns, const char *sname)
{
	knh_class_t cid = knh_NameSpace_getcid(ctx, ns, B(sname));
	if(cid == CLASS_unknown) {
		KNH_DIE("unknown class name: %s\n", sname);
	}
	return new_Object_init2(ctx, ClassTBL(cid));
}

/* ------------------------------------------------------------------------ */
/* [stmt] */

knh_flag_t knh_Stmt_flag_(CTX ctx, knh_Stmt_t *stmt, knh_bytes_t name, knh_flag_t flag)
{
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		Object *v = knh_DictMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, name);
		return (v != NULL) ? flag : 0;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [function] */

//static
//void NameSpace_setFuncClass(CTX ctx, knh_NameSpace_t *ns, knh_methodn_t mn, knh_class_t c)
//{
//	if(!MN_isGETTER(mn) && !MN_isSETTER(mn)) {
//		if(!IS_DictSet(DP(ns)->func2cidDictSet)) {
//			KNH_SETv(ctx, DP(ns)->func2cidDictSet, new_DictSet(ctx, 0));
//		}
//		knh_DictSet_set(ctx, DP(ns)->func2cidDictSet, knh_getFieldName(ctx, MN_toFN(mn)), (knh_uintptr_t)(c+1));
//	}
//}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_getFuncClass(CTX ctx, knh_NameSpace_t *ns, knh_methodn_t mn)
{
	if(!MN_isGETTER(mn) && !MN_isSETTER(mn)) {
		knh_bytes_t name = S_tobytes(knh_getFieldName(ctx, MN_toFN(mn)));
		L_TAIL:
		if(DP(ns)->func2cidDictSetNULL != NULL) {
			knh_uintptr_t cid = knh_DictSet_get(ctx, DP(ns)->func2cidDictSetNULL, name);
			if(cid != 0) {
				return (knh_class_t)(cid-1);
			}
		}
		if(ns->parentNULL != NULL) {
			ns = ns->parentNULL;
			goto L_TAIL;
		}
	}
	return CLASS_unknown; /* if not found */
}

knh_type_t knh_NameSpace_gettype(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	if(name.utext[0] == 'v') {
		if(name.len == 4 && name.utext[1] == 'o' &&
				name.utext[2] == 'i' && name.utext[3] == 'd') return TYPE_void;
		if(name.len == 3 && name.utext[1] == 'a' && name.utext[2] == 'r') {
			return TYPE_var;
		}
	}
	return knh_NameSpace_getcid(ctx, ns, name);
}

knh_type_t knh_NameSpace_tagcid(CTX ctx, knh_NameSpace_t *o, knh_class_t cid, knh_bytes_t tag)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_class_t bcid = ClassTBL(cid)->bcid;
	knh_printf(ctx, cwb->w, "%C:%B", bcid, tag);
	cid = knh_NameSpace_getcid(ctx, o, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	return cid;
}

/* ------------------------------------------------------------------------ */
/* [include] */

static void NameSpace_beginINCLUDE(CTX ctx, knh_NameSpace_t *newns, knh_NameSpace_t *oldns)
{
	knh_NameSpaceEX_t *tb = newns->b;
	void *tdlhdr = newns->dlhdr;
	newns->b = oldns->b;
	newns->dlhdr = oldns->dlhdr;
	oldns->b = tb;
	oldns->dlhdr = tdlhdr;
}

static void NameSpace_endINCLUDE(CTX ctx, knh_NameSpace_t *newns, knh_NameSpace_t *oldns)
{
	knh_NameSpaceEX_t *tb = newns->b;
	void *tdlhdr = newns->dlhdr;
	newns->b = oldns->b;
	newns->dlhdr = oldns->dlhdr;
	oldns->b = tb;
	oldns->dlhdr = tdlhdr;
}

static knh_status_t INCLUDE_eval(CTX ctx, knh_Stmt_t *stmt, knh_Array_t *resultsNULL)
{
	knh_status_t status = K_CONTINUE;
	knh_bytes_t include_name = S_tobytes(tkNN(stmt, 0)->text);
	knh_NameSpace_t *ns = K_GMANS;
	if(knh_bytes_startsWith(include_name, STEXT("func:"))) {
		if(ns->dlhdr != NULL) {
			const char *funcname = knh_bytes_next(include_name, ':').text;
			knh_Fpkgload pkgload = (knh_Fpkgload)knh_dlsym(ctx, LOG_DEBUG, ns->dlhdr, funcname);
			if(pkgload != NULL) {
				pkgload(ctx, knh_getPackageLoaderAPI(), ns);
				_RETURN(K_CONTINUE);
			}
		}
		knh_Stmt_toERR(ctx, stmt, ERROR_NotFound(ctx, "loader function", include_name.text));
		_RETURN(K_BREAK);
	}
	else if(knh_bytes_strcasecmp(include_name, STEXT("nativelink")) == 0) {
		if(ns->dlhdr == NULL) {
			knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, S_tobytes(ns->rpath), &phbuf);
			knh_path_reduce(ctx, ph, '.');
			knh_path_append(ctx, ph, 0, K_OSDLLEXT);
			ns->dlhdr = knh_path_dlopen(ctx, ph);
			if(ns->dlhdr != NULL) {
				knh_Fpkginit pkginit = (knh_Fpkginit)knh_dlsym(ctx, LOG_DEBUG, ns->dlhdr, "init");
				if(pkginit != NULL) {
					const knh_PackageDef_t *pkgdef = pkginit(ctx);
					if((long)pkgdef->crc32 != (long)K_API2_CRC32) {
						knh_dlclose(ctx, ns->dlhdr);
						ns->dlhdr = NULL;
						knh_Stmt_toERR(ctx, stmt, ERROR_IncompatiblePackage(ctx, include_name, pkgdef));
						status = K_BREAK;
					}
				}
				else {
					const knh_PackageDef_t pkgdef = {};
					knh_dlclose(ctx, ns->dlhdr);
					ns->dlhdr = NULL;
					knh_Stmt_toERR(ctx, stmt, ERROR_IncompatiblePackage(ctx, include_name, &pkgdef));
					status = K_BREAK;
				}
				knh_path_close(ctx, ph);
				goto L_RETURN;
			}
			knh_Stmt_toERR(ctx, stmt, ERROR_NotFound(ctx, "package", P_text(ph)));
			knh_path_close(ctx, ph);
			status = K_BREAK;
		}
	}
	else {
		knh_NameSpace_t *ns = new_NameSpace(ctx, K_GMANS);
		KNH_SETv(ctx, ctx->gma->scr->ns, ns);
		NameSpace_beginINCLUDE(ctx, ns, ns->parentNULL);
		status = knh_load(ctx, "script", include_name, resultsNULL);
		NameSpace_endINCLUDE(ctx, ns, ns->parentNULL);
		KNH_SETv(ctx, ctx->gma->scr->ns, ns->parentNULL);
	}
	L_RETURN:;
	knh_Stmt_done(ctx, stmt);
	return status;
}

/* ------------------------------------------------------------------------ */
/* [using] */

knh_status_t knh_loadScriptPackage(CTX ctx, knh_bytes_t path)
{
	knh_status_t status = K_CONTINUE;
	knh_bytes_t name = knh_bytes_next(path, ':');
	knh_DictMap_t *dmap = DP(ctx->sys)->PackageDictMap;
	knh_Script_t *scr = (knh_Script_t*)knh_DictMap_getNULL(ctx, dmap, name);
	if(scr == NULL) {
		const knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, ctx->share->rootns, path);
		status = K_BREAK;
		if(dspi->exists(ctx, ctx->share->rootns, path, dspi->thunk)) {
			knh_String_t *nsname = new_S(ctx, name);
			knh_Script_t *newscr = new_(Script);
			KNH_SETv(ctx, DP(newscr->ns)->nsname, nsname);
			knh_DictMap_set(ctx, dmap, nsname, newscr);
			scr = ctx->gma->scr;
			KNH_SETv(ctx, ctx->gma->scr, newscr);
			status = knh_load(ctx, "pkg", path, NULL);
			KNH_SETv(ctx, ctx->gma->scr, scr);
		}
	}
	return status;
}

/* ------------------------------------------------------------------------ */

static void NameSpace_setcid(CTX ctx, knh_NameSpace_t *ns, knh_String_t *name, knh_class_t cid)
{
	if(DP(ns)->name2cidDictSetNULL == NULL) {
		KNH_INITv(DP(ns)->name2cidDictSetNULL, new_DictSet0(ctx, 0, 1/*isCaseMap*/, "NameSpace.name2cid"));
	}
	else {
		knh_uintptr_t oldcid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, S_tobytes(name));
		if(oldcid != 0 && cid != oldcid - 1) {
			WARN_AlreadyDefinedClass(ctx, cid, (knh_class_t)(oldcid - 1));
			return;
		}
	}
	knh_DictSet_set(ctx, DP(ns)->name2cidDictSetNULL, name, (knh_uintptr_t)(cid+1));
}

static int StmtUSINGCLASS_eval(CTX ctx, knh_Stmt_t *stmt, size_t n)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, STEXT("pkg:"));
	knh_Token_t *tkPKG = tkNN(stmt, n), *tkN;
	knh_Bytes_write(ctx, cwb->ba, S_tobytes((tkPKG)->text));
	while(1) {
		tkN = tkNN(stmt, ++n);
		if(TT_(tkN) == TT_ASIS) break;
		if(TT_(tkN) == TT_DOT) continue;
		if(TT_(tkN) == TT_NAME) {
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_write(ctx, cwb->ba, S_tobytes((tkN)->text));
			continue;
		}
		break;
	}
	KNH_SETv(ctx, (tkPKG)->data, knh_cwb_newString(ctx, cwb));
	if(knh_loadScriptPackage(ctx, S_tobytes((tkPKG)->text)) == K_CONTINUE) {
		knh_NameSpace_t *ns = K_GMANS;
//		int isOVERRIDE = knh_Stmt_flag(ctx, stmt, "Override", 1);
		if(TT_(tkN) == TT_MUL) {
			knh_bytes_t pkgname = knh_bytes_last(S_tobytes((tkPKG)->text), 4/* strlen("pkg:") */);
			size_t cid;
			for(cid = 0; cid < ctx->share->sizeClassTBL; cid++) {
				if(ClassTBL(cid)->lname == NULL) continue;
				if(class_isPrivate(cid) && class_isGenerics(ctx, cid)) continue;
				knh_bytes_t cname = S_tobytes(ClassTBL(cid)->lname);
				if(knh_bytes_startsWith(cname, pkgname)
						&& cname.utext[pkgname.len] == '.' && isupper(cname.utext[pkgname.len+1])) {
					NameSpace_setcid(ctx, ns, ClassTBL(cid)->sname, (knh_class_t)cid);
				}
			}
		}
		else if(TT_(tkN) == TT_UNAME) {
			knh_class_t newcid;
			knh_String_t* cname = (tkN)->text;
			knh_Bytes_write(ctx, cwb->ba, knh_bytes_last(S_tobytes((tkPKG)->text), sizeof("pkg")));
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(cname));
			newcid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
			if(newcid == CLASS_unknown) {
				KNH_SETv(ctx, (tkPKG)->data, knh_cwb_newString(ctx, cwb));
				goto L_ERROR;
			}
			else {
//#ifdef TT_AS
//				if(n+2 < DP(stmt)->size &&
//					TT_(tkNN(stmt, n+1]) == TT_AS && TT_(DP(stmt)->tokens[n+2)) == TT_UNAME) {
//					cname = (tkNN(stmt, n+2))->text;
//				}
//#endif
				NameSpace_setcid(ctx, ns, cname, newcid);
			}
			knh_cwb_clear(cwb, 0);
		}
		knh_Stmt_done(ctx, stmt);
		return 1;
	}
	L_ERROR:;
	while(n + 1 < DP(stmt)->size) {
		if(TT_(tkNN(stmt, n)) == TT_OR) return 0;
		n++;
	}
	knh_Stmt_toERR(ctx, stmt, ERROR_NotFound(ctx, "package", S_tochar((tkPKG)->text)));
	return 0;
}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUALIAS_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	DBG_DUMP(ctx, stmt, KNH_NULL, "decl");
//	TODO();
//	return 1;
//}

///* ------------------------------------------------------------------------ */
//
//static
//char *knh_format_classurn(CTX ctx, char *buf, size_t bufsiz, knh_class_t bcid, knh_bytes_t urn)
//{
//	knh_snprintf(buf, bufsiz, KNH_SEMANTICS_FMT, CLASS__(bcid), urn.buf);
//	return buf;
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//void NameSpace_setTagName(CTX ctx, knh_NameSpace_t *o, knh_String_t *name, knh_class_t cid)
//{
//	KNH_ASSERT(IS_NameSpace(o));
//	DBG_ASSERT_cid(cid);
//	knh_DictSet_set(ctx, DP(o)->name2cidDictSet, name, (knh_uintptr_t)(cid+1));
////	knh_bytes_t n = S_tobytes(name);
////	knh_index_t loc = knh_bytes_index(n, ':');
////	if(loc != -1) {
////		n = knh_bytes_last(n, loc+1);
////		if(isupper(n.utext[0])) {
////			knh_DictSet_set(ctx, DP(o)->name2cidDictSet, new_String(ctx, n, name), (knh_uintptr_t)(cid+1));
////		}
////	}
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtXCLASS_decl(CTX ctx, knh_Stmt_t *stmt, knh_class_t bcid)
//{
//
//	knh_NameSpace_t *ns = K_GMANS;
//	knh_Token_t *tkclassn = tkNN(stmt, 0);
//	knh_Token_t *tkurn = tkNN(stmt, 1);
//
//	char bufcu[CLASS__AME_BUFSIZ];
//	knh_format_classurn(ctx, bufcu, sizeof(bufcu), bcid, knh_getURNAlias(ctx, knh_Token_tobytes(ctx, tkurn)));
//
//	knh_class_t cid = knh_findcid(ctx, B(bufcu));
//	if(cid == bcid || cid == CLASS_unknown) {
//		knh_Token_perror(ctx, tkurn, KC_EWARN, _("resource not found: %L"), tkurn);
//		cid = bcid;
//	}
//
//	knh_class_t oldcid = knh_NameSpace_findcid(ctx, ns, knh_Token_tobytes(ctx, tkclassn));
//	if(oldcid != CLASS_unknown && cid != oldcid) {
//		KNH_SYSLOG(ctx, LOG_WARNING, _("overriding %s"), knh_Token_tobytes(ctx, tkclassn));
//	}
//
//	KNH_ASSERT(IS_String((tkclassn)->data));
//	NameSpace_setTagName(ctx, ns, (tkclassn)->text, cid);
//	TODO();
//	return 1;
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUVOCAB_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_String);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUENUM_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Int);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUUNIT_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Float);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUFUNC_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	knh_NameSpace_t *ns = K_GMANS;
//	knh_Token_t *tk = tkNN(stmt, 0);
//	knh_bytes_t name = knh_Token_tobytes(ctx, tk);
//	if(SP(tk)->tt == TT_LONGNAME) {
//		knh_index_t loc = knh_bytes_rindex(name, '.');
//		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, knh_bytes_first(name, loc));
//		if(cid == CLASS_unknown) {
//			knh_Token_perror(ctx, tk, KC_ERR, _("unknown class: %L"), tk);
//			return 0;
//		}
//		knh_methodn_t mn = knh_getmn(ctx, knh_bytes_last(name, loc+1), MN_NONAME);
//		if(mn == MN_NONAME) {
//			knh_Token_perror(ctx, tk, KC_ERR, _("unknown method: %C.%L"), cid, tk);
//			return 0;
//		}
//		NameSpace_setFuncClass(ctx, ns, mn, cid);
//	}
//	else {
//		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, name);
//		if(cid == CLASS_unknown) {
//			knh_Token_perror(ctx, tk, KC_ERR, _("unknown class: %L"), tk);
//			return 0;
//		}
//		else {
//			DBG_ASSERT_cid(cid);
//			knh_Array_t *a = ClassTBL(cid)->methods;
//			size_t i;
//			for(i = 0; i < knh_Array_size(a); i++) {
//				knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(a, i);
//				KNH_ASSERT(IS_Method(mtd));
//				if(!Method_isStatic(mtd)) continue;
//				NameSpace_setFuncClass(ctx, ns, (mtd)->mn, cid);
//				if(CTX_isVerbose(ctx)) {
//					KNH_SYSLOG(ctx, LOG_NOTICE, _("using %C.%M"), cid, (mtd)->mn);
//				}
//			}
//		}
//	}
//	return 1;
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUMAPMAP_decl(CTX ctx, knh_Stmt_t *stmt)
//{
//	DBG_DUMP(ctx, stmt, KNH_NULL, "decl");
//	return 1;
//}


static knh_status_t USING_eval(CTX ctx, knh_Stmt_t *stmt)
{
	knh_status_t status = K_CONTINUE;
	size_t n = 0;
	L_TRYAGAIN:; {
		knh_Token_t *tkF = tkNN(stmt, n);
		if(TT_(tkF) == TT_NAME) {
			if(StmtUSINGCLASS_eval(ctx, stmt, n)) {
				goto L_RETURN;
			}
		}
	}
	while(n + 1 < DP(stmt)->size) {  // find next or
		knh_Token_t *tkF = tkNN(stmt, n);
		n++;
		if(TT_(tkF) == TT_OR) goto L_TRYAGAIN;
	}
	status = K_BREAK;
	L_RETURN:;
	knh_Stmt_done(ctx, stmt);
	return status;
}

/* ------------------------------------------------------------------------ */

static knh_Method_t *Script_getEvalMethod(CTX ctx, knh_Script_t *scr)
{
	knh_Method_t *mtd = knh_NameSpace_getMethodNULL(ctx, O_cid(scr), MN_LAMBDA);
	if(mtd == NULL) {
		mtd = new_Method(ctx, FLAG_Method_Hidden, O_cid(scr), MN_LAMBDA, NULL);
		KNH_SETv(ctx, DP(mtd)->mp, new_ParamArrayR0(ctx, TYPE_dyn/*TYPE_var*/));
		knh_NameSpace_addMethod(ctx, O_cid(scr), mtd);
	}
	return mtd;
}

void SCRIPT_asm(CTX ctx, knh_Stmt_t *stmtITR);

static knh_status_t SCRIPT_eval(CTX ctx, knh_Stmt_t *stmt, int isCompileOnly, knh_Array_t *resultsNULL)
{
	knh_status_t status = K_CONTINUE;
	BEGIN_LOCAL(ctx, lsfp, 5);
	knh_Script_t *scr = K_GMASCR;
	knh_Method_t *mtd = Script_getEvalMethod(ctx, scr);
	knh_class_t cid =  O_cid(ctx->evaled);
	if(stmt_isExpr(STT_(stmt)) && STT_(stmt) != STT_LET) {
		stmt = new_Stmt2(ctx, STT_RETURN, stmt, NULL);
		Stmt_setImplicit(stmt, 1);
	}
	KNH_SETv(ctx, lsfp[0].o, stmt);
	knh_Method_asm(ctx, mtd, NULL, cid, stmt, knh_Method_typing);
	if(Method_isAbstract(mtd) || STT_(stmt) == STT_ERR) {
		status = K_BREAK; goto L_RETURN;
	}
	if(!isCompileOnly) {
		int rtnidx=3+1, thisidx = rtnidx + K_CALLDELTA;
		DP(mtd)->uri = ULINE_uri(stmt->uline);
		KNH_SETv(ctx, lsfp[1+1].o, DP(mtd)->kcode);
		lsfp[thisidx+K_PCIDX].pc = NULL;
		klr_setmtdNC(ctx, lsfp[thisidx+K_MTDIDX], mtd);
		KNH_SETv(ctx, lsfp[thisidx].o, scr);
		KNH_SETv(ctx, lsfp[thisidx+1].o, ctx->evaled);
		lsfp[thisidx+1].ndata = O_data(ctx->evaled);
		klr_setesp(ctx, lsfp + thisidx+2);
		if(knh_VirtualMachine_run(ctx, lsfp + thisidx, CODE_LAUNCH) == NULL) {
			//DBG_P("returning sfpidx=%d, rtnidx=%d, %s %lld %ld %f", sfpidx_, sfpidx_ + rtnidx, O__(lsfp[rtnidx].o), lsfp[rtnidx].ivalue, lsfp[rtnidx].bvalue, lsfp[rtnidx].fvalue);
			if(STT_(stmt) == STT_RETURN && !Stmt_isImplicit(stmt)) {
				cid = O_cid(lsfp[rtnidx].o);
				KNH_SETv(ctx, ((knh_context_t*)ctx)->evaled, lsfp[rtnidx].o);
				if(resultsNULL != NULL) {
					knh_Array_add(ctx, resultsNULL, lsfp[rtnidx].o);
				}
			}
		}
		else {
			status = K_BREAK;
		}
	}
	L_RETURN:;
	END_LOCAL_(ctx, lsfp);
	return status;
}

static knh_status_t Stmt_eval(CTX ctx, knh_Stmt_t *stmtITR, knh_Array_t *resultsNULL);

static knh_status_t IF_eval(CTX ctx, knh_Stmt_t *stmt, knh_Array_t *resultsNULL)
{
	knh_status_t status = K_BREAK;
	if(Tn_typing(ctx, stmt, 0, TYPE_Boolean, 0)) {
		int isTrue = 1;
		if(!Tn_isCONST(stmt, 0)) {
			knh_Stmt_t *stmt2 = new_Stmt2(ctx, STT_RETURN, stmtNN(stmt, 0), NULL);
			//Stmt_setImplicit(stmt2, 1);
			status = SCRIPT_eval(ctx, stmt2, 0/*isCompileOnly*/, NULL);
			if(status != K_CONTINUE) goto L_RETURN;
			isTrue = IS_TRUE(ctx->evaled) ? 1 : 0;
		}
		else {
			isTrue = (Tn_isTRUE(stmt, 0)) ? 1: 0;
		}
		if(isTrue) {
			status = Stmt_eval(ctx, stmtNN(stmt, 1), resultsNULL);
		}
		else {
			status = Stmt_eval(ctx, stmtNN(stmt, 2), resultsNULL);
		}
	}
	L_RETURN: ;
	knh_Stmt_done(ctx, stmt);
	return status;
}


Object *knh_NameSpace_getConstNULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	L_TAIL:
	if(DP(ns)->constDictCaseMapNULL != NULL) {
		knh_index_t idx = knh_DictMap_index(DP(ns)->constDictCaseMapNULL, name);
		if(idx != -1) return knh_DictMap_valueAt(DP(ns)->constDictCaseMapNULL, idx);
	}
	if(ns->parentNULL != NULL) {
		ns = ns->parentNULL;
		goto L_TAIL;
	}
	return knh_getClassConstNULL(ctx, DP(ctx->gma)->this_cid, name);
}

static knh_status_t CONST_decl(CTX ctx, knh_Stmt_t *stmt)
{
	knh_status_t status = K_CONTINUE;
	knh_Token_t *tkN = tkNN(stmt, 0);
	knh_class_t cid = knh_Token_cid(ctx, tkN, CLASS_unknown);
	knh_NameSpace_t *ns = K_GMANS;
	Object *value = knh_NameSpace_getConstNULL(ctx, ns, TK_tobytes(tkN));
	if(cid != CLASS_unknown || value != NULL) {
		WARN_AlreadyDefined(ctx, tkN);
		_RETURN(K_BREAK);
	}
	if(Tn_typing(ctx, stmt, 1, TYPE_dyn, 0)) {
		if(Tn_isCONST(stmt, 1)) {
			value = Tn_const(stmt, 1);
		}
		else {
			knh_Stmt_t *stmt2 = new_Stmt2(ctx, STT_RETURN, stmtNN(stmt, 1), NULL);
			status = SCRIPT_eval(ctx, stmt2, 0/*isCompileOnly*/, NULL);
			if(status != K_CONTINUE) {
				_RETURN(K_BREAK);
			}
			value = ctx->evaled;
		}
		if(IS_Class(value)) {
			DBG_P("new cname %s %s", S_tochar(tkN->text), CLASS__(((knh_Class_t*)value)->cid));
			NameSpace_setcid(ctx, ns, tkN->text, ((knh_Class_t*)value)->cid);
		}
		else {
			if(DP(ns)->constDictCaseMapNULL == NULL) {
				KNH_INITv(DP(ns)->constDictCaseMapNULL,
					new_DictMap0(ctx, 0, 1/*isCaseMap*/, "NameSpace.lconstDictMap"));
			}
			DictMap_set_(ctx, DP(ns)->constDictCaseMapNULL, tkN->text, value);
		}
	}
	L_RETURN: ;
	knh_Stmt_done(ctx, stmt);
	return status;
}

/* ------------------------------------------------------------------------ */
/* [CLASS] */

static knh_flag_t knh_StmtCLASS_flag(CTX ctx, knh_Stmt_t *stmt)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		flag |= knh_Stmt_flag(ctx, stmt, "Final",     FLAG_Class_Final);
		flag |= knh_Stmt_flag(ctx, stmt, "Private",   FLAG_Class_Private);
		flag |= knh_Stmt_flag(ctx, stmt, "Interface", FLAG_Class_Interface);
		flag |= knh_Stmt_flag(ctx, stmt, "Singleton", FLAG_Class_Singleton);
//		flag |= knh_Stmt_flag(ctx, stmt, "Release",   FLAG_Class_Release);
	}
	return flag;
}

/* ------------------------------------------------------------------------ */

void knh_RefTraverse(CTX ctx, knh_Ftraverse ftr)
{
#ifdef K_USING_RCGC
	int i;
	for(i = ctx->ref_size - 1; i >= 0; i--) {
		knh_Object_t *ref = ctx->refs[i];
		ftr(ctx, ref);
	}
#endif
}

static knh_status_t CLASS_decl(CTX ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid;
	knh_Token_t *tkC = tkNN(stmt, 0); // CNAME
	knh_Token_t *tkE = tkNN(stmt, 2); // extends
	knh_ClassTBL_t *ct = NULL;
	knh_NameSpace_t *ns = K_GMANS;
	{
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(ns)->nsname));
		knh_Bytes_putc(ctx, cwb->ba, '.');
		knh_Bytes_write(ctx, cwb->ba, TK_tobytes(tkC));
		cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
		if(cid == CLASS_unknown) {  // new class //
			cid = new_ClassId(ctx);
			ct = varClassTBL(cid);
			knh_setClassName(ctx, cid, knh_cwb_newString(ctx, cwb), (tkC)->text);
			ct->cflag  = knh_StmtCLASS_flag(ctx, stmt);
			ct->magicflag  = KNH_MAGICFLAG(ct->cflag);
			NameSpace_setcid(ctx, ns, (tkC)->text, cid);
			KNH_INITv(ct->methods, K_EMPTYARRAY);
			KNH_INITv(ct->typemaps, K_EMPTYARRAY);

			// class C extends E ..
			ct->supcid = knh_Token_cid(ctx, tkE, CLASS_Object);
			if(class_isFinal(ct->supcid)) {
				knh_Stmt_toERR(ctx, stmt, ErrorExtendingFinalClass(ctx, ct->supcid));
				return 0;
			}
			ct->supTBL = ClassTBL(ct->supcid);
			ct->keyidx = ct->supTBL->keyidx;
			ct->metaidx = ct->supTBL->keyidx;
			((knh_ClassTBL_t*)ct->supTBL)->subclass += 1;
			KNH_INFO(ctx, "NEW_CLASS cid=%d, name='%s'", cid, CLASS__(cid));
			if(knh_StmtMETA_is(ctx, stmt, "Native")) {
				knh_NameSpace_t *ns = K_GMANS;
				if(ns->dlhdr != NULL) {
					knh_Fclass classload = (knh_Fclass)knh_dlsym(ctx, LOG_DEBUG, ns->dlhdr, S_tochar((tkC)->text));
					const knh_ClassDef_t *cdef = classload(ctx);
					KNH_ASSERT(cdef != NULL);
					ct->bcid = cid;
					ct->baseTBL = ct;
					knh_setClassDef(ct, cdef);
					ct->cflag = ct->cflag | cdef->cflag;
					ct->magicflag = KNH_MAGICFLAG(ct->cflag);
				}
				else {
					KNH_TODO("error message");
				}
			}
			else {
				knh_Object_t *obj = new_hObject_(ctx, ct);
				knh_Object_t *tmp = new_hObject_(ctx, ct);
				Object_setNullObject(obj, 1);
				ct->bcid = CLASS_Object;
				ct->baseTBL = ClassTBL(CLASS_Object);
				knh_setClassDef(ct, ct->baseTBL->ospi);
				obj->ref = NULL; tmp->ref = NULL;
				knh_setClassDefaultValue(ctx, cid, obj, NULL);
				KNH_INITv(ct->protoNULL, tmp);
			}
		}
		else {
			knh_cwb_close(cwb);
			ct = varClassTBL(cid);
			if(!(ct->bcid == CLASS_Object && ct->fields == NULL)) {
				knh_Stmt_toERR(ctx, stmt, ErrorRedefinedClass(ctx, S_tobytes((tkC)->text), cid));
				return 0;
			}
		}
	}
	(tkC)->cid = cid;
	if(IS_Tfield(cid)) {
		DBG_P("superclass=%s, fsize=%d, fcapacity=%d", CLASS__(ct->supcid), ct->supTBL->fsize, ct->supTBL->fcapacity);
		if(ct->supTBL->fcapacity > 0 && ct->fcapacity == 0) {
			ct->fields = (knh_fields_t*)KNH_MALLOC(ctx, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			knh_memcpy(ct->fields, ct->supTBL->fields, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			ct->fsize = ct->supTBL->fsize;
			ct->fcapacity = ct->supTBL->fcapacity;
			if(ct->fsize > 0) {
				knh_Object_t *suptmp = (knh_Object_t*)ct->supTBL->protoNULL, *supobj = ct->supTBL->defnull;
				knh_ObjectField_expand(ctx, ct->protoNULL, 0, ct->fsize);
				knh_ObjectField_expand(ctx, ct->defobj, 0, ct->fsize);
				knh_memcpy(ct->protoNULL->fields, suptmp->ref, ct->fsize*sizeof(knh_Object_t*));
				knh_memcpy(ct->defnull->ref, supobj->ref, ct->fsize*sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
#ifdef K_USING_CSTACK_TRAVERSE_
#define ARG RCinc
#else
#define ARG ctx->refs
#endif
				ct->supTBL->ospi->reftrace(ctx, suptmp, ARG);
#ifndef K_USING_CSTACK_TRAVERSE_
				knh_RefTraverse(ctx, RCinc);
#endif
				ct->supTBL->ospi->reftrace(ctx, supobj, ARG);
#ifndef K_USING_CSTACK_TRAVERSE_
				knh_RefTraverse(ctx, RCinc);
#endif
#undef ARG
#endif
			}
		}
	}
	DBG_P("DP(stmt)->size=%d", DP(stmt)->size);
	if(DP(stmt)->size == 4 && TT_(tkNN(stmt, 1)) == TT_ASIS) {
		knh_Stmt_done(ctx, stmt);
	}
	return K_CONTINUE;
}

static knh_status_t Stmt_eval(CTX ctx, knh_Stmt_t *stmtITR, knh_Array_t *resultsNULL)
{
	knh_status_t status = K_CONTINUE;
	BEGIN_LOCAL(ctx, lsfp, 3);
	knh_Stmt_t *stmt = stmtITR;
	KNH_SETv(ctx, lsfp[0].o, stmtITR); // lsfp[1] stmtNEXT
	while(stmt != NULL) {
		knh_Stmt_t *stmtNEXT = DP(stmt)->nextNULL;
		if(stmtNEXT != NULL) {
			KNH_SETv(ctx, lsfp[1].o, stmt);
			KNH_SETv(ctx, lsfp[2].o, stmtNEXT);
			KNH_FINALv(ctx, DP(stmt)->nextNULL);
			DP(stmt)->nextNULL = NULL;
		}
		ctx->gma->uline = stmt->uline;
		switch(STT_(stmt)) {
		case STT_NAMESPACE:
		{
			knh_NameSpace_t *ns = new_NameSpace(ctx, K_GMANS);
			KNH_SETv(ctx, K_GMANS, ns);
			status = Stmt_eval(ctx, stmtNN(stmt, 0), resultsNULL);
			DBG_ASSERT(K_GMANS == ns);
			DBG_ASSERT(ns->parentNULL != NULL);
			KNH_SETv(ctx, K_GMANS, ns->parentNULL);
			knh_Stmt_done(ctx, stmt);
			break;
		}
		case STT_IF: /* Conditional Compilation */
			status = IF_eval(ctx, stmt, resultsNULL);
			break;
		case STT_INCLUDE:
			status = INCLUDE_eval(ctx, stmt, resultsNULL);
			break;
		case STT_USING:
			status = USING_eval(ctx, stmt);
			break;
		case STT_CLASS:
			status = CLASS_decl(ctx, stmt);
			break;
		case STT_CONST:
			status = CONST_decl(ctx, stmt);
			break;
		case STT_ERR:
		case STT_BREAK:
			knh_Stmt_done(ctx, stmt);
			_RETURN(K_BREAK);
		}
		if(status != K_CONTINUE) {
			goto L_RETURN;
		}
		if(STT_(stmt) != STT_DONE) {
			SCRIPT_typing(ctx, stmt);
			SCRIPT_asm(ctx, stmt);
		}
		if(STT_(stmt) != STT_DONE) {
			status = SCRIPT_eval(ctx, stmt, knh_isCompileOnly(ctx), resultsNULL);
			if(status != K_CONTINUE) {
				goto L_RETURN;
			}
		}
		stmt = stmtNEXT;
	}
	L_RETURN:;
	END_LOCAL_(ctx, lsfp);
	ctx->gma->uline = 0;
	return status;
}

knh_bool_t knh_eval(CTX ctx, knh_InputStream_t *in, knh_Array_t *resultsNULL)
{
	int isCONTINUE = 1;
	BEGIN_LOCAL(ctx, lsfp, 3);
	KNH_SETv(ctx, lsfp[0].o, in);
	if(resultsNULL != NULL) {
		KNH_SETv(ctx, lsfp[1].o, resultsNULL);
	}
	LOCAL_NEW(ctx, lsfp, 2, knh_Stmt_t *, stmt, knh_InputStream_parseStmt(ctx, in));
	isCONTINUE = Stmt_eval(ctx, stmt, resultsNULL);
	END_LOCAL_(ctx, lsfp);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */

static int bytes_isempty(knh_bytes_t t)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		if(!isspace(t.utext[i])) return 0;
	}
	return 1;
}

static void Bytes_addQUOTE(CTX ctx, knh_Bytes_t *ba, knh_InputStream_t *in, int quote)
{
	int ch, prev = quote;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		knh_Bytes_putc(ctx, ba, ch);
		if(ch == quote && prev != '\\') {
			return;
		}
		prev = ch;
	}
}

static void Bytes_addCOMMENT(CTX ctx, knh_Bytes_t *ba, knh_InputStream_t *in)
{
	int ch, prev = 0, level = 1;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		knh_Bytes_putc(ctx, ba, ch);
		if(prev == '*' && ch == '/') level--;
		if(prev == '/' && ch == '*') level++;
		if(level == 0) return;
		prev = ch;
	}
}

static knh_InputStream_t* openScriptInputStreamNULL(CTX ctx, knh_NameSpace_t *ns, const char *scheme, knh_bytes_t path)
{
	knh_path_t phbuf, *ph = knh_path_open_(ctx, scheme, path, &phbuf);
	path.text = P_text(ph); path.len = ph->plen;
	const knh_StreamDSPI_t *dspi = knh_getStreamDSPI(ctx, ns, path);
	knh_InputStream_t *in = NULL;
	if(dspi->realpath(ctx, ns, ph)) {
		knh_io_t fio = dspi->fopen(ctx, ph, "r", ctx->mon);
		if(fio != IO_NULL) {
			in = new_InputStreamDSPI(ctx, fio, dspi);
			knh_bytes_t rpath = B(P_text(ph));
			knh_uri_t uri = knh_getURI(ctx, rpath);
			ULINE_setURI(in->uline, uri);
			KNH_SETv(ctx, DP(in)->urn, knh_getURN(ctx, uri));
			KNH_SETv(ctx, ns->rpath, knh_path_newString(ctx, ph, 0));
			DBG_P("URI=%d, nsname='%s' rpath='%s'", (int)uri, S_tochar(DP(ns)->nsname), S_tochar(ns->rpath));
		}
	}
	if(in == NULL) {
		KNH_WARN(ctx, "file not found '%s'", path.text);
	}
	knh_path_close(ctx, ph);
	return in;
}

static int readchunk(CTX ctx, knh_InputStream_t *in, knh_Bytes_t *ba)
{
	int ch;
	int prev = 0, isBLOCK = 0;
	int linenum = ULINE_line(in->uline);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\r') continue;
		knh_Bytes_putc(ctx, ba, ch);
		if(prev == '/' && ch == '*') {
			Bytes_addCOMMENT(ctx, ba, in);
			continue;
		}
		if(ch == '\'' || ch == '"' || ch == '`') {
			Bytes_addQUOTE(ctx, ba, in, ch);
			continue;
		}
		if(isBLOCK != 1 && prev == '\n' && ch == '\n') {
			break;
		}
		if(prev == '{') {
			isBLOCK = 1;
		}
		if(prev == '\n' && ch == '}') {
			isBLOCK = 0;
		}
		prev = ch;
	}
	return linenum;
}

KNHAPI2(knh_status_t) knh_load(CTX ctx, const char *scheme, knh_bytes_t path, knh_Array_t *resultsNULL)
{
	knh_bool_t status = K_BREAK;
	knh_InputStream_t *in = openScriptInputStreamNULL(ctx, K_GMANS, scheme, path);
	if(in != NULL) {
		knh_Bytes_t *ba = new_Bytes(ctx, "chunk", K_PAGESIZE);
		BEGIN_LOCAL(ctx, lsfp, 2);
		LOCAL_NEW(ctx, lsfp, 1, knh_InputStream_t*, bin, new_BytesInputStream(ctx, ba));
		KNH_SETv(ctx, lsfp[2].o, in);
		if(!knh_isCompileOnly(ctx)) {
			KNH_SECINFO(ctx, "running script path='%s'", path.text);
		}
		do {
			int linenum = 0;
			knh_Bytes_clear(ba, 0);
			if(!InputStream_isClosed(ctx, in)) {
				linenum = readchunk(ctx, in, ba);
			}
			if(!bytes_isempty(ba->bu)) {
				knh_uri_t uri = ULINE_uri(in->uline);
				knh_InputStream_setpos(ctx, bin, 0, BA_size(ba));
				bin->uline = linenum;
				ULINE_setURI(bin->uline, uri);
				//InputStream_setCharset(ctx, bin, DP(in)->enc);
				DBG_(if(knh_isSystemVerbose()) {
					fprintf(stderr, "\n>>>--------------------------------\n");
					fprintf(stderr, "%s<<<--------------------------------\n", knh_Bytes_ensureZero(ctx, ba));
				});
				status  = knh_eval(ctx, bin, resultsNULL);
			}
		} while(BA_size(ba) > 0 && status == K_CONTINUE);
		END_LOCAL_(ctx, lsfp);
	}
	return status;
}

/* ------------------------------------------------------------------------ */

knh_status_t konoha_initload(konoha_t konoha, const char *path)
{
	knh_status_t status;
	KONOHA_CHECK(konoha, 0);
	CTX ctx = (CTX)konoha.ctx;
	KONOHA_BEGIN(ctx);
	status = knh_load(ctx, "start", B(path), NULL);
	knh_stack_clear(ctx, ctx->stack);
	KONOHA_END(ctx);
	return status;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
