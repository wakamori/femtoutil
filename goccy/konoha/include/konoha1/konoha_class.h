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

#ifndef CLASS_KONOHA_H_
#define CLASS_KONOHA_H_

#include"konoha_t.h"
#include"konoha_name.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* Type Variable */
//## @TypeVariable class Tvoid Tvoid;
//## @TypeVariable class Tvar  Tvoid;

//#define CLASS_Tvoid             CLASS_Tvoid_
//#define CLASS_Tvar              CLASS_Tvar_
#define TYPE_void                 CLASS_Tvoid
#define TYPE_var                  CLASS_Tvar

// Tuple does not exist. See
//#define CLASS_Tuple             CLASS_Tvoid
//#define TYPE_Tuple              CLASS_Tvoid

/* ------------------------------------------------------------------------ */
//## class Object Object knh_ObjectField_t;
//## flag Object Ref           0 (%s)->h.magicflag has set * *;
//## flag Object NullObject    1 (%s)->h.magicflag is set  * *;
//## flag Object Classified    2 (%s)->h.magicflag is *    * *;
//## flag Object Modified      3 (%s)->h.magicflag is set  * *;
//## flag Object Local1       12 (%s)->h.magicflag is set  * *;
//## flag Object Local2       13 (%s)->h.magicflag is set  * *;
//## flag Object Local3       14 (%s)->h.magicflag is set  * *;
//## flag Object Local4       15 (%s)->h.magicflag is set  * *;

#ifndef K_FIELDSIZE
#define K_FIELDSIZE 64
#endif

#define	K_SMALLOBJECT_FIELDSIZE 3

typedef struct knh_ObjectField_t {
	knh_hObject_t h;
	Object  **fields;
	Object  *smallobject;  // small object
} knh_ObjectField_t ;

#define IS_NULL(o)     ((((o)->h.magicflag) & FLAG_Object_NullObject) == FLAG_Object_NullObject)
#define IS_NOTNULL(o)  ((((o)->h.magicflag) & FLAG_Object_NullObject) != FLAG_Object_NullObject)

#define CLASS_ObjectField   CLASS_Object
#define O_cid(o)   (((o)->h.cTBL)->cid)
#define O_bcid(o)  (((o)->h.cTBL)->bcid)
#define O_cTBL(o)     ((o)->h.cTBL)
#define O_p1(o)    (((o)->h.cTBL)->p1)
#define O_p2(o)    (((o)->h.cTBL)->p2)
#define IS_ObjectField(o)  (((o)->h.cTBL)->bcid == CLASS_Object)

#define new_H(C)         (knh_##C##_t*)new_hObject_(ctx, ClassTBL(CLASS_##C))
#define new_hObject(ctx, cid)          new_hObject_(ctx, ClassTBL(cid))
#define new_(C)          (knh_##C##_t*)new_Object_init2(ctx, ClassTBL(CLASS_##C))
#define new_O(C,cid)     (knh_##C##_t*)new_Object_init2(ctx, ClassTBL(cid))

/* ------------------------------------------------------------------------ */
//## @Immutable class Boolean Object;

typedef struct {
	union {
		knh_ndata_t   data;
		knh_bool_t    bvalue;
		knh_int_t     ivalue;
		knh_float_t   fvalue;
	};
} knh_num_t ;

typedef struct knh_Boolean_t {
	knh_hObject_t h;
	knh_num_t n;
} knh_Boolean_t;

#define IS_TRUE(o)         (O_bcid(o) == CLASS_Boolean && N_tobool(o))
#define IS_FALSE(o)        (O_bcid(o) == CLASS_Boolean && N_tobool(o) == 0)
#define new_Boolean(ctx, c)    ((c) ? KNH_TRUE : KNH_FALSE)

/* ------------------------------------------------------------------------ */
//## @Immutable class Number Object;

typedef struct knh_Number_t {
	knh_hObject_t h;
	knh_num_t n;
} knh_Number_t;

#define N_toint(o)         (((knh_Number_t*)o)->n.ivalue)
#define N_tofloat(o)       (((knh_Number_t*)o)->n.fvalue)
#define N_tobool(o)        (((knh_Number_t*)o)->n.bvalue)

/* ------------------------------------------------------------------------ */
//## @Immutable class Int Number;

typedef struct knh_Int_t {
	knh_hObject_t h;
	knh_num_t n;
} knh_Int_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Float Number;

typedef struct knh_Float_t {
	knh_hObject_t h;
	knh_num_t n;
} knh_Float_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class String Object;
//## flag String TextSgm 1 - is set  *   *;
//## flag String ASCII   2 - is set  is  *;

typedef const char*   knh_TEXT_t;

typedef struct knh_String_t {
	knh_hObject_t h;
	knh_bytes_t str;
	knh_hashcode_t hashCode;
} knh_String_t;

#define new_T(msg)            new_TEXT(ctx, CLASS_String, msg, 1)
#define new_S(ctx, msg)       new_String_(ctx, CLASS_String, msg, NULL)

#define S_tobytes(s)          ((s)->str)
#define S_tochar(s)           ((s)->str.text)
#define S_size(s)             ((s)->str.len)
#define S_equals(s, b)        knh_bytes_equals(S_tobytes(s), b)
#define S_startsWith(s, b)    knh_bytes_startsWith(S_tobytes(s), b)
#define S_endsWith(s, b)      knh_bytes_endsWith(S_tobytes(s), b)

#define CLASS_TEXT            (TYPE_This-1)
#define TYPE_TEXT             (TYPE_This-1)

/* ------------------------------------------------------------------------ */
//## class Bytes Object;
//## @Immutable class BytesIm Object;

typedef struct {
	size_t capacity;
	size_t wsize;
	size_t sorted;
	size_t dim;
	size_t x;
	size_t xy;
	size_t xyz;
} knh_dim_t ;

typedef struct knh_Bytes_t {
	knh_hObject_t h;
	knh_bytes_t   bu;
	const knh_dim_t    *dim;
	const char *DBG_name;
} knh_Bytes_t ;

#define KNH_SIZE(v)     v
#define k_grow(N)       ((N)*2)

#define BA_tobytes(o)   (o)->bu
#define BA_size(o)      (o)->bu.len
#define BA_tochar(o)    (o)->bu.text

#define B_equals(b, t)        (knh_bytes_strcmp(b, STEXT(t)) == 0)
#define B_startsWith(b, t)    knh_bytes_startsWith(b, STEXT(t))
#define B_endsWith(b, t)      knh_bytes_endsWith(b, STEXT(t))

