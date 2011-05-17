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

#include "konoha_t.h"

#ifndef KONOHA_VM_H_
#define KONOHA_VM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* KCODE */

#define rshift(rbp, x_) (&rbp[x_])
#define R_NEXTIDX (K_NEXTIDX)
#define Rn_(x)    (rshift(rbp,x)->ndata)
#define Ri_(x)    (rshift(rbp,x)->ivalue)
#define Rf_(x)    (rshift(rbp,x)->fvalue)
#define Rb_(x)    (rshift(rbp,x)->bvalue)
#define Ro_(x)    (rshift(rbp,x)->o)
#define Rh_(x)    (rshift(rbp,x)->hdr)
#define Rba_(x)   (rshift(rbp,x)->ba)
#define Ra_(x)    (rshift(rbp,x)->a)
#define Rx_(x)    (rshift(rbp,x)->ox)

#define RXo_(x)    (Rx_(x.i)->fields[x.n])
#define RXd_(x)   (*((knh_ndata_t*) &RXo_(x)))
#define SFP(rbp)  ((knh_sfp_t*)(rbp))
#define SFPIDX(n) ((n)/2)
#define RBP(sfp)  ((knh_rbp_t*)(sfp))

#define PC_NEXT(pc)   pc+1
#define PC_PREV(pc)   pc-1

#define USE_PROF(C)

#define VMPROF(OPCODE) USE_PROF({\
		knh_uint64_t t = knh_getTime();\
		_UTIME[OPCODE] += (t - _utime);\
		_UC[OPCODE] += 1;\
		_utime = t;\
	})\

/* [HALT] */

#define KLR_HALT(ctx) {\
	THROW_Halt(ctx, SFP(rbp), "HALT"); \
	goto L_RETURN;\
}\

/* [MOV, SET] */

/* NSET */

#define KLR_NSET(ctx, a, n) Rn_(a) = n;
#define KLR_NMOV(ctx, a, b) Rn_(a) = Rn_(b);

#define KLR_NNMOV(ctx, a, b, c, d) {\
		Rn_(a) = Rn_(b);\
		Rn_(c) = Rn_(d);\
	}\

#define KLR_NSET2(ctx, a, n, n2) {\
		Rn_(a) = n;\
		Rn_(a+R_NEXTIDX) = n2;\
	}\

#define KLR_NSET3(ctx, a, n, n2, n3) {\
		Rn_(a) = n;\
		Rn_(a+R_NEXTIDX) = n2;\
		Rn_(a+R_NEXTIDX+R_NEXTIDX) = n3;\
	}\

#define KLR_NSET4(ctx, a, n, n2, n3, n4) {\
		Rn_(a) = n;\
		Rn_(a+R_NEXTIDX) = n2;\
		Rn_(a+R_NEXTIDX+R_NEXTIDX) = n3;\
		Rn_(a+R_NEXTIDX+R_NEXTIDX+R_NEXTIDX) = n4;\
	}\

#define KLR_NMOVx(ctx, a, b)    Rn_(a) = RXd_(b)
#define KLR_XNSET(ctx, a, b)    RXd_(a) = b
#define KLR_XNMOV(ctx, a, b)    RXd_(a) = Rn_(b)
#define KLR_XNMOVx(ctx, a, b)   RXd_(a) = RXd_(b)

#define KLR_UNBOX(ctx, a, b, cid) {\
	Rn_(a) = O_data(Ro_(b));\
}\

/* OSET */

#define KLR_RCINC(ctx, a) {\
		RCGC_(Object *v_ = Ro_(a);)\
		knh_Object_RCinc(v_);\
	}\

#define KLR_RCDEC(ctx, a) {\
		Object *v_ = Ro_(a);\
		knh_Object_RCdec(v_);\
		if(Object_isRC0(v_)) {\
			knh_Object_RCfree(ctx, v_);\
		}\
	}\

#define KLR_RCINCx(ctx, a) {\
		RCGC_(Object *v_ = RXo_(a);)\
		knh_Object_RCinc(v_);\
	}\

#define KLR_RCDECx(ctx, a) {\
		Object *v_ = RXo_(a);\
		knh_Object_RCdec(v_);\
		if(Object_isRC0(v_)) {\
			knh_Object_RCfree(ctx, v_);\
		}\
	}\


