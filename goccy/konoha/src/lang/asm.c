/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_equals    1
#define USE_cwb_open      1
#define USE_RCinc

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* kcode */

#define K_INLINECODE    (10)

static void Tn_asm(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local);
static void BLOCK_asm(CTX ctx, knh_Stmt_t *stmtH, knh_type_t reqt);
#define GammaLabel(ctx, n)   (knh_BasicBlock_t*)knh_Array_n(DP(ctx->gma)->lstacks, n)
#define HAS_OPCODE(C)     1
//#define HAS_OPCODE(C)     ctx->share->jitSPI->isOPCODE(OPCODE_##C)

knh_BasicBlock_t* new_BasicBlockLABEL(CTX ctx)
{
	knh_BasicBlock_t *bb = new_(BasicBlock);
	bb->listNC = DP(ctx->gma)->insts;
	DP(bb)->id = knh_Array_size(bb->listNC);
	knh_Array_add(ctx, DP(ctx->gma)->insts, bb);
	return bb;
}

#if defined(K_USING_THCODE_)
#define TADDR   NULL, 0/*counter*/
#else
#define TADDR   0/*counter*/
#endif/*K_USING_THCODE_*/

#define ASMLINE  0

#define ASM(T, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, ## __VA_ARGS__}; \
		Gamma_asm(ctx, (knh_opline_t*)(&op_), sizeof(klr_##T##_t)); \
	}\

#define ASMop(T, OP, ...) { \
		klr_##T##_t op_ = {TADDR, OP, ASMLINE, ## __VA_ARGS__}; \
		Gamma_asm(ctx, (knh_opline_t*)(&op_), sizeof(klr_##T##_t)); \
	}\

#define ASMbranch(T, lb, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, NULL, ## __VA_ARGS__}; \
		ASM_BRANCH_(ctx, lb, (knh_opline_t*)(&op_), sizeof(klr_##T##_t)); \
	}\

#define BasicBlock_add(ctx, bb, T, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, ## __VA_ARGS__};\
		knh_BasicBlock_add_(ctx, bb, 0, (knh_opline_t*)(&op_), sizeof(klr_##T##_t));\
	}\

#ifdef K_USING_RBP_
#define NC_(sfpidx)    (((sfpidx) * 2) + 1)
#define OC_(sfpidx)    ((sfpidx) * 2)
#define SFP_(sfpidx)   ((sfpidx) * 2)
#else
#define NC_(sfpidx)    sfpidx
#define OC_(sfpidx)    sfpidx
#define SFP_(sfpidx)   sfpidx
#endif

#define RIX_(rix)      rix

static void BasicBlock_expand(CTX ctx, knh_BasicBlock_t *bb, size_t newsize)
{
	knh_opline_t* newbuf = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t) * newsize);
	knh_memcpy(newbuf, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	knh_bzero(newbuf + DP(bb)->capacity, (newsize - DP(bb)->capacity)*sizeof(knh_opline_t));
	KNH_FREE(ctx, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	DP(bb)->opbuf = newbuf;
	DP(bb)->capacity = newsize;
}

void knh_BasicBlock_add_(CTX ctx, knh_BasicBlock_t *bb, knh_ushort_t line, knh_opline_t *op, size_t size)
{
	if(DP(bb)->capacity == 0) {
		DP(bb)->opbuf = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t));
		DP(bb)->capacity = 1;
	}
	else if(DP(bb)->capacity == 1) {
		BasicBlock_expand(ctx, bb, 4);
	}
	else if(DP(bb)->size == DP(bb)->capacity) {
		BasicBlock_expand(ctx, bb, DP(bb)->capacity * 2);
	}
	{
		knh_opline_t *pc = DP(bb)->opbuf + DP(bb)->size;
		knh_memcpy(pc, op, size == 0 ? sizeof(knh_opline_t) : size);
#ifdef K_USING_RCGC
#ifdef K_USING_CSTACK_TRAVERSE_
#define ARG RCinc
#else
#define ARG ctx->refs
#endif
		knh_opline_reftrace(ctx, op, ARG);
		knh_RefTraverse(ctx, RCinc);
#undef ARG
#endif
		pc->line = line;
		DP(bb)->size += 1;
	}
}

static void _bBOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct);
static void _BOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct);
static void _OBOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct);

static void Gamma_asm(CTX ctx, knh_opline_t *op, size_t size)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	DBG_ASSERT(op->opcode != OPCODE_JMPF);
	if(DP(bb)->size > 0) {
		knh_opline_t *opP = DP(bb)->opbuf + (DP(bb)->size - 1);
		if(op->opcode == opP->opcode && op->opcode <= OPCODE_TR) {
			size_t i, size = knh_opcode_size(op->opcode);
			for(i = 0; i < size; i++) {
				if(op->data[i] != opP->data[i]) goto L_REMOVE;
			}
			KNH_P("PEEPHOLE: removed same one");
			return;
		}
		L_REMOVE:;
		if(opP->opcode == OPCODE_UNBOX && op->opcode == OPCODE_TR) {
			klr_UNBOX_t *opUNBOX = (klr_UNBOX_t*)opP;
			klr_TR_t *opTR = (klr_TR_t*)op;
			if(opUNBOX->b == opTR->a && (opTR->tr == _OBOX || opTR->tr == _BOX || opTR->tr == _bBOX)) {
				DBG_ASSERT(opUNBOX->a == opTR->b); // CHECK Object o = 1;
				DP(bb)->size -= 1;
				KNH_P("PEEPHOLE: removed UNBOX. IS OK?"); // is it ok?
				return;
			}
		}
		if(opP->opcode == OPCODE_OSET && op->opcode == OPCODE_TR) {
			klr_OSET_t *opOSET = (klr_OSET_t*)opP;
			klr_TR_t *opTR = (klr_TR_t*)op;
			if(opOSET->a == opTR->a && (opTR->tr == _OBOX || opTR->tr == _BOX || opTR->tr == _bBOX)) {
				KNH_P("PEEPHOLE: removed unnecesarry BOX"); // is it ok?
				return;
			}
		}
	}
	knh_BasicBlock_add_(ctx, bb, ULINE_line(ctx->gma->uline), op, size);
}

static int Gamma_asmJMPF(CTX ctx, klr_JMPF_t *op)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	DBG_ASSERT(op->opcode == OPCODE_JMPF);
	int swap = 0;
	while(DP(bb)->size > 0) {
		knh_opline_t *opP = DP(bb)->opbuf + (DP(bb)->size - 1);
		if(opP->opcode == OPCODE_bNOT) {
			klr_bNOT_t *opN = (klr_bNOT_t*)opP;
//			DBG_P("REWRITE JMPF index %d => %d", op->a, opN->a);
			op->a = opN->a;
			swap = (swap == 0) ? 1 : 0;
			DP(bb)->size -= 1;
			continue;
		}
		if(OPCODE_iEQ <= opP->opcode && opP->opcode <= OPCODE_iGTE && HAS_OPCODE(iJEQ)) {
			klr_iJEQ_t *opN = (klr_iJEQ_t*)opP;
			knh_sfpidx_t a = ((klr_iEQ_t*)opP)->a;
			knh_sfpidx_t b = ((klr_iEQ_t*)opP)->b;
			opN->jumppc = (op)->jumppc;
			opN->a = a; opN->b = b;
			opP->opcode = OPCODE_iJEQ + ((opP)->opcode - OPCODE_iEQ);
			return swap;
		}
		if(OPCODE_iEQC <= opP->opcode && opP->opcode <= OPCODE_iGTEC && HAS_OPCODE(iJEQC)) {
			klr_iJEQC_t *opN = (klr_iJEQC_t*)opP;
			knh_sfpidx_t a = ((klr_iEQC_t*)opP)->a;
			knh_int_t n = ((klr_iEQC_t*)opP)->n;
			opN->jumppc = (op)->jumppc;
			opN->a = a; opN->n = n;
			opP->opcode = OPCODE_iJEQC + ((opP)->opcode - OPCODE_iEQC);
			return swap;
		}
		if(OPCODE_fEQ <= opP->opcode && opP->opcode <= OPCODE_fGTE && HAS_OPCODE(fJEQ)) {
			klr_fJEQ_t *opN = (klr_fJEQ_t*)opP;
			knh_sfpidx_t a = ((klr_fEQ_t*)opP)->a;
			knh_sfpidx_t b = ((klr_fEQ_t*)opP)->b;
			opN->jumppc = (op)->jumppc;
			opN->a = a; opN->b = b;
			opP->opcode = OPCODE_fJEQ + ((opP)->opcode - OPCODE_fEQ);
			return swap;
		}
		if(OPCODE_fEQC <= opP->opcode && opP->opcode <= OPCODE_fGTEC && HAS_OPCODE(fJEQC)) {
			klr_fJEQC_t *opN = (klr_fJEQC_t*)opP;
			knh_sfpidx_t a = ((klr_fEQC_t*)opP)->a;
			knh_float_t n = ((klr_fEQC_t*)opP)->n;
			opN->jumppc = (op)->jumppc;
			opN->a = a; opN->n = n;
			opP->opcode = OPCODE_fJEQC + ((opP)->opcode - OPCODE_fEQC);
			return swap;
		}
		break;
	}
	knh_BasicBlock_add_(ctx, bb, ctx->gma->uline, (knh_opline_t*)op, 0);
	return swap;
}

/* ------------------------------------------------------------------------ */
/* new_KonohaCode */

static inline knh_opcode_t BasicBlock_opcode(knh_BasicBlock_t *bb)
{
	if(DP(bb)->size == 0) return OPCODE_NOP;
	return DP(bb)->opbuf->opcode;
}

//#define BB(bb)   OPCODE__(BasicBlock_opcode(bb))
//
//static void dumpBB(knh_BasicBlock_t *bb, const char *indent)
//{
//	size_t i;
////	DBG_P("%sid=%i, size=%d", indent, DP(bb)->id, DP(bb)->size);
//	if(bb->nextNC != NULL) {
////		DBG_P("%s\tnext=%d", indent, DP(bb->nextNC)->id);
//		if(indent[0] == 0) dumpBB(bb->nextNC, "\t");
//	}
//	if(bb->jumpNC != NULL) {
////		DBG_P("%s\tjump=%d", indent, DP(bb->jumpNC)->id);
//		if(indent[0] == 0) dumpBB(bb->jumpNC, "\t");
//	}
//	for(i = 0; i < DP(bb)->size; i++) {
//		knh_opline_t *op = DP(bb)->opbuf + i;
////		DBG_P("%s\t opcode=%s", indent, OPCODE__(op->opcode));
//		(void)op;
//	}
//}