/* ------------------------------------------------------------------------ */
//## class Iterator Object;

typedef struct knh_mapitr_t {
	size_t index;
	void *ptr;
} knh_mapitr_t;

#define K_MAPITR_INIT   {0, NULL}

typedef void (*knh_Ffree)(void *nptr);

typedef struct {
	Object    *source;
	void      *nptr;
	struct knh_mapitr_t mitr;
//	knh_int_t  pos;
//	union {
//		void*   ref;
//		struct knh_opline_t *pc; /* @see(Generator) */
//	};
	knh_Ffree freffree;
} knh_IteratorEX_t;

#define ITR(sfp)   sfp[0].it

typedef struct knh_Iterator_t {
	knh_hObject_t h;
	knh_IteratorEX_t *b;
	knh_Fitrnext fnext_1;
} knh_Iterator_t;

/* ------------------------------------------------------------------------ */
//## class Tuple Object;
//## typeO PairSS   Tuple 0 String String;
//## typeO PairST1  Tuple 0 String T1;
//## typeO PairT1T2 Tuple 0 T1 T2;

typedef knh_ObjectField_t knh_Tuple_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Range Object;
//## flag Range NDATA     1 - is set * *;
//## flag Range Inclusive 2 - is set * *;
//## type RangeInt  Range 0 Int;

typedef struct knh_Range_t {
	knh_hObject_t h;
	union {
		knh_Object_t  *ostart;
		knh_ndata_t    nstart;
	};
	union {
		knh_Object_t  *oend;
		knh_ndata_t    nend;
	};
} knh_Range_t;

/* ------------------------------------------------------------------------ */
//## class Array   Object;
//## class ArrayIm Object;
//## flag  Array   NDATA     1 - is set * *;
//## flag  ArrayIm NDATA     1 - is set * *;
//## type CmprT1  Func 1 T1 T1 Int;

typedef struct {
	size_t   (*index)(CTX ctx, knh_sfp_t *sfp, knh_int_t n, size_t size);
	void     (*get)(CTX ctx, knh_sfp_t *sfp, size_t n _RIX);
	void     (*set)(CTX ctx, struct knh_Array_t *, size_t n, knh_sfp_t *sfp);
	void     (*add)(CTX ctx, struct knh_Array_t *, knh_sfp_t *sfp);
} knh_ArrayAPI_t;

typedef struct knh_Array_t {
	knh_hObject_t h;
	union {
		knh_ndata_t             *nlist;
		knh_int_t               *ilist;
		knh_float_t             *flist;
		struct knh_Object_t    **list;
		struct knh_String_t    **strings;
		struct knh_Method_t    **methods;
		struct knh_TypeMap_t   **trans;
		struct knh_Token_t     **tokens;
		struct knh_Stmt_t      **stmts;
	};
	size_t size;
	const knh_dim_t *dim;
	const knh_ArrayAPI_t *api;
} knh_Array_t;

#define knh_Array_n(a,n)      (a)->list[(n)]
#define knh_Array_size(a)     (a)->size
#define knh_Array_trimSize(ctx, a, newsize)  knh_Array_clear(ctx, a, newsize)
#define knh_Array_add(ctx, a, o)    knh_Array_add_(ctx, a, UPCAST(o))
//#define knh_TOKENs_n(a, n)    ((knh_Token_t*)(a)->list[(n)])

///* ------------------------------------------------------------------------ */
//## class Map Object;
//## @Immutable class MapIm Object;

#define K_USE_FASTDMAP(STMT)  STMT

typedef struct knh_dentry_t {
K_USE_FASTDMAP(knh_uint64_t ukey;)
	union {
		knh_String_t  *key;
		knh_intptr_t   ikey;
		knh_floatptr_t fkey;
		knh_ndata_t    nkey;
	};
	union {
		Object           *value;
		knh_ndata_t    nvalue;
	};
} knh_dentry_t;

typedef struct knh_dmap_t {
	knh_dentry_t *dentry;
	size_t size;
	size_t capacity;
K_USE_FASTDMAP(knh_uint64_t (*strkeyuint)(knh_bytes_t);)
	int (*dentrycmpr)(const void *, const void *);
	int (*strcmpr)(knh_bytes_t, knh_bytes_t);
	size_t sorted;
	const char *DBGNAME;
} knh_dmap_t ;

typedef struct knh_hentry_t {
	knh_hashcode_t hcode;
	union {
		Object       *key;
		knh_String_t *skey;
		knh_ndata_t   nkey;
	};
	union {
		Object       *value;
		knh_ndata_t   nvalue;
	};
	struct knh_hentry_t *next;
} knh_hentry_t;

typedef struct knh_hmap_t {
	knh_hentry_t **hentry;
	size_t size;
	size_t hmax;
	size_t factor;
	const char *DBGNAME;
} knh_hmap_t;

typedef void  knh_map_t;

typedef struct knh_Map_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_hmap_t    *hmap;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_Map_t;

#define K_HASH_INITSIZE 83
#define KNH_HASH_FACTOR(i)     ((i * 4) / 3)
#define knh_Map_get(ctx, m, k)     m->dspi->getkey(ctx, m, k)

// DictMap, DictSet are old names of Map;

typedef struct knh_DictMap_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictMap_t;

#define new_DictMap0(ctx, N, F, NAME)   new_DictMap0_(ctx, N, F, NAME)
#define knh_DictMap_size(m)    ((m)->dmap)->size
#define knh_DictMap_set(ctx, m, k, v)      DictMap_set_(ctx, m, k, UPCAST(v))

typedef struct knh_DictSet_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictSet_t;

typedef void (*knh_Fdictset)(CTX, knh_DictSet_t*, knh_String_t *k, knh_uintptr_t);
#define new_DictSet0(ctx, N, F, NAME)   new_DictSet0_(ctx, N, F, NAME)
#define knh_DictSet_size(m)      ((m)->dmap)->size
#define knh_DictSet_keyAt(ds, n)    knh_DictMap_keyAt((knh_DictMap_t*)ds, n)
#define knh_DictSet_index(ds, key)  knh_DictMap_index((knh_DictMap_t*)ds, key)

/* ------------------------------------------------------------------------ */
//## @Immutable class Class Object;
//## flag Class Ref            0 (ClassTBL(%s))->cflag is * * *;
//## flag Class Immutable      2 (ClassTBL(%s))->cflag is * * *;
//## flag Class Expando        3 (ClassTBL(%s))->cflag is * * *;
//## flag Class Private!Public 4 (ClassTBL(%s))->cflag is * * *;
//## flag Class Final          5 (ClassTBL(%s))->cflag  is * * *;
//## flag Class Singleton      6 (ClassTBL(%s))->cflag  is * * *;
//## flag Class Unique         7 (ClassTBL(%s))->cflag  is * * *;
//## flag Class Interface      8 (ClassTBL(%s))->cflag  is * * *;
//## flag Class TypeVariable   9 (ClassTBL(%s))->cflag  is set * *;