#define klr_mov(ctx, v1, v2) {\
	Object *v1_ = (Object*)v1;\
	Object *v2_ = (Object*)v2;\
	knh_Object_RCinc(v2_);\
	knh_Object_RCdec(v1_);\
	if(Object_isRC0(v1_)) {\
		knh_Object_RCfree(ctx, v1_);\
	}\
	v1 = v2_;\
}\

#define KLR_OSET(ctx, a, v) {\
	klr_mov(ctx, Ro_(a), v);\
}\

#define KLR_OSET2(ctx, a, v, v2) {\
	KLR_OSET(ctx, a, v);\
	klr_mov(ctx, Ro_(a+R_NEXTIDX), v2);\
}\

#define KLR_OSET3(ctx, a, v, v2, v3) {\
	KLR_OSET2(ctx, a, v, v2);\
	klr_mov(ctx, Ro_(a+R_NEXTIDX+R_NEXTIDX), v3);\
}\

#define KLR_OSET4(ctx, a, v, v2, v3, v4) {\
	KLR_OSET3(ctx, a, v, v2, v3);\
	klr_mov(ctx, Ro_(a+R_NEXTIDX+R_NEXTIDX+R_NEXTIDX), v4);\
}\

#define KLR_OMOV(ctx, a, b) { \
	klr_mov(ctx, Ro_(a), Ro_(b));\
}\

#define KLR_ONMOV(ctx, a, b, c, d) {\
	KLR_OMOV(ctx, a, b);\
	KLR_NMOV(ctx, c, d);\
}\

#define KLR_OOMOV(ctx, a, b, c, d) {\
	KLR_OMOV(ctx, a, b);\
	KLR_OMOV(ctx, c, d);\
}\

#define KLR_OMOVx(ctx, a, b) {\
	knh_Object_t *v_ = RXo_(b);\
	klr_mov(ctx, Ro_(a), v_);\
}\

#define KLR_XMOV(ctx, a, b)     klr_mov(ctx, RXo_(a), Ro_(b))
#define KLR_XMOVx(ctx, a, b)    klr_mov(ctx, RXo_(a), RXo_(b))
#define KLR_XOSET(ctx, a, b)    klr_mov(ctx, RXo_(a), b)

/* ------------------------------------------------------------------------ */
/* [CALL] */

#define KLR_CHKSTACK(ctx, n) \
	if(unlikely(SFP(rshift(rbp, n)) > ctx->stacktop)) {\
		rbp = RBP(knh_stack_initexpand(ctx, SFP(rbp), n));\
	}\

#define KLR_SCALL(ctx, rtnidx, thisidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		prefetch((mtd_)->fcall_1);\
		knh_sfp_t *sfp_ = SFP(rshift(rbp, thisidx)); \
		sfp_[K_SHIFTIDX].shift = thisidx; \
		sfp_[K_PCIDX].pc = PC_NEXT(pc);\
		sfp_[K_MTDIDX].mtdNC = mtd_;\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		(mtd_)->fcall_1(ctx, sfp_, K_RTNIDX); \
		sfp_[K_MTDIDX].mtdNC = NULL;\
	} \

#define KLR_FASTCALL0(ctx, c, thisidx, rix, fcall, mtdO) { \
		fcall(ctx, SFP(rshift(rbp, thisidx)), (long)rix);\
	} \

/* ------------------------------------------------------------------------- */
/* VCALL */

#define KLR_VCALL(ctx, rtnidx, thisidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		prefetch((mtd_)->pc_start);\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		rbp = rshift(rbp, thisidx);\
		if(unlikely(SFP(rbp) > ctx->stacktop)) {\
			rbp = RBP(knh_stack_initexpand(ctx, SFP(rbp), 0));\
		}\
		rbp[K_SHIFTIDX2].shift = thisidx;\
		rbp[K_PCIDX2].pc = PC_NEXT(pc);\
		rbp[K_MTDIDX2].mtdNC = mtd_;\
		pc = (mtd_)->pc_start;\
		GOTO_PC(pc); \
	} \

#define KLR_VCALL_(ctx, rtnidx, thisidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		prefetch((mtd_)->pc_start);\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		rbp = rshift(rbp, thisidx);\
		rbp[K_SHIFTIDX2].shift = thisidx;\
		rbp[K_PCIDX2].pc = PC_NEXT(pc);\
		rbp[K_MTDIDX2].mtdNC = mtd_;\
		pc = (mtd_)->pc_start;\
		GOTO_PC(pc); \
	} \

#define KLR_JMP_(ctx, PC, JUMP)   KLR_RET(ctx)

