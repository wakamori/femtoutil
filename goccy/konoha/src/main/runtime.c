/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us.
 *
 * (1) GNU General Public License 3.0 (with K_UNDER_GPL)
 * (2) Konoha Non-Disclosure License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_strcmp    1
#define USE_bytes_parseint  1
#define USE_cwb_open      1
#define USE_cwb_tobytes   1
#define USE_cwb_size      1

// added by nakata
//#define USE_cwb_write 1

/* ************************************************************************ */

#define LIBNAME "konoha"
#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------------------ */
/* [ctxkey] */

#ifdef K_USING_THREAD
#ifdef CC_TYPE_TLS
static CC_TYPE_TLS  knh_context_t *curctx = NULL;
#else
static knh_thread_key_t ctxkey;
static knh_context_t *curctx = NULL;
#endif
#else
static knh_context_t *curctx = NULL;
#endif

/* ------------------------------------------------------------------------ */

//void knh_opcode_check(void);

void konoha_init(void)
{
	static int isInit = 0;
	if(isInit == 0) {
		knh_opcode_check();
#if defined(K_USING_THREAD) && !defined(CC_TYPE_TLS)
		knh_thread_key_create((knh_thread_key_t*)&ctxkey);
#endif
		knh_srand(0);
	}
	isInit = 1;
}

/* ------------------------------------------------------------------------ */
/* [Context] */

void knh_beginContext(CTX ctx, void **bottom)
{
	((knh_context_t*)ctx)->cstack_bottom = bottom;
#ifdef K_USING_THREAD
	knh_mutex_lock(ctx->ctxlock);  // by yoan
#if !defined(CC_TYPE_TLS)
	thread_setspecific(ctxkey, ctx);
#endif
	curctx = (knh_context_t*)ctx;
#else
	curctx = (knh_context_t*)ctx;
#endif
}

/* ------------------------------------------------------------------------ */

void knh_endContext(CTX ctx)
{
#ifdef K_USING_THREAD
#if !defined(CC_TYPE_TLS)
	thread_setspecific(ctxkey, NULL);
#endif
	knh_mutex_unlock(ctx->ctxlock);  // by yoan
	curctx = NULL;
#else
	curctx = NULL;
#endif
	((knh_context_t*)ctx)->cstack_bottom = NULL;
}

/* ------------------------------------------------------------------------ */

knh_context_t* knh_getCurrentContext(void)
{
#if defined(K_USING_THREAD) && !defined(CC_TYPE_TLS)
	knh_context_t* ctx = (knh_context_t*)knh_thread_getspecific(ctxkey);
	if(ctx == NULL) {
		ctx = curctx;
	}
#else
	knh_context_t* ctx = curctx;
#endif
	if(ctx == NULL) {
		fprintf(stderr, "NOT IN THE CONTEXT OF KONOHA\n");
		exit(1);
	}
	return ctx;
}

/* ------------------------------------------------------------------------ */
/* [opt] */

#if defined(K_USING_DEBUG)
static int systemVerbose = 1;
static int auditLevel    = LOG_NOTICE;
static int verboseLevel  = LOG_INFO;
#else
static int systemVerbose = 0;
static int auditLevel    = LOG_ERR;
static int verboseLevel  = LOG_CRIT;
#endif


knh_bool_t knh_isAuditLogging(int pe)
{
	return (pe <= auditLevel);
}

#ifdef K_USING_SYSLOG
static void _vsyslog(int pe, const char *fmt, va_list ap)
{
	vsyslog(pe, fmt, ap);
	if((pe <= verboseLevel)) {
		pseudo_vsyslog(pe, fmt, ap);
	}
}
static void _syslog(int pe, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	vsyslog(pe, fmt, ap);
	if((pe <= verboseLevel)) {
		pseudo_vsyslog(pe, fmt, ap);
	}
	va_end(ap);
}
#endif