#define FLAG_Field_Hidden          (knh_flag_t)(1<<0)
#define FLAG_Field_Protected       (knh_flag_t)(1<<1)
#define FLAG_Field_Getter          (knh_flag_t)(1<<2)
#define FLAG_Field_Setter          (knh_flag_t)(1<<3)
#define FLAG_Field_Key             (knh_flag_t)(1<<4)
#define FLAG_Field_Volatile        (knh_flag_t)(1<<5)
#define FLAG_Field_ReadOnly        (knh_flag_t)(1<<6)
#define FLAG_Field_Property        (knh_flag_t)(1<<7)
#define FLAG_Field_Principle       (knh_flag_t)(1<<8)

typedef struct knh_Class_t {
	knh_hObject_t h;
	knh_class_t   cid;
	knh_type_t    type;
	const knh_ClassTBL_t *cTBL;
} knh_Class_t;

/* class */

#define KNH_NULVAL(cid)  knh_getClassDefaultValue(ctx, cid)
#define KNH_TNULL(T)      (knh_##T##_t*)knh_getClassDefaultValue(ctx, CLASS_##T)
#define knh_Class_cid(c)     (knh_class_t)(c)->cid
typedef void (*knh_Fwritecid)(CTX ctx, struct knh_OutputStream_t *w, knh_class_t cid);

/* ------------------------------------------------------------------------ */
//## @Private @Immutable class ParamArray Object;
//## flag ParamArray VARGs  1 - is set * *;
//## flag ParamArray RVAR   2 - is set * *;

typedef struct {
	knh_type_t    type;  knh_fieldn_t  fn;
} knh_param_t ;

typedef struct knh_ParamArray_t {
	knh_hObject_t h;
	knh_ushort_t psize; knh_ushort_t rsize;
	union {
		knh_param_t* params;
		knh_param_t  p0;
	};
	union {
		knh_param_t p1;
		size_t capacity;
	};
	knh_param_t p2;
} knh_ParamArray_t;

#define new_ParamArray(ctx)         new_(ParamArray);
#define knh_param_hash(h, t, fn)    (fn + (((h * 31) + t) * 7))
#define knh_ParamArray_rget(pa, n)  knh_ParamArray_get(pa, (pa->psize) + n)

typedef struct {
	knh_type_t type;
	knh_short_t sfpidx;
	void *conv_func;
} knh_ffiparam_t;  // for ffi

/* ------------------------------------------------------------------------ */
//## class Method Object;
//## flag Method Private!Public  0 DP(%s)->flag is set * *;
//## flag Method Virtual!Final   1 DP(%s)->flag is set * *;
//## flag Method Debug           2 DP(%s)->flag is set * *;
//## flag Method Const           3 DP(%s)->flag is * * *;
//## flag Method Static          4 DP(%s)->flag is * * *;
//## flag Method ObjectCode      5 DP(%s)->flag is set * *;
//## flag Method Hidden          6 DP(%s)->flag is set * *;
//## flag Method Dynamic         7 DP(%s)->flag is set * *;
//## flag Method Immutable       8 DP(%s)->flag is set * *;
//## flag Method Message         9 DP(%s)->flag is set * *;
//## flag Method Controlled     10 DP(%s)->flag is set * *;

typedef struct {
	knh_flag_t     flag;  knh_ushort_t   delta;
	struct knh_ParamArray_t*    mp;
	void*                       cfunc;
	union {
		knh_Object_t            *objdata;
		struct knh_String_t     *source;
		struct knh_KonohaCode_t *kcode;
		struct knh_Gamma_t      *gma;       // Dynamic
	};
	knh_uri_t      uri;   knh_uri_t      domain;
	knh_uintptr_t  count;
} knh_MethodEX_t;

typedef struct knh_Method_t {
	knh_hObject_t        h;
	knh_MethodEX_t      *b;
	knh_class_t          cid;   knh_methodn_t  mn;
	knh_Fmethod          fcall_1;
	struct knh_opline_t *pc_start;
} knh_Method_t;

#define knh_Method_mn(mtd)    (mtd)->mn
#define knh_Method_mf(mtd)    DP(mtd)->mp
#define knh_ParamArray_psize(mf)    ((mf)->psize)
#define knh_Method_rsize(mtd)       ((DP(mtd)->mp)->rsize)
#define knh_Method_psize(mtd)       ((DP(mtd)->mp)->psize)

//#define knh_Method_pztype(mtd, n) (knh_ParamArray_get(DP(mtd)->mp, n))->type

#define knh_Method_ptype(ctx, mtd, cid, n) \
	knh_type_tocid(ctx, (knh_ParamArray_get(DP(mtd)->mp, n))->type, cid)

#define ClassTBL_getMethodNULL(ctx, c, mn)    knh_ClassTBL_findMethodNULL(ctx, c, mn, 0)
#define ClassTBL_getMethod(ctx, c, mn)        knh_ClassTBL_findMethodNULL(ctx, c, mn, 1)

//#define knh_getDefaultFmt(ctx, mn)   knh_ClassTBL_getFmt(ctx, CLASS_Tvoid, mn)

#define knh_stack_argc(ctx, sfp)      (ctx->esp - (sfp))

//#define KNH_NOTRACE         0
//#define KNH_SECURITYTRACE   1
//#define KNH_AUDITTRACE      2
//#define KNH_PROFILER        3
//#define KNH_STACKTRACE      4
//#define KNH_FUNCTIONTRACE   5

/* ------------------------------------------------------------------------ */
//## class TypeMap Object;
//## flag TypeMap Interface  1 - is set * *;
//## flag TypeMap Semantic   2 - is set * *;
//## flag TypeMap Const      3 - is set * *;

typedef struct knh_TypeMap_t {
	knh_hObject_t h;
	knh_class_t  scid; knh_class_t  tcid;
	knh_Ftypemap ftypemap_1;
	union {
		Object* mapdata;
		struct knh_Method_t  *mtd;
		struct knh_TypeMap_t *tmr1;
	};
	struct knh_TypeMap_t *tmr2;
} knh_TypeMap_t;

