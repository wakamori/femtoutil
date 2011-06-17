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

typedef struct knh_clib_t {
  void *handler;
} knh_clib_t;


static void Clib_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_clib_t));
}

static void Clib_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
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
/* [Cfunc] */

typedef struct knh_cfunc_t {
  knh_clib_t *clib;
  void *fptr;

  ffi_cif cif;
  int argCount;
  ffi_type *retT;
  ffi_type *argT[CLIB_ARGMAX];
  void *argV[CLIB_ARGMAX];
  void *retV;
} knh_cfunc_t;

static void Cfunc_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_cfunc_t));
}

static void Cfunc_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
		po->rawptr = NULL;
	}
}


DEFAPI(void) defCfunc(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Cfunc";
	cdef->init = Cfunc_init;
	cdef->free = Cfunc_free;
}

/* ------------------------------------------------------------------------ */

// Clib Clib.new(String libname) 
METHOD Clib_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_clib_t *clib = (knh_clib_t *)(sfp[0].p->rawptr);
  const char *libname = String_to(const char *, sfp[1]);
  fprintf(stderr, "libname:%s\n", libname);
  clib->handler = dlopen(libname, RTLD_LAZY);
  RETURN_(sfp[0].o);
}

// @Native Cfunc Clib.getSymbol(String sym, Class retT, Array<Class> argT, Namespace _, Cfunc _);
METHOD Clib_getSymbol(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_clib_t *clib = (knh_clib_t *)((sfp[0].p)->rawptr);
  const char *symname = String_to(const char *, sfp[1]);
  knh_Class_t *c = (knh_Class_t*)sfp[2].o;
  knh_Array_t *a = (knh_Array_t*)sfp[3].a;


  knh_cfunc_t *cfunc = KNH_MALLOC(ctx, sizeof(knh_cfunc_t));
  fprintf(stderr, "loading:%s from %p\n", symname, clib->handler);
  if ((cfunc->fptr = dlsym(clib->handler, symname))== NULL) {
	  fprintf(stderr, "ERROR!!!\n");
	}

  /* retT */
  if (c->cid == CLASS_Int) {
	cfunc->retT = &ffi_type_uint64;
  } else if (c->cid == CLASS_Float) {
	cfunc->retT = &ffi_type_double;
  } else {

  }

  /* argT */
  size_t argCount = knh_Array_size(a);
  fprintf(stderr, "argCount:%d\n", argCount);
  cfunc->argCount = argCount;
  int i = 0;
  for (; i < argCount; i++) {
	knh_Class_t *klass = (knh_Class_t*)knh_Array_n(a, i);
	knh_class_t cid = klass->cid;
	fprintf(stderr, "argT:%s\n", CLASS__(cid));

	if (cid == CLASS_Int) {
	  cfunc->argT[i] = &ffi_type_uint64;
	} else if (cid == CLASS_Float) {
	  cfunc->argT[i] = &ffi_type_double;
	} else {
	  // TODO
	}
  }

  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[5].p, cfunc);
  knh_cfunc_t *altf = (knh_cfunc_t*)po->rawptr;
  fprintf(stderr, "argC:%d\n", altf->argCount);
  RETURN_(po);
}

// @Native Float Cfunc.invoke(Float f)
METHOD Cfunc_invoke(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_cfunc_t *cfunc = (knh_cfunc_t *)((sfp[0].p)->rawptr);
  double arg1 = Float_to(double, sfp[1]);
  fprintf(stderr, "fptr:%p, %p, %d, %p, %p\n", 
		  cfunc->fptr,
		  &(cfunc->cif), cfunc->argCount,
		  cfunc->retT, cfunc->argT);
  //  double(*sin)(double) = (double(*)(double))cfunc->fptr;
  //  fprintf(stderr, "sin1.0:%f\n", sin(1.0));
  (cfunc->argV[0]) = &arg1;
  double return_f = 0.0;
  if (ffi_prep_cif(&(cfunc->cif), FFI_DEFAULT_ABI, cfunc->argCount,
				   cfunc->retT, cfunc->argT) == FFI_OK) {
	ffi_call(&(cfunc->cif), cfunc->fptr, &(cfunc->retV), cfunc->argV);
	return_f = *(double*)(&cfunc->retV);
	fprintf(stderr, "%f\n", return_f);
  }
  
  RETURNf_(return_f);
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

