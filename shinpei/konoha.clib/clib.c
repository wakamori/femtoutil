/* Glue 
 *  ffi package, for DEOS
 *  written by shinpei(c)2011
 */
#define K_INTERNAL 1
#include <konoha1.h>
#include <konoha1/konohalang.h>
#include <konoha1/inlinelibs.h>


#if defined(K_USING_MACOSX_)
//#define MACOSX
#include <ffi/ffi.h>
#elif defined(K_USING_LINUX_)
#include <ffi.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Glue] */
#include <dglue.h> // for DEOS
// #include <glue.h>

/* ------------------------------------------------------------------------ */
/* [CStructure] */

typedef struct knh_Structure_t {
  size_t param_size;
  ffi_type **params;
  knh_type_t *param_knh_types;
  knh_bytes_t *param_name;
} knh_Structure_t ;

static void Structure_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Structure_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defStructure(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Structure";
	cdef->init = Structure_init;
	cdef->free = Structure_free;
}

/* ------------------------------------------------------------------------ */
/* [Clib] */

typedef struct knh_CLib_t {
  void *handler;
} knh_CLib_t;

static void Clib_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Clib_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_CLib_t *clib = (knh_CLib_t*)po->rawptr;
	  if (clib->handler != NULL) {
		knh_dlclose(ctx, clib->handler);
	  }
	  KNH_FREE(ctx, clib, sizeof(knh_CLib_t));
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defClib(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Clib";
	cdef->init = Clib_init;
	cdef->free = Clib_free;
}

/* ------------------------------------------------------------------------ */
/* [Dglue:internal] */
#define DGLUE_NOT_UNBOXED (1 << 0)
#define DGLUE_UNBOXED (1 << 1)
#define DGLUE_TYPE_NUMBER (1 << 2)
#define DGLUE_TYPE_STRUCTURE (1 << 3)
#define DGLUE_TYPE_STRING (1 << 4)
#define DGLUE_TYPE_MAP (1 << 5)
#define DGLUE_TYPE_ARRAY (1 << 6)
#define DGLUE_TYPE_CLOSURE (1 << 7)

#define IS_DGLUE_NOT_UNBOXED(b) ((b & DGLUE_NOT_UNBOXED) == DGLUE_NOT_UNBOXED)
#define IS_DGLUE_UNBOXED(b) ((b & DGLUE_UNBOXED) == DGLUE_UNBOXED)
#define IS_TYPE_STRUCTURE(b) ((b & DGLUE_TYPE_STRUCTURE) == DGLUE_TYPE_STRUCTURE)
#define IS_TYPE_STRING(b) ((b & DGLUE_TYPE_STRING) == DGLUE_TYPE_STRING)
#define IS_TYPE_MAP(b) ((b & DGLUE_TYPE_MAP) == DGLUE_TYPE_MAP)
#define IS_TYPE_ARRAY(b) ((b & DGLUE_TYPE_ARRAY) == DGLUE_TYPE_ARRAY)
#define IS_TYPE_CLOSURE(b) ((b & DGLUE_TYPE_CLOSURE) == DGLUE_TYPE_CLOSURE)

#define CLIB_ARGMAX (5)

typedef struct knh_ClibFunc_t {
  int idx;
  void *fptr; /* function pointer */
  ffi_cif cif;
  int argCount;
  ffi_type *retT;
  int retT_original;
  ffi_type *argT[CLIB_ARGMAX];
  int argT_original[CLIB_ARGMAX];
  void *argV[CLIB_ARGMAX];
  void *retV;
} knh_ClibFunc_t;

#define CLIBFUNC_MAX (256)

typedef struct knh_ClibGlue_t {
  size_t num_func;
  knh_ClibFunc_t **funcs;
} knh_ClibGlue_t;

static void ClibFunc_init(CTX ctx, knh_ClibFunc_t *clibfunc)
{
  int i;
  clibfunc->idx = 0;
  clibfunc->fptr = NULL;
  clibfunc->argCount = 0;
  for (i = 0; i < CLIB_ARGMAX; i++) {
	clibfunc->argT[i] = NULL;
	clibfunc->argT_original[i] = 0;
	clibfunc->argV[i] = NULL;
  }
  clibfunc->retV = NULL;
}