static void BasicBlock_strip0(CTX ctx, knh_BasicBlock_t *bb)
{
	L_TAIL:;
	if(BasicBlock_isVisited(bb)) return;
	BasicBlock_setVisited(bb, 1);
	if(bb->jumpNC != NULL) {
		L_JUMP:;
		knh_BasicBlock_t *bbJ = bb->jumpNC;
		if(DP(bbJ)->size == 0 && bbJ->jumpNC != NULL && bbJ->nextNC == NULL) {
			//DBG_P("DIRECT JMP id=%d JMP to id=%d", DP(bbJ)->id, DP(bbJ->jumpNC)->id);
			DP(bbJ)->incoming -= 1;
			bb->jumpNC = bbJ->jumpNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		if(DP(bbJ)->size == 0 && bbJ->jumpNC == NULL && bbJ->nextNC != NULL) {
			//DBG_P("DIRECT JMP id=%d NEXT to id=%d", DP(bbJ)->id, DP(bbJ->nextNC)->id);
			DP(bbJ)->incoming -= 1;
			bb->jumpNC = bbJ->nextNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		if(bb->nextNC == NULL) {
			if(DP(bbJ)->incoming == 1 ) {
				//DBG_P("REMOVED %d JMP TO %d", DP(bb)->id, DP(bbJ)->id);
				bb->nextNC = bbJ;
				bb->jumpNC = NULL;
				goto L_NEXT;
			}
		}
		BasicBlock_strip0(ctx, bbJ);
	}
	if(bb->jumpNC != NULL && bb->nextNC != NULL) {
		bb = bb->nextNC;
		goto L_TAIL;
	}
	L_NEXT:;
	if(bb->nextNC != NULL) {
		knh_BasicBlock_t *bbN = bb->nextNC;
		if(DP(bbN)->size == 0 && bbN->nextNC != NULL && bbN->jumpNC == NULL) {
			//DBG_P("DIRECT NEXT id=%d to NEXT id=%d", DP(bbN)->id, DP(bbN->nextNC)->id);
			DP(bbN)->incoming -= 1;
			bb->nextNC = bbN->nextNC;
			DP(bb->nextNC)->incoming += 1;
			goto L_NEXT;
		}
		if(DP(bbN)->size == 0 && bbN->nextNC == NULL && bbN->jumpNC != NULL) {
			//DBG_P("DIRECT NEXT id=%d to JUMP id=%d", DP(bbN)->id, DP(bbN->jumpNC)->id);
			DP(bbN)->incoming -= 1;
			bb->nextNC = NULL;
			bb->jumpNC = bbN->jumpNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		bb = bb->nextNC;
		goto L_TAIL;
	}
}

static void BasicBlock_freebuf(CTX ctx, knh_BasicBlock_t *bb)
{
	KNH_FREE(ctx, DP(bb)->opbuf, sizeof(knh_opline_t) * DP(bb)->capacity);
	DP(bb)->capacity = 0;
	DP(bb)->size = 0;
	DP(bb)->opbuf = NULL;
}

static void BasicBlock_join(CTX ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbN)
{
	//DBG_P("join %d(%s) size=%d and %d(%s) size=%d", DP(bb)->id, BB(bb), DP(bb)->size, DP(bbN)->id, BB(bbN), DP(bbN)->size);
	bb->nextNC = bbN->nextNC;
	bb->jumpNC = bbN->jumpNC;
	if(DP(bbN)->size == 0) {
		return;
	}
	if(DP(bb)->size == 0) {
		DBG_ASSERT(DP(bb)->capacity == 0);
		DP(bb)->opbuf = DP(bbN)->opbuf;
		DP(bb)->capacity = DP(bbN)->capacity;
		DP(bb)->size = DP(bbN)->size;
		DP(bbN)->opbuf = NULL;
		DP(bbN)->capacity = 0;
		DP(bbN)->size = 0;
		return;
	}
	if(DP(bb)->capacity < DP(bb)->size + DP(bbN)->size) {
		BasicBlock_expand(ctx, bb, DP(bb)->size + DP(bbN)->size);
	}
	knh_memcpy(DP(bb)->opbuf + DP(bb)->size, DP(bbN)->opbuf, sizeof(knh_opline_t) * DP(bbN)->size);
	DP(bb)->size += DP(bbN)->size;
	BasicBlock_freebuf(ctx, bbN);
}

static void BasicBlock_strip1(CTX ctx, knh_BasicBlock_t *bb)
{
	L_TAIL:;
	if(!BasicBlock_isVisited(bb)) return;
	BasicBlock_setVisited(bb, 0);  // MUST call after strip0
	if(bb->jumpNC != NULL) {
		if(bb->nextNC == NULL) {
			bb = bb->jumpNC;
			goto L_TAIL;
		}
		else {
			//DBG_P("** branch next=%d, jump%d", DP(bb->nextNC)->id, DP(bb->jumpNC)->id);
			BasicBlock_strip1(ctx, bb->jumpNC);
			bb = bb->nextNC;
			goto L_TAIL;
		}
	}
	if(bb->nextNC != NULL) {
		knh_BasicBlock_t *bbN = bb->nextNC;
		if(DP(bbN)->incoming == 1 && BasicBlock_opcode(bbN) != OPCODE_RET) {
			BasicBlock_join(ctx, bb, bbN);
			BasicBlock_setVisited(bb, 1);
			goto L_TAIL;
		}
		bb = bb->nextNC;
		goto L_TAIL;
	}
}

static size_t BasicBlock_peephole(CTX ctx, knh_BasicBlock_t *bb)
{
#define _REMOVE(opX)   opX->opcode = OPCODE_NOP; bbsize--; continue;
#define _REMOVE2(opX, opX2)   opX->opcode = OPCODE_NOP; opX2->opcode = OPCODE_NOP; bbsize -= 2; continue;
#define _REMOVE3(opX, opX2, opX3)   opX->opcode = OPCODE_NOP; opX2->opcode = OPCODE_NOP; opX3->opcode = OPCODE_NOP; bbsize -= 3; continue;

	size_t i, bbsize = DP(bb)->size;
	for(i = 0; i < DP(bb)->size; i++) {
		knh_opline_t *op = DP(bb)->opbuf + i;
		if(op->opcode == OPCODE_NOP) {
			bbsize--;
		}
	}
	for(i = 1; i < DP(bb)->size; i++) {
		knh_opline_t *opP = DP(bb)->opbuf + (i - 1);
		knh_opline_t *op = DP(bb)->opbuf + i;
		if((op->opcode == OPCODE_fCAST || op->opcode == OPCODE_iCAST) && opP->opcode == OPCODE_NMOV) {
			klr_fCAST_t *opCAST = (klr_fCAST_t*)op;
			klr_NMOV_t *opNMOV = (klr_NMOV_t*)opP;
			if(opNMOV->a == opCAST->b && opCAST->a == opCAST->b) {
				opCAST->b = opNMOV->b;
				_REMOVE(opP);
			}
		}
		if(opP->opcode == OPCODE_NSET && op->opcode == OPCODE_NSET) {
			klr_NSET_t *op1 = (klr_NSET_t*)opP;
			klr_NSET_t *op2 = (klr_NSET_t*)op;
			if(op1->a + K_NEXTIDX != op2->a) continue;
			if(sizeof(knh_uintptr_t) == sizeof(knh_uint_t)) {
				klr_NSET_t *op3 = (klr_NSET_t*)(DP(bb)->opbuf + i + 1);
				klr_NSET_t *op4 = (klr_NSET_t*)(DP(bb)->opbuf + i + 2);
				if(op3->opcode != OPCODE_NSET || op2->a + K_NEXTIDX != op3->a) goto L_NSET2;
				if(op4->opcode == OPCODE_NSET && op3->a + K_NEXTIDX == op4->a) {
					klr_NSET4_t *opNSET = (klr_NSET4_t*)opP;
					opNSET->opcode = OPCODE_NSET4;
					opNSET->n2 = op2->n;
					opNSET->n3 = op3->n;
					opNSET->n4 = op4->n;
					_REMOVE3(op2, op3, op4);
				}
				else {
					klr_NSET3_t *opNSET = (klr_NSET3_t*)opP;
					opNSET->opcode = OPCODE_NSET3;
					opNSET->n2 = op2->n;
					opNSET->n3 = op3->n;
					_REMOVE2(op2, op3);
				}
			}
			L_NSET2:;
			klr_NSET2_t *opNSET = (klr_NSET2_t*)opP;
			opNSET->opcode = OPCODE_NSET2;
			opNSET->n2 = op2->n;
			_REMOVE(op2);
		}
		if(opP->opcode == OPCODE_OSET && op->opcode == OPCODE_OSET) {
			klr_OSET_t *op1 = (klr_OSET_t*)opP;
			klr_OSET_t *op2 = (klr_OSET_t*)op;
			if(op1->a + K_NEXTIDX != op2->a) continue;
			{
				klr_OSET_t *op3 = (klr_OSET_t*)(DP(bb)->opbuf + i + 1);
				klr_OSET_t *op4 = (klr_OSET_t*)(DP(bb)->opbuf + i + 2);
				if(op3->opcode != OPCODE_OSET || op2->a + K_NEXTIDX != op3->a) goto L_OSET2;
				if(op4->opcode == OPCODE_OSET && op3->a + K_NEXTIDX == op4->a) {
					klr_OSET4_t *opOSET = (klr_OSET4_t*)opP;
					opOSET->opcode = OPCODE_OSET4;
					opOSET->v2 = op2->o;
					opOSET->v3 = op3->o;
					opOSET->v4 = op4->o;
					_REMOVE3(op2, op3, op4);
				}
				else {
					klr_OSET3_t *opOSET = (klr_OSET3_t*)opP;
					opOSET->opcode = OPCODE_OSET3;
					opOSET->v2 = op2->o;
					opOSET->v3 = op3->o;
					_REMOVE2(op2, op3);
				}
			}
			L_OSET2:;
			klr_OSET2_t *opOSET = (klr_OSET2_t*)opP;
			opOSET->opcode = OPCODE_OSET2;
			opOSET->v2 = op2->o;
			_REMOVE(op2);
		}
		if(op->opcode == OPCODE_NMOV) {
#ifdef OPCODE_NNMOV
			if(opP->opcode == OPCODE_NMOV && HAS_OPCODE(NNMOV)) {
				klr_NNMOV_t *opMOV = (klr_NNMOV_t*)opP;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				opP->opcode = OPCODE_NNMOV;
				_REMOVE(op);
			}
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(ONMOV)) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)opP;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				opP->opcode = OPCODE_ONMOV;
				_REMOVE(op);
			}
#endif
		}
		if(op->opcode == OPCODE_OMOV) {
#ifdef OPCODE_OOMOV
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(OOMOV)) {
				klr_OOMOV_t *opMOV = (klr_OOMOV_t*)opP;
				opMOV->c = ((klr_OMOV_t*)op)->a;
				opMOV->d = ((klr_OMOV_t*)op)->b;
				opP->opcode = OPCODE_OOMOV;
				_REMOVE(op);
			}
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(ONMOV)) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)opP;
				opMOV->c = opMOV->a;
				opMOV->d = opMOV->b;
				opMOV->a = ((klr_OMOV_t*)op)->a;
				opMOV->b = ((klr_OMOV_t*)op)->b;
				opP->opcode = OPCODE_ONMOV;
				_REMOVE(op);
			}
#endif
		}
	}
	if(bbsize < DP(bb)->size) {
		knh_opline_t *opD = DP(bb)->opbuf;
		for(i = 0; i < DP(bb)->size; i++) {
			knh_opline_t *opS = DP(bb)->opbuf + i;
			if(opS->opcode == OPCODE_NOP) continue;
			if(opD != opS) {
				*opD = *opS;
			}
			opD++;
		}
		DP(bb)->size = bbsize;
	}
	return DP(bb)->size; /*bbsize*/;
}

#define BB_(bb)   (bb != NULL) ? DP(bb)->id : -1

static size_t BasicBlock_size(CTX ctx, knh_BasicBlock_t *bb, size_t c)
{
	L_TAIL:;
	if(bb == NULL || BasicBlock_isVisited(bb)) return c;
	BasicBlock_setVisited(bb, 1);
	if(bb->nextNC != NULL) {
		if(BasicBlock_isVisited(bb) || BasicBlock_opcode(bb->nextNC) == OPCODE_RET) {
			knh_BasicBlock_t *bb2 = new_BasicBlockLABEL(ctx);
			bb2->jumpNC = bb->nextNC;
			bb->nextNC = bb2;
		}
	}
	if(bb->jumpNC != NULL && bb->nextNC != NULL) {
		DBG_ASSERT(bb->jumpNC != bb->nextNC);
		c = BasicBlock_size(ctx, bb->nextNC, c + BasicBlock_peephole(ctx, bb));
		bb = bb->jumpNC; goto L_TAIL;
	}
	if(bb->jumpNC != NULL) { DBG_ASSERT(bb->nextNC == NULL);
		if(BasicBlock_opcode(bb->jumpNC) == OPCODE_RET) {
			BasicBlock_add(ctx, bb, JMP_);
		}
		else {
			BasicBlock_add(ctx, bb, JMP);
		}
		c = BasicBlock_peephole(ctx, bb) + c;
		bb = bb->jumpNC;
		goto L_TAIL;
	}
	c = BasicBlock_peephole(ctx, bb) + c;
	bb = bb->nextNC;
	goto L_TAIL;
}

static knh_opline_t* BasicBlock_copy(CTX ctx, knh_opline_t *dst, knh_BasicBlock_t *bb, knh_BasicBlock_t **prev)
{
	BasicBlock_setVisited(bb, 0);
	DBG_ASSERT(!BasicBlock_isVisited(bb));
//	DBG_P("BB%d: asm nextNC=BB%d, jumpNC=BB%d", BB_(bb), BB_(bb->nextNC), BB_(bb->jumpNC));
	if(DP(bb)->code != NULL) {
		//DBG_P("BB%d: already copied", BB_(bb));
		return dst;
	}
	if(prev[0] != NULL && prev[0]->nextNC == NULL && prev[0]->jumpNC == bb) {
		dst -= 1;
		//DBG_P("BB%d: REMOVE unnecessary JMP/(?%s)", BB_(bb), OPCODE__(dst->opcode));
		DBG_ASSERT(dst->opcode == OPCODE_JMP || dst->opcode == OPCODE_JMP_);
		prev[0]->jumpNC = NULL;
		prev[0]->nextNC = bb;
	}
	DP(bb)->code = dst;
	if(DP(bb)->size > 0) {
		size_t i;
		knh_memcpy(dst, DP(bb)->opbuf, sizeof(knh_opline_t) * DP(bb)->size);
		if(bb->jumpNC != NULL) {
			DP(bb)->opjmp = (dst + (DP(bb)->size - 1));
			DBG_ASSERT(knh_opcode_hasjump(DP(bb)->opjmp->opcode));
		}
		for(i = 0; i < DP(bb)->size; i++) {
			knh_opline_t *op = dst + i;
			if(op->opcode == OPCODE_VCALL) {
				if(BasicBlock_isStackChecked(bb)) {
					op->opcode = OPCODE_VCALL_;
				}
				else {
					BasicBlock_setStackChecked(bb, 1);
				}
			}
			if(op->opcode == OPCODE_iADDC) {
				klr_iADDC_t *opN = (klr_iADDC_t*)op;
				if(opN->a == opN->c && opN->n == 1) {
					op->opcode = OPCODE_iINC;
				}
			}
			if(op->opcode == OPCODE_iSUBC) {
				klr_iSUBC_t *opN = (klr_iSUBC_t*)op;
				if(opN->a == opN->c && opN->n == 1) {
					op->opcode = OPCODE_iDEC;
				}
			}
//			DBG_P("BB%d: [%ld] %s", BB_(bb), i, OPCODE__(op->opcode));
		}
		dst = dst + DP(bb)->size;
		BasicBlock_freebuf(ctx, bb);
		prev[0] = bb;
	}
	if(bb->nextNC != NULL) {
		//DBG_P("BB%d: NEXT=BB%d", BB_(bb), BB_(bb->nextNC));
		DBG_ASSERT(DP(bb->nextNC)->code == NULL);
		if(BasicBlock_isStackChecked(bb) && DP(bb->nextNC)->incoming == 1) {
			BasicBlock_setStackChecked(bb->nextNC, 1);
		}
		dst = BasicBlock_copy(ctx, dst, bb->nextNC, prev);
	}
	if(bb->jumpNC != NULL) {
		//DBG_P("BB%d: JUMP=%d", DP(bb)->id, BB_(bb->jumpNC));
		if(BasicBlock_isStackChecked(bb) && DP(bb->jumpNC)->incoming == 1) {
			BasicBlock_setStackChecked(bb->jumpNC, 1);
		}
		dst = BasicBlock_copy(ctx, dst, bb->jumpNC, prev);
	}
	return dst;
}

static void BasicBlock_setjump(knh_BasicBlock_t *bb)
{
	while(bb != NULL) {
		BasicBlock_setVisited(bb, 1);
		if(bb->jumpNC != NULL) {
			knh_BasicBlock_t *bbJ = bb->jumpNC;
			klr_JMP_t *j = (klr_JMP_t*)DP(bb)->opjmp;
			j->jumppc = DP(bbJ)->code;
			//DBG_P("jump from id=%d to id=%d %s jumppc=%p", DP(bb)->id, DP(bbJ)->id, OPCODE__(j->opcode), DP(bbJ)->code);
			bb->jumpNC = NULL;
			if(!BasicBlock_isVisited(bbJ)) {
				BasicBlock_setVisited(bbJ, 1);
				BasicBlock_setjump(bbJ);
			}
		}
		bb = bb->nextNC;
	}
}

static knh_KonohaCode_t* BasicBlock_link(CTX ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbRET)
{
	knh_KonohaCode_t *kcode = new_(KonohaCode);
	knh_BasicBlock_t *prev[1] = {};
	SP(kcode)->uri = ULINE_uri(ctx->gma->uline);
	SP(kcode)->codesize = BasicBlock_size(ctx, bb, 0) * sizeof(knh_opline_t);
	SP(kcode)->code = (knh_opline_t*)KNH_MALLOC(ctx, SP(kcode)->codesize);
	DP(bbRET)->code = SP(kcode)->code; // dummy
	{
		knh_opline_t *op = BasicBlock_copy(ctx, SP(kcode)->code, bb, prev);
		DBG_ASSERT(op - kcode->code > 0);
		DP(bbRET)->code = NULL;
		BasicBlock_copy(ctx, op, bbRET, prev);
		BasicBlock_setjump(bb);
	}
	return kcode;
}

/* ------------------------------------------------------------------------ */

METHOD knh_Fmethod_runVM(CTX ctx, knh_sfp_t *sfp _RIX)
{
	DBG_ASSERT(rix == K_RTNIDX);
	DBG_ASSERT(IS_Method(sfp[K_MTDIDX].mtdNC));
	knh_VirtualMachine_run(ctx, sfp, CODE_VEXEC);
}

