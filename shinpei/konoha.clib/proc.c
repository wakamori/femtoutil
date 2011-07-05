/* Process for DGlue
 *  ffi package, for DEOS
 *  written by shinpei(c)2011
 */

#include <konoha1.h>

//#define MACOSX
#if defined(K_USING_MACOSX_)
#include <signal.h>
#elif defined(K_USING_LINUX_)
#include <sys/wait.h>
#include <signal.h>
#endif

#include <unistd.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Glue] */
#include <dglue.h> // for DEOS
// #include <glue.h>

/* ------------------------------------------------------------------------ */
/* [Process] */

typedef struct knh_Process_t {
  char *path;
  size_t path_size;
  int pid;
} knh_Process_t;

static void Process_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
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

typedef struct knh_ProcessGlue_t {
  char *path;  
  size_t path_size;
} knh_ProcessGlue_t;

static void ProcessGlue_init(CTX ctx, knh_ProcessGlue_t *po)
{
  po->path = NULL;
}

static void ProcessGlue_free(CTX ctx, void *ptr)
{
	if (ptr != NULL) {
	  ptr = NULL;
	}
}


/* ------------------------------------------------------------------------ */
/* [Process] */
#define PROCESS_PATH_MAX 256

//@Native Process Process.new(String path, Process _);
METHOD Process_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Process_t *proc = (knh_Process_t*)KNH_MALLOC(ctx, sizeof(knh_Process_t));
  char *pname = String_to(char *, sfp[1]);
  size_t path_size = knh_strlen(pname);
  if (pname != NULL && path_size < PROCESS_PATH_MAX) {
	proc->path = KNH_MALLOC(ctx, path_size * sizeof(char));
	knh_memcpy(proc->path, pname, path_size+1);
	proc->path_size = path_size;
  }
  RETURN_(new_RawPtr(ctx, sfp[2].p, proc));
}

static
void SIGCHLD_handler(int signal)
{
  pid_t pid_child = 0;
  do {
	int status;
	pid_child = waitpid(-1, &status, WNOHANG);
	fprintf(stderr, "pid_child:%d\n", pid_child);
	if (WIFEXITED(status)) {
	  fprintf(stderr, "normal\n");
	} else if (WIFSIGNALED(status)) {
	  fprintf(stderr, "content fail\n");
	} else {
	  fprintf(stderr, "stopped!\n");
	}
  }  while (pid_child > 0);
}

//#include <crt_externs.h>
static METHOD Fmethod_wrapProcessWithoutPipe(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Func_t *fo = sfp[0].fo;
  knh_ProcessGlue_t *pglue = (knh_ProcessGlue_t*)(((fo->mtd)->b)->cfunc);
  char *arg1 = String_to(char *, sfp[1]);
  char *args[] = {pglue->path, arg1, NULL};
  //  char *args[] = {pglue->path, arg1};
#ifdef K_USING_POSIX_
  // retval is void, we don't need pipe
  /*  process handling */
  pid_t pid;
  struct sigaction act_child;
  sigemptyset(&act_child.sa_mask);
  act_child.sa_handler = SIGCHLD_handler;
  act_child.sa_flags = SA_NOCLDSTOP | SA_RESTART;
  sigaction(SIGCHLD, &act_child, NULL);

  if ((pid = fork()) < 0) {
	// ERROR
	  LOGSFPDATA = {__ERRNO__};
	  LIB_Failed("fork", "Process!!");
	  RETURNvoid_();
  }
  if (pid == 0) {
	/* child */
	if (execvp(pglue->path, args) < 0) {
	  LOGSFPDATA = {__ERRNO__};
	  LIB_Failed("execvp", "Process!!");
	  exit(1);
	}
  } else {
	fprintf(stderr, "pid=%d\n", pid);
	RETURNvoid_();
  }
#endif
}