static void ClibGlue_init(CTX ctx, knh_ClibGlue_t *cglue)
{
  cglue->num_func = 0;
  cglue->funcs = NULL;
}

static void ClibGlue_free(CTX ctx, void *ptr)
{
  if (ptr != NULL) {
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)ptr;
	KNH_FREE(ctx, cglue, sizeof(knh_ClibGlue_t));
	ptr = NULL;
  }
}

/* ------------------------------------------------------------------------ */
static knh_type_t knh_type_map[] = {
  TYPE_Object,
  TYPE_Tvoid,
  TYPE_Int, /* 8 */
  TYPE_Int,
  TYPE_Int, /* 16 */
  TYPE_Int,
  TYPE_Int, /* 32 */
  TYPE_Int,
  TYPE_Int, /* 64 */
  TYPE_Int,
  TYPE_Float,
  TYPE_Float,
  TYPE_Int, /* char */
  TYPE_Int,
  TYPE_Int, /* short */
  TYPE_Int,  
  TYPE_Int, /* int */
  TYPE_Int,
  TYPE_Int, /* long */
  TYPE_Int,
  TYPE_Float, /* longdouble*/
  TYPE_String
};

static ffi_type *ffi_type_map[] = {
  &ffi_type_pointer,
  &ffi_type_void,
  &ffi_type_uint8,
  &ffi_type_sint8,
  &ffi_type_uint16,
  &ffi_type_sint16,
  &ffi_type_uint32,
  &ffi_type_sint32,
  &ffi_type_uint64,
  &ffi_type_sint64,
  &ffi_type_float,
  &ffi_type_double,
  &ffi_type_uchar,
  &ffi_type_schar,
  &ffi_type_ushort,
  &ffi_type_sshort,
  &ffi_type_uint,
  &ffi_type_sint,
  &ffi_type_ulong,
  &ffi_type_slong,
  &ffi_type_longdouble,
  &ffi_type_pointer // STRING
};

static knh_IntData_t StructureConstInt[] = {
  {"FFI_POINTER", 1},
  {"FFI_VOID", 2},
  {"FFI_UINT8", 3},
  {"FFI_SINT8", 4},
  {"FFI_UINT16", 5},
  {"FFI_SINT16", 6},
  {"FFI_UINT32", 7},
  {"FFI_SINT32", 8},
  {"FFI_UINT64", 9},
  {"FFI_SINT64", 11},
  {"FFI_FLOAT", 12},
  {"FFI_DOUBLE", 13},
  {"FFI_UCHAR", 14},
  {"FFI_SCHAR", 15},
  {"FFI_USHORT", 16},
  {"FFI_SSHORT", 17},
  {"FFI_UINT", 18},
  {"FFI_SINT", 19},
  {"FFI_ULONG", 20},
  {"FFI_SLONG", 21},
  {"FFI_LONGDOUBLE", 22},
  {"FFI_STRING", 25},
  {NULL, 0}
};

DEFAPI(void) constStructure(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
  kapi->loadIntClassConst(ctx, cid, StructureConstInt);
}

