#define  USE_B 1
#define USE_new_bytes 1
#define USE_cwb 1
#define USE_cwb_open 1

#include <konoha1.h>


#include <unistd.h>
#include <spawn.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------------------ */
/* [Process] */



typedef struct knh_Process_t {
  char *path;
  int pid;
} knh_Process_t;


static void Process_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = (void*)KNH_MALLOC(ctx, sizeof(knh_Process_t));
  fprintf(stderr, "NEW!:%p\n", po);
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
  knh_Process_t *proc;
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
#define PATH_MAX 256

METHOD Process_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_RawPtr_t *po = sfp[0].p;
  fprintf(stderr, "%p\n", po);
  knh_Process_t *proc = (knh_Process_t*)po->rawptr;
  char *pname = String_to(char *, sfp[1]);
  size_t path_size = knh_strlen(pname);
  if (pname != NULL && path_size < PATH_MAX) {
	proc->path = KNH_MALLOC(ctx, path_size * sizeof(char));
	knh_memcpy(proc->path, pname, path_size+1);
  }
  RETURN_(po);
}


METHOD Process_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_RawPtr_t *po = sfp[0].p;
  knh_Process_t *proc = (knh_Process_t*)po->rawptr;
  knh_RawPtr_t *po2 = sfp[1].p;
  knh_LGlue_t *lg = (knh_LGlue_t*)po2->rawptr;
  lg->proc = proc;
  RETURN_(po2);
}

  //#include <crt_externs.h>

static METHOD Fmethod_wrapProcess(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_LGlue_t *lg = (knh_LGlue_t*)(((fo->mtd)->b)->cfunc);
  knh_Process_t *proc = lg->proc;
  //const char *arg1 = String_to(const char *, sfp[1]);
  //  char **args = {proc->path, arg1, NULL};
  char *args[] = {"ls", "-l", NULL};
#ifdef K_USING_POSIX_
  // create pipe
  int pipe_c2p[2], pipe_p2c[2];
  int pid;
  if (pipe(pipe_c2p) < 0) {
	fprintf(stderr, "POPEN C2P!!\n");
  }
  if (pipe(pipe_p2c) < 0) {
	fprintf(stderr, "POPEN P2C\n");
	close(pipe_c2p[0]/* R */);
	close(pipe_c2p[1]/* W */);
  }
  if ((pid = fork()) < 0) {
	close(pipe_c2p[0]);
	close(pipe_c2p[1]);
	close(pipe_p2c[0]);
	close(pipe_p2c[1]);
  }
  if (pid == 0) {
	/* child */
	close(pipe_p2c[1]); // parent writing in this pipe
	close(pipe_c2p[0]); // parent reading from this pipe
	dup2(pipe_p2c[0] , 0); // stdin
	dup2(pipe_c2p[1], 1); // stdout
	close(pipe_p2c[0]); // after duplicate, close pipe
	close(pipe_c2p[1]); // same as above
	if (execvp(proc->path, args) < 0) {
	  perror("execvp");
	  fprintf(stderr, "execvp error!\n");
	  exit(1);
	}
  } else {
	int status;
	close(pipe_p2c[0]);
	close(pipe_c2p[1]);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	char buf[K_PAGESIZE];
	//	while ((loaded = read(pipe_c2p[0], buf, 1)) != 0) {
	while(1) {
	  size_t size = read(pipe_c2p[0], buf, K_PAGESIZE);
	  //	  fprintf(stderr, "size:%d\n", size);
	  if (size > 0) {
		knh_bytes_t t = {{buf}, size};
		knh_Bytes_write(ctx, cwb->ba, t);
	  } else {
		break;
	  }
	};
	wait(&status);
	RETURN_(knh_cwb_newString(ctx, cwb));
  }

#endif

  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  RETURNi_(KNH_INT0);
	} else {
	  RETURN_(KNH_NULVAL(CLASS_t(rtype)));	  
	}
  }
}


METHOD Lglue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_RawPtr_t *po = sfp[0].p;
  knh_LGlue_t *lg = (knh_LGlue_t*)po->rawptr;
  if (lg->proc == NULL) {fprintf(stderr, "invalid Lglue\n"); RETURN_(sfp[2].fo);}
  knh_Class_t *klass = (knh_Class_t*)sfp[1].o;
  knh_Func_t *fo = sfp[2].fo;

  //fprintf(stderr, "%s\n", CLASS__(klass->cid));
  fprintf(stderr, "%s\n", CLASS__(O_cid(fo)));
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  //  knh_ParamArray_t *pa = tbl->cparam;

  fo->h.cTBL= tbl;
  knh_Method_t *mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapProcess);
  mtd->b->cfunc = (void*)lg;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  RETURN_(fo);
}

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
