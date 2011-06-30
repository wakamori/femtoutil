#ifndef _DGLUE_H_
#define _DGLUE_H_

#define GLUE_TYPE_UNKNOWN 0
#define GLUE_TYPE_INTERNAL 1
#define GLUE_TYPE_LOCAL 2
#define GLUE_TYPE_REMOTE 3

/* ------------------------------------------------------------------------ */
/* [Dependable Glue] */

typedef struct knh_GlueSPI_t {
  knh_RawPtr_t* (*getFunc)(CTX, knh_sfp_t * _RIX);
  void (*component_free)(CTX, void *);
  void (*glue_free)(CTX, void *);
} knh_GlueSPI_t;

typedef struct knh_Glue_t {
  int glueType; 
  void *componentInfo;
  void *glueInfo;
  /* Glue Service Provider Interface */
  knh_GlueSPI_t *gapi;
} knh_Glue_t;

knh_Glue_t *new_Glue(CTX ctx);

#endif /* _DGLUE_H_ */