static void opt_a(CTX ctx, int mode, const char *optstr)
{
	auditLevel = (mode <= LOG_CRIT) ? LOG_NOTICE /*defaults*/: mode;
#if defined(K_USING_SYSLOG)
	((knh_ServiceSPI_t*)ctx->spi)->syslogspi = "syslog";
	((knh_ServiceSPI_t*)ctx->spi)->syslog    = _syslog;
	((knh_ServiceSPI_t*)ctx->spi)->vsyslog    = _vsyslog;
	openlog("konoha", LOG_PID, LOG_LOCAL7);
	KNH_SYSLOG(ctx, NULL, LOG_NOTICE, "init", "version='%s', rev=%d, auditlevel=%d", K_VERSION, K_REVISION, auditLevel);
#ifdef K_DEOS_TRACE
		char *trace = knh_getenv(K_DEOS_TRACE);
		if(trace != NULL) {
			KNH_SYSLOG(ctx, NULL, LOG_NOTICE, K_DEOS_TRACE, "trace='%s'", trace);
		}
#endif
#else
	fprintf(stdout, "konoha: no available logging system.\n");
	exit(0);
#endif
}

KNHAPI2(knh_bool_t) knh_isSystemVerbose(void)
{
	return systemVerbose;
}

knh_bool_t knh_isVerboseLogging(int pe)
{
	return (pe <= verboseLevel);
}

static void opt_v(CTX ctx, int mode, const char *optstr)
{
	if(mode == 0) {
		systemVerbose = 1;
		dump_sysinfo(NULL, NULL, 1/*isALL*/);
		verboseLevel = LOG_INFO;
		KNH_ASSERT(sizeof(knh_intptr_t) == sizeof(void*));
		KNH_ASSERT(sizeof(knh_Token_t) <= sizeof(knh_Object_t));
		KNH_ASSERT(sizeof(knh_Stmt_t) <= sizeof(knh_Object_t));
		KNH_ASSERT(sizeof(knh_int_t) <= sizeof(knh_float_t));
#ifdef K_USING_RBP_
		KNH_ASSERT(sizeof(knh_rbp_t) * 2 == sizeof(knh_sfp_t));
#endif
	}
	else {
		verboseLevel = (mode <= LOG_CRIT) ? auditLevel: mode;
		if(auditLevel < verboseLevel) auditLevel = verboseLevel;
	}
}

static int warningLevel = LOG_WARNING;

knh_bool_t knh_shouldWarnMessage(int pe)
{
	return (pe <= warningLevel);
}

static void opt_W(CTX ctx, int mode, const char *optstr)
{
	if(mode == 0) mode = 10;
	if(mode > LOG_WARNING) {
		warningLevel = mode;
	}
}

static knh_bool_t isInteractiveMode = 0;
static knh_bool_t isCompileOnly = 0;

static void opt_i(CTX ctx, int mode, const char *optstr)
{
	isInteractiveMode = 1;
}


knh_bool_t knh_isCompileOnly(CTX ctx /* for future extension*/)
{
	//retunr CTX_isCompileOnly(ctx);
	return isCompileOnly;
}

static void opt_c(CTX ctx, int mode, const char *optstr)
{
	//CTX_setCompileOnly(ctx, 1);
	isCompileOnly = 1;
}

static void opt_g(CTX ctx, int mode, const char *optstr)
{
	CTX_setDebug(ctx, 1);
}

static void opt_O(CTX ctx, int mode, const char *optstr)
{
	DBG_P("OPTIMIZATION %d", mode);
	if(mode == 0) {
		//DP(ctx->gma)->cflag = 0; // NOTHING
	}
}

/* ----------------------------------------------------------------------- */

void knh_loadScriptPackageList(CTX ctx, const char *pkglist)
{
	if(pkglist != NULL) {
		knh_bytes_t t = {{pkglist}, knh_strlen(pkglist)};
		char buf[256];
		size_t i = 0;
		int isExists = 0;
		L_NEXT:;
		isExists = 0;
		while(i < t.len + 1) {
			char *c = buf + 4;
			buf[0] = 'p'; buf[1] = 'k'; buf[2] = 'g'; buf[3] = ':';
			while(i < t.len + 1) {
				int ch = t.ubuf[i];
				i++;
				if(ch ==':' || ch == ';' || ch == ',' || ch == 0) {
					*c = 0;
					DBG_P("loading '%s'", buf);
					if(!knh_loadScriptPackage(ctx, B(buf)) && isExists == 0) {
						KNH_WARN(ctx, "PackageNotFound package=%s", buf+8);
					}
					goto L_NEXT;
				}
				else {
					if(ch == '?') {
						isExists = 1; continue;
					}
					*c = ch;
				}
				c++;
				if(!(c - buf < 256)) {
					KNH_WARN(ctx, "too long name %s", pkglist);
					return ;
				}
			}
		}
	}
}