#define knh_findTypeMap(ctx, scid, tcid)     knh_findTypeMapNULL(ctx, scid, tcid, 1)
#define knh_getTypeMapNULL(ctx, scid, tcid)  knh_findTypeMapNULL(ctx, scid, tcid, 0)

/* ------------------------------------------------------------------------ */
//## @Immutable class Func Object;
//## cparam Func 1 R dynamic P dynamic;
//## flag Func StoredEnv 1 - is set * *;

typedef struct knh_Func_t {
	knh_hObject_t h;
	struct knh_Method_t* mtd;
	Object* baseNULL;
	knh_sfp_t *xsfp;
	size_t     xsize;
} knh_Func_t ;

/* ------------------------------------------------------------------------ */
//## class Thunk Object;
//## flag Thunk Evaluated 1 - is set * *;

typedef struct knh_Thunk_t {
	knh_hObject_t h;
	knh_sfp_t *envsfp;
	size_t     envsize;
} knh_Thunk_t;

///* ------------------------------------------------------------------------ */
////## class0 Thread Object;
//
//typedef struct knh_Thread_t {
//	knh_hObject_t h;
//	knh_context_t *context;
//	knh_thread_t  th;
//} knh_Thread_t;

/* ------------------------------------------------------------------------ */
//## class Exception Object;
//## flag0 Exception Logging 0 DP(%s)->flag is * * *;
//## flag0 Exception Caught  1 DP(%s)->flag is set * *;

//## expt Exception!! -;
//## expt Fatal!! -;
//## expt System!! -;
//## expt Security!! -;
//## expt SourceCode!! -;
//## expt Type!! -;
//## expt Assertion!! -;

typedef struct {
	knh_flag_t flag;   knh_ebi_t eid;
	knh_String_t *event;
	knh_String_t *msg;
	Object *bag;
	knh_Array_t*    tracesNULL;
	knh_uline_t     uline;
	int             sysloglevel;
} knh_ExceptionEX_t;

typedef struct knh_Exception_t {
	knh_hObject_t h;
	knh_ExceptionEX_t *b;
} knh_Exception_t;

#define new_Exception__s(ctx, s)     new_Exception__T(ctx, B(s))

/* ------------------------------------------------------------------------ */
//## @Private class ExceptionHandler Object;
//## flag ExceptionHandler Catching   1 - is set * *;

#ifdef K_USING_SETJMP_
#ifdef K_USING_POSIX_
typedef sigjmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         sigsetjmp(buf, 1)
#define knh_longjmp(buf, val)   siglongjmp(buf, val)
#else
typedef jmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         setjmp(buf)
#define knh_longjmp(buf, val)   longjmp(buf, val)
#endif

typedef struct {
	knh_jmpbuf_t jmpbuf;
	knh_intptr_t espidx;
} knh_ExceptionHandlerEX_t;

#else

typedef struct {
	struct knh_opline_t *pc;
	struct klr_TRY_t *op;
	knh_intptr_t sfpidx;
	void *return_address;
	void *frame_address;
	knh_uintptr_t stack_pointer;
} knh_ExceptionHandlerEX_t;

#endif/*K_USING_SETJMP_*/

typedef struct knh_ExceptionHandler_t {
	knh_hObject_t h;
	knh_ExceptionHandlerEX_t *b;
	knh_intptr_t espidx;
	struct knh_ExceptionHandler_t *parentNC;
	struct knh_Array_t *stacklist;
} knh_ExceptionHandler_t;

#define KNH_NOAPI(ctx, cwb, isThrowable) { \
		knh_throw_Unsupported(ctx, cwb, __FUNCTION__, __FILE__, __LINE__, isThrowable); \
	}\

#define KNH_THROW_OUTOFINDEX(ctx, n, max) { \
		knh_throw_OutOfIndex(ctx, n, max, __FILE__, __LINE__); \
	}\

/* ------------------------------------------------------------------------ */
//## @Immutable class Regex Object;

typedef void knh_regex_t;

typedef struct knh_Regex_t {
	knh_hObject_t h;
	knh_regex_t *reg;
	int eflags;
	const struct knh_RegexSPI_t *spi;
	struct knh_String_t *pattern;
} knh_Regex_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Converter Object;

typedef void knh_conv_t;

typedef struct knh_Converter_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConvDSPI_t *dspi;
} knh_Converter_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class StringEncoder Object;

typedef struct knh_StringEncoder_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConvDSPI_t *dspi;
} knh_StringEncoder_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class StringDecoder Object;

typedef struct knh_StringDecoder_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConvDSPI_t *dspi;
} knh_StringDecoder_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class StringConverter Object;

typedef struct knh_StringConveter_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConvDSPI_t *dspi;
} knh_StringConverter_t;

/* ------------------------------------------------------------------------ */
//## @Private class Semantics Object;

#define KNH_SEMANTICS_FMT    "%s{%s}"
struct  knh_Semantics_t;

typedef int (*knh_Fichk)(struct knh_Semantics_t *, knh_int_t v);
typedef int (*knh_Ficmp)(struct knh_Semantics_t *, knh_int_t v1, knh_int_t v2);

typedef int (*knh_Ffchk)(struct knh_Semantics_t *, knh_float_t v);
typedef int (*knh_Ffcmp)(struct knh_Semantics_t *, knh_float_t v1, knh_float_t v2);

typedef knh_String_t *(*knh_Fsnew)(CTX, knh_class_t cid, knh_bytes_t, struct knh_String_t *, int *);
typedef int (*knh_Fscmp)(struct knh_Semantics_t *, knh_bytes_t, knh_bytes_t);

typedef struct {
	knh_flag_t  flag;
	knh_class_t ucid;
	knh_class_t ubcid;
	knh_String_t* urn;
	knh_String_t* tag;
	struct knh_Int_t    *ivalue;
	struct knh_Float_t  *fvalue;
	struct knh_String_t *svalue;

	// int
	union {
		knh_int_t imin;
		knh_uint_t umin;
	};
	union {
		knh_int_t imax;
		knh_uint_t umax;
	};
	knh_Fichk fichk;
	knh_Ficmp ficmp;

	// float
	knh_float_t fmin;
	knh_float_t fmax;
	knh_float_t fstep;
	knh_Ffchk   ffchk;
	knh_Ffcmp   ffcmp;

	// String
	size_t bytelen;
	size_t charlen;
	Object* pattern;
//	struct knh_DictIdx_t* vocabDictIdx;
	struct knh_StringEncoder_t *bconv;
	knh_Fsnew    fsnew;
	knh_Fscmp    fscmp;
} knh_SemanticsEX_t;