static void Method_threadCode(CTX ctx, knh_Method_t *mtd, knh_KonohaCode_t *kcode)
{
	knh_Method_setFunc(ctx, mtd, knh_Fmethod_runVM);
	if(Method_isObjectCode(mtd)) {
		KNH_SETv(ctx, DP(mtd)->kcode, kcode);
	}else {
		Method_setObjectCode(mtd, 1);
		KNH_INITv(DP(mtd)->kcode, kcode);
	}
	//if(O_isTenure(mtd)) O_toTenure(kcode);
	(mtd)->pc_start = knh_VirtualMachine_run(ctx, ctx->esp + 1, SP(kcode)->code);
	if(knh_isSystemVerbose()) {
		knh_write_Object(ctx, KNH_STDOUT, UPCAST(mtd), FMT_dump);
		knh_write_EOL(ctx, KNH_STDOUT);
	}
}

static void Gamma_compile(CTX ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbRET)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	BasicBlock_strip0(ctx, bb);
	BasicBlock_strip1(ctx, bb);
#ifdef K_USING_SSA
	// SSA optimization
	knh_BasicBlock_optimize(ctx, bb);
#endif
	knh_KonohaCode_t *kcode = BasicBlock_link(ctx, bb, bbRET);
	DBG_ASSERT(IS_Method(mtd));
	Method_threadCode(ctx, mtd, kcode);
	KNH_SETv(ctx, DP(ctx->gma)->mtd, KNH_NULL);
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
}

static void ASM_LABEL(CTX ctx, knh_BasicBlock_t *label)
{
	if(label != NULL) {
		knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
		if(bb != NULL) {
			bb->nextNC = label;
			DP(label)->incoming += 1;
			bb->jumpNC = NULL;
		}
		DP(ctx->gma)->bbNC = label;
	}
}

static void ASM_JMP(CTX ctx, knh_BasicBlock_t *label)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	if(bb != NULL) {
		bb->nextNC = NULL;
		bb->jumpNC = label;  DP(label)->incoming += 1;
	}
	DP(ctx->gma)->bbNC = NULL; /*KNH_TNULL(BasicBlock);*/
}

static knh_BasicBlock_t* ASM_JMPF(CTX ctx, int flocal, knh_BasicBlock_t *lbJUMP)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	knh_BasicBlock_t *lbNEXT = new_BasicBlockLABEL(ctx);
	klr_JMPF_t op = {TADDR, OPCODE_JMPF, ASMLINE, NULL, NC_(flocal)};
	if(Gamma_asmJMPF(ctx, &op)) {
		bb->jumpNC = lbNEXT;
		bb->nextNC = lbJUMP;
	}
	else {
		bb->jumpNC = lbJUMP;
		bb->nextNC = lbNEXT;
	}
	DP(lbJUMP)->incoming += 1;
	DP(lbNEXT)->incoming += 1;
	DP(ctx->gma)->bbNC = lbNEXT;
	return lbJUMP;
}

static void ASM_BRANCH_(CTX ctx, knh_BasicBlock_t *jump, knh_opline_t *op, size_t size)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	knh_BasicBlock_t *newbb = new_BasicBlockLABEL(ctx);
	Gamma_asm(ctx, op, size);
	bb->jumpNC = jump;  DP(jump)->incoming += 1;
	bb->nextNC = newbb;  DP(newbb)->incoming += 1;
	DP(ctx->gma)->bbNC = newbb;
}

static void ASM_RET(CTX ctx, knh_Stmt_t *stmt)
{
//	while(DP(stmt)->nextNULL != NULL) {
//		stmt = DP(stmt)->nextNULL;
//		if(STT_(stmt) == STT_CHKOUT) {
//			CHKOUT_asm(ctx, stmt);
//		}
//	}
//	{
		knh_BasicBlock_t *bbEND = GammaLabel(ctx,  2);
		ASM_JMP(ctx, bbEND);
//	}
}

/* ------------------------------------------------------------------------ */
/* [func] */

static void _NULVAL(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	KNH_SETv(ctx, sfp[c].o, ct->fdefnull(ctx, ct->cid));
}
static void _PROP(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	Object *v = (Object*)knh_getPropertyNULL(ctx, S_tobytes(sfp[0].s));
	if(v == NULL) v = ct->fdefnull(ctx, ct->cid);
	KNH_SETv(ctx, sfp[c].o, v);
}
static void _NPROP(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	Object *v = (Object*)knh_getPropertyNULL(ctx, S_tobytes(sfp[0].s));
	if(v == NULL) {
		sfp[c].ivalue = 0;
	}
	else {
		sfp[c].ndata = O_data(v);
	}
}
static void _VARGS(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	knh_Array_t *a = new_Array(ctx, ct->cid, ctx->esp - (sfp - 1));
	a->api->add(ctx, a, sfp - 1);
	KNH_SETv(ctx, sfp[c].o, a);
}
static void _bBOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	knh_Object_t *b = sfp[0].bvalue ? KNH_TRUE : KNH_FALSE;
	KNH_SETv(ctx, sfp[c].o, b);
}
static void _BOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	Object *v = new_Boxing(ctx, sfp, ct);
	KNH_SETv(ctx, sfp[c].o, v);
}
static void _OBOX(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	knh_class_t cid2 = O_cid(sfp[0].o);
	if(IS_Tunbox(cid2)) {
		Object *v = new_Boxing(ctx, sfp, O_cTBL(sfp[0].o));
		KNH_SETv(ctx, sfp[c].o, v);
	}
}
static void _CWB(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	size_t pos = BA_size(ctx->bufa);
	KNH_SETv(ctx, sfp[c].o, ctx->bufw);
	sfp[c].ivalue = pos;
	//DBG_P("sfpidx=%d, pos=%zd", (sfp + c) - ctx->stack, pos);
}
static void _TOSTR(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	DBG_ASSERT(IS_OutputStream(sfp[0].w));
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	cwb->pos = (size_t)(sfp[0].ivalue); // reset
	//DBG_P("sfpidx=%d, pos=%zd", (sfp) - ctx->stack, cwb->pos);
	knh_String_t *s = knh_cwb_newString(ctx, cwb);
	KNH_SETv(ctx, sfp[c].o, s);
}
static void _ERR(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	if(IS_bString(sfp[0].o)) {
		CTX_setThrowingException(ctx, new_Error(ctx, EBI_newid, sfp[0].s));
	}
	else {
		DBG_ASSERT(IS_Exception(sfp[0].o));
		CTX_setThrowingException(ctx, sfp[0].e);
	}
}
static void _CHKTYPE(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct)
{
	if(!ClassTBL_isa_(ctx, O_cTBL(sfp[0].o), ct)) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "ClassCast!!: %C is not %C", O_cid(sfp[0].o), ct->cid);
		CTX_setThrowingException(ctx, new_Error(ctx, EBI_newid, knh_cwb_newString(ctx, cwb)));
		knh_throw(ctx, sfp, c);
	}
}
static knh_Method_t* _SETMTD(CTX ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	return mtd;
}
static knh_Method_t* _LOOKUPMTD(CTX ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	const knh_ClassTBL_t *t = O_cTBL(sfp[thisidx].o);
	if(t->cid != (mtd)->cid) {
		mtd = ClassTBL_getMethod(ctx, t, (mtd)->mn);
	}
	return mtd;
}
static knh_Method_t* _CHKMTD(CTX ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	const knh_ClassTBL_t *t = O_cTBL(sfp[thisidx].o);
	if(t->cid != (mtd)->cid) {
		mtd = ClassTBL_getMethod(ctx, t, (mtd)->mn);
	}
	knh_stack_typecheck(ctx, sfp + thisidx, mtd, NULL);
	return mtd;
}
//static int _ISSKIP(CTX ctx, knh_sfp_t *sfp, int a)
//{
//	return 0;
//}
/* SYSVAL */
static METHOD Fmethod_CTX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_((Object*)toContext(ctx));
}
static METHOD Fmethod_CTXIN(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->in);
}
static METHOD Fmethod_CTXOUT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->out);
}
static METHOD Fmethod_CTXERR(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->err);
}
static METHOD Fmethod_STDIN(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->in);
}
static METHOD Fmethod_STDOUT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->out);
}
static METHOD Fmethod_STDERR(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(DP(ctx->sys)->err);
}
static METHOD Fmethod_SYS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->sys);
}
static METHOD Fmethod_SCRIPT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->script);
}

static knh_Fmethod SYSVAL_MTD[] = {
	Fmethod_CTX,
	Fmethod_CTXIN,
	Fmethod_CTXOUT,
	Fmethod_CTXERR,
	Fmethod_STDIN,
	Fmethod_STDOUT,
	Fmethod_STDERR,
	Fmethod_SYS,
	Fmethod_SCRIPT,
};

/* ------------------------------------------------------------------------ */
/* [Gamma] */

#define TT_isSFPIDX(tk)   (TT_(tk) == TT_LOCAL || TT_(tk) == TT_FUNCVAR)
#define Token_index(tk)   Token_index_(ctx, tk)
static inline int Token_index_(CTX ctx, knh_Token_t *tk)
{
	return (int)(tk)->index + ((TT_(tk) == TT_LOCAL) ? DP(ctx->gma)->ebpidx : 0);
}

static void ASM_BOX2(CTX ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_class_t cid = CLASS_t(atype);
	knh_class_t bcid = ClassTBL(cid)->bcid;
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		knh_class_t rcid = ClassTBL(CLASS_t(reqt))->bcid;
		if(rcid != bcid && reqt != TYPE_void) {
			if(cid == CLASS_Boolean) {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _bBOX);
			}
			else if (IS_Tnumbox(reqt)) {
				Object *v = (cid == CLASS_Int) ? UPCAST(KNH_INT0) : UPCAST(KNH_FLOAT0);
				ASM(OSET, OC_(a), v);
			}
			else {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _BOX);
			}
		}
	}
	else if(atype == CLASS_Tdynamic && IS_Tnumbox(reqt)) {
		ASM(UNBOX, NC_(a), OC_(a), ClassTBL(reqt));
	}
	else if(IS_Tnumbox(atype) && reqt == CLASS_Tdynamic) {
		ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(atype), _OBOX);
	}
}

static void ASM_UNBOX(CTX ctx, knh_type_t atype, int a)
{
	if(IS_Tunbox(atype) || IS_Tnumbox(atype)) {
		ASM(UNBOX, NC_(a), OC_(a), ClassTBL(atype));
	}
}

#define ASML(idx) (idx < DP(ctx->gma)->espidx) ? (DP(ctx->gma)->espidx) : idx

static void ASM_MOVL(CTX ctx, knh_type_t reqt, int sfpidx, knh_type_t ltype, int local)
{
	if(sfpidx < DP(ctx->gma)->espidx) {
		if(IS_Tunbox(ltype)) {
			ASM(NMOV, NC_(sfpidx), NC_(local));
		}
		else {
#if defined(K_USING_RCGC) && defined(OPCODE_SWAP)
			ASM(SWAP, NC_(sfpidx), NC_(local));
#else
			ASM(OMOV, OC_(sfpidx), OC_(local));
#endif
		}
	}
}

static int Tn_put(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(TT_(tk) == TT_FUNCVAR || TT_(tk) == TT_LOCAL) return Token_index(tk);
	Tn_asm(ctx, stmt, n, reqt, sfpidx);
	return sfpidx;
}

static knh_Token_t* Tn_putTK(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(IS_Stmt(tk)) {
		Tn_asm(ctx, stmt, n, reqt, sfpidx);
		DBG_ASSERT(IS_Token(tkNN(stmt, n)));
		return tkNN(stmt, n);
	}
	return tk;
}


static void ASM_SMOVx(CTX ctx, knh_type_t atype, int a, knh_type_t btype, knh_sfx_t bx)
{
	if(IS_Tunbox(btype)) {
		ASM(NMOVx, NC_(a), bx)
		ASM_BOX2(ctx, atype, btype, (a));
	}
	else {
		ASM(OMOVx, OC_(a), bx);
		ASM_UNBOX(ctx, btype, a);
	}
}

static void ASM_SMOV(CTX ctx, knh_type_t atype, int a/*flocal*/, knh_Token_t *tkb)
{
	knh_type_t btype = SP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_ASIS:
			break;
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = (tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTBL(cid)->defnull) {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _NULVAL);
				break;
			}
			KNH_SETv(ctx, (tkb)->data, v);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, (tkb)->data, new_Type(ctx, (tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *v = (tkb)->data;
			if(IS_Tunbox(atype)) {
				ASM(NSET, NC_(a), O_data((tkb)->data));
			}
			else if(IS_Tnumbox(atype)) {
				ASM(NSET, NC_(a), O_data((tkb)->data));
				ASM(OSET, OC_(a), v);
			}
			else {
				ASM(OSET, OC_(a), v);
			}
			break;
		}
		case TT_FUNCVAR:
		case TT_LOCAL: {
			int b = Token_index(tkb);
			knh_type_t btype = SP(tkb)->type;
			if(IS_Tunbox(btype)) {
				ASM(NMOV, NC_(a), NC_(b));
				ASM_BOX2(ctx, atype, btype, a);
			}
			else {
				ASM(OMOV, OC_(a), OC_(b));
				if(IS_Tnumbox(btype)) {
					if(IS_Tnumbox(atype)) {
						ASM(NMOV, NC_(a), NC_(b));
					}
					else {
						ASM(TR, OC_(a), SFP_(b), RIX_(a-b), ClassTBL(atype), _OBOX);
					}
				}
			}
			break;
		}
		case TT_FIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(0), (size_t)b};
			if(IS_Token(tkb->token) && TT_isSFPIDX(tkb->token)) {
				bx.i = OC_(Token_index(tkb->token));
			}
			ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}
		case TT_SCRFIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(DP(ctx->gma)->scridx), (size_t)b};
			ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			size_t sysid = (tkb)->index;
			KNH_ASSERT(sysid < K_SYSVAL_MAX);
			ASM(FASTCALL0, SFP_(a), SFP_(a), 0, SYSVAL_MTD[sysid]);
			break;
		}
		case TT_PROPN: {
			ASM(OSET, OC_(a), (tkb)->data);
			if(IS_Tunbox(btype)) {
				ASM(TR, NC_(a), SFP_(a), RIX_(a-a), ClassTBL(CLASS_t(btype)), _NPROP);
			}
			else {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(CLASS_t(btype)), _PROP);
			}
			ASM_BOX2(ctx, atype, btype, a);
			break;
		}
		default: {
			DBG_ABORT("unknown TT=%s", TT__(TT_(tkb)));
		}
	}/*switch*/
}