/* ------------------------------------------------------------------------ */
//@Native Structure Structure.new(Array<Array<dynamic> values, Structure _)
METHOD Structure_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Array_t *a = sfp[1].a;
  knh_Structure_t *strc = (knh_Structure_t*)KNH_MALLOC(ctx, sizeof(knh_Structure_t));
  int i = 0, j = 0;
  size_t a_size = knh_Array_size(a);
  strc->param_size = a_size;
  strc->param_name = (knh_bytes_t*)KNH_MALLOC(ctx, sizeof(knh_bytes_t) * strc->param_size);
  strc->params = (ffi_type**)KNH_MALLOC(ctx, sizeof(ffi_type*) * strc->param_size);
  strc->param_knh_types = (knh_type_t*)KNH_MALLOC(ctx, sizeof(knh_type_t) * strc->param_size);
  for (i = 0; i < a_size; i++) {
	// param size;
	knh_Array_t *aa = (knh_Array_t *)knh_Array_n(a, i);
	//size_t aa_size = knh_Array_size(aa);
	{
	  // suppose its [<String>, <Int>]
	  knh_String_t *v1 = (knh_String_t *)knh_Array_n(aa, 0);
	  knh_Int_t *v2 = (knh_Int_t *)knh_Array_n(aa, 1);
	  strc->param_name[j] = S_tobytes(v1);
	  strc->params[j] = ffi_type_map[v2->n.ivalue];
	  strc->param_knh_types[j] = knh_type_map[v2->n.ivalue];
	}
  }
  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[2].p, strc);
  RETURN_(po);
}

/* ------------------------------------------------------------------------ */
// clibfuncs

static knh_ClibFunc_t* new_ClibFunc(CTX ctx)
{
  knh_ClibFunc_t *clibfunc = (knh_ClibFunc_t*)KNH_MALLOC(ctx, sizeof(knh_ClibFunc_t));
  ClibFunc_init(ctx, clibfunc);
  return clibfunc;
}

static int ClibGlue_add_ClibFunc(CTX ctx, knh_ClibGlue_t *cglue, knh_ClibFunc_t *clibfunc)
{
  if (cglue->num_func >= CLIBFUNC_MAX) return -1;
  cglue->funcs[cglue->num_func] = clibfunc;
  cglue->num_func++;
  return cglue->num_func;
}

/* ------------------------------------------------------------------------ */
//@Native Clib Clib.new(String libname, Clib _);
METHOD Clib_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  const char *libname = String_to(const char *, sfp[1]);
  knh_CLib_t *clib = (knh_CLib_t*)KNH_MALLOC(ctx, sizeof(knh_CLib_t));
  clib->handler = knh_dlopen(ctx, libname);
  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[2].p, clib);
  RETURN_(po);
}