#define KLR_RET(ctx) { \
		knh_intptr_t vshift = rbp[K_SHIFTIDX2].shift;\
		knh_opline_t *vpc = rbp[K_PCIDX2].pc;\
		rbp[K_MTDIDX2].mtdNC = NULL;\
		rbp = rshift(rbp, -vshift); \
		pc = vpc; \
		GOTO_PC(pc);\
	}\

#define KLR_YIELD(ctx, espidx) {\
		klr_setesp(ctx, SFP(rshift(rbp,espidx)));\
		goto L_RETURN;\
	}\

#define KLR_LOADMTD(ctx, thisidx, fmtd, mtdO) { \
		knh_Method_t *mtd_ = fmtd(ctx, SFP(rbp), SFPIDX(thisidx), mtdO);\
		if(mtd_ != mtdO) { \
			((klr_LOADMTD_t*)op)->mtdNC = mtd_;\
		}\
		klr_setmtdNC(ctx, rbp[thisidx+K_MTDIDX2], mtd_);\
	} \

#define KLR_CALL(ctx, rtnidx, thisidx, espshift) { \
		knh_Method_t *mtd_ = rbp[thisidx+K_MTDIDX2].mtdNC;\
		prefetch((mtd_)->pc_start);\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		rbp = rshift(rbp, thisidx);\
		rbp[K_SHIFTIDX2].shift = thisidx;\
		rbp[K_PCIDX2].pc = PC_NEXT(pc);\
		pc = (mtd_)->pc_start;\
		GOTO_PC(pc); \
	} \

#define KLR_VINVOKE(ctx, rtnidx, thisidx, espshift) { \
		knh_Method_t *mtd_ = (rbp[thisidx].fo)->mtd;\
		prefetch((mtd_)->pc_start);\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		rbp = rshift(rbp, thisidx);\
		rbp[K_SHIFTIDX2].shift = thisidx;\
		rbp[K_PCIDX2].pc = PC_NEXT(pc);\
		rbp[K_MTDIDX2].mtdNC = mtd_;\
		pc = (mtd_)->pc_start;\
		GOTO_PC(pc); \
	} \

#define KLR_THUNK(ctx, rtnidx, thisidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO == NULL ? rbp[thisidx+K_MTDIDX2].mtdNC : mtdO;\
		klr_setesp(ctx, SFP(rshift(rbp, espshift)));\
		knh_stack_newThunk(ctx, (knh_sfp_t*)rshift(rbp, thisidx));\
	} \

#define KLR_FUNCCALL(ctx) { \
		(rbp[K_MTDIDX2].mtdNC)->fcall_1(ctx, SFP(rbp), K_RTNIDX);\
		KLR_RET(ctx);\
	} \

#define KLR_VEXEC(ctx) {\
		knh_opline_t *vpc = PC_NEXT(pc);\
		pc = (rbp[K_MTDIDX2].mtdNC)->pc_start;\
		rbp[K_SHIFTIDX2].shift = 0;\
		rbp[K_PCIDX2].pc = vpc;\
		GOTO_PC(pc); \
	}\

#define KLR_ENTER(ctx) {\
		knh_opline_t *vpc = PC_NEXT(pc);\
		pc = (rbp[K_MTDIDX2].mtdNC)->pc_start;\
		rbp[K_SHIFTIDX2].shift = 0;\
		rbp[K_PCIDX2].pc = vpc;\
		GOTO_PC(pc); \
	}\


#define KLR_EXIT(ctx) {\
		pc = NULL; \
		goto L_RETURN;\
	}\

#define KLR_THCODE(ctx, th, uri) { \
		th(ctx, pc, OPJUMP); \
		pc = PC_NEXT(pc);\
		goto L_RETURN; \
	}\

/* ------------------------------------------------------------------------- */

#define KLR_iCAST(ctx, c, a) {\
	Ri_(c) = (knh_int_t)Rf_(a); \
}\

#define KLR_fCAST(ctx, c, a) {\
	Rf_(c) = (knh_float_t)Ri_(a); \
}\

#define KLR_SCAST(ctx, rtnidx, thisidx, rix, tmr)  { \
		knh_TypeMap_exec(ctx, tmr, SFP(rshift(rbp,thisidx)), rix); \
	} \

