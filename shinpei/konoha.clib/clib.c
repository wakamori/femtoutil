#include <konoha1.h>

#include <dglue.h> // for DEOS
// #include <glue.h>


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
  knh_CLib_t *clib  = (knh_CLib_t*)po->rawptr;
  clib->handler = NULL;
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

#define DGLUE_NOT_UNBOXED 0
#define DGLUE_UNBOXED 1

typedef struct knh_ClibGlue_t {
  void *fptr;
  ffi_cif cif;
  int argCount;
  ffi_type *retT;
  ffi_type *argT[CLIB_ARGMAX];
  int argT_isUnboxed[CLIB_ARGMAX];
  void *argV[CLIB_ARGMAX];
  void *retV;
} knh_ClibGlue_t;

  /*typedef struct knh_DGlue_t {
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
  */

static void ClibGlue_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_ClibGlue_t));
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)po->rawptr;
  int i;
  cglue->fptr = NULL;
  cglue->argCount = 0;
  for (i = 0; i < CLIB_ARGMAX; i++) {
	cglue->argT[i] = NULL;
	cglue->argT_isUnboxed[i] = 0;
	cglue->argV[i] = NULL;
  }
  cglue->retV = NULL;
}

static void Iglue_free(CTX ctx, void *ptr)
{
  if (ptr != NULL) {
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)ptr;
	KNH_FREE(ctx, cglue, sizeof(knh_ClibGlue_t));
	ptr = NULL;
  }
}

/*
DEFAPI(void) defDglue(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Dglue";
	cdef->init = Dglue_init;
	cdef->free = Dglue_free;
}
*/


/* ------------------------------------------------------------------------ */
// Clib Clib.new(String libname) 
METHOD Clib_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)(sfp[0].p->rawptr);
  const char *libname = String_to(const char *, sfp[1]);
  clib->handler = knh_dlopen(ctx, libname);
  RETURN_(sfp[0].o);
}

static METHOD Fmethod_wrapCLib(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)(((fo->mtd)->b)->cfunc);
  //  fprintf(stderr, "fptr:%p, %p, %d, %p, %p\n", 
  //		  dg->fptr,
  //		  &(dg->cif), dg->argCount,
  //		  dg->retT, dg->argT);
  
  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  int idx = 0;
	  for (idx = 0; idx < cglue->argCount; idx++) {
		if (cglue->argT_isUnboxed[idx] == DGLUE_UNBOXED) {
		  cglue->argV[idx] = &(sfp[idx+1].ndata);
		} else {
		  cglue->argV[idx] = &(sfp[idx+1].o);
		}
	  }
	  //	  double arg1 = Float_to(double, sfp[1]);
	  //	  (cglue->argV[0]) = &arg1;
	  if (rtype == TYPE_Int || rtype == TYPE_Boolean) {
		knh_int_t return_i = 0;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_i = *(knh_int_t*)(&cglue->retV);
		}
		RETURNi_(return_i);
	  } else if (rtype == TYPE_Float) {
		double return_f = 0.0;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_f = *(double*)(&cglue->retV);
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

static void CLibGlue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Glue_t *glue = (knh_Glue_t*)((sfp[0].p)->rawptr);
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)(glue->glueInfo);
  knh_CLib_t *clib = (knh_CLib_t*)glue->componentInfo;
  if (clib == NULL) {fprintf(stderr, "invalid Dglue\n"); RETURN_(sfp[0].o);}
  const char *symstr = String_to(const char *, sfp[1]);
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;
  knh_Func_t *fo = (knh_Func_t *)sfp[3].o;

  // requested type
  //  fprintf(stderr, "%s %s\n", CLASS__(O_cid(klass)), CLASS__(klass->cid));
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  if ((cglue->fptr = knh_dlsym(ctx, clib->handler, symstr, 0))== NULL) {
	fprintf(stderr, "dlsym_ERROR!!!\n");
  }

  // type a method from requested type
  // retT
  size_t argCount = pa->rsize;
  cglue->argCount = argCount;
  if (argCount < 3) {
	if (pa->p0.type == TYPE_Int) {
	  cglue->retT = &ffi_type_uint64;
	} else if (pa->p0.type == TYPE_Float) {
	  cglue->retT = &ffi_type_double;
	} else {
	  TODO();
	}
  } else {
	TODO();
  }

  //argT
  if (argCount < 3) {
	if (argCount == 1) {
	  if (pa->p1.type == TYPE_Int) {
		cglue->argT[0] = &ffi_type_uint64;
		cglue->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else if (pa->p1.type == TYPE_Float) {
		cglue->argT[0] = &ffi_type_double;
		cglue->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else {
		TODO();
	  }
	} else if (argCount == 2) {
	  if (pa->p1.type == TYPE_Int) {
		cglue->argT[0] = &ffi_type_uint64;
		cglue->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else if (pa->p1.type == TYPE_Float) {
		cglue->argT[0] = &ffi_type_double;
		cglue->argT_isUnboxed[0] = DGLUE_UNBOXED;
	  } else {
		TODO();
	  }

	  if (pa->p2.type == TYPE_Int) {
		cglue->argT[1] = &ffi_type_uint64;
		cglue->argT_isUnboxed[1] = DGLUE_UNBOXED;
	  } else if (pa->p2.type == TYPE_Float) {
		cglue->argT[1] = &ffi_type_double;
		cglue->argT_isUnboxed[1] = DGLUE_UNBOXED;
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
  mtd->b->cfunc = (void*)cglue;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  RETURN_(fo);
}


static knh_GlueSPI_t CLibGlueSPI = {
  CLibGlue_getFunc,
  /*CLib_component_free*/ NULL,
  /*CLib_glue_free*/ NULL
};


// @Native Glue Clib_genGlue (Glue _)
METHOD Clib_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)((sfp[0].p)->rawptr);
  knh_RawPtr_t *po = sfp[1].p;

  if (clib != NULL) {
	knh_Glue_t *glue = (knh_Glue_t *)po->rawptr;
	glue->gapi = &CLibGlueSPI;
	glue->componentInfo = (void*)clib;
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)KNH_MALLOC(ctx, sizeof(knh_ClibGlue_t));
	glue->glueInfo = (void*)new_RawPtr(ctx, po, cglue);
	RETURN_(po);
  }
  RETURN_(sfp[1].p);
}





/* ------------------------------------------------------------------------ */


#ifdef _SETUP

DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "clib");
	kapi->setPackageProperty(ctx, "version", "0.1");
	RETURN_PKGINFO("konoha.clib");
	
}

  /*DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "dffi");
	kapi->setPackageProperty(ctx, "version", "0.1");
	RETURN_PKGINFO("konoha.dffi");
}
  */
#endif /* _SETUP */

#ifdef __cplusplus
}
#endif

