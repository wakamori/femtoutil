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

#define USE_cwb_open

#include"commons.h"

#ifdef __cplusplus 
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [tokenizer] */

/* error */
#define KC_ERR       LOG_ERR
#define KC_TERROR    LOG_ERR
/* warning */
#define KC_EWARN     LOG_WARNING
#define KC_DWARN     LOG_WARNING
/* info */
#define KC_BAD       LOG_NOTICE
#define KC_NOTICE    LOG_NOTICE
#define KC_TINFO     LOG_INFO
#define KC_INFO      LOG_INFO
#define KC_DEBUG     LOG_DEBUG

/* ------------------------------------------------------------------------ */
/* @data */

static const char* KC__(int p)
{
	switch(p) {
	case LOG_EMERG:
	case LOG_ALERT:
	case LOG_CRIT:
	case LOG_ERR:  return "(error) ";
	case LOG_WARNING: return "(warning) ";
	case LOG_NOTICE:  return "(bad manner) ";
	case LOG_INFO:    return "(info) ";
	case LOG_DEBUG:   return "(debug) ";
	}
	return "(debug) ";
}

#ifdef K_USING_POSIX_
#define CTX_isTERM(ctx)  CTX_isInteractive(ctx)
#else
#define CTX_isTERM(ctx)  0
#endif

const char* TERM_BBOLD(CTX ctx)
{
	return CTX_isTERM(ctx) ? "\x1b[1m" : "";
}

const char* TERM_EBOLD(CTX ctx)
{
	return CTX_isTERM(ctx) ? "\x1b[0m" : "";
}

const char* TERM_BNOTE(CTX ctx, int pe)
{
	if(CTX_isTERM(ctx)) {
		if(pe <= LOG_WARNING) return "\x1b[1m\x1b[31m";
		return "\x1b[1m\x1b[32m";
	}
	return "";
}

const char* TERM_ENOTE(CTX ctx, int pe)
{
	return CTX_isTERM(ctx) ? "\x1b[0m" : "";
}

static knh_String_t *Gamma_vperror(CTX ctx, int pe, const char *fmt, va_list ap)
{
	knh_String_t *msg = TS_EMPTY;
	int isPRINT = (pe <= KC_DWARN) ? 1 : 0;
	if(pe != KC_DEBUG && (CTX_isInteractive(ctx) || knh_isCompileOnly(ctx))) {
		isPRINT = 1;
	}
	if(Gamma_isQuiet(ctx->gma) || ctx->gma->uline == 0) {
		isPRINT = 0;
	}
	if(isPRINT == 1) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_uline(ctx, cwb->w, ctx->gma->uline);
		knh_write_ascii(ctx, cwb->w, KC__(pe));
		knh_vprintf(ctx, cwb->w, fmt, ap);
		msg = knh_cwb_newString(ctx, cwb);
		knh_Array_add(ctx, DP(ctx->gma)->errmsgs, msg);
		fprintf(stderr, "%s - %s%s\n", TERM_BNOTE(ctx, pe), S_tochar(msg), TERM_ENOTE(ctx, pe));
	}
	return msg;
}