static void ASM_XMOVx(CTX ctx, knh_type_t atype, knh_sfx_t ax, knh_type_t btype, knh_sfx_t bx)
{
	int espidx = DP(ctx->gma)->espidx;
	if(IS_Tunbox(atype)) {
		if(IS_Tunbox(btype)) {
			ASM(XNMOVx, ax, bx);
		}
		else {
			ASM(OMOVx, NC_(espidx), bx);
			ASM_UNBOX(ctx, atype, espidx);
			ASM(XNMOV, ax, NC_(espidx));
		}
	}
	else {
		DBG_ASSERT(atype == TYPE_dyn || IS_Tnumbox(atype));
		if(IS_Tunbox(btype)) { // dynamic a = b; // int b;
			ASM(NMOVx, NC_(espidx), bx);
			ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(btype)), _BOX);
			ASM(XMOV, ax, OC_(espidx));
		}
		else {
			ASM(XMOVx, ax, bx);
		}
	}
}

static void ASM_XMOV(CTX ctx, knh_type_t atype, int a, size_t an, knh_Token_t *tkb)
{
	knh_sfx_t ax = {OC_(a), an};
	knh_type_t btype = SP(tkb)->type;
	long espidx = -1;
	DBG_ASSERT(DP(ctx->gma)->espidx != -1);
	switch(TT_(tkb)) {
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = (tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTBL(cid)->defnull) {
				espidx = DP(ctx->gma)->espidx;
				ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(cid), _NULVAL);
				break;
			}
			KNH_SETv(ctx, (tkb)->data, v);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, (tkb)->data, new_Type(ctx, (tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *v = (tkb)->data;
			if(IS_Tunbox(atype)) {
				ASM(XNSET, ax, O_data(v));
			}
			else {
				ASM(XOSET, ax, v);
			}
			break;
		}
		case TT_FUNCVAR:
		case TT_LOCAL: {
			int b = Token_index(tkb);
			ASM_BOX2(ctx, atype, btype, b);
			if(IS_Tunbox(atype)) {
				ASM(XNMOV, ax, NC_(b));
			}
			else {
				ASM(XMOV, ax, OC_(b));
			}
			break;
		}
		case TT_FIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(0), (size_t)b};
			if(IS_Token(tkb->token) && TT_isSFPIDX(tkb->token)) {
				bx.i = OC_(Token_index(tkb->token));
			}
			ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}
		case TT_SCRFIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(DP(ctx->gma)->scridx), (size_t)b};
			ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			size_t sysid = (tkb)->index;
			KNH_ASSERT(sysid < K_SYSVAL_MAX);
			espidx = DP(ctx->gma)->espidx;
			ASM(FASTCALL0, SFP_(espidx), SFP_(espidx), 0, SYSVAL_MTD[sysid]);
			break;
		}
		case TT_PROPN: {
			espidx = DP(ctx->gma)->espidx;
			ASM(OSET, OC_(espidx), (tkb)->data);
			if(IS_Tunbox(atype)) {
				ASM(TR, NC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(atype)), _NPROP);
			}
			else {
				ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(atype)), _PROP);
			}
			ASM_BOX2(ctx, atype, btype, espidx);
			break;
		}
		default: {
			DBG_ABORT("unknown TT=%d", TT_(tkb));
		}
	}/*switch*/
	if(espidx != -1) {
		if(IS_Tunbox(atype)) {
			ASM(XNMOV, ax, NC_(espidx));
		}
		else {
			ASM(XMOV, ax, OC_(espidx));
		}
	}
}

static void ASM_MOV(CTX ctx, knh_Token_t *tka, knh_Token_t *tkb)
{
	DBG_ASSERT(Token_isTyped(tka));
	DBG_ASSERT(IS_Token(tkb));
	DBG_ASSERT(Token_isTyped(tkb));
	knh_type_t atype = SP(tka)->type;
	if(TT_(tka) == TT_LOCAL || TT_(tka) == TT_FUNCVAR) {
		ASM_SMOV(ctx, atype, Token_index(tka), tkb);
	}
	else {
		DBG_ASSERT(TT_(tka) == TT_FIELD);
		int an = (int)(tka)->index;
		ASM_XMOV(ctx, atype, 0, an, tkb);
	}
}

static knh_BasicBlock_t* Tn_JMPIF(CTX ctx, knh_Stmt_t *stmt, size_t n, int isTRUE, knh_BasicBlock_t* label, int flocal)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(TT_(tk) == TT_CONST) {
		int isTRUE2 = IS_TRUE((tk)->data);
		ASM_SMOV(ctx, TYPE_Boolean, flocal, tk);
		if((!isTRUE && !isTRUE2) || (isTRUE && isTRUE2)) {
			ASM_JMP(ctx, label);
			return label;
		}
		return NULL;
	}
	if(TT_(tk) == TT_LOCAL || TT_(tk) == TT_FUNCVAR) {
		int index = Token_index(tk);
		if(isTRUE) {
			ASM(bNOT, NC_(index), NC_(index));
		}
		return ASM_JMPF(ctx, index, label);
	}
	else {
		Tn_asm(ctx, stmt, n, TYPE_Boolean, flocal);
		if(isTRUE) {
			ASM(bNOT, NC_(flocal), NC_(flocal));
		}
		return ASM_JMPF(ctx, flocal, label);
	}
}

/* ------------------------------------------------------------------------ */
/* [EXPR] */

static inline knh_int_t Tn_int(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = tkNN(stmt, n);
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return ((tk)->num)->n.ivalue;
}

static inline knh_float_t Tn_float(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = tkNN(stmt, n);
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return ((tk)->num)->n.fvalue;
}

static knh_opcode_t OPCODE_imn(knh_methodn_t mn, int diff)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_iNEG;
	case MN_opADD: return OPCODE_iADD + diff;
	case MN_opSUB: return OPCODE_iSUB + diff;
	case MN_opMUL: return OPCODE_iMUL + diff;
	case MN_opDIV: return OPCODE_iDIV+ diff;
	case MN_opMOD: return OPCODE_iMOD+ diff;
	case MN_opEQ:  return OPCODE_iEQ+ diff;
	case MN_opNOTEQ: return OPCODE_iNEQ+ diff;
	case MN_opLT:  return OPCODE_iLT+ diff;
	case MN_opLTE: return OPCODE_iLTE+ diff;
	case MN_opGT:  return OPCODE_iGT+ diff;
	case MN_opGTE: return OPCODE_iGTE+ diff;
#ifdef OPCODE_iAND
	case MN_opLAND :  return OPCODE_iAND  + diff;
	case MN_opLOR  :  return OPCODE_iOR   + diff;
	case MN_opLXOR :  return OPCODE_iXOR  + diff;
	case MN_opLSFT:  return OPCODE_iLSFT  + diff;
	case MN_opRSFT:  return OPCODE_iRSFT  + diff;
#endif
	}
	return OPCODE_NOP;
}

static knh_opcode_t OPCODE_fmn(knh_methodn_t mn, int diff)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_fNEG;
	case MN_opADD: return OPCODE_fADD + diff;
	case MN_opSUB: return OPCODE_fSUB + diff;
	case MN_opMUL: return OPCODE_fMUL + diff;
	case MN_opDIV: return OPCODE_fDIV + diff;
	case MN_opEQ:  return OPCODE_fEQ + diff;
	case MN_opNOTEQ: return OPCODE_fNEQ + diff;
	case MN_opLT:  return OPCODE_fLT + diff;
	case MN_opLTE: return OPCODE_fLTE + diff;
	case MN_opGT:  return OPCODE_fGT + diff;
	case MN_opGTE: return OPCODE_fGTE + diff;
	}
	return OPCODE_NOP;
}

static knh_bool_t OPR_hasCONST(CTX ctx, knh_Stmt_t *stmt, knh_methodn_t *mn, int swap)
{
	int isCONST = (TT_(tmNN(stmt, 2)) == TT_CONST);
	if(swap == 1 && TT_(tmNN(stmt, 1)) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == MN_opLT) newmn = MN_opGT;  /* 1 < n ==> n > 1 */
		else if(*mn == MN_opLTE) newmn = MN_opGTE; /* 1 <= n => n >= 1 */
		else if(*mn == MN_opGT) newmn = MN_opLT;
		else if(*mn == MN_opGTE) newmn = MN_opLTE;
		//DBG_P("swap %s ==> %s", MN__(*mn), MN__(newmn));
		*mn = newmn;
		isCONST = 1;
	}
	return isCONST;
}

