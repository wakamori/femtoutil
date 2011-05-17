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
#include"../../include/konoha1/konoha_code_.h"

#if defined(K_USING_LINUX_) || defined(K_USING_BSD_)
#include <sys/mman.h> // for PROT_READ,PROT_WRITE,PROT_EXEC
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

KNHAPI2(void) knh_invoke(CTX ctx, knh_Func_t *fo, knh_sfp_t *sfp /*rtnidx*/, int argc)
{
	if(fo->baseNULL != NULL) {
		KNH_SETv(ctx, sfp[K_CALLDELTA].o, (fo)->baseNULL);
	}
	KNH_SCALL(ctx, sfp, 0, (fo)->mtd, argc);
}

/* ------------------------------------------------------------------------ */

//void knh_stack_newThunk(CTX ctx, knh_sfp_t *sfp/*thisidx*/)
//{
//	knh_sfp_t *lsfp = sfp + K_RTNIDX;
//	size_t ctx->esp - lsfp;
//	knh_Method_t *mtd = sfp[K_MTDIDX].mtdNC;
//	knh_type_t rtype = knh_ParamArray_rtype(DP(mtd)->mp);
//	knh_class_t p1 = knh_type_tocid(ctx, rtype, O_cid(sfp[0].o));
//	knh_Thunk_t *thunk = new_(Thunk);
//}

///* ------------------------------------------------------------------------ */
//
//static int knh_stack_vpush(CTX ctx, knh_sfp_t *sfp, int thisidx, const char *fmt, va_list args)
//{
//	char *p = (char*)fmt;
//	int ch, n = thisidx + 1;
//	while((ch = *p) != '\0') {
//		p++;
//		switch(ch) {
//		case '%':
//			break;
//		case 'U': case 'D':
//			KNH_SETv(ctx, sfp[n].o, KNH_INT0); // NONNULL
//		case 'u': case 'd':
//			sfp[n].ivalue = (knh_int_t)va_arg(args, knh_intptr_t);
//			n++;
//			break;
//
//		case 'I': case 'L':
//			KNH_SETv(ctx, sfp[n].o, KNH_INT0); // NONNULL
//		case 'i': case 'l':
//			sfp[n].ivalue = (knh_int_t)va_arg(args, knh_int_t);
//			n++;
//			break;
//
//		case 'F':
//			KNH_SETv(ctx, sfp[n].o, KNH_FLOAT0);  // NONNULL
//		case 'f':
//			sfp[n].fvalue = (knh_float_t)va_arg(args, knh_float_t);
//			n++;
//			break;
//
//		case 'B': case 'b':
//			sfp[n].bvalue = (knh_intptr_t)va_arg(args, knh_intptr_t);
//			KNH_SETv(ctx, sfp[n].o, new_Boolean(ctx, sfp[n].bvalue));
//			break;
//
//		case 's':
//		{
//			char *s = (char*)va_arg(args, char*);
//			Object *o = NULL;
//			if(s == NULL) {
//				o = KNH_NULL;
//			}
//			else {
//				o = (Object*)new_S(ctx, B(s));
//			}
//			KNH_SETv(ctx, sfp[n].o, o);
//			n++;
//		}
//		break;
//
//		case 'o':
//		{
//			Object *o = (Object*)va_arg(args, knh_Object_t*);
//			KNH_SETv(ctx, sfp[n].o, o);
//			sfp[n].ndata = knh_Object_data(o);
//			n++;
//		}
//		break;
//		}/*switch*/
//	} /* while */
//	return n - (thisidx+1);
//}
//
///* ------------------------------------------------------------------------ */
//
//KNHAPI2(void) knh_putsfp(CTX ctx, knh_sfp_t *lsfp, int n, Object *obj)
//{
//	KNH_SETv(ctx, lsfp[n].o, obj);
//	lsfp[n].ndata = knh_Object_data(obj);
//}
//
///* ------------------------------------------------------------------------ */
//
//KNHAPI2(void) knh_Func_invokesfp(CTX ctx, knh_Func_t *fo, knh_sfp_t *lsfp, long rtnidx, long argc)
//{
//	if(fo->baseNULL != NULL) {
//		KNH_SETv(ctx, lsfp[rtnidx+K_CALLDELTA].o, (fo)->baseNULL);
//	}
//	KNH_SCALL(ctx, lsfp, rtnidx, (fo)->mtd, argc);
//}
//
///* ------------------------------------------------------------------------ */
//
//// knh_Func_finvoke(ctx, c, "oo", sfp[1].o, sfp[2].o)
//// knh_Func_finvoke(ctx, c, "ii", 1, 2)
//
//KNHAPI2(knh_sfp_t*) knh_Func_invokef(CTX ctx, knh_Func_t *c, const char *fmt, ...)
//{
//	BEGIN_LOCAL(ctx, lsfp, 0);
//	int argc;
//	va_list args;
//	va_start(args , fmt);
//	argc = knh_stack_vpush(ctx, lsfp, K_CALLDELTA/*this*/, fmt, args);
//	va_end(args);
//	knh_Func_invokesfp(ctx, c, lsfp, 0/*rtnidx*/, argc);
//	END_LOCAL(ctx, lsfp);
//	return lsfp;
//}
//