static METHOD Fmethod_wrapProcess(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_ProcessGlue_t *pglue = (knh_ProcessGlue_t*)(((fo->mtd)->b)->cfunc);
  char *arg1 = String_to(char *, sfp[1]);
  char *args[] = {pglue->path, arg1, NULL};
  //  char *args[] = {pglue->path, arg1};
#ifdef K_USING_POSIX_
  // create pipe when ret value is string;
  int pipe_c2p[2], pipe_p2c[2];
  int pid;
  if (pipe(pipe_c2p) < 0) {
	LOGSFPDATA = {__ERRNO__};
	LIB_Failed("pipe c2p", "ContentFail!!");
  }
  if (pipe(pipe_p2c) < 0) {
	close(pipe_c2p[0]/* R */);
	close(pipe_c2p[1]/* W */);
	LOGSFPDATA = {__ERRNO__};
	LIB_Failed("pipe p2c", "ContentFail!!");
  }
  /*  process handling */
  struct sigaction act_child;
  sigemptyset(&act_child.sa_mask);
  act_child.sa_handler = SIGCHLD_handler;
  act_child.sa_flags = SA_NOCLDSTOP | SA_RESTART;
  sigaction(SIGCHLD, &act_child, NULL);

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
	if (execvp(pglue->path, args) < 0) {
	  //	  perror("execvp");
	  LOGSFPDATA = {__ERRNO__};
	  LIB_Failed("execvp", "Process!!");
	  exit(1);
	}
  } else {
	int status = 0;
	close(pipe_p2c[0]);
	close(pipe_c2p[1]);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	char buf[K_PAGESIZE];
	while(1) {
	  size_t size = read(pipe_c2p[0], buf, K_PAGESIZE);
	  if (size > 0) {
		knh_bytes_t t = {{buf}, size};
		knh_Bytes_write(ctx, cwb->ba, t);
	  } else {
		break;
	  }
	};
	fprintf(stderr, "pid=%d\n", pid);
	//	waitpid(pid, &status, WUNTRACED);
	//	wait(&status);
	if (WIFEXITED(status)) {
	  LOGSFPDATA = {sDATA("path", pglue->path), iDATA("pid", pid)};
	  fprintf(stderr, "normal\n");
	  LIB_OK("Process executed");
	} else if (WIFSIGNALED(status)) {
	  LOGSFPDATA = {sDATA("path", pglue->path), iDATA("pid", pid)};
	  fprintf(stderr, "content fail\n");
	  LIB_Failed("process", "ContentFail!!");
	} else {
	  LOGSFPDATA = {sDATA("path", pglue->path), iDATA("pid", pid)};
	  fprintf(stderr, "stopped!\n");
	  LIB_Failed("process", "Stopped!!");
	}
	RETURN_(knh_cwb_newString(ctx, cwb));
  }
#endif
  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  RETURN_(KNH_INT0);
	} else {
	  RETURN_(KNH_NULVAL(CLASS_t(rtype)));	  
	}
  }
}

//@Native @Throwable var Glue.getFunc(String symbol, Class _, Func _);
static knh_RawPtr_t *ProcessGlue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_RawPtr_t *po = sfp[0].p;
  knh_Glue_t *glue = (knh_Glue_t*)po->rawptr;
  knh_ProcessGlue_t *pglue = (knh_ProcessGlue_t*)(glue->glueInfo);
  knh_Process_t *proc = (knh_Process_t*)(glue->componentInfo);

  if (proc == NULL) {
	//	LOGSFPDATA = {__ERRNO__};
	//	LIB_Failed("invaid proc", "ContentFail!!");
	return NULL;
  }

  //  const char *symbol = String_to(const char*, sfp[1]);
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;
  knh_Func_t *fo = sfp[3].fo;
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  // return is void or String
  knh_param_t *rt = knh_ParamArray_rget(pa, 0);
  knh_Method_t *mtd;
  if (rt->type == TYPE_String) {
	mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapProcess);
  } else if (rt->type == TYPE_void) {
	mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapProcessWithoutPipe);
  } else {
	//	LOGSFPDATA = {__ERRNO__};
	//	LIB_Failed("Process ret value is only {String|Void}", "ContentFail!!");
	return NULL;
  }

  fo->h.cTBL= tbl;
  //  knh_Method_t *mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapProcess);
  mtd->b->cfunc = (void*)pglue;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  return (knh_RawPtr_t*)fo;
}

static knh_GlueSPI_t ProcessGlueSPI = {
  ProcessGlue_getFunc,
  /*  Process_component_free*/ NULL,
  ProcessGlue_free
};

// @Native Glue Process_genGlue (Glue _)
METHOD Process_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Process_t *proc = (knh_Process_t*)((sfp[0].p)->rawptr);
  if (proc != NULL) {
	knh_Glue_t *glue = new_Glue(ctx);
	glue->glueType = GLUE_TYPE_LOCAL;
	glue->gapi = &ProcessGlueSPI;
	glue->componentInfo = (void*)proc;
	knh_ProcessGlue_t *pglue = (knh_ProcessGlue_t*)KNH_MALLOC(ctx, sizeof(knh_ProcessGlue_t));
	ProcessGlue_init(ctx, pglue);
	DBG_ASSERT(proc->path_size > 0);
	pglue->path = KNH_MALLOC(ctx, proc->path_size * sizeof(char));
	knh_memcpy(pglue->path, proc->path, proc->path_size+1);
	pglue->path_size = proc->path_size;
	glue->glueInfo = (void*)pglue;
	RETURN_(new_RawPtr(ctx, sfp[1].p, glue));
  }
  RETURN_(sfp[1].p);
}




#ifdef __cplusplus
}
#endif