static void CALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static int OPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
	if(IS_NULL(mtd)) return 0;
	knh_methodn_t mn = (mtd)->mn;
	knh_class_t cid = CLASS_t(SP(tkNN(stmt, 1))->type);
	knh_opcode_t opcode;
	if(cid == CLASS_Boolean && mn == MN_opNOT) {
		int a = Tn_put(ctx, stmt, 1, TYPE_Boolean, local + 1);
		ASM(bNOT, NC_(sfpidx), NC_(a));
		return 1;
	}
	if(cid == CLASS_Int && ((opcode = OPCODE_imn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			ASM(iNEG, NC_(sfpidx), NC_(a));
			return 1;
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD ||
				mn == MN_opLSFT || mn == MN_opRSFT) swap = 0;
		if(OPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			knh_int_t b = Tn_int(stmt, 2);
			if(b == 0 && (mn == MN_opDIV || mn == MN_opMOD)) {
				b = 1;
				WarningDividedByZero(ctx);
			}
			opcode = OPCODE_imn(mn, (OPCODE_iADDC - OPCODE_iADD));
			ASMop(iADDC, opcode, NC_(sfpidx), NC_(a), b);
		}
		else {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			int b = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
			ASMop(iADD, opcode, NC_(sfpidx), NC_(a), NC_(b));
		}
		return 1;
	} /* CLASS_Int */
	if(cid == CLASS_Float && ((opcode = OPCODE_fmn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			ASM(fNEG, NC_(sfpidx), NC_(a));
			return 1;
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap = 0;
		if(OPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			knh_float_t b = Tn_float(stmt, 2);
			if(b == K_FLOAT_ZERO && mn == MN_opDIV) {
				b = K_FLOAT_ONE;
				WarningDividedByZero(ctx);
			}
			opcode = OPCODE_fmn(mn, (OPCODE_fADDC - OPCODE_fADD));
			ASMop(fADDC, opcode, NC_(sfpidx), NC_(a), b);
		}
		else {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			int b = Tn_put(ctx, stmt, 2, TYPE_Float, local + 2);
			ASMop(fADD, opcode, NC_(sfpidx), NC_(a), NC_(b));
		}
		return 1;
	} /* CLASS_Float */
	CALL_asm(ctx, stmt, reqt, sfpidx);
	return 0;
}

/* ------------------------------------------------------------------------ */
/* CALL */

static knh_type_t Tn_ptype(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_class_t cid, knh_Method_t *mtd)
{
	if(!IS_Method(mtd)) {
		return TYPE_dyn;  // boxing
	}
	if(n == 1) { // base
		if(IS_Tunbox(cid) && cid != (mtd)->cid) {
			/* Object, Number, dynamic */
			cid = (mtd)->cid;
		}
		DBG_(
		if(cid != (mtd)->cid) {
			DBG_P("** WATCH OUT ** mtd_cid=%s, (mtd)->cid=%s", CLASS__(cid), CLASS__((mtd)->cid));
		});
		return cid;
	}
	else {
		knh_type_t ptype = knh_ParamArray_getptype(DP(mtd)->mp, n - 2);
		return knh_type_tocid(ctx, ptype, cid);
	}
}

static int CALLPARAMs_asm(CTX ctx, knh_Stmt_t *stmt, size_t s, int local, knh_class_t cid, knh_Method_t *mtd)
{
	size_t i;
	if(s == 1 && Method_isStatic(mtd)) s = 2;   // ignoring static caller, like Script
	if(DP(stmt)->size + DP(ctx->gma)->espidx > 32) {
		ASM(CHKSTACK, SFP_(DP(stmt)->size + DP(ctx->gma)->espidx));
	}
	for(i = s; i < DP(stmt)->size; i++) {
		knh_type_t reqt = Tn_ptype(ctx, stmt, i, cid, mtd);
		Tn_asm(ctx, stmt, i, reqt, local + i + (K_CALLDELTA-1));
	}
	if(Stmt_isTAILRECURSION(stmt) && Gamma_isTailRecursion(ctx->gma)) {
		knh_BasicBlock_t *lbBEGIN = GammaLabel(ctx, 1);
		for(i = s; i < DP(stmt)->size; i++) {
			knh_type_t reqt = Tn_ptype(ctx, stmt, i, cid, mtd);
			if(IS_Tunbox(reqt)) {
				ASM(NMOV, NC_(i-1), NC_(local + i + (K_CALLDELTA-1)));
			}
			else {
				ASM(OMOV, OC_(i-1), OC_(local + i + (K_CALLDELTA-1)));
			}
		}
		ASM_JMP(ctx, lbBEGIN);
		return 0;
	}
	return 1;
}

static void ASM_INLINE(CTX ctx, int sfpshift, knh_opline_t *code, size_t isize)
{
	size_t i, last = isize;
	knh_BasicBlock_t* bb[K_INLINECODE];
	for(i = 0; i < isize; i++) {
		bb[i] = new_BasicBlockLABEL(ctx);
		bb[i]->nextNC = NULL;
		bb[i]->jumpNC = NULL;
		if(code[i].opcode == OPCODE_RET) {
			last = i; break;
		}
	}
	KNH_ASSERT(last != isize);
	ASM_LABEL(ctx, bb[0]);
	for(i = 0; i < last; i++) {
		knh_opline_t opbuf, *op;
		opbuf = code[i]; op = &opbuf;
		knh_opcode_shift(op, sfpshift);
		if(op->opcode == OPCODE_JMP_) {
			op->opcode = OPCODE_JMP;
		}
		if(op->opcode != OPCODE_JMP) {
			knh_BasicBlock_add_(ctx, bb[i], op->line, op, 0);
			bb[i]->nextNC = bb[i+1];
			DP(bb[i+1])->incoming += 1;
		}
		if(knh_opcode_hasjump(op->opcode)) {
			int jmpidx = code - (knh_opline_t*)(op->p[0]);
			if (jmpidx < 0) jmpidx = -jmpidx;
			DBG_ASSERT(jmpidx < (int)isize);
			bb[i]->jumpNC = bb[jmpidx];
			DP(bb[jmpidx])->incoming += 1;
			op->p[0] = NULL;
		}
	}
	DP(ctx->gma)->bbNC = bb[last];
	DBG_ASSERT(DP(bb[last])->incoming > 0);
}

#define ESP_(sfpidx, args)   SFP_(sfpidx + args + K_CALLDELTA + 1)

static knh_r_t RTNIDX_(CTX ctx, int sfpidx, knh_type_t rtype)
{
	if(rtype == TYPE_void) return -1;
	if(IS_Tunbox(rtype)) return NC_(sfpidx);
	return OC_(sfpidx);
}

static void ASM_CALL(CTX ctx, knh_type_t reqt, int sfpidx, knh_type_t rtype, knh_Method_t *mtd, int isStatic, size_t args)
{
	int thisidx = sfpidx + K_CALLDELTA;
	DBG_ASSERT(IS_Method(mtd));
	KNH_ASSERT(sfpidx >= DP(ctx->gma)->espidx);
	if(Method_isFinal(mtd) || isStatic) {
		if(Gamma_hasJIT(ctx)) {
			ASM(LOADMTD, SFP_(thisidx), _SETMTD, mtd);
			ASM(CALL, RTNIDX_(ctx, sfpidx, rtype), SFP_(thisidx), ESP_(sfpidx, args));
		}
		else if(Method_isKonohaCode(mtd) || DP(ctx->gma)->mtd == mtd) {
			if(Gamma_isInlineFunction(ctx->gma) && DP(ctx->gma)->mtd != mtd) {
				knh_KonohaCode_t *kcode = DP(mtd)->kcode;
				size_t isize = kcode->codesize / sizeof(knh_opline_t);
				if(isize < K_INLINECODE) {
					NoticeInliningMethod(ctx, mtd);
					if(isize-1 > 0) {
						ASM_INLINE(ctx, sfpidx + K_CALLDELTA, kcode->code + 1, isize - 1);
					}
					return;
				}
			}
			if(args == 0) {
				ASM(VCALL_, RTNIDX_(ctx, sfpidx, rtype), SFP_(thisidx), ESP_(sfpidx, args), mtd);
			}
			else {
				ASM(VCALL, RTNIDX_(ctx, sfpidx, rtype), SFP_(thisidx), ESP_(sfpidx, args), mtd);
			}
		}
		else {
			ASM(SCALL, RTNIDX_(ctx, sfpidx, rtype), SFP_(thisidx), ESP_(sfpidx, args), mtd);
		}
	}
	else {
		ASM(LOADMTD, SFP_(thisidx), _LOOKUPMTD, mtd);
		ASM(CALL, RTNIDX_(ctx, sfpidx, rtype), SFP_(thisidx), ESP_(sfpidx, args));
	}
}

//static knh_Method_t* knh_lookupDynamicMethod(CTX ctx, knh_methodn_t mn)
//{
//	size_t cid, i, size = ctx->share->sizeClassTBL;
//	for(cid = 0; cid < size; cid++) {
//		knh_Array_t *a = ClassTBL(cid)->methods;
//		for(i = 0; i < knh_Array_size(a); i++) {
//			if((a->methods[i])->mn == mn) return a->methods[i];
//		}
//	}
//	return knh_ClassTBL_getMethod(ctx, CLASS_Tvoid, mn);
//}

#ifdef OPCODE_FASTCALL0
#define isNativeCompiled(kcode) (IS_KonohaCode(kcode) && KonohaCode_isNativeCompiled(kcode))
static inline knh_bool_t Method_isFASTCALL0(CTX ctx, knh_Method_t *mtd)
{
	if(Method_isAbstract(mtd) || Method_isKonohaCode(mtd) || isNativeCompiled(DP(mtd)->kcode) || ParamArray_isVARGs(DP(mtd)->mp)) return 0;
	return 1;
}
#endif

static void ASM_CHKIDX(CTX ctx, int aidx, int nidx)
{
#ifdef OPCODE_CHKIDX
	long i;
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	for(i = (long)DP(bb)->size - 1; i >= 0; i--) {
		klr_CHKIDX_t *op = (klr_CHKIDX_t*)(DP(bb)->opbuf + i);
		int opcode = op->opcode;
		if(opcode == OPCODE_CHKIDXC && op->a == aidx && op->n == nidx) {
			return;
		}
		if(OPCODE_SCALL <= opcode && opcode <= OPCODE_VCALL_) break;
	}
	ASM(CHKIDX, aidx, nidx);
#endif
}

static void ASM_CHKIDXC(CTX ctx, int aidx, int n)
{
#ifdef OPCODE_CHKIDX
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	long i;
	for(i = (long)DP(bb)->size - 1; i >= 0; i--) {
		klr_CHKIDXC_t *op = (klr_CHKIDXC_t*)(DP(bb)->opbuf + i);
		int opcode = op->opcode;
		if(opcode == OPCODE_CHKIDXC && op->a == aidx) {
			if(op->n < n) op->n = n;
			return;
		}
		if(OPCODE_SCALL <= opcode && opcode <= OPCODE_VCALL_) break;
	}
	ASM(CHKIDXC, aidx, n);
#endif
}


static void CALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Token_t *tkMTD = tkNN(stmt, 0);
	knh_Method_t *mtd = (tkMTD)->mtd;
	knh_class_t cid = Tn_cid(stmt, 1);
	if(!IS_Method(mtd)) {
		size_t i;
		for(i = 1; i < DP(stmt)->size; i++) {
			Tn_asm(ctx, stmt, i, TYPE_dyn, local + i + (K_CALLDELTA-1));
		}
		mtd = KNH_TNULL(Method);
		ASM(LOADMTD, SFP_(local+K_CALLDELTA), _CHKMTD, mtd);
		ASM(CALL, SFP_(local+K_CALLDELTA), ESP_(local, DP(stmt)->size - 2));
		ASM_MOVL(ctx, reqt, sfpidx, TYPE_dyn, local);
		return;
	}
	knh_class_t mtd_cid = (mtd)->cid;
	knh_methodn_t mtd_mn = (mtd)->mn;
//	if(cid == CLASS_Func && mtd_mn == MN_invoke) {
//		knh_StmtPARAMs_asm(ctx, stmt, 1, local, cid, mtd);
//		KNH_ASM(LDMETHOD, klr_invokeFunc, (local+K_CALLDELTA), mtd);
//		KNH_ASM(CALL, (local), (knh_ushort_t)DP(stmt)->size + 1);
//		KNH_ASM_MOVL(ctx, reqt, sfpidx, TYPE_dyn, local);
//		return;
//	}
	if(mtd_cid == CLASS_Array) {
		knh_class_t p1 = knh_class_p1(cid);
		if(mtd_mn == MN_get) {
			int a = Tn_put(ctx, stmt, 1, cid, local + 1);
			if(Tn_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)Tn_int(stmt, 2);
				if(n < 0) {
					goto L_USECALL;
				}
				ASM_CHKIDXC(ctx, OC_(a), n);
				if(IS_Tunbox(p1)) {
					ASM(NGETIDXC, NC_(sfpidx), OC_(a), n);
					ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					ASM(OGETIDXC, OC_(sfpidx), OC_(a), n);
					ASM_UNBOX(ctx, reqt, sfpidx);
				}
			}
			else {
				int an = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
				ASM_CHKIDX(ctx, OC_(a), NC_(an));
				if(IS_Tunbox(p1)) {
					ASM(NGETIDX, NC_(sfpidx), OC_(a), NC_(an));
					ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					ASM(OGETIDX, OC_(sfpidx), OC_(a), NC_(an));
					DBG_ASSERT(reqt != CLASS_Object);
					ASM_UNBOX(ctx, reqt, sfpidx);
				}
			}
			return;
		}
		if(mtd_mn == MN_set) {
			int a = Tn_put(ctx, stmt, 1, cid, local + 1);
			knh_type_t ptype = knh_Method_ptype(ctx, mtd, cid, 1);
			int v = Tn_put(ctx, stmt, 3, ptype, local + 3);
			if(Tn_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)Tn_int(stmt, 2);
				if(n < 0) {
					goto L_USECALL;
				}
				knh_class_t p1 = knh_class_p1(cid);
				ASM_CHKIDXC(ctx, OC_(a), n);
				if(IS_Tunbox(p1)) {
					ASM(NSETIDXC, NC_(sfpidx), OC_(a), n, NC_(v));
					ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					ASM(OSETIDXC, OC_(sfpidx), OC_(a), n, OC_(v));
					ASM_UNBOX(ctx, reqt, sfpidx);
				}
			}
			else {
				int an = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
				ASM_CHKIDX(ctx, OC_(a), NC_(an));
				if(IS_Tunbox(p1)) {
					ASM(NSETIDX, NC_(sfpidx), OC_(a), NC_(an), NC_(v));
					ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					ASM(OSETIDX, OC_(sfpidx), OC_(a), NC_(an), OC_(v));
					ASM_UNBOX(ctx, reqt, sfpidx);
				}
			}
			return;
		}
	}
#if defined(OPCODE_BGETIDX)
	if(mtd_cid == CLASS_Bytes) {
		if(mtd_mn == MN_get) {
			int a = Tn_put(ctx, stmt, 1, CLASS_Bytes, local + 1);
			if(Tn_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)Tn_int(stmt, 2);
				ASM_CHKIDXC(ctx, OC_(a), n);
				ASM(BGETIDXC, NC_(sfpidx), OC_(a), n);
				ASM_BOX2(ctx, reqt, CLASS_Int, sfpidx);
			}
			else {
				int an = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
				ASM_CHKIDX(ctx, OC_(a), NC_(an));
				ASM(BGETIDX, NC_(sfpidx), OC_(a), NC_(an));
				ASM_BOX2(ctx, reqt, CLASS_Int, sfpidx);
			}
			return;
		}
		if(mtd_mn == MN_set) {
			int a = Tn_put(ctx, stmt, 1, CLASS_Bytes, local + 1);
			int v = Tn_put(ctx, stmt, 3, CLASS_Int, local + 3);
			if(Tn_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)Tn_int(stmt, 2);
				if(n < 0) {
					goto L_USECALL;
				}
				ASM_CHKIDXC(ctx, OC_(a), n);
				ASM(BSETIDXC, NC_(sfpidx), OC_(a), n, NC_(v));
				ASM_BOX2(ctx, reqt, CLASS_Int, sfpidx);
			}
			else {
				int an = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
				ASM_CHKIDX(ctx, OC_(a), NC_(an));
				ASM(BSETIDX, NC_(sfpidx), OC_(a), NC_(an), NC_(v));
				ASM_BOX2(ctx, reqt, CLASS_Int, sfpidx);
			}
			return;
		}
	}
#endif
#ifdef OPCODE_bNUL
	if(mtd_cid == CLASS_Object) {
		if(mtd_mn == MN_isNull) {
			int a = Tn_put(ctx, stmt, 1, cid, local + 1);
			ASM_BOX2(ctx, CLASS_Object, cid, a); /* thanks ide */
			ASM(bNUL, NC_(sfpidx), OC_(a));
			return;
		}
		else if(mtd_mn == MN_isNotNull) {
			int a = Tn_put(ctx, stmt, 1, cid, local + 1);
			ASM_BOX2(ctx, CLASS_Object, cid, a);
			ASM(bNN, NC_(sfpidx), OC_(a));
			return;
		}
	}
#endif
	{
		knh_index_t deltaidx = knh_Method_indexOfGetterField(mtd);
		if(deltaidx != -1) {
			int b = Tn_put(ctx, stmt, 1, mtd_cid, local + 1);
			knh_type_t type = knh_ParamArray_rtype(DP(mtd)->mp);
			knh_sfx_t bx = {OC_(b), deltaidx};
			ASM_SMOVx(ctx, reqt, sfpidx, type, bx);
			return;
		}
		deltaidx = knh_Method_indexOfSetterField(mtd);
		if(deltaidx != -1) {
			int b = Tn_put(ctx, stmt, 1, mtd_cid, local + 1);
			knh_type_t reqt2 = knh_Method_ptype(ctx, mtd, cid, 0);
			knh_Token_t *tkV = Tn_putTK(ctx, stmt, 2, reqt, local + 2);
			ASM_XMOV(ctx, reqt2, b, deltaidx, tkV);
			if(reqt != TYPE_void) {
				ASM_SMOV(ctx, reqt, sfpidx, tkV);
			}
			return;
		}
	}
	L_USECALL:;
	{
#ifdef OPCODE_FASTCALL0
		knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
		if(DP(stmt)->size == 2 && Method_isFASTCALL0(ctx, mtd)) {
			int a = local;
			if(!Method_isStatic(mtd)) {
				a = Tn_put(ctx, stmt, 1, TYPE_cid(mtd_cid), local + 1);
			}
			//KNH_P("FASTCALL %s.%s", CLASS__((mtd)->cid), MN__((mtd)->mn));
			ASM(FASTCALL0, RTNIDX_(ctx, sfpidx, rtype), SFP_(a), RIX_(sfpidx - a), SP(mtd)->fcall_1, mtd);
			return;
		}
		if(DP(stmt)->size == 3 && Method_isStatic(mtd) && Method_isFASTCALL0(ctx, mtd)) {
			knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, 0);
			knh_type_t ptype = knh_type_tocid(ctx, p->type, mtd_cid);
			int a = Tn_put(ctx, stmt, 2, ptype, local + 2);
			//KNH_P("STATIC FASTCALL %s.%s", CLASS__((mtd)->cid), MN__((mtd)->mn));
			ASM(FASTCALL0, RTNIDX_(ctx, sfpidx, rtype), SFP_(a - 1), RIX_(sfpidx - (a - 1)), SP(mtd)->fcall_1, mtd);
			return;
		}
#endif
		if(CALLPARAMs_asm(ctx, stmt, 1, local, cid, mtd)) {
			knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
			ASM_CALL(ctx, reqt, local, rtype, mtd, Method_isStatic(mtd), DP(stmt)->size - 2);
			ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
		}
	}
}

static void FUNCCALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
	knh_class_t cid = Tn_cid(stmt, 1);
	knh_ParamArray_t *pa = ClassTBL(cid)->cparam;
	size_t i;
	for(i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		knh_type_t reqt = knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid);
		Tn_asm(ctx, stmt, i+2, reqt, local + i + (K_CALLDELTA+1));
	}
	Tn_asm(ctx, stmt, 1, cid, local + K_CALLDELTA);
	if(Stmt_isDYNCALL(stmt)) {
		int a = local + K_CALLDELTA;
		ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _CHKTYPE);
	}
	knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
	ASM_CALL(ctx, reqt, local, rtype, mtd, Method_isStatic(mtd), DP(stmt)->size - 2);
	ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
}

static void EXPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static void CALL1_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	if(IS_Stmt(DP(stmt)->stmtPOST)) {  /* a++ */
		int local = ASML(sfpidx);
		Tn_asm(ctx, stmt, 0, reqt, local);
		EXPR_asm(ctx, DP(stmt)->stmtPOST, reqt, local+1);
		ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
	else {
		Tn_asm(ctx, stmt, 0, reqt, sfpidx);
	}
}

