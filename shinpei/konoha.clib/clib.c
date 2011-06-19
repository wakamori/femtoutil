#include <konoha1.h>

//#define MACOSX
#include <ffi/ffi.h>
#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Clib] */

#define CLIB_ARGMAX 3

typedef struct knh_CLib_t {
  void *handler;
} knh_CLib_t;


static void Clib_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_CLib_t));
}

static void Clib_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_CLib_t *clib = (knh_CLib_t*)po->rawptr;
	  if (clib->handler != NULL) {
		dlclose(clib->handler);
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

#define DGLUE_NOT_UNBOXED 0
#define DGLUE_UNBOXED 1


typedef struct knh_DGlue_t {
  knh_CLib_t *clib;
  void *fptr;

  ffi_cif cif;
  int argCount;
  ffi_type *retT;
  ffi_type *argT[CLIB_ARGMAX];
  int argT_isUnboxed[CLIB_ARGMAX];
  void *argV[CLIB_ARGMAX];
  void *retV;
} knh_DGlue_t;

static void Dglue_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_DGlue_t));
  knh_DGlue_t *dg = (knh_DGlue_t*)po->rawptr;
  int i;
  dg->clib = NULL;
  dg->fptr = NULL;
  dg->argCount = 0;
  for (i = 0; i < CLIB_ARGMAX; i++) {
	dg->argT[i] = NULL;
	dg->argT_isUnboxed[i] = 0;
	dg->argV[i] = NULL;
  }
  dg->retV = NULL;
}

static void Dglue_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_DGlue_t *dg = (knh_DGlue_t*)po->rawptr;
	  if (dg->clib != NULL) Clib_free(ctx, (knh_RawPtr_t*)dg->clib);
	  po->rawptr = NULL;
	}
}


DEFAPI(void) defDglue(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Dglue";
	cdef->init = Dglue_init;
	cdef->free = Dglue_free;
}

/* ------------------------------------------------------------------------ */

// Clib Clib.new(String libname) 
METHOD Clib_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)(sfp[0].p->rawptr);
  const char *libname = String_to(const char *, sfp[1]);
  clib->handler = dlopen(libname, RTLD_LAZY);
  RETURN_(sfp[0].o);
}

static METHOD Fmethod_wrapCLib(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_DGlue_t *dg = (knh_DGlue_t*)(((fo->mtd)->b)->cfunc);
  //  fprintf(stderr, "fptr:%p, %p, %d, %p, %p\n", 
  //		  dg->fptr,
  //		  &(dg->cif), dg->argCount,
  //		  dg->retT, dg->argT);
  
  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  int idx = 0;
	  for (idx = 0; idx < dg->argCount; idx++) {
		if (dg->argT_isUnboxed[idx] == DGLUE_UNBOXED) {
		  dg->argV[idx] = &(sfp[idx+1].ndata);
		} else {
		  dg->argV[idx] = &(sfp[idx+1].o);
		}
	  }
	  //	  double arg1 = Float_to(double, sfp[1]);
	  //	  (dg->argV[0]) = &arg1;
	  if (rtype == TYPE_Int || rtype == TYPE_Boolean) {
		knh_int_t return_i = 0;
		if (ffi_prep_cif(&(dg->cif), FFI_DEFAULT_ABI, dg->argCount,
						 dg->retT, dg->argT) == FFI_OK) {
		  ffi_call(&(dg->cif), dg->fptr, &(dg->retV), dg->argV);
		  return_i = *(knh_int_t*)(&dg->retV);
		}
		RETURNi_(return_i);
	  } else if (rtype == TYPE_Float) {
		double return_f = 0.0;
		if (ffi_prep_cif(&(dg->cif), FFI_DEFAULT_ABI, dg->argCount,
						 dg->retT, dg->argT) == FFI_OK) {
		  ffi_call(&(dg->cif), dg->fptr, &(dg->retV), dg->argV);
		  return_f = *(double*)(&dg->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURNf_(return_f);
	  } 
	} else {
	  // its Object
	  RETURN_(KNH_NULVAL(CLASS_t(rtype)));
	}
  }
}

// @Native Dglue Clib_genDglue (Dglue _)
METHOD Clib_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)((sfp[0].p)->rawptr);
  knh_RawPtr_t *po = sfp[1].p;

  if (clib != NULL) {
	knh_DGlue_t *dg = (knh_DGlue_t*)po->rawptr;
	dg->clib = clib;
	RETURN_(po);
  }
  RETURN_(sfp[1].p);
}


/* @Native var Dglue.getFunc(String sym, Class _, Func _); */
METHOD Dglue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_DGlue_t *dg = (knh_DGlue_t*)((sfp[0].p)->rawptr);
  knh_CLib_t *clib = dg->clib;
  if (dg->clib == NULL) {fprintf(stderr, "invalid Dglue\n"); RETURN_(sfp[0].o);}
  const char *symstr = String_to(const char *, sfp[1]);
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;
  knh_Func_t *fo = (knh_Func_t *)sfp[3].o;

  // requested type
  //  fprintf(stderr, "%s %s\n", CLASS__(O_cid(klass)), CLASS__(klass->cid));
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  if ((dg->fptr = dlsym(clib->handler, symstr))== NULL) {
	fprintf(stderr, "dlsym_ERROR!!!\n");
  }

  // type a method from requested type
  // retT
  size_t argCount = pa->rsize;
  dg->argCount = argCount;
  if (argCount < 3) {
	if (pa->p0.type == TYPE_Int) {
	  dg->retT = &ffi_type_uint64;
	} else if (pa->p0.type == TYPE_Float) {
	  dg->retT = &ffi_type_double;
	} else {
	  // TODO!!
	}
  } else {
	TODO();
  }

  //argT
  if (argCount < 3) {
	if (argCount == 1) {
	  if (pa->p1.type == TYPE_Int) {
		dg->argT[0] = &ffi_type_uint64;
		dg->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else if (pa->p1.type == TYPE_Float) {
		dg->argT[0] = &ffi_type_double;
		dg->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else {
		TODO();
	  }
	} else if (argCount == 2) {
	  if (pa->p1.type == TYPE_Int) {
		dg->argT[0] = &ffi_type_uint64;
		dg->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else if (pa->p1.type == TYPE_Float) {
		dg->argT[0] = &ffi_type_double;
		dg->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else {
		TODO();
	  }

	  if (pa->p2.type == TYPE_Int) {
		dg->argT[1] = &ffi_type_uint64;
		dg->argT_isUnboxed[1] = DGLUE_UNBOXED;
	  } else if (pa->p2.type == TYPE_Float) {
		dg->argT[1] = &ffi_type_double;
		dg->argT_isUnboxed[1] = DGLUE_UNBOXED;
	  } else {
		TODO();
	  }
	} // there are no more... argCount == 0 ?
  } else {
	// more than 3 args
	TODO();
  }
  // type Func object
  fo->h.cTBL= tbl;
  //cid = knh_class_Generics(ctx, CLASS_Func, pa);
  
  // set wrapper method
  knh_Method_t *mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapCLib);
  mtd->b->cfunc = (void*)dg;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  RETURN_(fo);
}



/* ------------------------------------------------------------------------ */


#ifdef _SETUP
DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "clib");
	kapi->setPackageProperty(ctx, "version", "0.0");
	RETURN_PKGINFO("konoha.clib");
}
#endif /* _SETUP */

#ifdef __cplusplus
}
#endif