static void knh_setStartUpPackage(CTX ctx, int mode, const char *optstr)
{
	if(optstr != NULL) {
		DBG_P("package loading='%s'", optstr);
		knh_loadScriptPackageList(ctx, optstr);
	}
}

static void opt_version(CTX ctx, int mode, const char *optstr)
{
	dump_sysinfo(NULL, NULL, 1/*ALL*/);
	exit(0);
}

static const char *HELPMSG =
		"usage: konoha [options]  [file | -]\n"
		"options and arguments (and corresponding environment variables):\n"
		"-a     : audit\n"
		"-c     : compile and verification only (never running)\n"
		"-g     : debug support for testing script\n"
		"-h     : print this help message and exit (also --help)\n"
		"-i     : invoke interactive shell after running script\n"
		"-O     : optimize generated bytecode slightly\n"
		"-v     : verbose; -v0 is a strong system-level verbose\n"
		"-V     : show version and sysinfo and exit (also --version)\n"
		"-W arg : warning control in compilation\n"
		"file   : program read from script file\n"
		"-      : program read from stdin\n"
		"\n"
		"Other environment variables:\n"
		"KONOHAHOME  : alternate to %s\n"
		"KONOHAENC   : alternate to %s\n"
		"\n";

static void opt_help(CTX ctx, int mode, const char *optstr)
{
	knh_String_t* home = knh_getPropertyNULL(ctx, STEXT("konoha.path"));
	fprintf(stdout, HELPMSG, S_tochar(home), knh_getSystemEncoding());
	exit(0);
}

static FILE* uout = NULL;  // @see Assurance_write

static void opt_utest(CTX ctx, int mode, const char *optstr)
{
	char fname[80];
	const char *prefix = knh_getenv("UTEST");
	if(prefix == NULL) prefix = "AssuranceCases";
	knh_snprintf(fname, sizeof(fname), "%s%d.txt", prefix, (int)K_REVISION);
	uout = fopen(fname, "a");
}

FILE *knh_getUFILE(void)
{
#ifdef K_USING_DEBUG
	return stdout;
#else
	return uout;
#endif
}

/* ----------------------------------------------------------------------- */

typedef void (*knh_Fsetopt)(CTX, int, const char *);

#define OPT_EMPTY    0
#define OPT_NUMBER   1
#define OPT_STRING   2

typedef struct {
	const char *name;
	int type;
	knh_Fsetopt setopt;
} knh_optdata_t ;

static knh_optdata_t optdata[] = {
	{"-a", OPT_NUMBER, opt_a},
	{"-c", OPT_EMPTY, opt_c},
	{"-i", OPT_EMPTY, opt_i},
	{"--version", OPT_EMPTY, opt_v},
	{"-g", OPT_NUMBER, opt_g},
	{"-v", OPT_NUMBER, opt_v},
	{"-O", OPT_NUMBER, opt_O},
	{"-P", OPT_STRING, knh_setStartUpPackage},
	{"-W", OPT_NUMBER, opt_W},
	{"-h", OPT_EMPTY, opt_help},
	{"--utest", OPT_EMPTY, opt_utest},
	{"--help", OPT_EMPTY, opt_help},
	{"-V", OPT_EMPTY, opt_version},
	{"--version", OPT_EMPTY, opt_version},
	{NULL, OPT_EMPTY, NULL}, // END
};

static knh_optdata_t *knh_getoptdata(const char *name)
{
	knh_optdata_t *d = optdata;
	while(d->name != NULL) {
		size_t len = knh_strlen(d->name);
		if(knh_strncmp(d->name, name, len) == 0) {
			return d;
		}
		d++;
	}
	return NULL;
}

/* ----------------------------------------------------------------------- */

knh_bytes_t knh_bytes_nsname(knh_bytes_t t)
{
	size_t i, s = 0;
	for(i = t.len - 1; i > 0; i--) {
		if(t.utext[i] == '/' || t.utext[i] == '\\') {
			s = i + 1;
			break;
		}
	}
	for(i = s; i < t.len; i++) {
		if(t.utext[i] == '.') {
			t.utext = t.utext + s;
			t.len = i - s;
			return t;
		}
	}
	t.utext = t.utext + s;
	t.len = t.len - s;
	return t;
}