#define KLR_TCAST(ctx, rtnidx, thisidx, rix, tmr)  { \
		knh_TypeMap_t *tmr_ = tmr; \
		knh_sfp_t *sfp_ = SFP(rshift(rbp,thisidx));\
		knh_class_t scid = SP(tmr_)->scid, this_cid = O_cid(sfp_[0].o);\
		if(this_cid != scid) {\
			tmr_ = knh_findTypeMap(ctx, scid, SP(tmr)->tcid);\
			KNH_SETv(ctx, ((klr_TCAST_t*)op)->cast, tmr_);\
		}\
		knh_TypeMap_exec(ctx, tmr_, sfp_, rix); \
	} \

#define KLR_ACAST(ctx, rtnidx, thisidx, rix, tmr)  { \
		knh_TypeMap_t *tmr_ = tmr; \
		knh_class_t tcid = SP(tmr_)->tcid, this_cid = O_cid(Ro_(thisidx));\
		if(!class_isa(this_cid, tcid)) {\
			knh_class_t scid = SP(tmr_)->scid;\
			if(this_cid != scid) {\
				tmr_ = knh_findTypeMap(ctx, scid, tcid);\
				KNH_SETv(ctx, ((klr_ACAST_t*)op)->cast, tmr_);\
			}\
			knh_TypeMap_exec(ctx, tmr_, SFP(rshift(rbp,thisidx)), rix); \
		}\
	} \

#define KLR_TR(Ctx, c, a, rix, ct, f) { \
	f(ctx, SFP(rshift(rbp, a)), (long)rix, ct);\
}\

/* ------------------------------------------------------------------------ */

#define KLR_JMP(ctx, PC, JUMP) {\
	PC; \
	goto JUMP; \
}\

#define KLR_ONCE(ctx, PC, JUMP) { \
	((klr_ONCE_t*)op)->opcode = OPCODE_JMP;\
}\

#define KLR_bNUL(ctx, c, a)  Rb_(c) = IS_NULL(Ro_(a))
#define KLR_bNN(ctx, c, a)   Rb_(c) = IS_NOTNULL(Ro_(a))

#define KLR_JMPF(ctx, PC, JUMP, n) \
	if(Rb_(n)) {\
	}else{ \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR_DYJMP(ctx, PC, JUMP, n, fcheck) \
	if(fcheck(ctx, SFP(rbp), SFPIDX(n))) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

/* ------------------------------------------------------------------------- */

#define KLR_NEXT(ctx, PC, JUMP, rtnidx, ib, rix) { \
	knh_sfp_t *itrsfp_ = SFP(rshift(rbp, ib)); \
	DBG_ASSERT(IS_bIterator(itrsfp_[0].it));\
	if(!((itrsfp_[0].it)->fnext_1(ctx, itrsfp_, rix))) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \
} \

/* ------------------------------------------------------------------------- */

//#define NPC  /* for KNH_TRY */

#ifdef K_USING_SETJMP_

#define KLR_TRY(ctx, PC, JUMP, hn)  {\
	knh_ExceptionHandler_t* _hdr = Rh_(hn); \
	if(!IS_ExceptionHandler(_hdr)) { \
		_hdr = new_(ExceptionHandler); \
		klr_mov(ctx, Ro_(hn), _hdr); \
	} \
	int jump = knh_setjmp(DP(_hdr)->jmpbuf); \
	if(jump == 0) {\
		_hdr->espidx = (ctx->esp - ctx->stack); \
		_hdr->parentNC = ctx->ehdrNC;\
		((knh_context_t*)ctx)->ehdrNC = _hdr; \
	} else { \
		klr_setesp(ctx, (ctx->stack + _hdr->espidx));\
		((knh_context_t*)ctx)->ehdrNC = _hdr->parentNC;\
		KLR_JMP(ctx, PC, JUMP);\
	}\
} \

#define KLR_TRYEND(ctx, hn)  {\
	knh_ExceptionHandler_t* _hdr = Rh_(hn); \
	DBG_ASSERT(IS_ExceptionHandler(_hdr)); \
	((knh_context_t*)ctx)->ehdrNC = _hdr->parentNC;\
} \

#else