static void NEW_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx), thisidx = local + K_CALLDELTA;
	knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
	knh_class_t cid = (tkNN(stmt, 1))->cid;
	if(DP(stmt)->size == 2 && (mtd)->cid == CLASS_Object && (mtd)->mn == MN_new) {
		ASM(TR, OC_(sfpidx), SFP_(thisidx), RIX_(sfpidx-thisidx), ClassTBL(cid), TR_NEW);
	}
	else {
		ASM(TR, OC_(thisidx), SFP_(thisidx), RIX_(thisidx-thisidx), ClassTBL(cid), TR_NEW);
		CALLPARAMs_asm(ctx, stmt, 2, local, cid, mtd);
		ASM_CALL(ctx, reqt, local, cid, mtd, 1, DP(stmt)->size - 2);
		ASM_MOVL(ctx, cid, sfpidx, SP(stmt)->type, local);
	}
}

static void TCAST_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_type_t srct = Tn_type(stmt, 1);
	if(srct != reqt) {
		int local = ASML(sfpidx);
		knh_Token_t *tkC = tkNN(stmt, 0);
		knh_TypeMap_t *trl = (tkC)->mpr;
		Tn_asm(ctx, stmt, 1, srct, local);
		if(IS_TypeMap(trl)) {
			knh_class_t scid = SP(trl)->scid, tcid = SP(trl)->tcid;
			if(1/*TypeMap_isFinal(trl)*/) {
				if(scid == CLASS_Int && tcid == CLASS_Float) {
					ASM(fCAST, NC_(local), NC_(local));
				}
				else if(scid == CLASS_Float && tcid == CLASS_Int) {
					ASM(iCAST, NC_(local), NC_(local));
				}
				else {
					ASM_BOX2(ctx, TYPE_Object, srct, local);
					ASM(SCAST, RTNIDX_(ctx, local, stmt->type), SFP_(local), RIX_(local-local), trl);
				}
			}
			else {
				ASM_BOX2(ctx, TYPE_Object, srct, local);
				if(IS_Tunbox(srct) && IS_Tnumbox(reqt)) {

				}
				else {
					ASM(TCAST, RTNIDX_(ctx, local, stmt->type), SFP_(local), RIX_(local-local), trl);
				}
			}
		}
		else {
			if(Stmt_isDOWNCAST(stmt)) {
				ASM(TR, OC_(local), SFP_(local), RIX_(local-local), ClassTBL((tkC)->cid), _CHKTYPE);
			}
			else {
				DBG_ABORT("TODO: ACAST?");
				//KNH_ASM(ACAST, local, KNH_NULVAL(CLASS_TypeMap));
			}
		}
		ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
}

static void ALT_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
//	knh_BasicBlock_t* label = new_BasicBlockLABEL(ctx);
//	int local = ASML(sfpidx);
//	int i, size = DP(stmt)->size;
//	knh_type_t reqc = CLASS_t(reqt);
//	for(i = 0; i < size - 1; i++) {
//		Tn_asm(ctx, stmt, i, reqc, local);
//		Tn_ASM_JMPNN(ctx, stmt, i, label);
//	}
//	KNH_ASM_LABEL(ctx, label);
//	KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	DBG_ABORT("TODO: ALT");
}

static void OR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	int i, size = DP(stmt)->size;
	knh_BasicBlock_t*  lbTRUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFALSE = new_BasicBlockLABEL(ctx);
	for(i = 0; i < size; i++) {
		Tn_JMPIF(ctx, stmt, i, 1/*TRUE*/, lbTRUE, local);
	}
	ASM(NSET, NC_(sfpidx), O_data(KNH_FALSE));
	ASM_JMP(ctx, lbFALSE);
	ASM_LABEL(ctx, lbTRUE);
	ASM(NSET, NC_(sfpidx), O_data(KNH_TRUE));
	ASM_LABEL(ctx, lbFALSE); // false
	//KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void AND_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	int i, size = DP(stmt)->size;
	knh_BasicBlock_t*  lbTRUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFALSE = new_BasicBlockLABEL(ctx);
	for(i = 0; i < size; i++) {
		Tn_JMPIF(ctx, stmt, i, 0/*FALSE*/, lbFALSE, local);
	}
	ASM(NSET, NC_(sfpidx), O_data(KNH_TRUE));
	ASM_JMP(ctx, lbTRUE);
	ASM_LABEL(ctx, lbFALSE); // false
	ASM(NSET, NC_(sfpidx), O_data(KNH_FALSE));
	ASM_LABEL(ctx, lbTRUE);   // TRUE
	//KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void TRI_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_BasicBlock_t*  lbelse = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbend  = new_BasicBlockLABEL(ctx);
	//Tn_asm(ctx, stmt, 0, TYPE_Boolean, local);
	Tn_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbelse, local);
	Tn_asm(ctx, stmt, 1, reqt, local);
	ASM_JMP(ctx, lbend);
	/* else */
	ASM_LABEL(ctx, lbelse);
	Tn_asm(ctx, stmt, 2, reqt, local);
	ASM_LABEL(ctx, lbend);
	ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
}

static void LET_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tkL = tkNN(stmt, 1);
	if(TT_(tkL) == TT_LOCAL || TT_(tkL) == TT_FUNCVAR) {
		int index = Token_index(tkL);
		Tn_asm(ctx, stmt, 2, SP(tkL)->type, index);
	}
	else if(IS_Token(tkNN(stmt, 2))) {
		ASM_MOV(ctx, tkL, tkNN(stmt, 2));
	}
	else {
		int local = ASML(sfpidx);
		knh_term_t tt = TT_(tkL); /* NOTE: tkL is reused inside stmt */
		knh_short_t index = (tkL)->index;
		Tn_asm(ctx, stmt, 2, SP(tkL)->type, local);
		TT_(tkL) = tt;
		(tkL)->index = index;
		ASM_MOV(ctx, tkL, tkNN(stmt, 2));
		if(local == sfpidx) return;  // not necessary
	}
	if(stmt->type != TYPE_void && reqt != TYPE_void) {
		ASM_SMOV(ctx, reqt, sfpidx, tkL);
	}
}

static METHOD Fmethod_empty(CTX ctx, knh_sfp_t *sfp _RIX) {}

static knh_Method_t* Gamma_getFmt(CTX ctx, knh_class_t cid, knh_methodn_t mn0)
{
	knh_methodn_t mn = mn0;
	knh_NameSpace_t *ns = K_GMANS;
	knh_Method_t *mtd = knh_NameSpace_getFmtNULL(ctx, ns, cid, mn);
	if(mtd == NULL) {
		WarningUndefinedFmt(ctx, cid, mn0);
		mtd = new_Method(ctx, 0, cid, mn0, Fmethod_empty);
		KNH_SETv(ctx, DP(mtd)->mp, KNH_TNULL(ParamArray));
		knh_NameSpace_addFmt(ctx, ns, mtd);
	}
	return mtd;
}

static void W1_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int isCWB = 0;
	int local = ASML(sfpidx);
	size_t thisidx = local + K_CALLDELTA;
	knh_Method_t *mtdf  = tkNN(stmt, 0)->mtd;
	DBG_ASSERT(DP(stmt)->size == 3);
	if(TT_(tmNN(stmt, 1)) == TT_ASIS) {
		isCWB = 1;
		ASM(TR, OC_(thisidx), SFP_(thisidx), RIX_(thisidx-thisidx), ClassTBL(CLASS_OutputStream), _CWB);
		KNH_SETv(ctx, tmNN(stmt, 1), knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_FUNCVAR, TYPE_OutputStream, thisidx));
	}
	else {
		DBG_ASSERT(TT_(tkNN(stmt, 1)) == TT_FUNCVAR);
		DBG_ASSERT(Tn_type(stmt, 1) == TYPE_OutputStream);
		thisidx = tkNN(stmt, 1)->index;
	}
	if(IS_Method(mtdf)) {
		knh_type_t reqt = Tn_type(stmt, 2);
		Tn_asm(ctx, stmt, 2, reqt, thisidx + 1);
//		if(Method_isFinal(mtdf)) {
			ASM(SCALL, -1, SFP_(thisidx), ESP_((thisidx-K_CALLDELTA), 1), mtdf);
//		}
//		else {
//			KNH_TODO("non final formatter");
//		}
	}
	if(isCWB) {
		ASM(TR, OC_(sfpidx), SFP_(thisidx), RIX_(sfpidx-thisidx), ClassTBL(CLASS_String), _TOSTR);
	}
}

static void FMTCALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int isCWB = 0;
	int local = ASML(sfpidx);
	size_t i, thisidx = local + (DP(stmt)->size - 2) + K_CALLDELTA;
	for(i = 2; i < DP(stmt)->size; i++) {
		knh_type_t reqt = Tn_type(stmt, i);
		Tn_asm(ctx, stmt, i, reqt, local + i - 2);
	}
	if(TT_(tmNN(stmt, 1)) == TT_ASIS) {
		isCWB = 1;
		ASM(TR, OC_(thisidx), SFP_(thisidx), RIX_(thisidx-thisidx), ClassTBL(CLASS_OutputStream), _CWB);
		knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_FUNCVAR, TYPE_OutputStream, thisidx);
	}
	else {
		DBG_ASSERT(TT_(tkNN(stmt, 1)) == TT_FUNCVAR);
		DBG_ASSERT(Tn_type(stmt, 1) == TYPE_OutputStream);
		thisidx = tkNN(stmt, 1)->index;
	}
	{
		knh_Stmt_t *stmtW = stmtNN(stmt, 0);
		while(stmtW != NULL) {
			if(STT_(stmtW) == STT_W1) {
				DBG_ASSERT(tkNN(stmt, 1) == tkNN(stmtW, 1));
				if(TT_(tkNN(stmtW, 2)) == TT_NUM) {
					knh_Token_t *tkIDX = tkNN(stmtW, 2);
					tkIDX->index += local;
					TT_(tkIDX) = TT_FUNCVAR;
				}
				W1_asm(ctx, stmtW, reqt, thisidx - K_CALLDELTA);
			}
			stmtW = DP(stmtW)->nextNULL;
		}
	}
	if(isCWB) {
		ASM(TR, OC_(sfpidx), SFP_(thisidx), RIX_(sfpidx-thisidx), ClassTBL(CLASS_String), _TOSTR);
	}
}

static void SEND_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	size_t i, thisidx = local + K_CALLDELTA;
	int isCWB = 0;
	if(TT_(tmNN(stmt, 1)) == TT_ASIS) {
		isCWB = 1;
		ASM(TR, OC_(thisidx), SFP_(thisidx), RIX_(thisidx-thisidx), ClassTBL(CLASS_OutputStream), _CWB);
		KNH_SETv(ctx, tmNN(stmt, 1), knh_Token_toTYPED(ctx, tkNN(stmt, 1), TT_FUNCVAR, TYPE_OutputStream, thisidx));
	}
	else {
		Tn_asm(ctx, stmt, 1, TYPE_OutputStream, thisidx);
	}
	for(i = 2; i < DP(stmt)->size; i++) {
		if(STT_(stmtNN(stmt, i)) == STT_W1) {
			knh_Stmt_t *stmtIN = stmtNN(stmt, i);
			DBG_ASSERT(TT_(tkNN(stmtIN, 1)) == TT_ASIS);
			KNH_SETv(ctx, tkNN(stmtIN, 1), tkNN(stmt, 1));
			W1_asm(ctx, stmtIN, TYPE_void, local + 1);
		}
		else {
			knh_Method_t *mtd = NULL;
			knh_class_t cid = Tn_cid(stmt, i);
			Tn_asm(ctx, stmt, i, cid/* not TYPE_Object*/, thisidx + 1);
			if(cid == CLASS_String) {
				mtd = knh_NameSpace_getMethodNULL(ctx, CLASS_OutputStream, MN_send);
				DBG_ASSERT(mtd != NULL);
			}
			else {
				mtd = Gamma_getFmt(ctx, cid, MN__s);
			}
			ASM(SCALL, -1, SFP_(thisidx), ESP_((thisidx-K_CALLDELTA), 1), mtd);
		}
	}
	if(isCWB) {
		ASM(TR, OC_(sfpidx), SFP_(thisidx), RIX_(sfpidx-thisidx), ClassTBL(CLASS_String), _TOSTR);
	}
}

/* ------------------------------------------------------------------------ */

#define CASE_ASM(XX, ...) case STT_##XX : { \
		XX##_asm(ctx, stmt, ## __VA_ARGS__); \
		break;\
	}\

static void EXPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	switch(STT_(stmt)) {
	CASE_ASM(LET, reqt, sfpidx);
	CASE_ASM(FUNCCALL, reqt, sfpidx);
	CASE_ASM(CALL, reqt, sfpidx);
	CASE_ASM(CALL1, reqt, sfpidx);
	CASE_ASM(OPR, reqt, sfpidx);
	CASE_ASM(NEW, reqt, sfpidx);
	CASE_ASM(TCAST, reqt, sfpidx);
	CASE_ASM(AND, reqt, sfpidx);
	CASE_ASM(OR, reqt, sfpidx);
	CASE_ASM(ALT, reqt, sfpidx);
	CASE_ASM(TRI, reqt, sfpidx);
	CASE_ASM(SEND, reqt, sfpidx);
	CASE_ASM(W1, reqt, sfpidx);
	CASE_ASM(FMTCALL, reqt, sfpidx);
	default:
		DBG_ABORT("unknown stt=%s", TT__(STT_(stmt)));
	}
	ASM_BOX2(ctx, reqt, SP(stmt)->type, sfpidx);
}

static void Tn_asm(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local)
{
	if(IS_Token(tkNN(stmt, n))) {
		knh_Token_t *tk = tkNN(stmt, n);
		ASM_SMOV(ctx, reqt, local, tk);
	}
	else {
		EXPR_asm(ctx, stmtNN(stmt, n), reqt, local);
	}
	if(IS_Token(tkNN(stmt, n))) {
		knh_Token_t *tk = tkNN(stmt, n);
		//DBG_P("@START %s tk=%p tk->index=%d local=%d esp=%d", TT__(tk->tt), tk, tk->index, local, DP(ctx->gma)->espidx);
		if(TT_(tk) == TT_LOCAL) {
			knh_Token_toTYPED(ctx, tk, TT_FUNCVAR, tk->type, tk->index + DP(ctx->gma)->ebpidx);
		}
		if(TT_(tk) != TT_FUNCVAR) {
			knh_Token_toTYPED(ctx, tk, TT_FUNCVAR, reqt, local);
		}
		//DBG_P("@END %s tk=%p tk->index=%d local=%d esp=%d", TT__(tk->tt), tk, tk->index, local, DP(ctx->gma)->espidx);
	}
	else {
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_FUNCVAR, reqt, local);
		KNH_SETv(ctx, tkNN(stmt, n), tk);
	}
	DBG_ASSERT(IS_Token(tkNN(stmt, n)));
}

/* ------------------------------------------------------------------------ */
/* [LABEL]  */