static int konoha_parseopt(konoha_t konoha, int argc, const char **argv)
{
	KONOHA_CHECK(konoha, 1);
	CTX ctx = konoha.ctx;
	int n;
	for(n = 1; n < argc; n++) {
		const char *t = argv[n];
		if(t[0] != '-') {
			goto L_SET_KONOHA_ARGV;
		}
		else {
			knh_optdata_t *d = knh_getoptdata(t);
			int optnum = 1;     // default
			const char* optstr = NULL;  // default
			if(d == NULL) {
				opt_help(ctx, optnum, optstr);
				return 0;
			}
			if(d->type == OPT_NUMBER) {
				t += knh_strlen(d->name);
				if(t[0] == '=') t++;
				if(isalnum(t[0])) {
					knh_int_t v = 0;
					knh_bytes_parseint(B((char*)t), &v);
					optnum = (int)v;
				}
			}
			else if(d->type == OPT_STRING) {
				t += knh_strlen(d->name);
				if(t[0] == '=') {
					optstr = t + 1;
				}
				else if(t[0] != 0) {
					optstr = t;
				}
				else if(n + 1 < argc) {
					n++;
					optstr = argv[n];
					if(optstr[0] == '-') {
						n--; optstr = NULL;
					}
				}
			}
			d->setopt(ctx, optnum, optstr);
		}
	}
	L_SET_KONOHA_ARGV:;
	argc = argc - n;
	argv = argv + n;
	{
		int i;
		knh_Array_t *a = new_Array(ctx, CLASS_String, argc);
		for(i = 1; i < argc; i++) {
			knh_Array_add(ctx, a, new_T(argv[i]));
		}
		knh_DictMap_set(ctx, DP(ctx->sys)->props, new_T("script.argv"), a);
		if(argc > 0) {
			knh_String_t *s = new_T(argv[0]);
			knh_DictMap_set(ctx, DP(ctx->sys)->props, new_T("script.name"), s);
			s = new_S(ctx, knh_bytes_nsname(S_tobytes(s)));
			KNH_SETv(ctx, DP(ctx->share->rootns)->nsname, s);
		}
	}
	return n;
}

/* ------------------------------------------------------------------------ */

static int konoha_runMain(konoha_t konoha, int argc, const char **argv)
{
	KONOHA_CHECK(konoha, -1);
	CTX ctx = (CTX)konoha.ctx;
	KONOHA_BEGIN(konoha.ctx);
	knh_Method_t *mtd = ClassTBL_getMethodNULL(ctx, O_cTBL(ctx->script), MN_main);
	int res = 0;
	if(mtd != NULL) {
		int thisidx = 1 + K_CALLDELTA;
		BEGIN_LOCAL(ctx, lsfp, 5);
		lsfp[1].ivalue = 0;
		lsfp[thisidx+K_PCIDX].pc = NULL;
		klr_setmtdNC(ctx,lsfp[thisidx+K_MTDIDX], mtd);
		KNH_SETv(ctx, lsfp[thisidx].o, ctx->script);
		KNH_SETv(ctx, lsfp[thisidx+1].o, knh_getPropertyNULL(ctx, STEXT("script.argv")));
		klr_setesp(ctx, lsfp + thisidx+2);
		knh_VirtualMachine_run(ctx, lsfp + thisidx, CODE_LAUNCH);
		res = (int)lsfp[1].ivalue;
		END_LOCAL_(ctx, lsfp);
	}
	KONOHA_END(ctx);
	return res;
}

/* ------------------------------------------------------------------------ */
/* [shell] */

static void knh_showWelcome(CTX ctx, knh_OutputStream_t *w)
{
	const knh_sysinfo_t *sysinfo = knh_getsysinfo();
	knh_printf(ctx, w, "%s%s %s(%s) %s (rev:%d, %s %s)%s\n",
		TERM_BBOLD(ctx),
		sysinfo->konoha_type, sysinfo->konoha_version, sysinfo->konoha_codename,
		K_DISTTYPE, ((knh_intptr_t)K_REVISION), __DATE__, __TIME__, TERM_EBOLD(ctx));
	//knh_printf(ctx, w, "[%s] on %s (%d, %s)\n", CC_TYPE, sysinfo->kern_ostype, sysinfo->konoha_systembits, knh_getSystemEncoding());
	knh_printf(ctx, w, "options: %sused_memory:%d kb\n",
		sysinfo->konoha_options, (knh_intptr_t)(ctx->stat->usedMemorySize / 1024));
}