#define KLR_TRY(ctx, PC, JUMP, hn)  {\
	knh_ExceptionHandler_t* _hdr = Rh_(hn); \
	if(!IS_ExceptionHandler(_hdr)) { \
		_hdr = new_(ExceptionHandler); \
		klr_mov(ctx, Ro_(hn), _hdr); \
	} \
	_hdr = ExceptionHandler_setjmp(ctx, _hdr); \
	if(_hdr == NULL) {\
		knh_ExceptionHandlerEX_t* _hdrEX = DP(Rh_(hn));\
		_hdrEX->pc  = PC_NEXT(pc); \
		_hdrEX->op  = op;\
		_hdrEX->sfpidx = (SFP(rbp) - ctx->stack); \
		_hdr = Rh_(hn);\
		_hdr->espidx = (ctx->esp - ctx->stack); \
		_hdr->parentNC = ctx->ehdrNC;\
		((knh_context_t*)ctx)->ehdrNC = _hdr; \
	} else { \
		knh_ExceptionHandlerEX_t* _hdrEX = DP(_hdr);\
		pc = _hdrEX->pc; \
		rbp = RBP(ctx->stack + _hdrEX->sfpidx);\
		klr_setesp(ctx, (ctx->stack + _hdr->espidx));\
		op = _hdrEX->op;\
		((knh_context_t*)ctx)->ehdrNC = _hdr->parentNC;\
		KLR_JMP(ctx, PC, JUMP);\
	}\
} \

#define KLR_TRYEND(ctx, hn)  {\
	knh_ExceptionHandler_t* _hdr = Rh_(hn); \
	DBG_ASSERT(IS_ExceptionHandler(_hdr)); \
	DP(_hdr)->return_address = NULL;\
	DP(_hdr)->frame_address  = NULL;\
	((knh_context_t*)ctx)->ehdrNC = _hdr->parentNC;\
} \

#endif

#define KLR_THROW(ctx, start) { \
	knh_throw(ctx, SFP(rbp), SFPIDX(start)); \
} \

#define KLR_ASSERT(ctx, start, uline) { \
	knh_assert(ctx, SFP(rbp), SFPIDX(start), uline); \
} \

#define KLR_ERROR(ctx, start, msg) { \
	knh_Exception_t *e_ = new_Error(ctx, EBI_SourceCode, msg);\
	CTX_setThrowingException(ctx, e_);\
	knh_throw(ctx, SFP(rbp), SFPIDX(start)); \
} \

#define KLR_CATCH0(ctx, PC, JUMP, en, emsg)

#define KLR_CATCH(ctx, PC, JUMP, en, emsg) { \
		if(!isCATCH(ctx, rbp, en, emsg)) { \
			KLR_JMP(ctx, PC, JUMP); \
		} \
	} \

#define KLR_CHKIN(ctx, on, fcheckin)  {\
		knh_Object_t *o_ = Ro_(on);\
		fcheckin(ctx, SFP(rbp), o_);\
		Context_push(ctx, o_);\
	}\

#define KLR_CHKOUT(ctx, on, fcheckout)  {\
		knh_Object_t *o_ = Context_pop(ctx);\
		DBG_ASSERT(o_ == Ro_(on));\
		fcheckout(ctx, o_, 0);\
	}\

/* ------------------------------------------------------------------------ */

#define KLR_P(ctx, fprint, flag, msg, n) fprint(ctx, SFP(rbp), op)

#define KLR_PROBE(ctx, fprobe, n) { \
	fprobe(ctx, SFP(rbp), n, pc);\
}\

/* ------------------------------------------------------------------------ */

#define KLR_bNOT(ctx, c, a)     Rb_(c) = !(Rb_(a))

#define KLR_iINC(ctx, a)       Ri_(a)++
#define KLR_iDEC(ctx, a)       Ri_(a)--

#define KLR_iNEG(ctx, c, a)     Ri_(c) = -(Ri_(a))
#define KLR_iTR(ctx, c, a, f)      Ri_(c) = f((long)Ri_(a))

#define KLR_iADD(ctx, c, a, b)  Ri_(c) = (Ri_(a) + Ri_(b))
#define KLR_iADDC(ctx, c, a, n) Ri_(c) = (Ri_(a) + n)
#define KLR_iSUB(ctx, c, a, b)  Ri_(c) = (Ri_(a) - Ri_(b))
#define KLR_iSUBC(ctx, c, a, n) Ri_(c) = (Ri_(a) - n)
#define KLR_iMUL(ctx, c, a, b)  Ri_(c) = (Ri_(a) * Ri_(b))
#define KLR_iMULC(ctx, c, a, n) Ri_(c) = (Ri_(a) * n)
#define KLR_iDIV(ctx, c, a, b)  Ri_(c) = (Ri_(a) / Ri_(b));
#define KLR_iDIV2(ctx, c, a, b)  { \
		SYSLOG_iZERODIV(ctx, sfp, Ri_(b)); \
		Ri_(c) = (Ri_(a) / Ri_(b)); \
	} \