static void Gamma_pushLABEL(CTX ctx, knh_Stmt_t *stmt, knh_BasicBlock_t *lbC, knh_BasicBlock_t *lbB)
{
	knh_Object_t *tkL = NULL;
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		tkL = knh_DictMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, S_tobytes(TS_ATlabel));
	}
	if(tkL == NULL) {
		tkL = KNH_NULVAL(CLASS_Tdynamic);
	}
	knh_Array_add(ctx, DP(ctx->gma)->lstacks, tkL);
	knh_Array_add(ctx, DP(ctx->gma)->lstacks, lbC);
	knh_Array_add(ctx, DP(ctx->gma)->lstacks, lbB);
	knh_Array_add(ctx, DP(ctx->gma)->lstacks, KNH_NULL);
}

static void Gamma_popLABEL(CTX ctx)
{
	knh_Array_t *a = DP(ctx->gma)->lstacks;
	DBG_ASSERT(knh_Array_size(a) - 4 >= 0);
	knh_Array_clear(ctx, a, knh_Array_size(a) - 4);
}

/* ------------------------------------------------------------------------ */
/* [IF, WHILE, DO, FOR, FOREACH]  */

static knh_Token_t *Tn_it(knh_Stmt_t *stmt, size_t n)
{
	DBG_ASSERT(n < DP(stmt)->size);
	knh_Token_t *tkIT = tkNN(stmt, n);
	DBG_ASSERT(TT_(tkIT) == TT_LOCAL);
	return tkIT;
}

static inline void Tn_asmBLOCK(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	DBG_ASSERT(IS_Stmt(stmtNN(stmt, n)));
	BLOCK_asm(ctx, stmtNN(stmt, n), reqt);
}

static void IF_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_BasicBlock_t*  lbELSE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbEND  = new_BasicBlockLABEL(ctx);
	/* if */
	lbELSE = Tn_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbELSE, DP(ctx->gma)->espidx);
	/* then */
	Tn_asmBLOCK(ctx, stmt, 1, reqt);
	ASM_JMP(ctx, lbEND);
	/* else */
	ASM_LABEL(ctx, lbELSE);
	Tn_asmBLOCK(ctx, stmt, 2, reqt);
	/* endif */
	ASM_LABEL(ctx, lbEND);
}

static void SWITCH_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Stmt_t *stmtCASE;
	knh_Token_t *tkIT = Tn_it(stmt, 2);
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t *lbNEXT = NULL;
	Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	ASM_LABEL(ctx, lbCONTINUE);
	//switch(it)
	Tn_asm(ctx, stmt, 0, SP(tkIT)->type, Token_index(tkIT));
	stmtCASE = stmtNN(stmt, 1);
	while(stmtCASE != NULL) {
		// case 'a' :
		if(STT_(stmtCASE) == STT_CASE && !Tn_isASIS(stmtCASE, 0)) {
			knh_BasicBlock_t *lbEND = new_BasicBlockLABEL(ctx);
			DP(ctx->gma)->espidx = DP(stmtCASE)->espidx + DP(ctx->gma)->ebpidx;
			Tn_JMPIF(ctx, stmtCASE, 0, 0/*FALSE*/, lbEND, DP(ctx->gma)->espidx);
			if(lbNEXT != NULL) {
				ASM_LABEL(ctx, lbNEXT); lbNEXT = NULL;
			}
			Tn_asmBLOCK(ctx, stmtCASE, 1, reqt);
			lbNEXT = new_BasicBlockLABEL(ctx);
			ASM_JMP(ctx, lbNEXT);
			ASM_LABEL(ctx, lbEND);
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	if(lbNEXT != NULL) {
		ASM_LABEL(ctx, lbNEXT); lbNEXT = NULL;
	}
	stmtCASE = stmtNN(stmt, 1);
	while(stmtCASE !=NULL) {
		if(STT_(stmtCASE) == STT_CASE && Tn_isASIS(stmtCASE, 0)) {
			Tn_asmBLOCK(ctx, stmtCASE, 1, reqt);
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	ASM_LABEL(ctx, lbBREAK);
	Gamma_popLABEL(ctx);
}

static void ASM_JUMPLABEL(CTX ctx, knh_Stmt_t *stmt, int delta)
{
	size_t s = knh_Array_size(DP(ctx->gma)->lstacks);
	if(s < 4) {
		knh_Stmt_toERR(ctx, stmt, ERROR_OnlyTopLevel(ctx, cSTT_(stmt)));
	}
	else {
		knh_Token_t *tkL = NULL;
		knh_BasicBlock_t *lbBLOCK = NULL;
		if(DP(stmt)->size == 1) {
			tkL = tkNN(stmt, 0);
			if(TT_(tkL) == TT_ASIS) tkL = NULL;
		}
		if(tkL != NULL) {
			int i;
			knh_bytes_t lname = S_tobytes((tkL)->text);
			for(i = s - 4; i >= 0; i -= 4) {
				knh_Token_t *tkSTACK = DP(ctx->gma)->lstacks->tokens[i];
				if(IS_NOTNULL(tkSTACK) && S_equals((tkSTACK)->text, lname)) {
					lbBLOCK = GammaLabel(ctx,  i + delta);
					goto L_JUMP;
				}
			}
			ErrorUndefinedLabel(ctx, tkL);
			return;
		}
		lbBLOCK = GammaLabel(ctx,  s - 4 + delta);
		L_JUMP:;
		ASM_JMP(ctx, lbBLOCK);
	}
}

static void CONTINUE_asm(CTX ctx, knh_Stmt_t *stmt)
{
	ASM_JUMPLABEL(ctx, stmt, 1);
}

static void BREAK_asm(CTX ctx, knh_Stmt_t *stmt)
{
	ASM_JUMPLABEL(ctx, stmt, 2);
}

static void WHILE_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	ASM_LABEL(ctx, lbCONTINUE);
	if(!Tn_isTRUE(stmt, 0)) {
		Tn_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
		//ASM_CHECK_INFINITE_LOOP(ctx, stmt);
	}
	Tn_asmBLOCK(ctx, stmt, 1, TYPE_void);
	ASM_JMP(ctx, lbCONTINUE);
	ASM_LABEL(ctx, lbBREAK);
	Gamma_popLABEL(ctx);
}

static void DO_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	ASM_LABEL(ctx, lbCONTINUE);
	//ASM_CHECK_INFINITE_LOOP(ctx, stmt);
	Tn_asmBLOCK(ctx, stmt, 0, TYPE_void);
	Tn_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
	ASM_JMP(ctx, lbCONTINUE);
	ASM_LABEL(ctx, lbBREAK);
	Gamma_popLABEL(ctx);
}

static void FOR_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbREDO = new_BasicBlockLABEL(ctx);
	Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	/* i = 1 part */
	Tn_asmBLOCK(ctx, stmt, 0, TYPE_void);
	ASM_JMP(ctx, lbREDO);
	/* i++ part */
	ASM_LABEL(ctx, lbCONTINUE); /* CONTINUE */
	//ASM_CHECK_INFINITE_LOOP(ctx, stmt);
	Tn_asmBLOCK(ctx, stmt, 2, TYPE_void);
	/* i < 10 part */
	ASM_LABEL(ctx, lbREDO);
	if(!Tn_isTRUE(stmt, 1)) {
		Tn_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
	}
	Tn_asmBLOCK(ctx, stmt, 3, TYPE_void);
	ASM_JMP(ctx, lbCONTINUE);
	ASM_LABEL(ctx, lbBREAK);
	Gamma_popLABEL(ctx);
}

/* ------------------------------------------------------------------------ */

static void FOREACH_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbC = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbB = new_BasicBlockLABEL(ctx);
	Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	{
		knh_Token_t *tkN = tkNN(stmt, 0);
		int varidx = Token_index(tkN);
		int itridx = Token_index(tkNN(stmt, 3));
		Tn_asm(ctx, stmt, 1, TYPE_Iterator, itridx);
		ASM_LABEL(ctx, lbC);
		DBG_P("variable=%d, iterator=%d", varidx, itridx);
		//DBG_ASSERT(varidx == itridx);
		ASMbranch(NEXT, lbB, RTNIDX_(ctx, varidx, (tkN)->type), SFP_(itridx), RIX_(varidx - itridx));
	}
	Tn_asmBLOCK(ctx, stmt, 2, TYPE_void);
	ASM_JMP(ctx, lbC);
	/* end */
	ASM_LABEL(ctx, lbB);
	Gamma_popLABEL(ctx);
}

/* ------------------------------------------------------------------------ */
/* [TRY] */

#define Gamma_inTry(ctx)  IS_Stmt(DP(ctx->gma)->finallyStmt)

static void Gamma_setFINALLY(CTX ctx, knh_Stmt_t *stmt)
{
	if(IS_NOTNULL(stmt)) {
		if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
			ErrorMisplaced(ctx);
			KNH_HINT(ctx, "try"); // not nested try
			return;
		}
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
	else { /* stmt == null */
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
}

static void ASM_FINALLY(CTX ctx)
{
	if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
		BLOCK_asm(ctx, DP(ctx->gma)->finallyStmt, TYPE_void);
	}
}

static void TRY_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t*  lbCATCH   = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFINALLY = new_BasicBlockLABEL(ctx);
	knh_Token_t *tkIT = Tn_it(stmt, 3/*HDR*/);
	Gamma_setFINALLY(ctx, stmtNN(stmt, 2/*finally*/));
	/* try { */
	ASMbranch(TRY, lbCATCH, OC_((tkIT)->index));
	Tn_asmBLOCK(ctx, stmt, 0/*try*/, TYPE_void);
	//KNH_ASM(TRYEND, ((tkIT)->index));
	ASM_JMP(ctx, lbFINALLY);
	Gamma_setFINALLY(ctx, (knh_Stmt_t*)KNH_NULL); // InTry
	/* catch */
	ASM_LABEL(ctx, lbCATCH);
	knh_Stmt_t *stmtCATCH = stmtNN(stmt, 1/*catch*/);
	DBG_ASSERT(IS_Stmt(stmtCATCH));
	while(stmtCATCH != NULL) {
		if(SP(stmtCATCH)->stt == STT_CATCH) {
			knh_String_t *emsg = tkNN(stmtCATCH, 0)->text;
			knh_Token_t *tkN = tkNN(stmtCATCH, 1);
			DBG_ASSERT(IS_String(emsg));
			DBG_ASSERT(TT_(tkN) == TT_LOCAL);
			lbCATCH = new_BasicBlockLABEL(ctx);
			ASMbranch(CATCH, lbCATCH, OC_((tkN)->index), emsg);
			Tn_asmBLOCK(ctx, stmtCATCH, 2, TYPE_void);
			ASM_JMP(ctx, lbFINALLY);  /* GOTO FINALLY */
			ASM_LABEL(ctx, lbCATCH); /* _CATCH_NEXT_ */
		}
		stmtCATCH = DP(stmtCATCH)->nextNULL;
	}
	ASM_LABEL(ctx, lbFINALLY); /* FINALLY */
	Tn_asmBLOCK(ctx, stmt, 2/*finally*/, TYPE_void);
	ASM(THROW, SFP_(((tkIT)->index)-1));
}

static void ASSURE_asm(CTX ctx, knh_Stmt_t *stmt)
{
	int index = Token_index(tkNN(stmt, 2)); // it
	Tn_asm(ctx, stmt, 0, CLASS_Assurance, index);
	ASM(CHKIN, OC_(index), ClassTBL(CLASS_Assurance)->ospi->checkin);
	Tn_asmBLOCK(ctx, stmt, 1, TYPE_void);
	ASM(CHKOUT, OC_(index), ClassTBL(CLASS_Assurance)->ospi->checkout);
}


static void THROW_asm(CTX ctx, knh_Stmt_t *stmt)
{
	int start = 0, espidx = DP(ctx->gma)->espidx;
	if(Gamma_inTry(ctx)) {
		start = espidx;
	}
	Tn_asm(ctx, stmt, 0, TYPE_Exception, espidx);
	ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_Exception), _ERR);
	ASM(THROW, SFP_(start));
}

static void RETURN_asm(CTX ctx, knh_Stmt_t *stmt)
{
	ASM_FINALLY(ctx);
	size_t size = DP(stmt)->size;
	if(size == 1) {
		knh_type_t rtype = Tn_type(stmt, 0);
		Tn_asm(ctx, stmt, 0, rtype, K_RTNIDX);
		if(IS_Tunbox(rtype)) {
			knh_ParamArray_t *pa = DP(DP(ctx->gma)->mtd)->mp;
			knh_param_t *p = knh_ParamArray_rget(pa, 0);
			ASM_BOX2(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, K_RTNIDX);
		}
	}
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		EXPR_asm(ctx, DP(stmt)->stmtPOST, Tn_type(stmt, 0), DP(DP(stmt)->stmtPOST)->espidx+1);
	}
	if(!Stmt_isImplicit(stmt)) {
		ASM_RET(ctx, stmt);
	}
}

static void YIELD_asm(CTX ctx, knh_Stmt_t *stmt)
{
	KNH_TODO("yield");
}


static void ERR_asm(CTX ctx, knh_Stmt_t *stmt)
{
	int start = 0, espidx = DP(ctx->gma)->espidx;
	knh_Token_t *tkERR = tkNN(stmt, 0);
	DBG_ASSERT(DP(stmt)->size > 0);
	DBG_ASSERT(TT_(tkERR) == TT_ERR);
	if(Gamma_inTry(ctx)) start = espidx;
	DBG_ASSERT(IS_String((tkERR)->text));
	ASM(ERROR, SFP_(start), (tkERR)->text);
	if(DP(stmt)->nextNULL != NULL) {
		KNH_FINALv(ctx, DP(stmt)->nextNULL);
		DP(stmt)->nextNULL = NULL;
	}
}

/* ------------------------------------------------------------------------ */
/* [PRINT] */

//static int Gamma_isDEBUG(CTX ctx)
//{
//	if(Method_isDebug(DP(ctx->gma)->mtd)) return 1;
//	return CTX_isDebug(ctx);
//}

//static void ASM_SKIP(CTX ctx, knh_BasicBlock_t* lbskip)
//{
//	if(!Method_isDebug(DP(ctx->gma)->mtd)) {
//		ASMbranch(DYJMP, lbskip, SFP_(0), _ISSKIP);
//	}
//}

static knh_flag_t PRINT_flag(CTX ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		Object *v = knh_DictMap_getNULL(ctx,  DP(o)->metaDictCaseMap, STEXT("Time"));
		if(v != NULL) {
			flag |= K_FLAG_PF_TIME;
		}
	}
	return flag;
}