static int shell_checkstmt(knh_bytes_t t)
{
	size_t i = 0;
	int ch, quote = 0, nest = 0;
	int hasDOC = 0;
	L_NORMAL:
	for(; i < t.len; i++) {
		ch = t.ubuf[i];
		if(ch == '{' || ch == '[' || ch == '(') nest++;
		if(ch == '}' || ch == ']' || ch == ')') nest--;
		if(ch == '\'' && i > 0 && (islower(t.text[i-1]) || t.text[i-1] == '\'')) continue; // a' prime
		if(ch == '\'' || ch == '"' || ch == '`') {
			quote = ch; i++;
			goto L_QUOTE;
		}
		if(ch == '\n') hasDOC = 0;
		if(ch == '#') {
			hasDOC = 1;
//			quote = '\n'; i++;
//			goto L_QUOTE;
		}
	}
	return (hasDOC == 1) ? 1 : nest;
	L_QUOTE:
	DBG_ASSERT(i > 0);
	for(; i < t.len; i++) {
		ch = t.ubuf[i];
		if(t.ubuf[i-1] != '\\' && ch == quote) {
			i++;
			goto L_NORMAL;
		}
	}
	return 1;
}

static void shell_restart(CTX ctx)
{
//	knh_NameSpace_t *ns = new_NameSpace(ctx, NULL);
//	DBG_ASSERT(ns->b->aliasDictMapNULL == NULL);
//	ctx->wshare->sysAliasDictMapNULL = DP(ctx->share->rootns)->aliasDictMapNULL;
//	DP(ctx->share->rootns)->aliasDictMapNULL = NULL;
//	KNH_SETv(ctx, ((knh_share_t*)ctx->share)->rootns, ns);
//	KNH_SETv(ctx, ((knh_context_t*)ctx)->script, new_(Script));
//	{
//		knh_Gamma_t *newgma = new_(Gamma);
//		KNH_SETv(ctx, ((knh_context_t*)ctx)->gma, newgma);
//		KNH_INITv(DP(newgma)->symbolDictMap, new_DictMap0(ctx, 256, 0/*isCaseMap*/, "Gamma.symbolDictMap"));
//		KNH_INITv(DP(newgma)->constPools, new_Array0(ctx, 0));
//		KNH_INITv(DP(newgma)->script, ctx->script);
//	}
	KNH_TODO("restart");
}

void knh_dumpKeyword(CTX ctx, knh_OutputStream_t *w);

static knh_status_t shell_command(CTX ctx, const char *cmd)
{
	knh_bytes_t t = {{cmd}, knh_strlen(cmd)};
	if(B_equals(t, "quit") || B_equals(t, "exit") || B_equals(t, "bye")) {
		return K_BREAK;
	}
	if(B_equals(t, "keyword")) {
		knh_dumpKeyword(ctx, KNH_STDOUT);
		return K_REDO;
	}
	if(B_equals(t, "restart")) {
		fputs(
"=============================================================================="
"\n\n\n", stdout);
		shell_restart(ctx);
		return K_REDO;
	}
	return K_CONTINUE;
}

/* ------------------------------------------------------------------------ */
/* standard shell api */

static void* shell_init(CTX ctx, const char *msg, const char *optstr)
{
	fputs(msg, stdout);
	CTX_setInteractive(ctx, 1);
	CTX_setDebug(ctx, 1);
	return NULL; // nostatus
}

static void shell_display(CTX ctx, void *status, const char *msg)
{
	fputs(msg, stdout);
}

static void shell_cleanup(CTX ctx, void *status)
{
	CTX_setInteractive(ctx, 0);
}

#ifdef K_PREVIEW
static const char* SecurityAlertMessage =  "** FOR EVALUATION/DEVELOPMENT USE ONLY **";
#else
static const char* SecurityAlertMessage = NULL;
#endif