typedef struct knh_Semantics_t* (*knh_fspec)(CTX ctx, knh_bytes_t urn);

typedef struct knh_Semantics_t {
	knh_hObject_t h;
	knh_SemanticsEX_t *b;
} knh_Semantics_t;

/* ------------------------------------------------------------------------ */
/* InputStream, OutputStream */

#define KNH_STDIN          (ctx->in)
#define KNH_STDOUT         (ctx->out)
#define KNH_STDERR         (ctx->err)

/* ------------------------------------------------------------------------ */
//## class InputStream Object;

typedef knh_uintptr_t knh_io_t;
#define IO_NULL   ((knh_io_t)0)
#define IO_BUF    ((knh_io_t)1)
#define K_STREAM_BUFSIZ  K_PAGESIZE

typedef struct {
	knh_String_t*  urn;
	union {
		knh_io_t fio;
	};
	union {
		struct knh_Bytes_t  *ba;
		struct knh_String_t *str;
	};
	size_t pos;  size_t posend;
	struct knh_Monitor_t *mon;
	size_t stat_size;
} knh_InputStreamEX_t;

typedef struct knh_InputStream_t {
	knh_hObject_t h;
	knh_InputStreamEX_t *b;
	knh_uline_t  uline;
	const struct knh_StreamDSPI_t *dspi;
	struct knh_StringDecoder_t*    decNULL;
} knh_InputStream_t;

/* ------------------------------------------------------------------------ */
//## class OutputStream Object;
//## flag OutputStream BOL            1 - is set * *;
//## flag OutputStream AutoFlush      2 - is set is set;
//## flag OutputStream UTF8           3 - has set * *;

typedef struct {
	knh_String_t*  urn;
	knh_io_t fio;
	struct knh_Bytes_t *ba;
	struct knh_Monitor_t *mon;
	size_t stat_size;
	knh_String_t*  NEWLINE;
	knh_String_t*  TAB;
	knh_short_t    indent;
} knh_OutputStreamEX_t;

typedef struct knh_OutputStream_t {
	knh_hObject_t h;
	knh_OutputStreamEX_t *b;
	knh_uline_t  uline;
	const struct knh_StreamDSPI_t *dspi;
	struct knh_StringEncoder_t* encNULL;
} knh_OutputStream_t;

#define knh_putc(ctx, w, ch)       knh_OutputStream_putc(ctx, w, ch)
#define knh_write(ctx, w, s)       knh_OutputStream_write(ctx, w, s)
#define knh_flush(ctx, w)          knh_OutputStream_flush(ctx, w, 0)
#define knh_write_delim(ctx, w)    knh_write_ascii(ctx, w, ", ")
#define knh_write_dots(ctx, w)     knh_write_ascii(ctx, w, "...")
#define knh_write_fn(ctx, w, fn)   knh_write_ascii(ctx, w, FN__(fn))
#define knh_write__O(ctx, w, o)    knh_write_Object(ctx, w, MN__k, o)

/* ------------------------------------------------------------------------ */
//## class Connection Object;
//## class ResultSet Object;

typedef void   knh_qconn_t;
typedef void   knh_qcur_t;

typedef struct knh_Connection_t {
	knh_hObject_t h;
	knh_qconn_t                  *conn;
	const struct knh_QueryDPI_t  *dspi;
	knh_String_t                 *urn;
} knh_Connection_t;

/* ------------------------------------------------------------------------ */

#define knh_ResultSet_CTYPE__null    0
#define knh_ResultSet_CTYPE__integer 1
#define knh_ResultSet_CTYPE__float   2
#define knh_ResultSet_CTYPE__text    3  /* UTF-8*/
#define knh_ResultSet_CTYPE__bytes   4
#define knh_ResultSet_CTYPE__Object  5

typedef struct {
	knh_type_t type;
	knh_ushort_t ctype;
	knh_String_t *name;
	size_t start;
	size_t len;
	int dbtype;
} knh_dbschema_t;

typedef struct {
	struct knh_Connection_t *conn;
	knh_qcur_t              *qcur;
	void   (*qcurfree)(knh_qcur_t *); /* necessary if conn is closed before */
	knh_String_t            *tableName;
	knh_class_t              tcid;
	knh_ushort_t             column_size;
	knh_dbschema_t          *column;
	knh_Bytes_t             *databuf;
	size_t                   count;
} knh_ResultSetEX_t;

typedef struct knh_ResultSet_t {
	knh_hObject_t h;
	knh_ResultSetEX_t *b;
} knh_ResultSet_t;

/* ------------------------------------------------------------------------ */
//## class NameSpace Object;

typedef struct knh_NameSpace {
	knh_String_t *nsname;
	struct knh_DictSet_t*   dspiDictSetNULL;
	struct knh_DictMap_t*   constDictCaseMapNULL;
	struct knh_DictMap_t*   macroDictMapNULL;

	struct knh_DictSet_t*   name2cidDictSetNULL;
	struct knh_DictSet_t*   func2cidDictSetNULL;
	struct knh_Array_t*     methodsNULL;
	struct knh_Array_t*     formattersNULL;
} knh_NameSpaceEX_t;

typedef struct knh_NameSpace_t {
	knh_hObject_t h;
	knh_NameSpaceEX_t *b;
	struct knh_NameSpace_t   *parentNULL;
	knh_String_t             *rpath;
	void                     *dlhdr;
} knh_NameSpace_t;

/* ------------------------------------------------------------------------ */
//## @Private @Singleton class Script Object;

typedef struct knh_Script_t {
	knh_hObject_t h;
	Object **fields;
	size_t dummy;
	struct knh_NameSpace_t *ns;
} knh_Script_t ;

/* ------------------------------------------------------------------------ */
//## @Singleton @Struct class System Object;

typedef struct {
	knh_String_t *name;
	knh_methodn_t parent_mn;
	knh_methodn_t parentMF;
	knh_class_t   principle_mtd;
	knh_class_t   mtdfCID;
} 	knh_nameinfo_t ;

typedef struct {
	knh_uintptr_t sysid;
	size_t     ctxcount;

	struct knh_DictMap_t*      props;
	struct knh_InputStream_t*  in;
	struct knh_OutputStream_t* out;
	struct knh_OutputStream_t* err;
	struct knh_String_t*       enc;

	struct knh_DictSet_t       *tokenDictSet;
	struct knh_DictSet_t   *nameDictCaseSet;
	size_t                      namecapacity;
	knh_nameinfo_t             *nameinfo;
	struct knh_DictSet_t      *urnDictSet;
	struct knh_Array_t        *urns;

	struct knh_DictSet_t      *ClassNameDictSet;
	struct knh_DictSet_t  *EventDictCaseSet;
	struct knh_DictMap_t      *PackageDictMap;
	struct knh_DictMap_t      *URNAliasDictMap;
	struct knh_DictSet_t      *dspiDictSet;
//	struct knh_DictSet_t *SpecFuncDictSet;
//	struct knh_Array_t   *UsingResources;
//	struct knh_DictMap_t *listenerDictMap;
//	struct knh_DictMap_t *trustedPathDictSet;
} knh_SystemEX_t;