static METHOD Fmethod_wrapCLib(CTX ctx, knh_sfp_t *sfp _RIX)
{
  //  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  //  knh_param_t *rp = knh_ParamArray_rget(DP(sfp[K_MTDIDX].mtdNC)->mp, 0);
  knh_Func_t *fo = sfp[0].fo;
  knh_Method_t *mtd = fo->mtd;
  knh_param_t *rp = knh_ParamArray_rget(DP(mtd)->mp, 0);
  knh_type_t rtype = rp->type;
  knh_ClibFunc_t *clibfunc = (knh_ClibFunc_t*)(((fo->mtd)->b)->cfunc);
  fprintf(stderr, "wrapFunc,idx:%d ftype:%s, rtype:%s\n", clibfunc->idx, TYPE__(O_cid(fo)), TYPE__(rtype));
  fprintf(stderr, "cfunc:%p\n", clibfunc->fptr);
  int idx = 0;
  // push arg values
  for (idx = 0; idx < clibfunc->argCount; idx++) {
	if (IS_DGLUE_UNBOXED(clibfunc->argT_original[idx])) {
	  clibfunc->argV[idx] = &(sfp[idx+1].ndata);
	  fprintf(stderr, "val:%d:%d\n", idx+1, sfp[idx+1].ivalue);
	} else {
	  //TODO: now, we cannot distinguish object from string
	  if (IS_TYPE_STRING(clibfunc->argT_original[idx])) {
		clibfunc->argV[idx] = &((sfp[idx+1].s)->str.text);
	  } else if (IS_TYPE_STRUCTURE(clibfunc->argT_original[idx])) {
		clibfunc->argV[idx] = &((sfp[idx+1].p)->rawptr);
	  } else {
		// TODO? : array? map?
		clibfunc->argV[idx] = &(sfp[idx+1].o);
	  }
	}
  } /* for loop for argT*/

  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  if (rtype == TYPE_Int || rtype == TYPE_Boolean) {
		knh_int_t return_i = 0;
		if (ffi_prep_cif(&(clibfunc->cif), FFI_DEFAULT_ABI, clibfunc->argCount,
						 clibfunc->retT, clibfunc->argT) == FFI_OK) {
		  ffi_call(&(clibfunc->cif), clibfunc->fptr, &(clibfunc->retV), clibfunc->argV);
		  return_i = *(knh_int_t*)(&clibfunc->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURNi_(return_i);
	  } else if (rtype == TYPE_Float) {
		double return_f = 0.0;
		if (ffi_prep_cif(&(clibfunc->cif), FFI_DEFAULT_ABI, clibfunc->argCount,
						 clibfunc->retT, clibfunc->argT) == FFI_OK) {
		  ffi_call(&(clibfunc->cif), clibfunc->fptr, &(clibfunc->retV), clibfunc->argV);
		  return_f = *(double*)(&clibfunc->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURNf_(return_f);
	  } 
	} else { // IS not unbox 
	  fprintf(stderr, "ffi called\n");
	  if (rtype == TYPE_String) {
		// its String
		char *return_s = NULL;
		if (ffi_prep_cif(&(clibfunc->cif), FFI_DEFAULT_ABI, clibfunc->argCount,
						 clibfunc->retT, clibfunc->argT) == FFI_OK) {
		  ffi_call(&(clibfunc->cif), clibfunc->fptr, &(clibfunc->retV), clibfunc->argV);
		  return_s = *(char**)(&clibfunc->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURN_(new_String(ctx, return_s));
	  } else {
		// its Object
		fprintf(stderr, "returning rawptr\n");
		void *return_ptr = NULL;
		if (ffi_prep_cif(&(clibfunc->cif), FFI_DEFAULT_ABI, clibfunc->argCount,
						 clibfunc->retT, clibfunc->argT) == FFI_OK) {
		  ffi_call(&(clibfunc->cif), clibfunc->fptr, &(clibfunc->retV), clibfunc->argV);
		  return_ptr = *(void**)(&clibfunc->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		fprintf(stderr, "rawptr:'%p'\n", return_ptr);
		RETURN_(new_RawPtr(ctx, (knh_RawPtr_t*)KNH_NULVAL(CLASS_Tvar), return_ptr));
	  }
	} // end of IS_Tunbox 
  } // end of is_VOID
  RETURNvoid_();
}

static knh_RawPtr_t *ClibGlue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Glue_t *glue = (knh_Glue_t*)((sfp[0].p)->rawptr);
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)(glue->glueInfo);
  knh_CLib_t *clib = (knh_CLib_t*)glue->componentInfo;

  if (clib == NULL) {
	fprintf(stderr, "invalid Dglue\n");
	return (knh_RawPtr_t*)(sfp[3].o);
  }
  const char *symstr = String_to(const char *, sfp[1]);
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;

  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  knh_ClibFunc_t *clibfunc = new_ClibFunc(ctx);
  if ((clibfunc->idx = ClibGlue_add_ClibFunc(ctx, cglue, clibfunc)) == -1) {
	fprintf(stderr, "cannot add clibfunc, there are %ld funcs already.\n", cglue->num_func);
	return (knh_RawPtr_t*)(sfp[3].o);
  }

  if ((clibfunc->fptr = knh_dlsym(ctx, clib->handler, symstr, 0))== NULL) {
	fprintf(stderr, "dlsym_ERROR!!!\n");
  }
  // type a method from requested type
  size_t argCount = pa->psize;
  clibfunc->argCount = argCount;

  // return value typing
  DBG_ASSERT(pa->rsize == 1);
  knh_param_t *p = knh_ParamArray_rget(pa, 0);
  //  fprintf(stderr, "rtype:%s\n", TYPE__(p->type));
  if (p->type == TYPE_Int || p->type == TYPE_Boolean) {
	clibfunc->retT = &ffi_type_sint64;
	clibfunc->retT_original = DGLUE_UNBOXED;
  } else if (p->type == TYPE_Float) {
	clibfunc->retT = &ffi_type_double;
	clibfunc->retT_original = DGLUE_UNBOXED;
  } else if (p->type == TYPE_String) {
	clibfunc->retT = &ffi_type_pointer;
	clibfunc->retT_original = DGLUE_NOT_UNBOXED | DGLUE_TYPE_STRING;
  } else {
	// what about class, that made from Structure
	// it is also a pointer
	clibfunc->retT = &ffi_type_pointer;
	clibfunc->retT_original = DGLUE_NOT_UNBOXED | DGLUE_TYPE_STRUCTURE;
  }

  // arguments typing
  size_t idx = 0;
  for (idx = 0; idx < argCount; idx++) {
	knh_param_t *p = knh_ParamArray_get(pa, idx);
	if (p->type == TYPE_Int || p->type == TYPE_Boolean) {
	  clibfunc->argT[idx] = &ffi_type_sint64;
	  clibfunc->argT_original[idx] = DGLUE_UNBOXED;
	} else if (p->type == TYPE_Float) {
	  clibfunc->argT[idx] = &ffi_type_double;
	  clibfunc->argT_original[idx] = DGLUE_UNBOXED;
	} else if (p->type == TYPE_String) {
	  clibfunc->argT[idx] = &ffi_type_pointer;
	  clibfunc->argT_original[idx] = DGLUE_NOT_UNBOXED | DGLUE_TYPE_STRING;

	} else {
	  // here, we have to distinguish Structure class from the others
	  // suppose they are Structure;
	  clibfunc->argT[idx] = &ffi_type_pointer;
	  clibfunc->argT_original[idx] = DGLUE_NOT_UNBOXED | DGLUE_TYPE_STRUCTURE;
	}
  }
  // type Func object
  knh_Func_t *ret_func = (knh_Func_t*)new_Object_init2(ctx, tbl);
  //  fo->h.cTBL= tbl;
  // set wrapper method
  //  knh_Method_t *mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapCLib);
  knh_Method_t *mtd = ret_func->mtd;
  knh_Method_setFunc(ctx, mtd, Fmethod_wrapCLib);
  mtd->b->cfunc = (void*)clibfunc;
  mtd->cid = O_cid(klass);
  KNH_SETv(ctx, ((mtd)->b)->mp, pa);
  return (knh_RawPtr_t*)ret_func;
}

static knh_GlueSPI_t CLibGlueSPI = {
  ClibGlue_getFunc,
  /*  Clib_component_free*/ NULL,
  ClibGlue_free
};

/* ------------------------------------------------------------------------ */
// @Native Glue Clib_genGlue (Glue _)
METHOD Clib_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)((sfp[0].p)->rawptr);
  if (clib != NULL) {
	knh_Glue_t *glue = new_Glue(ctx);
	glue->glueType = GLUE_TYPE_INTERNAL;
	glue->gapi = &CLibGlueSPI;
	glue->componentInfo = (void*)clib;
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)KNH_MALLOC(ctx, sizeof(knh_ClibGlue_t));
	ClibGlue_init(ctx, cglue);
	cglue->funcs = (knh_ClibFunc_t**)KNH_MALLOC(ctx, sizeof(knh_ClibFunc_t*) * CLIBFUNC_MAX);
	glue->glueInfo = (void*)cglue;
	RETURN_(new_RawPtr(ctx, sfp[1].p, glue));
  }
  RETURN_(sfp[1].p);
}

/* ------------------------------------------------------------------------ */

#define _FGETTER     FLAG_Field_Getter
#define _FSETTER     FLAG_Field_Setter

static METHOD Fmethod_structgetter(CTX ctx, knh_sfp_t *sfp _RIX)
{
  //	int delta = DP(sfp[K_MTDIDX].mtdNC)->delta;
  //	RETURN_((sfp[0].ox)->fields[delta]);
  
}

static knh_Method_t *new_StructGetter(CTX ctx, knh_class_t cid, knh_methodn_t mn, knh_type_t type, int idx)
{
  knh_Fmethod f = Fmethod_structgetter;
  knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, f);
  DP(mtd)->delta = idx;
  KNH_SETv(ctx, DP(mtd)->mp, new_ParamArrayR0(ctx, type));
  return mtd;
}

