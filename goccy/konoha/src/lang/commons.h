#ifndef COMMONS_H_
#define COMMONS_H_

#define K_INTERNAL
#include"../../include/konoha1.h"
#include"../../include/konoha1/konoha_code_.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

#define TT_UNTYPED TT_EOT

#define MN_newLIST   MN_new__LIST
#define MN_newARRAY  MN_new__ARRAY
#define MN_newTUPLE  MN_new__TUPLE
#define MN_newMAP    MN_new__MAP

#define TYPE_STMT      TYPE_var

#define K_SYSVAL_CTX    0
#define K_SYSVAL_CTXIN  1
#define K_SYSVAL_CTXOUT 2
#define K_SYSVAL_CTXERR 3
#define K_SYSVAL_STDIN  4
#define K_SYSVAL_STDOUT 5
#define K_SYSVAL_STDERR 6
#define K_SYSVAL_OS     7
#define K_SYSVAL_SCRIPT 8
#define K_SYSVAL_MAX    9

#define K_FLAG_PF_STDERR      1
#define K_FLAG_PF_EOL         (1<<1)
#define K_FLAG_PF_TIME        (1<<2)
#define K_FLAG_PF_FUNC        (1<<3)
#define K_FLAG_PF_LINE        (1<<4)
#define K_FLAG_PF_NAME        (1<<5)
#define K_FLAG_PF_BOL         (1<<6)

#define stmt_isExpr(stt)   ((STT_LET <= stt && stt <= STT_CALL1) || stt == STT_FUNCTION)
const char* TT__(knh_term_t tt);
#define TK__(tk)   TT__((tk)->tt)

#define TK_tobytes(tk)           S_tobytes((tk)->text)

#define TK_typing(tk, tt0, type0, index0) { \
		TT_(tk) = tt0;\
		SP(tk)->type = type0;\
		(tk)->index = index0;\
	}\

#define _NOTWARN     1
#define _NOTCHECK    (1<<1)
#define _BCHECK      (1<<2)
#define _NOTYPEMAP     (1<<3)
#define _ICAST       (1<<4)
#define _CONSTASIS   (1<<5)
#define _CONSTONLY   (1<<6)

#define TYPING(ctx, stmt, n, reqt, mode) {\
		knh_Token_t *tkRES_ = Tn_typing(ctx, stmt, n, reqt, mode);\
		if(TT_(tkRES_) == TT_ERR) {\
			return tkRES_;\
		}\
	}\

#define ADD_FLAG(flag, T, F) \
	if(knh_DictMap_getNULL(ctx, DP(o)->metaDictCaseMap, STEXT(T)) != NULL) {\
		flag |= F;\
	}\

#define isSINGLEFOREACH(stmt)   (DP(stmt)->size == 3)

#define tmNN(stmt, n)        (stmt)->terms[(n)]
#define tkNN(stmt, n)        (stmt)->tokens[(n)]
#define stmtNN(stmt, n)      (stmt)->stmts[(n)]

#define Tn__(stmt, n)        TT__(TT_(tmNN(stmt, n)))
#define TT_TERMs(stmt, n)    TT_(tmNN(stmt, n))
#define Tn_isCONST(stmt,n)   (TT_(tmNN(stmt, n)) == TT_CONST)
#define Tn_isASIS(stmt,n)    (TT_(tmNN(stmt, n)) == TT_ASIS)
#define Tn_isCID(stmt,n)     (TT_(tmNN(stmt, n)) == TT_CID)
#define Tn_const(stmt, n)    (tkNN(stmt, n))->data
#define Tn_isNULL(stmt, n)   (TT_TERMs(stmt, n) == TT_NULL)

#define Tn_isTRUE(stmt, n)   (Tn_isCONST(stmt, n) && IS_TRUE(Tn_const(stmt, n)))
#define Tn_isFALSE(stmt, n)  (Tn_isCONST(stmt, n) && IS_FALSE(Tn_const(stmt, n)))
#define Tn_type(stmt, n)     SP(tmNN(stmt, n))->type
#define Tn_cid(stmt, n)      CLASS_t(Tn_type(stmt, n))
//#define Tn_bcid(stmt, n)     ctx->share->ClassTBL[Tn_cid(stmt,n)].bcid

#define knh_Stmt_flag(ctx, stmt, text, flag)   knh_Stmt_flag_(ctx, stmt, STEXT(text), flag)
#define knh_StmtMETA_is(ctx, stmt, anno)       StmtMETA_is_(ctx, stmt, STEXT(anno))

#define Gamma_isCompilingFmt(ctx)    MN_isFMT((DP(ctx->gma)->mtd)->mn)

/* ------------------------------------------------------------------------ */

#define TM(o)             ((knh_Token_t*)o)
#define cSTT_(o)          TT__(STT_(o))

#define token_isNested(tt)  \
	(tt == TT_BRACE || tt == TT_PARENTHESIS || tt == TT_BRANCET)

#define TT_(tk)        SP(tk)->tt
#define TT_isSTR(tt)   (tt == TT_STR || tt == TT_TSTR || tt == TT_ESTR)
#define TT_isBINARY(tt) (TT_IS <= (tt) && (tt) <= TT_RSFT)

#define STT_(stmt)   SP(stmt)->stt
#define knh_Stmt_add(ctx, stmt, ...)   knh_Stmt_add_(ctx, stmt, ## __VA_ARGS__, NULL)

//#define TK1(tc)          (tc)->ts[((tc)->c)+1]
//#define IST_(tk, t)      S_equals((tk)->text, STEXT(t))
//#define TTn_(tk)         (tk)->tt_next

/* ------------------------------------------------------------------------ */

#define FL(o)            ((Object*)o)
#define _(s)           s
#define KNH_HINT(ctx, token)

/* ------------------------------------------------------------------------ */

#define SEPARATOR_IT    2
#define FOREACH_ITR     (FOREACH_loop+1)
#define FOREACH_SPR     (FOREACH_loop+2)
#define TRY_HDR         3
#define TEST_IT         2

/* ------------------------------------------------------------------------ */

#define METHOD_getSize  MN_toGETTER(FN_size)

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif

#endif /*COMMONS_H_*/