typedef struct knh_System_t {
	knh_hObject_t h;
	knh_SystemEX_t *b;
} knh_System_t;

/* ------------------------------------------------------------------------ */
//## class Context Object;
//## flag Context Debug        1 ((knh_context_t*)%s)->flag is set is set;
//## flag Context CompileOnly  2 ((knh_context_t*)%s)->flag is set * *;
//## flag Context Interactive  3 ((knh_context_t*)%s)->flag is set * *;

typedef struct knh_Context_t {
	knh_hObject_t h;
	const knh_context_t *ctx;
} knh_Context_t;


/* ------------------------------------------------------------------------ */
//## class Monitor Object;
//## flag Monitor  Throwable        1 - is set * *;

typedef struct knh_Monitor_t {
	knh_hObject_t h;
	int loglevel;
	void (*trace)(CTX, knh_sfp_t*, struct knh_Monitor_t*, const char *ns, const char *event, const char *fmt, ...);
} knh_Monitor_t;

/* ------------------------------------------------------------------------ */
//## class Assurance Object;

typedef struct knh_Assurance_t {
	knh_hObject_t h;
	knh_uintptr_t aid;
	struct knh_String_t *msg;
	knh_intptr_t stime;
} knh_Assurance_t;

/* ------------------------------------------------------------------------ */
/* konohac.h */

/* ------------------------------------------------------------------------ */
//## @Struct class Token Object;
//## flag Token BOL           0 SP(%s)->flag0 is  set * *;
//## flag Token DOT           1 SP(%s)->flag0 is  set * *;
//## flag Token LCASE         2 SP(%s)->flag0 has set * *;
//## flag Token ISBOOL        3 SP(%s)->flag0 is set * *;
//## flag Token Getter        4 SP(%s)->flag0 is set * *;
//## flag Token Setter        5 SP(%s)->flag0 is set * *;
//## flag Token ExceptionType 6 SP(%s)->flag0 is set * *;
//## flag Token Immutable     7 SP(%s)->flag0 is set * *;
//## flag Token MEMO1         8 SP(%s)->flag0 is set * *;

// TT_BRACE, TT_PARENTHESIS, TT_BRANCET
#define Token_isPLUSLINE(o)  Token_isMEMO1(o)
#define Token_setPLUSLINE(o, b) Token_setMEMO1(o, b)

// TT_NAME
#define Token_isPNAME(o)  Token_isMEMO1(o)
#define Token_setPNAME(o, b) Token_setMEMO1(o, b)

// TT_UNAME
#define Token_isBYTE(o)  Token_isMEMO1(o)
#define Token_setBYTE(o, b) Token_setMEMO1(o, b)

// TT_LOCAL,
#define Token_isSUPER(o)      Token_isMEMO1(o)
#define Token_setSUPER(o, b)  Token_setMEMO1(o, b)

// TT_LOCAL, TT_FIELD, TT_SCRIPT
#define Token_isReadOnly(tk)   Token_isBOL(tk)
#define Token_setReadOnly(tk, b)   Token_setBOL(tk, b)

typedef knh_ushort_t   knh_term_t;
#ifndef MN_OPSIZE
#include "konohalang.h"
#endif

typedef struct knh_Term_t {
	knh_hObject_t h;
	void *ref;
	knh_uline_t uline;                   //Term
	knh_type_t type; knh_term_t  tt;     //Term
} knh_Term_t;

#define Term_isTyped(o)     (o->type != TYPE_var)

typedef struct knh_Token_t {
	knh_hObject_t h;
	union {
		Object* data;
		struct knh_Array_t*  list;
		struct knh_String_t* text;
		struct knh_Token_t*  token;
		struct knh_Stmt_t*   stmt;
		struct knh_Method_t* mtd;
		struct knh_TypeMap_t* mpr;
		struct knh_Int_t     *num;
	};
	knh_uline_t uline;                  // Term
	knh_type_t type; knh_term_t  tt;    // Term
	knh_flag_t   flag0;
	union {
		knh_short_t   index;
		knh_class_t   cid;
		knh_methodn_t mn;
	};
} knh_Token_t;

#define Token_isTyped(o)    (SP(o)->type != TYPE_var)
#define new_TokenCONST(ctx, d)  new_TokenCONST_(ctx, UPCAST(d))
#define new_TermCONST(ctx, d)   TM(new_TokenCONST_(ctx, UPCAST(d)))

/* ------------------------------------------------------------------------ */
//## @Struct class Stmt Object;
//## flag Stmt STOPITR    1 DP(%s)->flag0 is set * *;
//## flag Stmt CONST      2 DP(%s)->flag0 is set * *;
//## flag Stmt Memo1      4 DP(%s)->flag0 is set * *;
//## flag Stmt Memo2      5 DP(%s)->flag0 is set * *;

/* STT_METHOD*/
#define StmtMETHOD_isVARGs(s)          Stmt_isMemo1(s)
#define StmtMETHOD_setVARGs(s,b)       Stmt_setMemo1(s,b)
#define StmtMETHOD_isFFI(s)            Stmt_isMemo2(s)
#define StmtMETHOD_setFFI(s,b)         Stmt_setMemo2(s,b)

/* STT_PTYPEMAP*/
#define Stmt_isDOWNCAST(s)       Stmt_isMemo1(s)
#define Stmt_setDOWNCAST(s,b)    Stmt_setMemo1(s,b)

/* STT_CALL*/
#define Stmt_isTAILRECURSION(s)       Stmt_isMemo1(s)
#define Stmt_setTAILRECURSION(s,b)    Stmt_setMemo1(s,b)
#define Stmt_isCLASSCONSTDEF(s)       Stmt_isMemo2(s)
#define Stmt_setCLASSCONSTDEF(s,b)    Stmt_setMemo2(s,b)

/* STT_FUNCCALL*/
#define Stmt_isDYNCALL(s)       Stmt_isMemo1(s)
#define Stmt_setDYNCALL(s,b)    Stmt_setMemo1(s,b)

