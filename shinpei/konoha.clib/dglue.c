#include <konoha1.h>
#include <dglue.h>

/* ------------------------------------------------------------------------ */

knh_Glue_t *new_Glue(CTX ctx)
{
  knh_Glue_t *glue = (knh_Glue_t*)KNH_MALLOC(ctx, sizeof(knh_Glue_t));
  glue->glueType = GLUE_TYPE_UNKNOWN;
  // TODO: we need NullObject here
  glue->componentInfo = NULL; 
  glue->glueInfo = NULL;
  glue->gapi = (knh_GlueSPI_t*)KNH_MALLOC(ctx, sizeof(knh_GlueSPI_t));
  knh_GlueSPI_t *gapi = glue->gapi;
  gapi->component_free = NULL;
  gapi->glue_free = NULL;
  gapi->getFunc = NULL;
  return glue;
}

static void Glue_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Glue_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_Glue_t *glue = (knh_Glue_t*)po->rawptr;
	  knh_GlueSPI_t *gapi = glue->gapi;
	  if (glue->componentInfo != NULL) {
		//gapi->component_free(ctx, glue->componentInfo);
	  }
	  if (glue->glueInfo != NULL) gapi->glue_free(ctx, glue->glueInfo);
	  KNH_FREE(ctx, gapi, sizeof(knh_GlueSPI_t));
	  KNH_FREE(ctx, glue, sizeof(knh_Glue_t));
	  po->rawptr = NULL;
	}
}


DEFAPI(void) defGlue(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Glue";
	cdef->init = Glue_init;
	cdef->free = Glue_free;
}

/* ------------------------------------------------------------------------ */

// @Native var Glue.getFunc(String symbol, Class_, Func _);
METHOD Glue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Glue_t *glue = (knh_Glue_t*)((sfp[0].p)->rawptr);
  knh_GlueSPI_t *gapi = glue->gapi;
  knh_RawPtr_t *ret = NULL;
  switch (glue->glueType) {
  case GLUE_TYPE_INTERNAL:
	ret = gapi->getFunc(ctx, sfp, K_RIX);
	break;
  case GLUE_TYPE_LOCAL:
	ret = gapi->getFunc(ctx, sfp, K_RIX);
	break;
  case GLUE_TYPE_REMOTE:

	break;
  default:
	break;
  }
  if (ret == NULL) {
	// getFunc failed. return Func NullObject
	RETURN_(KNH_NULVAL(CLASS_Func));
  } else {
	RETURN_(ret);
  }
}


#ifdef _SETUP
DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "clib");
	kapi->setPackageProperty(ctx, "version", "0.2");
	RETURN_PKGINFO("konoha.clib");
}

  /*DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->setPackageProperty(ctx, "name", "dffi");
	kapi->setPackageProperty(ctx, "version", "0.2");
	RETURN_PKGINFO("konoha.dffi");
}
  */
#endif /* _SETUP */
