#include <konoha1.h>


#include <unistd.h>
#include <spawn.h>


#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------------------------------------------ */
/* [Process] */

typedef struct knh_Process_t {
  int pid;
} knh_Process_t;


static void Process_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_Process_t));
}

static void Process_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defProcess(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Process";
	cdef->init = Process_init;
	cdef->free = Process_free;
}


/* ------------------------------------------------------------------------ */
/* [Dglue:Local] */

#define DGLUE_NOT_UNBOXED 0
#define DGLUE_UNBOXED 1


typedef struct knh_LGlue_t {
  knh_Process_t *process;
} knh_LGlue_t;

static void Lglue_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_LGlue_t));
}

static void Lglue_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  po->rawptr = NULL;
	}
}


DEFAPI(void) defLglue(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Lglue";
	cdef->init = Lglue_init;
	cdef->free = Lglue_free;
}

/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */


#ifdef _SETUP
DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "process");
	kapi->setPackageProperty(ctx, "version", "0.0");
	RETURN_PKGINFO("konoha.process");
}
#endif /* _SETUP */


#ifdef __cplusplus
}
#endif