/* STT_RETURN */
#define Stmt_isImplicit(s)       Stmt_isMemo1(s)
#define Stmt_setImplicit(s,b)    Stmt_setMemo1(s,b)

typedef struct {
	knh_flag_t   flag0;
	knh_ushort_t espidx;
	knh_ushort_t size;
	knh_ushort_t capacity;
	union {
		struct knh_DictMap_t* metaDictCaseMap;
		struct knh_String_t*  errMsg;
		struct knh_Stmt_t*    stmtPOST;
	};
	//size_t wstart;
	struct knh_Stmt_t* nextNULL;
} knh_StmtEX_t;

typedef struct knh_Stmt_t {
	knh_hObject_t h;
	knh_StmtEX_t *b;
	knh_uline_t uline;                 // Term
	knh_type_t type; knh_term_t  stt;  // Term
	union {
		struct knh_Term_t**  terms;
		struct knh_Token_t** tokens;
		struct knh_Stmt_t**  stmts;
	};
} knh_Stmt_t;

#define Stmt_isTyped(o)        ((o)->type != TYPE_var)

/* ------------------------------------------------------------------------ */
//## @Struct @Private class Gamma Object;
//## flag00 Gamma   0 DP(%s)->flag is  set   * *;
//## flag Gamma Quiet      1 DP(%s)->flag is  set   * *;
//## flag Gamma Throwable  2 DP(%s)->flag is  set   * *;
//## flag Gamma PROCEED    3 DP(%s)->flag has found * *;
//## flag Gamma REGISTER   4 DP(%s)->flag has found * *;
//## flag Gamma YEILD      5 DP(%s)->flag has found * *;
//## flag Gamma FIELD      6 DP(%s)->flag has found * *;
//## flag Gamma XLOCAL     7 DP(%s)->flag has found * *;
//## flag Gamma SCRIPT     8 DP(%s)->flag has found * *;

//## flag Gamma InlineFunction  0 DP(%s)->cflag is set * *;
//## flag Gamma TailRecursion   1 DP(%s)->cflag is set * *;

#ifndef K_GAMMAMAX
#define K_GAMMAMAX 64
#endif

typedef struct {
	knh_flag_t    flag  ;
	knh_short_t   ucnt  ;
	knh_type_t    type  ;
	knh_fieldn_t  fn    ;
	Object        *value;
	knh_Token_t   *tkIDX;
} knh_gmafields_t ;

#define K_GMASCR   ((ctx->gma)->scr)
#define K_GMANS    ((ctx->gma)->scr->ns)

typedef struct {
	knh_flag_t                 flag;
	knh_flag_t                 cflag;
//	struct knh_NameSpace_t*    ns;
//	struct knh_Script_t*       script;
	struct knh_Stmt_t*         stmt;
	struct knh_Method_t*       mtd;
	knh_class_t                this_cid;

	/*gamma*/
	struct knh_Gamma_t        *parentNULL;
	knh_gmafields_t*           gf; /* type environment */
	knh_ushort_t               gsize;
	knh_ushort_t               gcapacity;
	knh_short_t                ebpidx;
	knh_short_t                espidx;
	knh_ushort_t               psize; /* param size */
	knh_short_t                scridx;

	struct knh_BasicBlock_t    *bbNC;
	struct knh_Array_t         *insts;  // bbNC->listNC
	struct knh_Array_t         *lstacks;
	struct knh_Stmt_t          *finallyStmt;
	struct knh_Array_t         *errmsgs;
} knh_GammaEX_t;

typedef struct knh_Gamma_t {
	knh_hObject_t h;
	knh_GammaEX_t *b;
	knh_uline_t uline;         // same as Term
	knh_Script_t *scr;
} knh_Gamma_t;

typedef knh_bool_t (*knh_Ftyping)(CTX, knh_Method_t *, knh_Stmt_t *, knh_type_t, knh_Stmt_t *);

///* ------------------------------------------------------------------------ */

typedef knh_uintptr_t          knh_opcode_t;
typedef knh_intptr_t           knh_sfpidx_t;
typedef knh_intptr_t           knh_r_t;
typedef knh_intptr_t           knh_ro_t;
typedef knh_intptr_t           knh_rn_t;
typedef void*                  knh_dummyptr_t;

#define klr_Ffcall    knh_Fmethod
typedef void (*klr_Fth)(CTX, struct knh_opline_t *, void**);
typedef knh_Method_t* (*klr_Fmethod)(CTX, knh_sfp_t *, int, knh_Method_t*);
typedef long   (*klr_Finttr)(long u);
typedef double (*klr_Ffloattr)(double u);
typedef void (*klr_Ftr)(CTX, knh_sfp_t*, knh_sfpidx_t, const knh_ClassTBL_t *);
typedef void (*klr_Fcheckin)(CTX, knh_sfp_t *, knh_Object_t *);
typedef void (*klr_Fcheckout)(CTX, knh_Object_t *, int);
struct klr_P_t;
typedef void (*klr_Fprint)(CTX, knh_sfp_t*, struct klr_P_t*);
typedef void (*klr_Fprobe)(CTX, knh_sfp_t* , knh_sfpidx_t n, struct knh_opline_t *pc);

typedef struct {
	knh_dummyptr_t _d1;
	knh_dummyptr_t _d2;
	knh_dummyptr_t _d3;
	knh_dummyptr_t _d4;
	knh_dummyptr_t _d5;
} knh_opdummy_t;

typedef struct {
	knh_sfpidx_t i;
	knh_sfpidx_t n;
} knh_sfx_t;

#if defined(K_USING_THCODE_)
#define KCODE_HEAD \
	void *codeaddr; \
	size_t count; \
	knh_ushort_t opcode; \
	knh_ushort_t line

#else
#define KCODE_HEAD \
	size_t count; \
	knh_opcode_t opcode; \
	knh_uintptr_t line \

#endif/*K_USING_THCODE_*/

typedef struct knh_opline_t {
	KCODE_HEAD;
	union {
		knh_intptr_t data[5];
		void *p[5];
		knh_uchar_t *u[5];
	};
} knh_opline_t;

#define DBG_ASSERT_OPCODE(c) \
	if(((knh_opcode_t)c) >= OPCODE_MAX) {\
		DBG_P("Invalid opcode=%d", (int)c); \
		DBG_ASSERT(c < OPCODE_MAX); \
	}\

/* ------------------------------------------------------------------------ */
//## class BasicBlock Object;
//## flag BasicBlock Visited  1 - is set  *   *;
//## flag BasicBlock StackChecked 2 - is set * *;