/* ------------------------------------------------------------------------ */
/* [Func] */
/* ------------------------------------------------------------------------ */

//static void knh_sfp_copy(CTX ctx, knh_sfp_t *dst, knh_sfp_t *src, size_t size)
//{
//	size_t i;
//	for(i = 0; i < size; i++) {
//		dst[i].ndata = src[i].ndata;
//		KNH_SETv(ctx, dst[i].o, src[i].o);
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_Func_copyEnv(CTX ctx, knh_Func_t *cc, knh_sfp_t *sfp)
//{
//	size_t stacksize = ctx->esp - sfp;
//	size_t* hstack = (size_t*)KNH_MALLOC(ctx, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
//	knh_sfp_t *envsfp = (knh_sfp_t*)(&hstack[1]);
//	hstack[0] = stacksize;
//	knh_sfp_copy(ctx, sfp, envsfp, stacksize);
//	(cc)->envsfp = envsfp;
//	Func_setStoredEnv(cc, 1);
//	DBG_P("STORED %d", (int)stacksize);
//}

///* ------------------------------------------------------------------------ */
///* [Generator] */
///* ------------------------------------------------------------------------ */
//
//knh_class_t knh_Method_gencid(CTX ctx, knh_Method_t *mtd, knh_class_t cid)
//{
//	knh_type_t rtype = knh_type_tocid(ctx, knh_Method_rztype(mtd), cid);
//	cid = CLASS_t(rtype);
//	DBG_ASSERT_cid(cid);
//	KNH_ASSERT(ClassTBL(cid)->bcid == CLASS_Iterator);
//	return ClassTBL(cid)->p1;
//}
//
///* ------------------------------------------------------------------------ */
//
//static knh_code_t *knh_KonohaCode_yeildingNext(knh_code_t *pc)
//{
//	knh_opline_t *op = (knh_opline_t*)pc;
//	if(op->opcode != OPCODE_NOP) return NULL;
//	pc += knh_opcode_size(op->opcode);
//	return pc;
//}
///* ------------------------------------------------------------------------ */
//
//static
//ITRNEXT knh_Generator_fnext(CTX ctx, knh_sfp_t *sfp, int n)
//{
//	knh_Iterator_t *it = sfp[0].it;
//	knh_Func_t *cc = (knh_Func_t*)DP(it)->source;
//	knh_code_t *pc = knh_KonohaCode_yeildingNext(DP(it)->pc);
//	if(pc == NULL) {
//		ITREND_();
//	}
//	else {
//		size_t stacksize = (cc)->hstacksize[-1];
//		knh_sfp_copy(ctx, (cc)->envsfp, sfp+1, stacksize);
//		KNH_CALLGEN(ctx, sfp, 1, mtd, pc, stacksize);  /* args is reset to esp size */
//		if(((knh_opline_t*)pc)->opcode == OPCODE_YEILDBREAK) {
//			ITREND_();
//		}
//		DBG_P("stacksize=%d, %d", (int)stacksize, (int)(cc)->hstacksize[-1]);
//		knh_sfp_copy(ctx, sfp+1, (cc)->envsfp, (cc)->hstacksize[-1]);
//		DP(it)->pc = pc;
//		KNH_ITRNEXT_envsfp(ctx, sfp, n, (cc)->envsfp);
//	}
//}

