#include <konoha1.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <dglue.h>

/* ------------------------------------------------------------------------ */
//DFFI log functions

FILE *dlog = NULL;

typedef struct dffi_ring_entry_t {
  knh_logdata_t *entry;
} dffi_ring_entry_t;

#define DFFI_RING_BUFFER_ENTRIES 16

typedef struct dffi_ringbuffer_t {
  dffi_ring_entry_t *head;
  dffi_ring_entry_t rbuf[DFFI_RING_BUFFER_ENTRIES];
} dffi_ringbuffer_t;

char *dffi_getRecord(void)
{
  char *ptr = g_DFFILogBuf;
  if (ptr == NULL) {
	fprintf(stderr, "no logbuf so far...\n");
	_Exit(0);
  }
  return ptr;
}

static inline void dffi_log_writeBuf(char *buf, const char* str, size_t len) {
  memcpy(buf, str, len);
  //  snprintf(buf, len,  "%s ", str);
}

static struct tm pre_tm;
static int changed_time = 0;
static char *time_buf[32];
#define ENTRY_SIZE 80
#define BUF_ENTRY 32
static char logbuf[ENTRY_SIZE * BUF_ENTRY];
static int entries = 0;


static inline void dffi_log_flush(char *buf)
{
  if (entries == BUF_ENTRY) {
	entries = 0;
  //fprintf(dlog, "%s\n", buf);
  //  fflush(dlog);
  }

}

static inline const char* knh_format_w3cdtf(char *buf, size_t bufsiz, struct tm *tmp)
{
	// 2001-08-02T10:45:23+09:00
  knh_snprintf(buf, bufsiz, "%04d-%02d-%02dT%02d:%02d:%02d%+02d:%02d",
		(int)(tmp->tm_year + 1900), (int)(tmp->tm_mon + 1), tmp->tm_mday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (int)(tmp->tm_gmtoff / (60 * 60)), 0);
	return (const char*)buf;
}

inline void dffi_record(knh_logdata_t _logdata[], const char* str)
{
  CTX ctx = knh_getCurrentContext();
  char *lcur = logbuf;
  time_t t;
  struct tm tm;
  const char  *t_buf = NULL;
  time(&t);
  localtime_r(&t, &tm);
  if (pre_tm.tm_sec != tm.tm_sec) {
	memcpy(&pre_tm, &tm, sizeof(struct tm));
	t_buf = knh_format_w3cdtf(time_buf, sizeof(time_buf), &tm);
	changed_time = 1;
  } else {
	t_buf = time_buf;
  }
  size_t len = strlen(t_buf)+1;
  dffi_log_writeBuf(lcur, t_buf, len-1);
  lcur += len-1;
  dffi_log_writeBuf(lcur, str, strlen(str));
  entries++;
  dffi_log_flush(logbuf);
}

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
  time_t t;
  time(&t);
  localtime_r(&t, &pre_tm);
  char *str;
  str = knh_format_w3cdtf(time_buf, sizeof(time_buf), &pre_tm);
  dlog = fopen("log.txt", "w");
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


#ifdef __cplusplus
}
#endif