#define KLR_iDIVC(ctx, c, a, n)  Ri_(c) = (Ri_(a) / n)
#define KLR_iMOD(ctx, c, a, b)  Ri_(c) = (Ri_(a) % Ri_(b))
#define KLR_iMOD2(ctx, c, a, b)  { \
		SYSLOG_iZERODIV(ctx, sfp, Ri_(b)); \
		Ri_(c) = (Ri_(a) % Ri_(b)); \
	} \

#define KLR_iMODC(ctx, c, a, n)  Ri_(c) = (Ri_(a) % n)

#define KLR_iEQ(ctx, c, a, b)  Rb_(c) = (Ri_(a) == Ri_(b));
#define KLR_iEQC(ctx, c, a, n)  Rb_(c) = (Ri_(a) == n);
#define KLR_iNEQ(ctx, c, a, b)  Rb_(c) = (Ri_(a) != Ri_(b));
#define KLR_iNEQC(ctx, c, a, n)  Rb_(c) = (Ri_(a) != n);
#define KLR_iLT(ctx, c, a, b)  Rb_(c) = (Ri_(a) < Ri_(b));
#define KLR_iLTC(ctx, c, a, n)  Rb_(c) = (Ri_(a) < n);
#define KLR_iLTE(ctx, c, a, b)  Rb_(c) = (Ri_(a) <= Ri_(b));
#define KLR_iLTEC(ctx, c, a, n)  Rb_(c) = (Ri_(a) <= n);
#define KLR_iGT(ctx, c, a, b)  Rb_(c) = (Ri_(a) > Ri_(b));
#define KLR_iGTC(ctx, c, a, n)  Rb_(c) = (Ri_(a) > n);
#define KLR_iGTE(ctx, c, a, b)  Rb_(c) = (Ri_(a) >= Ri_(b));
#define KLR_iGTEC(ctx, c, a, n)  Rb_(c) = (Ri_(a) >= n);

#define KLR_iANDC(ctx, c, a, n)  Ri_(c) = (Ri_(a) & (n))
#define KLR_iORC(ctx, c, a, n)   Ri_(c) = (Ri_(a) | (n))
#define KLR_iXORC(ctx, c, a, n)  Ri_(c) = (Ri_(a) ^ (n))
#define KLR_iLSFTC(ctx, c, a, n) Ri_(c) = (Ri_(a) << (n))
#define KLR_iRSFTC(ctx, c, a, n) Ri_(c) = (Ri_(a) >> (n))

#define KLR_iAND(ctx, c, a, b)   KLR_iANDC(ctx, c, a, Ri_(b))
#define KLR_iOR(ctx, c, a, b)    KLR_iORC(ctx, c, a, Ri_(b))
#define KLR_iXOR(ctx, c, a, b)   KLR_iXORC(ctx, c, a, Ri_(b))
#define KLR_iLSFT(ctx, c, a, b)  KLR_iLSFTC(ctx, c, a, Ri_(b))
#define KLR_iRSFT(ctx, c, a, b)  KLR_iRSFTC(ctx, c, a, Ri_(b))

#define BR_(EXPR, PC, JUMP) if(EXPR) {} else {KLR_JMP(ctx, PC, JUMP); }

#define KLR_bJNUL(ctx, PC, JUMP, a)    BR_(IS_NULL(Ro_(a)), PC, JUMP)
#define KLR_bJNN(ctx, PC, JUMP, a)     BR_(IS_NOTNULL(Ro_(a)), PC, JUMP)

#define KLR_bJNOT(ctx, PC, JUMP, a)     BR_(!Rb_(a), PC, JUMP)
#define KLR_iJEQ(ctx, PC, JUMP, a, b)   BR_((Ri_(a) == Ri_(b)), PC, JUMP)
#define KLR_iJEQC(ctx, PC, JUMP, a, n)  BR_((Ri_(a) == n), PC, JUMP)
#define KLR_iJNEQ(ctx, PC, JUMP, a, b)  BR_((Ri_(a) != Ri_(b)), PC, JUMP)
#define KLR_iJNEQC(ctx, PC, JUMP, a, n) BR_((Ri_(a) != n), PC, JUMP)
#define KLR_iJLT(ctx, PC, JUMP, a, b)   BR_((Ri_(a) < Ri_(b)), PC, JUMP)
#define KLR_iJLTC(ctx, PC, JUMP, a, n)  BR_((Ri_(a) < n), PC, JUMP)
#define KLR_iJLTE(ctx, PC, JUMP, a, b)  BR_((Ri_(a) <= Ri_(b)), PC, JUMP)
#define KLR_iJLTEC(ctx, PC, JUMP, a, n) BR_((Ri_(a) <= n), PC, JUMP)
#define KLR_iJGT(ctx, PC, JUMP, a, b)   BR_((Ri_(a) > Ri_(b)), PC, JUMP)
#define KLR_iJGTC(ctx, PC, JUMP, a, n)  BR_((Ri_(a) > n), PC, JUMP)
#define KLR_iJGTE(ctx, PC, JUMP, a, b)  BR_((Ri_(a) >= Ri_(b)), PC, JUMP)
#define KLR_iJGTEC(ctx, PC, JUMP, a, n) BR_((Ri_(a) >= n), PC, JUMP)