void knh_setSecurityAlertMessage(const char *msg, int isNeedFree)
{
	static int isSecurityAlertNeedFree = 0;
	if(SecurityAlertMessage != NULL && isSecurityAlertNeedFree) {
		free((void*)SecurityAlertMessage);
	}
	SecurityAlertMessage = msg;
	isSecurityAlertNeedFree = isNeedFree;
}

static void knh_showSecurityAlert(CTX ctx, knh_OutputStream_t *w)
{
	if(SecurityAlertMessage != NULL /*&& CTX_isInteractive(ctx)*/) {
		knh_write_ascii(ctx, w, TERM_BBOLD(ctx));
		knh_write_ascii(ctx, w, "SECURITY ALERT: ");
		knh_write_ascii(ctx, w, SecurityAlertMessage);
		knh_write_ascii(ctx, w, TERM_EBOLD(ctx));
		knh_write_EOL(ctx, w); knh_write_EOL(ctx, w);
		knh_setSecurityAlertMessage(NULL, 0);
	}
}

static knh_status_t readstmt(CTX ctx, knh_cwb_t *cwb)
{
	int line = 1;
	knh_status_t status = K_CONTINUE;
	knh_cwb_clear(cwb, 0);
	fputs(TERM_BBOLD(ctx), stdout);
	while(1) {
		int check;
		char *ln = ctx->spi->readline(line == 1 ? ">>> " : "    ");
		if(ln == NULL) {
			knh_cwb_clear(cwb, 0);
			status = K_BREAK;
			break;
		}
		if(line > 1) knh_Bytes_putc(ctx, cwb->ba, '\n');
		knh_Bytes_write(ctx, cwb->ba, B(ln));
		free(ln);
		if((check = shell_checkstmt(knh_cwb_tobytes(cwb))) > 0) {
			line++;
			continue;
		}
		if(check < 0) {
			fputs("(Cancelled)...\n", stdout);
			knh_cwb_clear(cwb, 0);
		}
		break;
	}
	if(knh_cwb_size(cwb) > 0) {
		const char *p = knh_cwb_tochar(ctx, cwb);
		ctx->spi->add_history(p);
	}
	fputs(TERM_EBOLD(ctx), stdout);
	return status;
}

#ifdef K_USING_READLINE
extern char *readline(const char *) __attribute__ ((weak_import));
extern int  add_history(const char *) __attribute__ ((weak_import));

#else

static char* readline(const char* prompt)
{
	static int checkCTL = 0;
	int ch, pos = 0;
	static char linebuf[1024]; // THREAD-UNSAFE
	fputs(prompt, stdout);
	while((ch = fgetc(stdin)) != EOF) {
		//fprintf(stderr, "%d: %c\n", ch, ch);
		if(ch == '\r') continue;
		if(ch == 27) {
			/* ^[[A */;
			fgetc(stdin); fgetc(stdin);
			if(checkCTL == 0) {
				fprintf(stdout, " - readline would provide you with better shell experience.\n");
				checkCTL = 1;
			}
			continue;
		}
		if(ch == '\n' || pos == sizeof(linebuf) - 1) {
			linebuf[pos] = 0;
			break;
		}
		linebuf[pos] = ch;
		pos++;
	}
	if(ch == EOF) return NULL;
	{
		char *p = (char*)malloc(pos+1);
		knh_memcpy(p, linebuf, pos+1);
		return p;
	}
}

static int add_history(const char* line)
{
	return 0;
}
#endif

static void knh_linkDynamicReadline(CTX ctx)
{
	if(ctx->spi->readline == NULL) {
		void *handler = knh_dlopen(ctx, "libreadline" K_OSDLLEXT);
		if(handler != NULL) {
			void *f = knh_dlsym(ctx, LOG_DEBUG, handler, "readline");
			if(f != NULL) {
				((knh_ServiceSPI_t*)ctx->spi)->readlinespi = "libreadline";
				((knh_ServiceSPI_t*)ctx->spi)->readline = (char* (*)(const char*))f;
			}
			else {
				goto L_STDIN;
			}
			f = knh_dlsym(ctx, LOG_DEBUG, handler, "add_history");
			if(f != NULL) {
				((knh_ServiceSPI_t*)ctx->spi)->add_history = (int (*)(const char*))f;
			}
			else {
				((knh_ServiceSPI_t*)ctx->spi)->add_history = add_history;
			}
			return;
		}
		L_STDIN:;
		((knh_ServiceSPI_t*)ctx->spi)->readlinespi = "stdin";
		((knh_ServiceSPI_t*)ctx->spi)->readline = readline;
		((knh_ServiceSPI_t*)ctx->spi)->add_history = add_history;
	}
}