///* ------------------------------------------------------------------------ */
//
//static
//knh_Iterator_t* new_Generator(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_Func_t *cc = (knh_Func_t*)new_Object_init(ctx, FLAG_Func, CLASS_Func, 0);
//	KNH_INITv((cc)->mtd, sfp[K_MTDIDX].mtdNC);
//	KNH_INITv((cc)->base, sfp[0].o);
//	{
//		knh_class_t cid = knh_Method_gencid(ctx, sfp[K_MTDIDX].mtdNC, O_cid(sfp[0].o));
//		knh_Iterator_t *it = new_Iterator(ctx, cid, UPCAST(cc), knh_Generator_fnext);
//		knh_code_t *pc = (sfp[K_MTDIDX].mtdNC)->pc_start;
//		KNH_ASSERT(((klr_CHKESP_t*)pc)->opcode == OPCODE_CHKESP);
//		size_t stacksize = 1 + ((klr_CHKESP_t*)pc)->a1;
//		size_t* hstack = (size_t*)KNH_MALLOC(ctx, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
//		knh_sfp_t *envsfp = (knh_sfp_t*)(&hstack[1]);
//		hstack[0] = stacksize;
//		knh_sfp_copy(ctx, sfp - 1, envsfp, stacksize);
//		(cc)->envsfp = envsfp;
//		Func_setStoredEnv(cc, 1);
//		DBG_P("STORED %d", (int)stacksize);
//		DP(it)->pc = pc;
//		return it;
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//METHOD knh_Fmethod_generator(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	RETURN_(new_Generator(ctx, sfp));
//}
//
///* ------------------------------------------------------------------------ */
//
//int Method_isGenerator(Method *mtd)
//{
//	return (mtd->fcall_1 == knh_Fmethod_generator);
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_Method_toGenerator(knh_Method_t *mtd)
//{
//	mtd->fcall_1 = knh_Fmethod_generator;
//}

/* ------------------------------------------------------------------------ */
/* [Thunk] */