static knh_Token_t *Gamma_perror(CTX ctx, int pe, const char *fmt, ...)
{
	knh_String_t *msg;
	va_list ap;
	va_start(ap, fmt);
	msg = Gamma_vperror(ctx, pe, fmt, ap);
	va_end(ap);
	if(pe < KC_DWARN) {
		knh_Token_t *tkERR = new_(Token);
		tkERR->tt = TT_ERR;
		tkERR->uline = (ctx->gma)->uline;
		KNH_SETv(ctx, (tkERR)->data, msg);
		return tkERR;
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */
/* script */

knh_Token_t* ERROR_NotFound(CTX ctx, const char *whatis, const char *t)
{
	return Gamma_perror(ctx, KC_ERR, _("%s not found %s"), whatis, t);
}
void WARN_NotFound(CTX ctx, const char *whatis, const char *t)
{
	Gamma_perror(ctx, KC_DWARN, _("%s not found %s"), whatis, t);
}
knh_Token_t* ERROR_IncompatiblePackage(CTX ctx, knh_bytes_t path, const knh_PackageDef_t *pkgdef)
{
	return Gamma_perror(ctx, KC_ERR, "Incompatible: konoha(buildid=%d, CRC32=%d), %B(buildid=%d, CRC32=%d)",
		(int)K_BUILDID, (int)K_API2_CRC32, path, (int)pkgdef->buildid, (int)pkgdef->crc32);
}
knh_Token_t* ErrorRedefinedClass(CTX ctx, knh_bytes_t cname, knh_class_t cid)
{
	return Gamma_perror(ctx, KC_ERR, _("re-definition of %B(%d)"), cname, cid);
}
knh_Token_t* ErrorExtendingFinalClass(CTX ctx, knh_class_t cid)
{
	return Gamma_perror(ctx, KC_ERR, _("cannot extends final class %C"), cid);
}
void WARN_MuchBetter(CTX ctx, const char *token)
{
	Gamma_perror(ctx, KC_DWARN, _("%s is better"), token);
}
void WarningMethodName(CTX ctx, const char *name)
{
	Gamma_perror(ctx, KC_DWARN, _("%s should starts with lowercase"), name);
}

/* ------------------------------------------------------------------------ */
/* tokenizer */

static knh_Token_t *knh_Token_toERR(CTX ctx, knh_Token_t *tk, const char *fmt, ...)
{
	if(TT_(tk) != TT_ERR) {
		knh_String_t *msg;
		va_list ap;
		va_start(ap, fmt);
		ctx->gma->uline = tk->uline;
		msg = Gamma_vperror(ctx, KC_ERR, fmt, ap);
		va_end(ap);
		TT_(tk) = TT_ERR;
		KNH_SETv(ctx, (tk)->data, msg);
	}
	return tk;
}

void BadPracticeSemicolon(CTX ctx)
{
	if(!CTX_isInteractive(ctx)) Gamma_perror(ctx, KC_BAD, "needs ;");
}
void WarningUnexpectedCharacter(CTX ctx)
{
	Gamma_perror(ctx, KC_DWARN, _("unexpected multi-byte character"));
}
void WarningMisplaced(CTX ctx)
{
	Gamma_perror(ctx, KC_DWARN, _("misplaced and then ignored"));
}
knh_Token_t *ErrorHazardousStatement(CTX ctx)
{
	return Gamma_perror(ctx, KC_ERR, _("hazardous statement"));
}
void WarningNoEffect(CTX ctx)
{
	Gamma_perror(ctx, KC_DWARN, _("no effect"));
}
knh_Token_t* ErrorMisplaced(CTX ctx)
{
	return Gamma_perror(ctx, KC_ERR, _("misplaced"));
}
knh_Token_t* ErrorStaticType(CTX ctx, const char *msg)
{
	knh_Token_t* tkERR = Gamma_perror(ctx, KC_ERR, _("%s is not dynamic"), msg);
	DBG_ABORT("stop why?");
	return tkERR;
}
knh_Token_t* ErrorFieldAddition(CTX ctx, knh_class_t cid)
{
	return Gamma_perror(ctx, KC_ERR, _("%C is unable to add new fields"), cid);
}
knh_Token_t* ERROR_Block(CTX ctx, const char* block)
{
	return Gamma_perror(ctx, KC_ERR, _("syntax error: wrong %s"), block);
}
knh_Token_t* ERROR_text(CTX ctx, const char *keyword K_TRACEARGV)
{
	knh_Token_t *tk = Gamma_perror(ctx, KC_ERR, ("syntax error: %s"), keyword);
	KNH_HINT(ctx, keyword);
	return tk;
}
knh_Token_t* ERROR_Token(CTX ctx, knh_Token_t *tk K_TRACEARGV)
{
	if(TT_(tk) != TT_ERR) {
		tk = ERROR_text(ctx, IS_String((tk)->text) ? S_tochar((tk)->text) : TK__(tk) K_TRACEDATA);
	}
	return tk;
}
knh_Token_t* ERROR_Stmt(CTX ctx, knh_Stmt_t *stmt K_TRACEARGV)
{
	DBG_ASSERT(STT_(stmt) != STT_ERR);
	return ERROR_text(ctx, TT__(stmt->stt) K_TRACEDATA);
}
knh_Token_t* SyntaxErrorTokenIsNot(CTX ctx, knh_Token_t *tk, const char* whatis)
{
	DBG_P("TT(tk)=%s", TT__(tk->tt));
	return Gamma_perror(ctx, KC_ERR, ("%O is not %s"), tk, whatis);
}
knh_Token_t* ERROR_Required(CTX ctx, knh_Token_t *tk, const char *stmtexpr, const char *token)
{
	return knh_Token_toERR(ctx, tk, _("%s? needs %s"), stmtexpr, token);
}
void WarningNotInitialized(CTX ctx, knh_Token_t *tk, const char *tool)
{
	Gamma_perror(ctx, KC_EWARN, _("%s is not installed"), tool);
}
knh_Token_t* ErrorRegexCompilation(CTX ctx, knh_Token_t *tk, const char *regname, const char *regdata)
{
	return knh_Token_toERR(ctx, tk, _("%s compile error: /%s/"), regname, regdata);
}
knh_Token_t* ERROR_Undefined(CTX ctx, const char *whatis, knh_class_t cid, knh_Token_t *tk)
{
	if(cid != CLASS_unknown) {
		return knh_Token_toERR(ctx, tk, _("undefined %s: %C.%O"), whatis, cid, tk);
	}
	else {
		return knh_Token_toERR(ctx, tk, _("undefined %s: %O"), whatis, tk);
	}
}
knh_Token_t* ERROR_UndefinedName(CTX ctx, knh_Token_t *tk)
{
	return knh_Token_toERR(ctx, tk, _("undefined name: %O"), tk);
}
void WARN_UndefinedName(CTX ctx, knh_Token_t *tk)
{
	Gamma_perror(ctx, KC_EWARN, _("undefined name: %O"), tk);
}
void WARN_AlreadyDefined(CTX ctx, knh_Token_t *tk)
{
	Gamma_perror(ctx, KC_DWARN, _("already defined: %O"), tk);
}
void WARN_AlreadyDefinedClass(CTX ctx, knh_class_t cid, knh_class_t oldcid)
{
	Gamma_perror(ctx, KC_DWARN, _("%C is already defined: %C"), cid, oldcid);
}
knh_Token_t* ERROR_Denied(CTX ctx, const char *why, knh_Token_t *tk)
{
	return knh_Token_toERR(ctx, tk, _("%s: %O"), why, tk);
}
void WarningUndefined(CTX ctx, const char *whatis, const char *prefix, const char *msg)
{
	Gamma_perror(ctx, KC_DWARN, _("undefined %s: %s%s"), whatis, prefix, msg);
}
void WarningUndefinedOfClass(CTX ctx, const char *whatis, knh_class_t cid, const char *prefix, const char *msg)
{
	Gamma_perror(ctx, KC_DWARN, _("undefined %s of %C: %s%s"), whatis, cid, prefix, msg);
}
void WarningUnknownClass(CTX ctx, knh_Token_t *tk, knh_class_t defc)
{
	Gamma_perror(ctx, KC_DWARN, _("unknown class: %L ==> %C"), tk, defc);
}
knh_Token_t* ErrorTokenCannotAssign(CTX ctx, knh_Token_t *tk)
{
	return knh_Token_toERR(ctx, tk, _("unable to assign to %L"), tk);
}
knh_Token_t* ErrorUnsupportedConstructor(CTX ctx, knh_class_t mtd_cid)
{
	return Gamma_perror(ctx, KC_ERR, _("the constructor of %C is not supported"), mtd_cid);
}
knh_Token_t* ErrorUndefinedBehavior(CTX ctx, const char *token)
{
	return Gamma_perror(ctx, KC_ERR, _("undefined behavior of %s"), token);
}
knh_Token_t* ERROR_RequiredParameter(CTX ctx)
{
	return Gamma_perror(ctx, KC_ERR, _("needs a parameter to infer its type"));
}
void WARN_WrongTypeParam(CTX ctx, knh_class_t cid)
{
	if(cid != CLASS_unknown) {
		Gamma_perror(ctx, KC_DWARN, "%C<>: wrong type parameter", cid);
	}
	DBG_ABORT("h");
}
void InfoType(CTX ctx, const char *prefix, knh_bytes_t name, knh_type_t type)
{
	Gamma_perror(ctx, KC_TINFO, "suppose %s%B has type %T", prefix, name, type);
}
knh_Token_t* ErrorNumericType(CTX ctx, knh_type_t reqt)
{
	return Gamma_perror(ctx, KC_TERROR, _("not numeric: %C"), reqt);
}
void WarningOverflow(CTX ctx, const char *floatorint, knh_bytes_t t)
{
	Gamma_perror(ctx, KC_EWARN, _("%s overflow: %B"), floatorint, t);
}
//knh_Token_t* ErrorUnknownResourceName(CTX ctx, knh_bytes_t path)
//{
//	return Gamma_perror(ctx, KC_ERR, "unknown path: %B", path);
//}
void WarningUnusedName(CTX ctx, knh_Token_t *tk, knh_fieldn_t fn)
{
	knh_uline_t uline = ctx->gma->uline;
	ctx->gma->uline = tk->uline;
	Gamma_perror(ctx, KC_DWARN, _("unused %N"), fn);
	ctx->gma->uline = uline;
}
knh_Token_t* ErrorDifferentlyDeclaredType(CTX ctx, knh_fieldn_t fn, knh_type_t type)
{
	return Gamma_perror(ctx, KC_TERROR, _("differently declared: previous type %T %N"), type, fn);
}
knh_Token_t* ErrorTooManyVariables(CTX ctx)
{
	return Gamma_perror(ctx, KC_ERR, _("too many variables"));
}
//void WarningTooManyReturnValues(CTX ctx)
//{
//	Gamma_perror(ctx, KC_DWARN, _("too many return values"));
//}
void WARN_UseDefaultValue(CTX ctx, const char *whatis, knh_type_t type)
{
	Gamma_perror(ctx, KC_DWARN, _("%s default value of %T"), whatis, type);
}
knh_Token_t* ErrorUndefinedMethod(CTX ctx, knh_Token_t *tkMN)
{
	return Gamma_perror(ctx, KC_ERR, _("undefined method: %L"), tkMN);
}
void WarningNoFmt(CTX ctx, const char *fmt)
{
	Gamma_perror(ctx, KC_DWARN, "no such formatter: '%s'", fmt);
}
void WarningIllegalFormatting(CTX ctx, const char *fmt)
{
	Gamma_perror(ctx, KC_DWARN, "illegal formatting at: `.. %s`", fmt);
}

knh_Token_t* ErrorNotStaticMethod(CTX ctx, knh_Method_t *mtd)
{
	return Gamma_perror(ctx, KC_ERR, _("not static: %C.%M"), (mtd)->cid, (mtd)->mn);
}
knh_Token_t *ERROR_Unsupported(CTX ctx, const char *whatis, knh_class_t cid, const char *symbol)
{
	if(symbol == NULL) {
		return Gamma_perror(ctx, KC_ERR, "unsupported %s", whatis);
	}
	else if(cid == CLASS_unknown) {
		return Gamma_perror(ctx, KC_ERR, "unsupported %s: %s", whatis, symbol);
	}
	else {
		return Gamma_perror(ctx, KC_ERR, "unsupported %s: %s in %C", whatis, symbol, cid);
	}
}
void WARN_Unsupported(CTX ctx, const char *msg)
{
	Gamma_perror(ctx, KC_DWARN, _("unsupported %s"), msg);
}
void WARN_Ignored(CTX ctx, const char *whatis, knh_class_t cid, const char *symbol)
{
	if(symbol == NULL) {
		Gamma_perror(ctx, KC_DWARN, "ignored %s", whatis, symbol);
	}
	else if(cid == CLASS_unknown) {
		Gamma_perror(ctx, KC_DWARN, "ignored %s: %s", whatis, symbol);
	}
	else {
		Gamma_perror(ctx, KC_DWARN, "ignored %s: %C.%s", whatis, cid, symbol);
	}
}
void WarningUnnecessaryOperation(CTX ctx, const char *msg)
{
	Gamma_perror(ctx, KC_DWARN, "unnecessary operation: %s", msg);
}
void WarningTooManyParameters(CTX ctx)
{
	Gamma_perror(ctx, KC_DWARN, _("too many parameters"));
}
knh_Token_t* ERROR_Needs(CTX ctx, const char *whatis)
{
	return Gamma_perror(ctx, KC_ERR, _("%s is necessary"), whatis);
}
knh_Token_t* ERROR_MustBe(CTX ctx, const char *whatis, const char* token)
{
	return Gamma_perror(ctx, KC_ERR, "%s must be %s", token, whatis);
}
void WarningNullable(CTX ctx, knh_class_t cid)
{
	Gamma_perror(ctx, KC_DWARN, "%C doesn't take null", cid);
}
knh_Token_t* ErrorComparedDiffrentType(CTX ctx, knh_type_t t1, knh_type_t t2)
{
	return Gamma_perror(ctx, KC_TERROR, _("comparison of different type: %T %T"), t1, t2);
}
/* type error */
knh_Token_t *TERROR_Token(CTX ctx, knh_Token_t *tk, knh_class_t type, knh_class_t reqt)
{
	return knh_Token_toERR(ctx, tk, ("%O has type %T, not %T"), tk, type, reqt);
}
knh_Token_t *TypeErrorStmtNN(CTX ctx, knh_Stmt_t *stmt, int n, knh_type_t reqt, knh_type_t type)
{
	return Gamma_perror(ctx, KC_TERROR, _("%s(%d) has type %T, not %T"), TT__(SP(stmt)->stt), n, reqt, type);
}
knh_Token_t* TypeErrorCallParam(CTX ctx, int n, knh_Method_t *mtd, knh_class_t reqt, knh_class_t type)
{
	if(IS_Method(mtd)) {
		return Gamma_perror(ctx, KC_TERROR, _("%C.%M(#%d) has type %T, not %T"), (mtd)->cid, (mtd)->mn, n - 1, reqt, type);
	}
	else {
		KNH_ASSERT(IS_String(mtd));
		const char *fname = S_tochar((knh_String_t*)mtd);
		return Gamma_perror(ctx, KC_TERROR, _("%s(#d) has type %T, not %T"), fname, n - 1, reqt, type);
	}
}
knh_Token_t *TypeErrorNotFuncion(CTX ctx, knh_type_t reqt)
{
	return Gamma_perror(ctx, KC_TERROR, _("function is typed as %T"), reqt);
}
void WarningUpcast(CTX ctx, knh_class_t tcid, knh_class_t scid)
{
	Gamma_perror(ctx, KC_DWARN, _("upcast (%C)expr of %C"), tcid, scid);
}

void WarningDowncast(CTX ctx, knh_class_t tcid, knh_class_t scid)
{
	Gamma_perror(ctx, KC_DWARN, _("downcast (%C)expr of %C"), tcid, scid);
}

knh_Token_t* ErrorNoSuchTransCast(CTX ctx, knh_class_t tcid, knh_class_t scid)
{
	return Gamma_perror(ctx, KC_ERR, _("no translation: %C ==> %C"), scid, tcid);
}
//knh_Token_t* ErrorNoResourceHandler(CTX ctx, knh_bytes_t path)
//{
//	return Gamma_perror(ctx, KC_ERR, _("path may be uninstalled: %B"), path);
//}
//knh_Token_t* ErrorType(CTX ctx, knh_bytes_t path, knh_type_t reqt)
//{
//	return Gamma_perror(ctx, KC_INFO, _("%B: must NOT be %T"), path, reqt);
//}
void WarningDuplicatedDefault(CTX ctx)
{
	Gamma_perror(ctx, KC_EWARN, _("multiple default in switch"));
}
void WarningNotConstant(CTX ctx)
{
Gamma_perror(ctx, KC_DWARN, _("case takes a constant value"));
}
void WarningAlwaysFalseAssertion(CTX ctx)
{
Gamma_perror(ctx, KC_EWARN, _("always throw Assertion!!"));
}
void WarningDifferentMethodClass(CTX ctx, knh_bytes_t name, knh_class_t cid)
{
	Gamma_perror(ctx, KC_DWARN, _("different class: %B ==> %C"), name, cid);
}
void WarningDeprecated(CTX ctx, const char *msg)
{
	Gamma_perror(ctx, KC_DWARN, _("depreciated %s"), msg);
}
knh_Token_t* ErrorFinalMethod(CTX ctx, knh_class_t cid, knh_methodn_t mn)
{
	return Gamma_perror(ctx, KC_ERR, _("%C.%M is final"), cid, mn);
}
knh_Token_t* ErrorDifferentlyDefinedMethod(CTX ctx, knh_class_t mtd_cid, knh_methodn_t mn)
{
	return Gamma_perror(ctx, KC_TERROR, _("%C.%M must be defined the same"), mtd_cid, mn);
}

/* ------------------------------------------------------------------------ */

knh_Token_t* ERROR_OnlyTopLevel(CTX ctx, const char* stmt)
{
	return Gamma_perror(ctx, KC_ERR, _("available only at the top level: %s"), stmt);
}
knh_Token_t* ErrorUndefinedLabel(CTX ctx, knh_Token_t *tk)
{
	return Gamma_perror(ctx, KC_ERR, _("undefined label: %L"), tk);
}
void WarningAbstractMethod(CTX ctx, knh_Method_t *mtd)
{
	Gamma_perror(ctx, KC_DWARN, "abstract? %C.%M", (mtd)->cid, (mtd)->mn);
}
void WarningDividedByZero(CTX ctx)
{
	Gamma_perror(ctx, KC_DWARN, _("divided by zero"));
}

void WarningUndefinedFmt(CTX ctx, knh_class_t cid, knh_methodn_t mn)
{
	Gamma_perror(ctx, KC_DWARN, _("undefined formatter: %M for %C"), mn, cid);
}

void NoticeInliningMethod(CTX ctx, knh_Method_t *mtd)
{
	Gamma_perror(ctx, KC_DEBUG, _("inlining: %C.%M"), (mtd)->cid, (mtd)->mn);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