static void _PRINTh(CTX ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, struct klr_P_t *op)
{
	knh_flag_t flag = (knh_flag_t)op->flag;
	knh_write_ascii(ctx, w, TERM_BNOTE(ctx, LOG_NOTICE));
	if(FLAG_is(flag, K_FLAG_PF_BOL)) {
		if(FLAG_is(flag, K_FLAG_PF_LINE)) {
			knh_Method_t *mtd = sfp[-1].mtdNC;
			knh_uline_t uline = op->line;
			DBG_ASSERT(IS_Method(mtd));
			ULINE_setURI(uline, DP(mtd)->uri);
			knh_write_uline(ctx, w, uline);
		}
	}
	if(IS_bString(op->msg)) {
		if((op->msg)->str.len > 0) {
			knh_write_utf8(ctx, w, S_tobytes(op->msg), !String_isASCII(op->msg));
		}
	}
	if(FLAG_is(flag, K_FLAG_PF_NAME)) {
		knh_putc(ctx, w, '=');
	}
}

static void _PRINTln(CTX ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, struct klr_P_t *op)
{
	knh_flag_t flag = (knh_flag_t)op->flag;
	if(FLAG_is(flag, K_FLAG_PF_EOL)) {
		knh_write_ascii(ctx, w, TERM_ENOTE(ctx, LOG_NOTICE));
		knh_write_EOL(ctx, w);
	}
	else {
		knh_putc(ctx, w, ',');
		knh_putc(ctx, w, ' ');
	}
}

static void _PRINT(CTX ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	_PRINTh(ctx, sfp, w, op);
	knh_write_Object(ctx, w, sfp[op->n].o, FMT_data);
	_PRINTln(ctx, sfp, w, op);
}

static void _PRINTm(CTX ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	_PRINTh(ctx, sfp, w, op);
	if(FLAG_is(((knh_flag_t)op->flag), K_FLAG_PF_EOL)) {
		knh_write_ascii(ctx, w, TERM_ENOTE(ctx, LOG_NOTICE));
		knh_write_EOL(ctx, w);
	}
}

static void _PRINTi(CTX ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	_PRINTh(ctx, sfp, w, op);
	knh_write_ifmt(ctx, w, K_INT_FMT, sfp[op->n].ivalue);
	_PRINTln(ctx, sfp, w, op);
}

static void _PRINTf(CTX ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	_PRINTh(ctx, sfp, w, op);
	knh_write_ffmt(ctx, w, K_FLOAT_FMT, sfp[op->n].fvalue);
	_PRINTln(ctx, sfp, w, op);
}

static void _PRINTb(CTX ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	_PRINTh(ctx, sfp, w, op);
	knh_write_bool(ctx, w, sfp[op->n].bvalue);
	_PRINTln(ctx, sfp, w, op);
}

static void PRINT_asm(CTX ctx, knh_Stmt_t *stmt)
{
	knh_flag_t flag = PRINT_flag(ctx, stmt) | K_FLAG_PF_BOL | K_FLAG_PF_LINE;
	long i, espidx = DP(ctx->gma)->espidx;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Token_t *tkn = tkNN(stmt, i);
		if(TT_(tkn) != TT_CONST || !IS_String((tkn)->data)) {
			knh_class_t cid = Tn_cid(stmt, i);
			Tn_asm(ctx, stmt, i, cid, espidx + i);
		}
	}
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_flag_t mask = 0;
		knh_String_t *msg = (knh_String_t*)KNH_NULL;
		L_REDO:;
		knh_Token_t *tkn = tkNN(stmt, i);
		if(i == (long)DP(stmt)->size - 1) {
			mask |= K_FLAG_PF_EOL;
		}
		if(TT_(tkn) == TT_CONST && IS_String((tkn)->data)) {
			if(Token_isPNAME(tkn)) { /* name= */
				msg = (tkn)->text;
				mask |= K_FLAG_PF_NAME; i++;
				goto L_REDO;
			}
			DBG_ASSERT(stmt->uline == ctx->gma->uline);
			ASM(P, _PRINTm, flag | mask, (tkn)->text, 0); flag = 0;
		}
		else {
			knh_class_t cid = Tn_cid(stmt, i);
//			Tn_asm(ctx, stmt, i, cid, espidx);
			if(IS_Tint(cid)) {
				ASM(P, _PRINTi, flag | mask, msg, espidx+i);
			}
			else if(IS_Tfloat(cid)) {
				ASM(P, _PRINTf, flag | mask, msg, espidx+i);
			}
			else if(cid == CLASS_Boolean) {
				ASM(P, _PRINTb, flag | mask, msg, espidx+i);
			}
			else {
				ASM(P, _PRINT, flag | mask, msg, espidx+i);
			}
			flag=0;
		}
	}
}

static void ASSERT_asm(CTX ctx, knh_Stmt_t *stmt)
{
	int espidx = DP(ctx->gma)->espidx;
	knh_BasicBlock_t* lbskip = new_BasicBlockLABEL(ctx);
//	if(!isRelease) {
//		ASMbranch(DYJMP, lbskip, SFP_(0), _ISSKIP);
//	}
	/* if */
	Tn_JMPIF(ctx, stmt, 0, 1, lbskip, DP(ctx->gma)->espidx);
	/*then*/
	ASM(ASSERT, SFP_(espidx), stmt->uline);
	ASM_LABEL(ctx, lbskip);
}

/* ------------------------------------------------------------------------ */

static void BLOCK_asm(CTX ctx, knh_Stmt_t *stmtH, knh_type_t reqt)
{
	knh_Stmt_t *stmt = stmtH;
	while(stmt != NULL) {
		knh_type_t etype = (DP(stmt)->nextNULL == NULL) ? reqt : TYPE_void;
		ctx->gma->uline = stmt->uline;
		DP(ctx->gma)->espidx = DP(stmt)->espidx + DP(ctx->gma)->ebpidx;
		//DBG_P("ASM %s %p etype=%s,%s, espidx=%d", TT__(STT_(stmt)), stmt, TYPE__(etype), TYPE__(stmt->type), DP(ctx->gma)->espidx);
		switch(STT_(stmt)) {
		case STT_REGISTER:
		{
			size_t i;
			for(i = 0; i < DP(stmt)->size; i++) {
				if(IS_Stmt(tmNN(stmt, i)) && STT_(stmtNN(stmt, i)) == STT_LET) {
					LET_asm(ctx, stmtNN(stmt, i), TYPE_void, DP(ctx->gma)->espidx);
				}
			}
			break;
		}
		case STT_BLOCK :
		{
			BLOCK_asm(ctx, stmtNN(stmt, 0), etype);
			break;
		}
		CASE_ASM(IF, etype);
		CASE_ASM(SWITCH, etype);
		CASE_ASM(WHILE);
		CASE_ASM(DO);
		CASE_ASM(FOR);
		CASE_ASM(FOREACH);
		CASE_ASM(BREAK);
		CASE_ASM(CONTINUE);
		CASE_ASM(TRY);
		CASE_ASM(THROW);
		CASE_ASM(RETURN);
		CASE_ASM(YIELD);
		CASE_ASM(PRINT);
		CASE_ASM(ASSURE);
		CASE_ASM(ASSERT);
		CASE_ASM(ERR);
		case STT_DECL: case STT_DONE: break;
		default:
			EXPR_asm(ctx, stmt, etype, DP(ctx->gma)->espidx);
		}
		stmt = DP(stmt)->nextNULL;
	}
}

/* ------------------------------------------------------------------------ */

static void _THCODE(CTX ctx, knh_opline_t *pc, void **codeaddr)
{
#ifdef K_USING_THCODE_
	while(1) {
		DBG_ASSERT_OPCODE(pc->opcode);
		pc->codeaddr = codeaddr[pc->opcode];
		if(pc->opcode == OPCODE_RET) break;
		pc++;
	}
#endif
}

void knh_Method_asm(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t ittype, knh_Stmt_t *stmtB, knh_Ftyping typing)
{
	typing(ctx, mtd, stmtP, ittype, stmtB);
	DBG_ASSERT(knh_Array_size(DP(ctx->gma)->insts) == 0);
	knh_BasicBlock_t* lbINIT = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBEGIN = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbEND = new_BasicBlockLABEL(ctx);
	SP(ctx->gma)->uline = SP(stmtB)->uline;
	DP(ctx->gma)->bbNC = lbINIT;
	Gamma_pushLABEL(ctx, stmtB, lbBEGIN, lbEND);
	ASM(THCODE, _THCODE, ULINE_uri(SP(stmtB)->uline));
	if(Method_isStatic(mtd) && Gamma_hasFIELD(ctx->gma)) {
		ASM(TR, OC_(0), SFP_(0), RIX_(0), ClassTBL(DP(ctx->gma)->this_cid), _NULVAL);
	}
	ASM_LABEL(ctx, lbBEGIN);
	{
		long i;
		knh_gmafields_t *gf = DP(ctx->gma)->gf;
		for(i = 1; i < (long)DP(ctx->gma)->psize + 1; i++) {
			if(IS_NOTNULL(gf[i].value)) {
//					knh_BasicBlock_t*  lb = new_BasicBlockLABEL(ctx);
//					KNH_ASM(JMPNN, TADDRx lb, i);
//					KNH_ASM(OSET, (i), value);
//					KNH_ASM_UNBOX(ctx, O_cid(value), i);
//					KNH_ASM_LABEL(ctx, lb);
			}
		}
		for(; i < DP(ctx->gma)->ebpidx; i++) {
			knh_type_t type = gf[i].type;
			if(gf[i].fn == FN_vargs) {
				knh_class_t cid = knh_class_p1(CLASS_t(type));
				DBG_ASSERT_cid(cid);
				ASM(TR, OC_(i), SFP_(i), RIX_(i-i), ClassTBL(cid), _VARGS);// i++;
				continue;
			}
			if(IS_Tunbox(type)) {
				ASM(NSET, NC_(i), 0);
			}
			else {
				knh_class_t cid = CLASS_t(type);
				knh_Object_t *v = KNH_NULVAL(cid);
				if(v != ClassTBL(cid)->defnull) {
					ASM(TR, OC_(i), SFP_(i), RIX_(i-i), ClassTBL(cid), _NULVAL);
				}
				else {
					ASM(OSET, OC_(i), v);
				}
				if(IS_Tnumbox(type)) {
					ASM(NSET, NC_(i), 0);
				}
			}
		}
	}
	BLOCK_asm(ctx, stmtB, knh_ParamArray_rtype(DP(mtd)->mp));
	ASM_LABEL(ctx, lbEND);
	ASM(RET);
	Gamma_popLABEL(ctx);
	DBG_ASSERT(knh_Array_size(DP(ctx->gma)->lstacks) == 0);
	Gamma_compile(ctx, lbINIT, lbEND);
}

/* ------------------------------------------------------------------------ */
/* [loadSystem] */

static knh_opline_t* opline_findOPCODE(CTX ctx, knh_opline_t *op, knh_opcode_t opcode)
{
	while(1) {
		if(op->opcode == opcode) return op;
		if(op->opcode == OPCODE_RET) break;
		op++;
	}
	KNH_ASSERT(ctx == NULL); // DON'T NOT HAPPEN
	return NULL;
}

static void _STACKTRACE(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t n, knh_opline_t *pc)
{
	if(IS_Exception(ctx->e)) {
		knh_write_Object(ctx, KNH_STDERR, UPCAST(ctx->e), FMT_dump);
	}
}

void knh_loadScriptSystemKonohaCode(CTX ctx)
{
	BEGIN_LOCAL(ctx, lsfp, 4);
	knh_BasicBlock_t* ia = new_(BasicBlock);
	knh_BasicBlock_t* ib = new_(BasicBlock);
	knh_BasicBlock_t* ic = new_(BasicBlock);
	knh_BasicBlock_t* id = new_(BasicBlock);
	KNH_SETv(ctx, lsfp[0].o, ia);
	KNH_SETv(ctx, lsfp[1].o, ib);
	KNH_SETv(ctx, lsfp[2].o, ic);
	KNH_SETv(ctx, lsfp[3].o, id);
	BasicBlock_add(ctx, ia, THCODE, _THCODE, URI_unknown);
	BasicBlock_add(ctx, ia, TRY, NULL/*lb*/, OC_(-(K_CALLDELTA+1)));  // LAUNCH
	ia->nextNC = ib;
	ia->jumpNC = ic;
	BasicBlock_add(ctx, ib, ENTER);
	BasicBlock_add(ctx, ib, TRYEND, OC_(-(K_CALLDELTA+1)));
	BasicBlock_add(ctx, ib, EXIT);
	ib->nextNC = ic;
	BasicBlock_add(ctx, ic, PROBE, _STACKTRACE, 0);
	BasicBlock_add(ctx, ic, EXIT);
	BasicBlock_add(ctx, ic, FUNCCALL);               // FUNCCALL
	BasicBlock_add(ctx, ic, VEXEC);                  // VEXEC
	BasicBlock_add(ctx, ic, EXIT);
	ic->nextNC = id;
	BasicBlock_add(ctx, id, RET);  // NEED TERMINATION
	{
		knh_KonohaCode_t *kcode = BasicBlock_link(ctx, ia, id);
		knh_opline_t *pc = knh_VirtualMachine_run(ctx, ctx->esp, SP(kcode)->code);
		knh_setClassDefaultValue(ctx, CLASS_KonohaCode, kcode, NULL);
		((knh_share_t*)ctx->share)->PC_LAUNCH = opline_findOPCODE(ctx, pc, OPCODE_TRY);
		((knh_share_t*)ctx->share)->PC_FUNCCALL = opline_findOPCODE(ctx, pc, OPCODE_FUNCCALL);
		((knh_share_t*)ctx->share)->PC_VEXEC = opline_findOPCODE(ctx, pc, OPCODE_VEXEC);
	}
	END_LOCAL_NONGC(ctx, lsfp);
}

/* ------------------------------------------------------------------------ */

typedef struct knh_funcname_t {
	void *func;
	char *name;
} knh_funcname_t;

#define _FUNC(func, name) { (void*) func, (char *) name }

static struct knh_funcname_t _FuncData[] = {
	_FUNC(_PRINT, "PRINT"), _FUNC(_BOX, "BOX"), _FUNC(TR_NEW, "NEW"),
	_FUNC(_NULVAL, "NULL"), _FUNC(_CWB, "CWB"), _FUNC(_TOSTR, "TOSTR"),
	_FUNC(_SETMTD, "setMethod"), _FUNC(_LOOKUPMTD, "lookupMethod"),
	_FUNC(_PROP, "PROP"), _FUNC(_bBOX, "bBOX"), _FUNC(_OBOX, "OBOX"), _FUNC(_VARGS, "VARGS"),
	_FUNC(_ERR, "ERR"), _FUNC(_CHKTYPE, "CHKTYPE"),
	_FUNC(_CHKMTD, "checkParams"),
	_FUNC(NULL, NULL),
};

void knh_write_vmfunc(CTX ctx, knh_OutputStream_t *w, void *f)
{
	knh_funcname_t *d = _FuncData;
	while(d->func != NULL) {
		if(d->func == f) {
			knh_write(ctx, w, B(d->name));
			return;
		}
		d++;
	}
	knh_printf(ctx, w, "func:%p", f);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
