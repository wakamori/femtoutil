#ifndef _DGLUE_H_
#define _DGLUE_H_

#define GLUE_TYPE_UNKNOWN 0
#define GLUE_TYPE_INTERNAL 1
#define GLUE_TYPE_LOCAL 2
#define GLUE_TYPE_REMOTE 3

/* ------------------------------------------------------------------------ */
/* [Dependable Glue] */

typedef struct knh_GlueSPI_t {
  METHOD (*getFunc)(CTX, knh_sfp_t * _RIX);
  void (*component_free)(CTX, knh_RawPtr_t *);
  void (*glue_free)(CTX, knh_RawPtr_t *);
} knh_GlueSPI_t;

typedef struct knh_Glue_t {
  int glueType; 
  void *componentInfo;
  void *glueInfo;
  /* Glue Service Provider Interface */
  knh_GlueSPI_t *gapi;
} knh_Glue_t;


static void Glue_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_Glue_t));
  knh_Glue_t *glue = (knh_Glue_t*)po->rawptr;
  glue->glueType = DGLUE_TYPE_UNKNOWN;
  // TODO: we need NullObject here
  glue->componentInfo = NULL; 
  glue->glueInfo = NULL;
  glue->gapi = (knh_GlueSPI_t*)KNH_MALLOC(ctx, sizeof(knh_GlueSPI_t));
  knh_GlueSPI_t *gapi = pdg->gapi;
  gapi->component_free = NULL;
  gapi->glue_free = NULL;
  gapi->getFunc = NULL;
}

static void Glue_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_Glue_t *glue = (knh_Glue_t*)po->rawptr;
	  knh_GlueSPI_t *gapi = dg->gapi;
	  if (glue->componentInfo != NULL) gapi->component_free(ctx, (knh_RawPtr_t*)glue->componentInfo);
	  if (glue->glueInfo != NULL) gapi->glue_free(ctx, (knh_RawPtr_t*)glue->glueInfo);
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

METHOD Glue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Glue_t *glue = (knh_Glue_t*)((sfp[0].p)->rawptr);
  knh_GlueSPI_t *gapi = glue->gapi;

  switch (glue->glueType) {
  case GLUE_TYPE_INTERNAL:

	break;
  case GLUE_TYPE_LOCAL:
	break;
  case GLUE_TYPE_REMOTE:
	break;

  default:
	break;
  }


}

/* ------------------------------------------------------------------------ */

#endif /* _DGLUE_H_ */