typedef struct knh_BasicBlockEX_t {
	knh_ushort_t id;     knh_ushort_t incoming;
	knh_opline_t *opbuf;
	knh_ushort_t size;   knh_ushort_t capacity;
	knh_ushort_t bottom; knh_ushort_t top;
	knh_opline_t *code;
	knh_opline_t *opjmp;
} knh_BasicBlockEX_t ;

typedef struct knh_BasicBlock_t {
	knh_hObject_t h;
	knh_BasicBlockEX_t *b;
	struct knh_Array_t *listNC;
	struct knh_BasicBlock_t *nextNC;
	struct knh_BasicBlock_t *jumpNC;
} knh_BasicBlock_t ;

/* ------------------------------------------------------------------------ */
//## class KonohaCode Object;
//## flag KonohaCode NativeCompiled  1 - is set  *   *;

typedef struct knh_KonohaCode_t {
	knh_hObject_t h;
	knh_opline_t*  code;
	size_t        codesize;
	knh_String_t *source;
	knh_uri_t     uri; knh_uri_t     domain;
} knh_KonohaCode_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Immutable Object;

typedef struct knh_Immutable_t {
	knh_hObject_t h;
} knh_Immutable_t;

/* ------------------------------------------------------------------------ */
//## class KindOf Object;

typedef struct knh_KindOf_t {
	knh_hObject_t h;
} knh_KindOf_t;

/* ------------------------------------------------------------------------ */
//## @Cyclic class Tdynamic Object knh_dynamic_t;

typedef struct knh_RawPtr_t {
	knh_hObject_t h;
} knh_RawPtr_t ;

#define CLASS_dynamic   CLASS_Tdynamic
#define TYPE_dyn        TYPE_Tdynamic
#define TYPE_dynamic    TYPE_Tdynamic

/* ------------------------------------------------------------------------ */

#define SYSLOG_iZERODIV(ctx, sfp, n)
#define SYSLOG_fZERODIV(ctx, sfp, n)

#define SYSLOG_iZERODIV2(ctx, sfp, n)  \
	if(unlikely(n == 0)) { \
		THROW_Arithmetic(ctx, sfp, "zero divided"); \
	}\

#define SYSLOG_fZERODIV2(ctx, sfp, n)  \
	if(unlikely(n == K_FLOAT_ZERO)) { \
		THROW_Arithmetic(ctx, sfp, "zero divided"); \
	}\

#define K_CALLDELTA   4
#define K_RTNIDX    (-4)
#define K_SHIFTIDX  (-3)
#define K_PCIDX     (-2)
#define K_MTDIDX    (-1)
#define K_TMRIDX   0
#define K_SELFIDX   0

//#ifdef K_USING_RBP_
#define K_NEXTIDX    2
#define K_SHIFTIDX2  (-5)
#define K_PCIDX2     (-3)
#define K_MTDIDX2    (-1)
//#else
//#define K_NEXTIDX    1
//#define K_SHIFTIDX2  (-3)
//#define K_PCIDX2     (-2)
//#define K_MTDIDX2    (-1)
//#endif

#define klr_setesp(ctx, newesp)  ((knh_context_t*)ctx)->esp = (newesp)
#define klr_setmtdNC(ctx, sfpA, mtdO)   sfpA.mtdNC = mtdO

knh_opline_t* knh_VirtualMachine_run(CTX, knh_sfp_t *, knh_opline_t *);
#define Method_isKonohaCode(mtd) ((mtd)->fcall_1 == knh_Fmethod_runVM)

#define Gamma_hasJIT(ctx)     0
#define CODE_VEXEC     (ctx->share->PC_VEXEC)
#define CODE_LAUNCH    (ctx->share->PC_LAUNCH)
#define CODE_FUNCCALL  (ctx->share->PC_FUNCCALL)
//#define CODE_ABSTRACT  (ctx->share->PC_ABSTRACT)

#define KNH_SCALL(ctx, lsfp, rtnidx, mtdO, argc) { \
		knh_intptr_t thisidx_ = rtnidx+ K_CALLDELTA;\
		DBG_ASSERT(IS_Method(mtdO));\
		klr_setmtdNC(ctx, lsfp[thisidx_+K_MTDIDX], mtdO);\
		klr_setesp(ctx, lsfp + thisidx_ + argc + 1);\
		if(Method_isKonohaCode(mtdO)) {\
			knh_VirtualMachine_run(ctx, lsfp + thisidx_, CODE_VEXEC);\
		}else {\
			(mtdO)->fcall_1(ctx, lsfp + thisidx_, K_RTNIDX);\
		}\
		klr_setmtdNC(ctx, lsfp[thisidx_+K_MTDIDX], NULL);\
		klr_setesp(ctx, lsfp + rtnidx);\
	} \

#define KNH_SELFCALL(ctx, sfp, mtd, rix) { \
		(mtd)->fcall_1(ctx, sfp, rix);\
	} \

#define klr_settrlNC(ctx, sfpA, trlO)   sfpA.tmrNC = trlO

#define KNH_SCAST(ctx, lsfp, rtnidx, trl)  { \
		knh_TypeMap_t *trl_ = trl;\
		klr_settrlNC(ctx, lsfp[rtnidx], trl_);\
		(trl_)->ftypemap_1(ctx, lsfp + rtnidx, 0); \
	} \

#define BEGIN_LOCAL(ctx, lsfp, n) \
		knh_sfp_t *lsfp = knh_stack_local(ctx, n);\
		int sfpidx_ = lsfp - ctx->stack;\

#define UPDATE_LOCAL(ctx, lsfp)   lsfp = ctx->stack + sfpidx_;

#define END_LOCAL(ctx, lsfp, rvalue) \
	((knh_context_t*)ctx)->esp = ctx->stack + sfpidx_ + 1;\
	KNH_SETv(ctx, ((knh_context_t*)ctx)->esp[-1].o, rvalue);\
	knh_stack_gc(ctx, 0/*isALL*/);\

#define END_LOCAL_(ctx, lsfp) \
	((knh_context_t*)ctx)->esp = ctx->stack + sfpidx_;\
	knh_stack_gc(ctx, 0/*isALL*/);\

#define END_LOCAL_NONGC(ctx, lsfp) \
	((knh_context_t*)ctx)->esp = ctx->stack + sfpidx_;\

#define LOCAL_NEW(ctx, lsfp, n, T, V, O) \
	T V = O;\
	KNH_SETv(ctx, lsfp[n].o, V);\

#define KNH_SECURE(ctx, sfp) knh_checkSecurityManager(ctx, sfp)

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*CLASS_KONOHA_H_*/

