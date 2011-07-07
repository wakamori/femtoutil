

#define K_INTERNAL 1
#include <konoha1.h>
#include <konoha1/konohalang.h>
#include <konoha1/inlinelibs.h>


//#define MACOSX
#if defined(K_USING_MACOSX_)

#elif defined(K_USING_LINUX_)

#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Glue] */
#include <dglue.h> // for DEOS
// #include <glue.h>

/* ------------------------------------------------------------------------ */
/* [Webservice] */

typedef struct knh_Webservice_t {
  int sd; // socket
  char *path;
  size_t path_size;
} knh_Webservice_t;

static void Webservice_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Webservice_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defWebservice(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
  cdef->name = "Webservice";
  cdef->init = Webservice_init;
  cdef->free = Webservice_free;
}

/* ------------------------------------------------------------------------ */
/* [Dglue:Remote] */

typedef struct knh_WebserviceGlue_t {
  char *path;  
  size_t path_size;
} knh_WebserviceGlue_t;

static void WebserviceGlue_init(CTX ctx, knh_WebserviceGlue_t *po)
{
  po->path = NULL;
}

static void WebserviceGlue_free(CTX ctx, void *ptr)
{
	if (ptr != NULL) {
	  ptr = NULL;
	}
}

/* ------------------------------------------------------------------------ */
/* [Webservice] */

#define WS_PATH_MAX (256)

//@Native Webservice Webservice.new(String path, Webservice _);
METHOD Webservice_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Webservice_t *proc = (knh_Webservice_t*)KNH_MALLOC(ctx, sizeof(knh_Webservice_t));
  char *pname = String_to(char *, sfp[1]);
  size_t path_size = knh_strlen(pname);
  if (pname != NULL && path_size < WS_PATH_MAX) {
	proc->path = KNH_MALLOC(ctx, path_size * sizeof(char));
	knh_memcpy(proc->path, pname, path_size+1);
	proc->path_size = path_size;
  }
  RETURN_(new_RawPtr(ctx, sfp[2].p, proc));
}

static METHOD Fmethod_wrapWebservice(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_WebserviceGlue_t *wglue = (knh_WebserviceGlue_t*)(((fo->mtd)->b)->cfunc);
  char *arg1 = String_to(char *, sfp[1]);

}

//@Native @Throwable var Glue.getFunc(String symbol, Class _, Func _);
static knh_RawPtr_t *WebserviceGlue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_RawPtr_t *po = sfp[0].p;
  knh_Glue_t *glue = (knh_Glue_t*)po->rawptr;
  knh_WebserviceGlue_t *wglue = (knh_Webservice_t*)(glue->glueInfo);
  knh_Webservice_t *ws = (knh_Webservice_t*)(glue->componentInfo);

  if (ws == NULL) {
	//	LOGSFPDATA = {__ERRNO__};
	//	LIB_Failed("invaid proc", "ContentFail!!");
	return NULL;
  }
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;
  knh_Func_t *fo = sfp[3].fo;
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  knh_param_t *rt = knh_ParamArray_rget(pa, 0);
  knh_Method_t *mtd;
  if(rt->type == TYPE_String) {
	mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapWebservice);
  } else {
	return NULL;
  }
  fo->h.cTBL = tbl;
  mtd->b->cfunc = (void*)wglue;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  return (knh_RawPtr_t*)fo;
}

static knh_GlueSPI_t WebserviceGlueSPI = {
  WebserviceGlue_getFunc,
  /*Webservice_component_free*/ NULL,
  WebserviceGlue_free
};



#ifdef __cplusplus
}
#endif