//static knh_class_t knh_addThunkClass(CTX ctx, knh_class_t cid, knh_String_t *name, knh_type_t rtype)
//{
//	knh_ClassTBL_t *t = NULL;
//	knh_flag_t mask = 0;
//	if(cid == CLASS_newid) {
//		cid = new_ClassId(ctx);
//	}
//
//	/* knh_ClassTBL_t */ t = varClassTBL(cid);
//	KNH_ASSERT(ClassTBL(cid)->sname == NULL);
//	if(class_isTypeVariable(CLASS_t(rtype))) {
//		mask = FLAG_Class_TypeVariable;
//		DBG_P("TypeVarable: %s", S_tochar(name));
//	}
//	knh_setClassName(ctx, cid, name);
//	t->cflag  = ClassTBL(CLASS_Thunk).cflag | mask;
//	t->oflag  = ClassTBL(CLASS_Thunk).oflag;
//	t->cspi  = ClassTBL(CLASS_Thunk)->cspi;
//
//	t->bcid   = CLASS_Thunk;
//	t->supcid = ClassTBL(CLASS_Thunk)->supcid;
//	t->offset = ClassTBL(CLASS_Thunk).offset;
//
//	t->size = ClassTBL(CLASS_Thunk).size;
//	t->bsize  = ClassTBL(CLASS_Thunk).bsize;
//
//	KNH_INITv(t->methods, ClassTBL(CLASS_Thunk).methods);
//	KNH_INITv(t->typemaps, K_EMPTYARRAY);
//	t->p1 = rtype;
//	t->p2 = CLASS_Tvoid;
//	return cid;
//}
//
///* ------------------------------------------------------------------------ */
//
//knh_class_t knh_class_Thunk(CTX ctx, knh_type_t rtype)
//{
//	knh_class_t cid = CLASS_Thunk;
//	if(CLASS_t(rtype) != CLASS_Tdynamic) {
//		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_write_cid0(ctx, cwb->w, CLASS_Thunk);
//		knh_putc(ctx, cwb->w, '<');
//		knh_write_ltype(ctx, cwb->w, rtype);
//		knh_putc(ctx, cwb->w, '>');
//		cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
//		if(cid == CLASS_unknown) {
//			cid = knh_addThunkClass(ctx, CLASS_newid, knh_cwb_newString(ctx, cwb), rtype);
//		} else {
//			knh_cwb_close(cwb);
//		}
//	}
//	return cid;
//}
//
///* ------------------------------------------------------------------------ */
//
//knh_Thunk_t* new_Thunk(CTX ctx, knh_type_t rtype, knh_sfp_t *sfp, size_t size)
//{
//	knh_class_t cid = knh_class_Thunk(ctx, rtype);
//	knh_Thunk_t *thk = (knh_Thunk_t*)new_Object_init(ctx, FLAG_Thunk, cid, 0);
//	if(size == 1) {
//		(thk)->envsize = 1;
//		(thk)->envsfp = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * (thk)->envsize);
//		KNH_INITv((thk)->envsfp[0].o, sfp[0].o);
//		(thk)->envsfp[0].ndata = sfp[0].ndata;
//		Thunk_setEvaluated(thk, 1);
//	}
//	else {
//		size_t i;
//		DBG_ASSERT(IS_Method(sfp[0].mtd));
//		(thk)->envsize = 1 + size;
//		(thk)->envsfp = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * (thk)->envsize);
//		for(i = 1; i < (thk)->envsize; i++) {
//			KNH_INITv((thk)->envsfp[i].o, sfp[i-1].o);
//			(thk)->envsfp[i].ndata = sfp[i-1].ndata;
//		}
//		KNH_INITv((thk)->envsfp[0].o, KNH_NULL);
//		(thk)->envsfp[0].ndata = 0;
//	}
//	return thk;
//}


/* ------------------------------------------------------------------------ */
/* [ide's closure extension] */
/* ------------------------------------------------------------------------ */

//int knh_fcallback_cmpr(Object *obj, Object *obj2)
//{
//	CTX ctx = knh_getCurrentContext();
//	BEGIN_LOCAL(ctx, lsfp, 5);
//	knh_putsfp(ctx, lsfp, 3, obj); /*this*/
//	knh_putsfp(ctx, lsfp, 4, obj2);
//	knh_Func_invokesfp(ctx, (knh_Func_t*)(-1), lsfp, 0, /* argc*/1);
//	END_LOCAL(ctx, lsfp);
//	return (int)lsfp[1].ivalue;
//}

/* ------------------------------------------------------------------------ */

//void *knh_generateCallBackFunc(CTX ctx, void *func, knh_Func_t *c)
//{
//	// (1) duplicate func;
//	// (2) replace (-1) with c
//	//
//	return func;
//}

/* ------------------------------------------------------------------------ */

//static inline int knh_pow2(int n)
//{
//	int p = 0;
//	if(n <= 0) return 1;
//	--n;
//	for(;n != 0;n >>=1) {
//		p = (p << 1) + 1;
//	}
//	return p + 1;
//}