/* ------------------------------------------------------------------------ */

#define KLR_fNEG(ctx, c, a)     Rf_(c) = -(Rf_(a))
#define KLR_fTR(ctx, c, a, f)      Rf_(c) = f((double)Rf_(a))

#define KLR_fADD(ctx, c, a, b)  Rf_(c) = (Rf_(a) + Rf_(b))
#define KLR_fADDC(ctx, c, a, n) Rf_(c) = (Rf_(a) + n)
#define KLR_fSUB(ctx, c, a, b)  Rf_(c) = (Rf_(a) - Rf_(b))
#define KLR_fSUBC(ctx, c, a, n) Rf_(c) = (Rf_(a) - n)
#define KLR_fMUL(ctx, c, a, b)  Rf_(c) = (Rf_(a) * Rf_(b))
#define KLR_fMULC(ctx, c, a, n) Rf_(c) = (Rf_(a) * n)
#define KLR_fDIV(ctx, c, a, b)  Rf_(c) = (Rf_(a) / Rf_(b))
#define KLR_fDIV2(ctx, c, a, b)  { \
		SYSLOG_fZERODIV2(ctx, sfp, Rf_(b)); \
		Rf_(c) = (Rf_(a) / Rf_(b)); \
	} \

#define KLR_fDIVC(ctx, c, a, n)  Rf_(c) = (Rf_(a) / n)

#define KLR_fEQ(ctx, c, a, b) Rb_(c) = (Rf_(a) == Rf_(b))
#define KLR_fEQC(ctx, c, a, n) Rb_(c) = (Rf_(a) == n)
#define KLR_fNEQ(ctx, c, a, b)  Rb_(c) = (Rf_(a) != Rf_(b))
#define KLR_fNEQC(ctx, c, a, n)  Rb_(c) = (Rf_(a) != n)
#define KLR_fLT(ctx, c, a, b)  Rb_(c) = (Rf_(a) < Rf_(b))
#define KLR_fLTC(ctx, c, a, n)  Rb_(c) = (Rf_(a) < n)

#define KLR_fLTE(ctx, c, a, b)  Rb_(c) = (Rf_(a) <= Rf_(b))
#define KLR_fLTEC(ctx, c, a, n) Rb_(c) = (Rf_(a) <= n)
#define KLR_fGT(ctx, c, a, b)  Rb_(c) = (Rf_(a) > Rf_(b))
#define KLR_fGTC(ctx, c, a, n)  Rb_(c) = (Rf_(a) > n)
#define KLR_fGTE(ctx, c, a, b)  Rb_(c) = (Rf_(a) >= Rf_(b))
#define KLR_fGTEC(ctx, c, a, n)  Rb_(c) = (Rf_(a) >= n)

#define KLR_fJEQ(ctx, PC, JUMP, a, b)   BR_((Rf_(a) == Rf_(b)), PC, JUMP)
#define KLR_fJEQC(ctx, PC, JUMP, a, n)  BR_((Rf_(a) == n), PC, JUMP)
#define KLR_fJNEQ(ctx, PC, JUMP, a, b)  BR_((Rf_(a) != Rf_(b)), PC, JUMP)
#define KLR_fJNEQC(ctx, PC, JUMP, a, n) BR_((Rf_(a) != n), PC, JUMP)
#define KLR_fJLT(ctx, PC, JUMP, a, b)   BR_((Rf_(a) < Rf_(b)), PC, JUMP)
#define KLR_fJLTC(ctx, PC, JUMP, a, n)  BR_((Rf_(a) < n), PC, JUMP)
#define KLR_fJLTE(ctx, PC, JUMP, a, b)  BR_((Rf_(a) <= Rf_(b)), PC, JUMP)
#define KLR_fJLTEC(ctx, PC, JUMP, a, n) BR_((Rf_(a) <= n), PC, JUMP)
#define KLR_fJGT(ctx, PC, JUMP, a, b)   BR_((Rf_(a) > Rf_(b)), PC, JUMP)
#define KLR_fJGTC(ctx, PC, JUMP, a, n)  BR_((Rf_(a) > n), PC, JUMP)
#define KLR_fJGTE(ctx, PC, JUMP, a, b)  BR_((Rf_(a) >= Rf_(b)), PC, JUMP)
#define KLR_fJGTEC(ctx, PC, JUMP, a, n) BR_((Rf_(a) >= n), PC, JUMP)