static void knh_shell(CTX ctx)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	void *shell_status = NULL;
	BEGIN_LOCAL(ctx, lsfp, 2);
	LOCAL_NEW(ctx, lsfp, 0, knh_Array_t *, results, new_Array0(ctx, 0));
	LOCAL_NEW(ctx, lsfp, 1, knh_InputStream_t *, bin, new_BytesInputStream(ctx, new_Bytes(ctx, "shell", K_PAGESIZE)));
	knh_linkDynamicReadline(ctx);
	knh_showWelcome(ctx, cwb->w);
	knh_showSecurityAlert(ctx, cwb->w);
	shell_status = shell_init(ctx, knh_cwb_tochar(ctx, cwb), NULL);
	while(1) {
		size_t i;
		knh_status_t status = readstmt(ctx, cwb);
		if(status == K_BREAK) break;
		if(knh_cwb_size(cwb) == 0) continue;
		status = shell_command(ctx, knh_cwb_tochar(ctx, cwb));
		if(status == K_BREAK) break;
		if(status == K_REDO) continue;
		knh_Bytes_clear(DP(bin)->ba, 0);
		knh_Bytes_write(ctx, DP(bin)->ba, knh_cwb_tobytes(cwb));
		knh_InputStream_setpos(ctx, bin, 0, BA_size(DP(bin)->ba));
		knh_cwb_clear(cwb, 0);
		SP(bin)->uline = 1; // always line1
		knh_eval(ctx, bin, results);
		knh_OutputStream_flush(ctx, ctx->out, 1);
		if(ctx->out != DP(ctx->sys)->out) {
			knh_Bytes_t *outbuf = DP(ctx->out)->ba;
			knh_write(ctx, cwb->w, outbuf->bu);
			knh_Bytes_clear(outbuf, 0);
		}
		knh_cwb_clear(cwb, 0); // necessary (because of some bugs)
		for(i = 0; i < knh_Array_size(results); i++) {
			knh_Object_t *o = results->list[i];
			knh_write_Object(ctx, cwb->w, o, FMT_dump);
		}
		knh_showSecurityAlert(ctx, cwb->w);
		if(knh_cwb_size(cwb) !=0) {
			shell_display(ctx, shell_status, knh_cwb_tochar(ctx, cwb));
			knh_cwb_clear(cwb, 0);
		}
		knh_Array_clear(ctx, results, 0);
		knh_cwb_clear(cwb, 0);
	}
	shell_cleanup(ctx, shell_status);
	knh_cwb_close(cwb);
	END_LOCAL_(ctx, lsfp);
}

static void konoha_shell(konoha_t konoha, char *optstr)
{
	KONOHA_CHECK_(konoha);
	CTX ctx = (CTX)konoha.ctx;
	KONOHA_BEGIN(konoha.ctx);
	knh_loadScriptPackageList(ctx, "konoha.i?");
#ifdef K_USING_SECURITY_ALERT
	knh_askSecurityAlert(ctx);
#endif
	CTX_setInteractive(ctx, 1);
	knh_shell(ctx);
	KONOHA_END(ctx);
}

/*************************************************************************** */

void konoha_main(konoha_t konoha, int argc, const char **argv)
{
	const char** args = argv;
	int n = konoha_parseopt(konoha, argc, args);
	if(argc - n == 0) {
		konoha_shell(konoha, NULL);
	}
	else {
		if(uout != NULL) {
			fprintf(uout, "testing: %s\n", args[n]);
			fflush(uout);
		}
		if(konoha_initload(konoha, args[n]) == K_CONTINUE && !knh_isCompileOnly(konoha.ctx)) {
			konoha_runMain(konoha, argc - n, args + n);
			if(isInteractiveMode) {
				konoha_shell(konoha, NULL);
			}
		}
	}
	if(uout != NULL) {
		fclose(uout);
	}
}

/*************************************************************************** */

#ifdef __cplusplus
}
#endif