/* ------------------------------------------------------------------------ */
// HOWTO USE callback func
//
// first, prepare callback function like "void knh_qsort(void *d1, void *d2)"
// and add additional argument "knh_Func_t* cc" to knh_qsort.
// So, interface of knh_qsort is like "void knh_qsort(void *d1, void *d2 ,knh_Func_t* cc)"
//
// Next, make dummy callback function, dummy_callback.
// dummy_callback's interface is same as previous knh_qsort interface.
// void dummy_callback(void *d1, void *d2);
// dummy_callback function is not used, but these replica use.
//
//// void Array.myqsort(Func c);
//METHOD Array_myqsort(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_Array_t   *a  = (knh_Array_t *) sfp[0].o;
//	knh_Func_t *cc = (knh_Func_t *) sfp[1].cc;
//	void *callback = knh_copy_callbackfunc((void*)dummy_callback,(void *)knh_qsort,cc);
//	((void (*)(int))callback)(a->list,a->size, sizeof(Object*),callback);
//}
//
//static int knh_qsort(void *d1, void *d2 ,knh_Func_t* cc)
//{
//	CTX ctx = knh_getCurrentContext();
//	BEGIN_LOCAL(ctx, lsfp);
//	Object *arg1 = (Object *)new_Int(ctx,(int)d1);
//	Object *arg2 = (Object *)new_Int(ctx,(int)d2);
//	knh_putsfp(ctx, lsfp, 2, arg1);
//	knh_putsfp(ctx, lsfp, 3, arg2);
//	knh_Func_invokesfp(ctx, cc, lsfp, /* argc*/ 2);
//	return (int) lsfp[0].ivalue;
//}
//
//static int dummy_callback(void *d1, void *d2)
//{
//	return knh_qsort(d1,d2,(knh_Func_t*) -1);
//}
//
//#define MAX_FUNC_SIZE 256
//// copy callback func and return function that allocate form heap.
//void *knh_copyCallbackfunc(CTX ctx, void *func, void *target,knh_Func_t *cc)
//{
//	void *callback = NULL;
//	TODO();
//#if !defined(K_USED_TO_BE_ON_LKM) && !defined(KONOHA/ON_TB) && !defined(K_USING_WINTHREAD_)
//	int i,_ffffffff = -1,jmp_pos = -1;
//	knh_uchar_t *f = (knh_uchar_t *) func;
//
//	for (i = 0; i < MAX_FUNC_SIZE; i++) {
//		// find 0xffffffff(that is dummy forknh_Func_t pointer)
//		if(*(int*) &f[i] == -1) {
//			_ffffffff = i;
//			i+=3;
//		}
//		// find jmp instraction
//		// if arch is i386, jmp instraction is start at "0xe8"
//		if(f[i] == 0xe8) {
//			jmp_pos = i;
//		}
//		// epilog of function is always 0xc9,0xc3.
//		if(f[i] == 0xc9 && f[i+1] == 0xc3) {
//			i+=2;
//			break;
//		}
//		if(f[i] == 0xc9 && f[i+1] == 0xe9){
//			jmp_pos = i + 1;
//			i+=6;
//			break;
//		}
//	}
//	size_t size = knh_pow2(i);
//	callback = valloc(size);
//	memcpy(callback,func,i);
//	mprotect(callback, size, PROT_READ | PROT_WRITE | PROT_EXEC);
//
//	f = (knh_uchar_t *)callback;
//
//	// patch for 0xffffffff
//	if(_ffffffff > 0) {
//#ifdef __x86_64__
//		union opcode { knh_uchar_t code[8]; int ival;} op;
//		op.ival = (intptr_t)cc;
//		//*(int*) &f[_ffffffff] = op.ival;
//		memcpy(&f[_ffffffff], op.code, 8);
//#else
//		*(int*) &f[_ffffffff] = (int)cc;
//#endif
//	}
//
//	// patch for function relocation.
//	if(jmp_pos > 0) {
//		intptr_t diff = (intptr_t)target - (intptr_t)(&f[jmp_pos] + 5);
//		*(int*) &f[jmp_pos+1] = (int) diff;
//	}
//#endif /* K_USED_TO_BE_ON_LKM */
//	return callback;
//}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