// @Native void Clib_defineClass (String classname, Structure strc, Namespace _)
METHOD Clib_defineClass(CTX ctx, knh_sfp_t *sfp _RIX)
{
  char *classname = String_to(char *, sfp[1]);
  knh_String_t *str_classname = sfp[1].s;
  knh_ClassTBL_t *ct = NULL;
  knh_RawPtr_t *po = sfp[2].p;
  knh_Structure_t *strc = (knh_Structure_t*)po->rawptr;
  knh_NameSpace_t *ns = sfp[3].ns;
  knh_class_t cid;
  int i = 0;

  knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
  knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(ns)->nsname));
  knh_Bytes_putc(ctx, cwb->ba, '.');
  knh_Bytes_write(ctx, cwb->ba, B(classname));
  cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));

  // CLASS_decl
  if  (cid == CLASS_unknown) {
	//new class
	cid = new_ClassId(ctx);
	ct = varClassTBL(cid);
	knh_String_t *name = knh_cwb_newString(ctx, cwb);
	knh_setClassName(ctx, cid, name, str_classname);
	//	ct->cflag  = knh_StmtCLASS_flag(ctx, stmt);
	ct->cflag = 0; // none
	ct->magicflag  = KNH_MAGICFLAG(ct->cflag);
	//	NameSpace_setcid(ctx, ns, knh_cwb_newString(ctx, cwb), cid);
	{
	  if(DP(ns)->name2cidDictSetNULL == NULL) {
		KNH_INITv(DP(ns)->name2cidDictSetNULL, new_DictSet0(ctx, 0, 1/*isCaseMap*/, "NameSpace.name2cid"));
	  } else {
		knh_uintptr_t oldcid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, S_tobytes(name));
		if(oldcid != 0 && cid != oldcid - 1) {
		  WARN_AlreadyDefinedClass(ctx, cid, (knh_class_t)(oldcid - 1));
		  return;
		}
	  }
	  //	  fprintf(stderr, "set!!:%d, name:%s, ns:%p, n2cid:%p\n", cid, S_tochar(name), ns, DP(ns)->name2cidDictSetNULL);
	  knh_DictSet_set(ctx, DP(ns)->name2cidDictSetNULL, str_classname, (knh_uintptr_t)(cid+1));
	}
	KNH_INITv(ct->methods, K_EMPTYARRAY);
	KNH_INITv(ct->typemaps, K_EMPTYARRAY);

	ct->supTBL = ClassTBL(ct->supcid);
	ct->keyidx = ct->supTBL->keyidx;
	ct->metaidx = ct->supTBL->metaidx;
	((knh_ClassTBL_t*)ct->supTBL)->subclass += 1;

	{
	  LOGSFPDATA = {cDATA("name", cid), iDATA("cid", cid)};
	  LIB_OK("konoha.dffi:new_class");
	}

	knh_Object_t *obj = new_hObject_(ctx, ct);
	knh_Object_t *tmp = new_hObject_(ctx, ct);
	Object_setNullObject(obj, 1);
	ct->bcid = CLASS_Object;
	ct->baseTBL = ClassTBL(CLASS_Object);
	knh_setClassDef(ctx, ct, ct->baseTBL->cdef);
	obj->ref = NULL; tmp->ref = NULL;
	knh_setClassDefaultValue(ctx, cid, obj, NULL);
	KNH_INITv(ct->protoNULL, tmp);
	if(IS_Tfield(cid)) {
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
			}
		}
	}
  } else {
	ct = varClassTBL(cid);
	if (!(ct->bcid == CLASS_Object && ct->fields == NULL)) {
	  fprintf(stderr, "redefining class %s\n", classname);
	  return;
	} else {
	  //TODO()
	  fprintf(stderr, "alreaady class!\n");
	}
  }
  
  /* make fields */
  // basic flow.
  // DECLFIELD_typing
  //  -decl_flag(_fgetter, _fsetter
  //  -decl3_typing(..)
  // Gamma_declareClassField
  //  -add_classField

  //  knh_flag_t flag  = _FGETTER | _FSETTER;
  knh_flag_t flag  = 0;
  for (i = 0; i < strc->param_size; i++) {
	knh_fieldn_t fn = knh_getfnq(ctx, strc->param_name[i], FN_NEWID);
	if (strc->param_knh_types[i] == TYPE_Object) {
	  knh_class_addField(ctx, cid, flag, TYPE_Object, fn);
	} else if (strc->param_knh_types[i] == TYPE_String) {
	  // its String
	  knh_class_addField(ctx, cid, flag, TYPE_String, fn);
	} else if (strc->param_knh_types[i] == TYPE_Int) {
	  // its Float
	  knh_class_addField(ctx, cid, flag, TYPE_Int, fn);
	} else {
	  // its Integer or void
	  knh_class_addField(ctx, cid, flag, TYPE_Float, fn);
	}
  }
  size_t fsize = ct->fsize;
  knh_ObjectField_t *of;

  knh_ObjectField_expand(ctx, ct->protoNULL, fsize, ct->fsize);
  of = ct->protoNULL;
  size_t fi = 0;
  for(i = 0; i < strc->param_size; i++) {
	//	ObjectField_add(ctx, ct->protoNULL, fsize + fi, TYPE_Int, gf[i].tk);
	if (strc->param_knh_types[i] == TYPE_Int) {
	  knh_int_t *nn = (knh_int_t *)(of + (fsize + fi));
	  nn[0] = 0;
	  fi++;
	  DBLNDATA_(if(IS_Tunbox(TYPE_Int)) fi++;);
	} else if (strc->param_knh_types[i] == TYPE_Float) {
	  knh_float_t *nn = (knh_float_t *)(of + (fsize + fi));
	  nn[0] = 0.0;
	  fi++;
	  DBLNDATA_(if(IS_Tunbox(TYPE_Float)) fi++;);
	} else {
	  // object , or void
	  KNH_INITv(of->fields[i], KNH_NULVAL(knh_type_tocid(ctx, strc->param_knh_types[i], O_cid(of))));
	}
  }

  knh_ObjectField_expand(ctx, ct->defobj, fsize, ct->fsize);
  of = ct->defobj;
  fi = 0;
  knh_Method_t *mtd = NULL;
  for(i = 0; i < strc->param_size; i++) {
	//	ObjectField_add(ctx, ct->defobj, fsize + fi, TYPE_Int, NULL);
	if (strc->param_knh_types[i] == TYPE_Int) {
	  knh_int_t *nn = (knh_int_t *)(of + (fsize + fi));
	  nn[0] = 0;
	  fi++;
	  DBLNDATA_(if(IS_Tunbox(TYPE_Int)) fi++;);
	} else if (strc->param_knh_types[i] == TYPE_Float) {
	  knh_float_t *nn = (knh_float_t *)(of + (fsize + fi));
	  nn[0] = 0.0;
	  fi++;
	  DBLNDATA_(if(IS_Tunbox(TYPE_Float)) fi++;);
	} else {
	  // object or void
	  KNH_INITv(of->fields[i], KNH_NULVAL(knh_type_tocid(ctx, strc->param_knh_types[i], O_cid(of))));
	  //	  mtd = new_StructGetter(ctx, cid, 
	}
  }
  knh_ClassTBL_setObjectCSPI(ctx, ct);
  
  // add constructor
  mtd = new_Method(ctx, 0, cid, MN_new, NULL); // we can make FMethod as a method
  knh_ParamArray_t *pa2 = new_ParamArray(ctx);
  KNH_SETv(ctx, DP(mtd)->mp, pa2);
  knh_ClassTBL_addMethod(ctx, ct, mtd, 0 /* ischeck*/);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