/* ------------------------------------------------------------------------ */

#define klr_array_index(ctx, n, size)   (size_t)n

#ifdef OPCODE_CHKIDX
#define klr_array_check(n, size)
#else
#define klr_array_check(n, size) \
	if(unlikely(n >= size)) THROW_OutOfRange(ctx, SFP(rbp), n, size)

#endif

#define KLR_CHKIDX(ctx, aidx, nidx) {\
		size_t size_ = (rbp[aidx].a)->size;\
		size_t n_ = Ri_(nidx);\
		if(unlikely(n_ >= size_)) THROW_OutOfRange(ctx, SFP(rbp), n_, size_);\
	}\

#define KLR_CHKIDXC(ctx, aidx, n) {\
		size_t size_ = (rbp[aidx].a)->size;\
		if(unlikely(n >= size_)) THROW_OutOfRange(ctx, SFP(rbp), n, size_);\
	}\

#define KLR_BGETIDXC(ctx, cidx, aidx, N) {\
		knh_bytes_t *b_ = &BA_tobytes(rbp[aidx].ba);\
		size_t n_ = klr_array_index(ctx, N, b_->len);\
		klr_array_check(n_, b_->len);\
		Ri_(cidx) = b_->utext[n_];\
	}\

#define KLR_BGETIDX(ctx, cidx, aidx, nidx) KLR_BGETIDXC(ctx, cidx, aidx, Ri_(nidx))

#define KLR_BSETIDXC(ctx, cidx, aidx, N, vidx) {\
		knh_bytes_t *b_ = &BA_tobytes(Rba_(aidx));\
		size_t n_ = klr_array_index(ctx, N, b_->len);\
		klr_array_check(n_, b_->len);\
		b_->ubuf[n_] = (knh_uchar_t)Ri_(vidx);\
		Ri_(cidx) = Ri_(vidx);\
	}\

#define KLR_BSETIDX(ctx, cidx, aidx, nidx, vidx) KLR_BSETIDXC(ctx, cidx, aidx, Ri_(nidx), vidx)

#define KLR_OGETIDXC(ctx, cidx, aidx, N) {\
		knh_Array_t *a_ = Ra_(aidx);\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		Object *v_ = (a_)->list[n_];\
		klr_mov(ctx, Ro_(cidx), v_);\
	}\

#define KLR_OGETIDX(ctx, cidx, aidx, nidx) KLR_OGETIDXC(ctx, cidx, aidx, Ri_(nidx))

#define KLR_OSETIDXC(ctx, cidx, aidx, N, vidx) {\
		knh_Array_t *a_ = Ra_(aidx);\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		klr_mov(ctx, (a_)->list[n_], Ro_(vidx));\
		klr_mov(ctx, Ro_(cidx), Ro_(vidx));\
	}\

#define KLR_OSETIDX(ctx, cidx, aidx, nidx, vidx) KLR_OSETIDXC(ctx, cidx, aidx, Ri_(nidx), vidx)

#define KLR_NGETIDXC(ctx, cidx, aidx, N) {\
		knh_Array_t *a_ = Ra_(aidx);\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		Rn_(cidx) = (a_)->nlist[n_];\
	}\

#define KLR_NGETIDX(ctx, cidx, aidx, nidx) KLR_NGETIDXC(ctx, cidx, aidx, Ri_(nidx))

#define KLR_NSETIDXC(ctx, cidx, aidx, N, vidx) {\
		knh_Array_t *a_ = Ra_(aidx);\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		Rn_(cidx) = (a_)->nlist[n_] = Rn_(vidx);\
	}\

#define KLR_NSETIDX(ctx, cidx, aidx, nidx, vidx) KLR_NSETIDXC(ctx, cidx, aidx, Ri_(nidx), vidx)

#define KLR_NOP(ctx)

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_VM_H_*/
