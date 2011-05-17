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

#define USE_bytes_rindex  1
#define USE_cwb_open      1

#include"commons.h"
#include"../../include/konoha1/konoha_code_.h"

#if defined(K_USING_POSIX_)
#include<time.h>
#include<errno.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [file] */

const char* knh_sfile(const char *file)
{
	if(file != NULL) {
		knh_bytes_t t;
		t.text = file; t.len = knh_strlen(file);
		int loc = knh_bytes_rindex(t, '/');
		if(loc == -1) {
			return file;
		}
		return t.text + loc + 1;
	}
	return "(unknown)";
}

/* ------------------------------------------------------------------------ */

void dbg_p(const char *file, const char *func, int line, const char *fmt, ...)
{
#ifndef K_USING_DEBUG
	if(knh_isSystemVerbose()) {
#endif
		va_list ap;
		va_start(ap , fmt);
		fflush(stdout);
		fprintf(stderr, "DEBUG(%s:%d) ", func, line);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end(ap);
#ifndef K_USING_DEBUG
	}
#endif
}

/* ------------------------------------------------------------------------ */

void todo_p(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "TODO(%s:%d) ", knh_sfile(file), line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

static void knh_write_cline(CTX ctx, knh_OutputStream_t *w, const char *file, knh_uintptr_t line)
{
	knh_putc(ctx, w, '(');
	knh_write_ascii(ctx, w, knh_sfile(file));
	knh_putc(ctx, w, ':');
	knh_write_dfmt(ctx, w, K_INTPTR_FMT, line);
	knh_putc(ctx, w, ')');
	knh_putc(ctx, w, ' ');
}

void knh_write_uline(CTX ctx, knh_OutputStream_t *w, knh_uline_t uline)
{
	knh_uri_t uri = ULINE_uri(uline);
	knh_uintptr_t line = ULINE_line(uline);
	if(uline == 0 || uri == URI_unknown || line == 0) {
		//knh_write_ascii(ctx, w, "");
	}
	else {
		knh_write_cline(ctx, w, FILENAME__(uri), line);
	}
}

static const char* knh_format_uline(CTX ctx, char *buf, size_t bufsiz, knh_uline_t uline)
{
	knh_uri_t uri = ULINE_uri(uline);
	knh_uintptr_t line = ULINE_line(uline);
	if(uline == 0 || uri == URI_unknown || line == 0) {
		buf[0] = 0;
	}
	else {
		snprintf(buf, bufsiz, "(%s:%d) ", knh_sfile(FILENAME__(uri)), (int)line);
	}
	return (const char*)buf;
}

void readuline(FILE *fp, char *buf, size_t bufsiz)
{
	int ch;
	size_t p = 0;
	while((ch = fgetc(fp)) == -1) {
		if(ch == ' ' || ch == '\t') continue;
	}
	while((ch = fgetc(fp)) == -1) {
		if(ch == '\n') break;
		buf[p] = ch; p++;
		if(p + 1 < bufsiz) continue;
	}
	buf[p] = 0;
}

const char* knh_readuline(CTX ctx, knh_uline_t uline, char *buf, size_t bufsiz)
{
	knh_uri_t uri = ULINE_uri(uline);
	size_t line = ULINE_line(uline);
	buf[0] = 0;
	if(uline != 0 && uri != URI_unknown && line != 0) {
		char const *fname = FILENAME__(uri);
		FILE *fp = fopen(fname, "r");
		if(fp != NULL) {
			size_t linec = 1;
			int ch;
			if(line == line) {
				readuline(fp, buf, bufsiz);
			}
			else {
				while((ch = fgetc(fp)) == -1) {
					if(ch == '\n') {
						linec++;
						if(linec == line) {
							readuline(fp, buf, bufsiz);
							break;
						}
					}
				}
			}
			fclose(fp);
		}
	}
	return (const char*)buf;
}

/* ------------------------------------------------------------------------ */
/* [throw] */

static knh_bool_t isCalledMethod(CTX ctx, knh_sfp_t *sfp)
{
	knh_Method_t *mtd = sfp[0].mtdNC;
	if(knh_isObject(ctx, mtd) && IS_Method(mtd)) {
		//DBG_P("FOUND mtdNC: shift=%d, pc=%d", sfp[-2].shift, sfp[-1].pc);
		return 1;
	}
	return 0;
}

static knh_uline_t sfp_uline(CTX ctx, knh_sfp_t *sfp)
{
	knh_opline_t *pc = sfp[K_PCIDX].pc;
	DBG_ASSERT(isCalledMethod(ctx, sfp + K_MTDIDX));
	if(pc == NULL) return 0;
	{
		int line = (pc-1)->line;
		while(pc->opcode != OPCODE_THCODE) pc--;
		knh_uri_t uri = ((klr_THCODE_t*)pc)->uri;
		return new_ULINE(uri, line);
	}
}

static knh_uline_t knh_stack_uline(CTX ctx, knh_sfp_t *sfp)
{
	if(sfp != NULL) {
		DBG_ASSERT(isCalledMethod(ctx, sfp + K_MTDIDX));
		return sfp_uline(ctx, sfp);
	}
	else {
		//DBG_P("Find Called Method.. ");
		sfp = ctx->esp;
		while(ctx->stack + 3 < sfp) {
			if(sfp[K_MTDIDX].mtdNC != NULL && isCalledMethod(ctx, sfp + K_MTDIDX)) {
				return sfp_uline(ctx, sfp);
			}
			sfp--;
		}
	}
	return 0;
}




void knh_write_sfp(CTX ctx, knh_OutputStream_t *w, knh_type_t type, knh_sfp_t *sfp, int level)
{
	if(IS_Tunbox(type)) {
		if(IS_Tint(type)) {
			knh_write_ifmt(ctx, w, K_INT_FMT, sfp[0].ivalue);
		}
		else if(IS_Tfloat(type)) {
			knh_write_ffmt(ctx, w, K_FLOAT_FMT, sfp[0].fvalue);
		}
		else {
			knh_write_bool(ctx, w, sfp[0].bvalue);
		}
	}
	else {
		knh_write_Object(ctx, w, sfp[0].o, level);
	}
}

static void knh_Exception_addStackTrace(CTX ctx, knh_Exception_t *e, knh_sfp_t *sfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Method_t *mtd = sfp[K_MTDIDX].mtdNC;
	if((mtd)->mn != MN_LAMBDA) {
		int i = 0, psize = knh_Method_psize(mtd);
		knh_uline_t uline = knh_stack_uline(ctx, sfp);
		knh_write_uline(ctx, cwb->w, uline);
		knh_write_type(ctx, cwb->w, (mtd)->cid);
		knh_putc(ctx, cwb->w, '.');
		knh_write_mn(ctx, cwb->w, (mtd)->mn);
		knh_putc(ctx, cwb->w, '(');
		for(i = 0; i < psize; i++) {
			knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, i);
			knh_type_t type = knh_type_tocid(ctx, p->type, O_cid(sfp[0].o));
			if(i > 0) {
				knh_putc(ctx, cwb->w, ',');
			}
			knh_write_fn(ctx, cwb->w, p->fn);
			knh_putc(ctx, cwb->w, '=');
			knh_write_sfp(ctx, cwb->w, type, &sfp[i+1], FMT_line);
		}
		knh_putc(ctx, cwb->w, ')');
		if(DP(e)->tracesNULL == NULL) {
			KNH_INITv(DP(e)->tracesNULL, new_Array(ctx, CLASS_String, 0));
		}
		knh_Array_add(ctx, DP(e)->tracesNULL, knh_cwb_newString(ctx, cwb));
	}
}

/* ------------------------------------------------------------------------ */

void knh_throw(CTX ctx, knh_sfp_t *sfp, long start)
{
	if(IS_Exception(ctx->e)) {
		knh_sfp_t *sp = (sfp == NULL) ? ctx->esp : sfp + start;
		knh_ExceptionHandler_t *hdr = ctx->ehdrNC;
		if(DP(ctx->e)->uline == 0) {
			DP(ctx->e)->uline = knh_stack_uline(ctx, sfp);
		}
		while(ctx->stack <= sp) {
			if(sp[0].mtdNC != NULL && isCalledMethod(ctx, sp)) {
				knh_Exception_addStackTrace(ctx, ctx->e, sp+1);
				sp[0].mtdNC = 0;
			}
			if(sp[0].hdr == hdr) {
				size_t i = 0, size = knh_Array_size(hdr->stacklist);
				for(i = 0; i < size; i++) {
					knh_Object_t *o = knh_Array_n(hdr->stacklist, i);
					O_cTBL(o)->ospi->checkout(ctx, o, 1);
				}
				knh_Array_trimSize(ctx, hdr->stacklist, 0);
#ifdef K_USING_SETJMP_
				knh_longjmp(DP(hdr)->jmpbuf, 1);
#else
				knh_ExceptionHandler_longjmp(ctx, hdr);
#endif
				goto L_NOCATCH;
			}
			sp--;
		}
		L_NOCATCH:;
		knh_write_Object(ctx, KNH_STDERR, UPCAST(ctx->e), FMT_dump);
		knh_exit(ctx, 0);
	}
}

static knh_Exception_t* new_Assertion(CTX ctx, knh_uline_t uline)
{
	knh_Exception_t* e = new_(Exception);
	char buf[256] = {'A', 's', 's', 'e', 'r', 't', 'i', 'o', 'n', '!', '!', ':', ' '};
	char *mbuf = buf + 13;
	knh_readuline(ctx, uline, mbuf, sizeof(buf)-13);
	if(mbuf[0] == 0) {
		knh_uri_t uri = ULINE_uri(uline);
		size_t line = ULINE_line(uline);
		knh_snprintf(buf, sizeof(buf), "Assertion!!: %s at line %ld", FILENAME__(uri), line);
	}
	DP(e)->eid = EBI_Assertion;
	DP(e)->flag = ctx->share->EventTBL[0].flag;
	{
		knh_bytes_t t = {{(const char*)buf}, knh_strlen(buf)};
		KNH_SETv(ctx, DP(e)->msg, new_S(ctx, t));
		KNH_SETv(ctx, DP(e)->event, DP(e)->msg);
	}
	DP(e)->uline = uline;
	return e;
}

void knh_assert(CTX ctx, knh_sfp_t *sfp, long start, knh_uline_t uline)
{
	CTX_setThrowingException(ctx, new_Assertion(ctx, uline));
	knh_throw(ctx, sfp, start);
}

knh_bool_t isCATCH(CTX ctx, knh_rbp_t *rbp, int en, knh_String_t *event)
{
	knh_Exception_t *e = ctx->e;
	int res = 0;
	if(knh_bytes_strcasecmp(S_tobytes(event), S_tobytes(DP(e)->event)) != 0) {
		knh_ebi_t eid = knh_geteid(ctx, S_tobytes(event), EBI_unknown);
		if(eid != EBI_unknown) {
			res = expt_isa(ctx, DP(e)->eid, eid);
		}
	}
	if(res == 1) {
		KNH_SETv(ctx, rbp[en].o, e);
		KNH_SETv(ctx, ((knh_context_t*)ctx)->e, KNH_NULL);
	}
	return res;
}

void Context_push(CTX ctx, knh_Object_t *o)
{
	knh_Array_t *a = ctx->ehdrNC->stacklist;
	knh_Array_add(ctx, a, o);
}

knh_Object_t *Context_pop(CTX ctx)
{
	knh_Array_t *a = ctx->ehdrNC->stacklist;
	DBG_ASSERT(a->size > 0);
	a->size -= 1;
	{
		knh_Object_t *o = a->list[a->size];
		KNH_FINALv(ctx, a->list[a->size]);
		return o;
	}
}

/* ------------------------------------------------------------------------ */
/* [syslog] */

#define K_EVENT_FORMAT " <%s:%s> "

static void knh_traceCFMT(CTX ctx, int pe, int isThrowable, const char *ns, const char *event, knh_uline_t uline, knh_sfp_t *sfp, const char *fmt, va_list ap)
{
	char newfmt[256];
	char linefmt[80];
	knh_format_uline(ctx, linefmt, sizeof(linefmt), uline);
	knh_snprintf(newfmt, sizeof(newfmt), "%s+" K_INT_FMT K_EVENT_FORMAT "%s%s", ctx->trace, ctx->seq, ns, event, linefmt, fmt);
	ctx->spi->vsyslog(pe, newfmt, ap);
	((knh_context_t*)ctx)->seq += 1;
	if(ctx->ehdrNC != NULL && isThrowable) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_vprintf(ctx, cwb->w, fmt, ap);
		knh_Exception_t *e = new_Error(ctx, EBI_Fatal, knh_cwb_newString(ctx, cwb));
		DP(e)->uline = uline;
		CTX_setThrowingException(ctx, e);
		knh_throw(ctx, sfp, 0);
	}
}

static void knh_tracePERROR(CTX ctx, int pe, int isThrowable, const char *ns, const char *event, knh_uline_t uline, knh_sfp_t *sfp, const char *fmt, va_list ap)
{
	int errno_ = errno;
#if defined(K_USING_WIN32_)
	char emsg[256];
	strerror_s(emsg, sizeof(emsg), errno_);
#elif defined(K_USING_POSIX_)
	char emsg[256];
	strerror_r(errno_, emsg, sizeof(emsg));
#else
	char *emsg = strerror(errno_);
#endif
	char newfmt[512];
	char linefmt[80];
	knh_format_uline(ctx, linefmt, sizeof(linefmt), uline);
	knh_snprintf(newfmt, sizeof(newfmt), "%s+" K_INT_FMT K_EVENT_FORMAT "%s%s ERRNO=%d, ERR='%s'",
			ctx->trace, ctx->seq, ns, event, linefmt, fmt, errno_, emsg);
	errno = 0;
	ctx->spi->vsyslog(pe, newfmt, ap);
	((knh_context_t*)ctx)->seq += 1;
	if(ctx->ehdrNC != NULL && isThrowable) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_vprintf(ctx, cwb->w, fmt, ap);
		knh_Exception_t *e = new_Error(ctx, EBI_Fatal, knh_cwb_newString(ctx, cwb));
		DP(e)->uline = uline;
		CTX_setThrowingException(ctx, e);
		knh_throw(ctx, sfp, 0);
	}
}

static void knh_traceKFMT(CTX ctx, int pe, int isThrowable, const char *ns, const char *event, knh_uline_t uline, knh_sfp_t *sfp, const char *fmt, va_list ap)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_printf(ctx, cwb->w, "%s+%i" K_EVENT_FORMAT, ctx->trace, ctx->seq, ns, event);
	knh_write_uline(ctx, cwb->w, uline);
	knh_vprintf(ctx, cwb->w, fmt, ap);
	ctx->spi->syslog(pe, knh_cwb_tochar(ctx, cwb));
	((knh_context_t*)ctx)->seq += 1;
	if(ctx->ehdrNC != NULL && isThrowable) {
		knh_Exception_t *e = new_Error(ctx, EBI_Fatal, knh_cwb_newString(ctx, cwb));
		DP(e)->uline = uline;
		CTX_setThrowingException(ctx, e);
		knh_throw(ctx, sfp, 0);
	}
	knh_cwb_clear(cwb, 0);
}

void knh_vtrace(CTX ctx, knh_sfp_t *sfp, int pe, const char *ns, const char *event, int isThrowable, const char *fmt, va_list ap)
{
	if(pe <= LOG_CRIT) isThrowable = 1;
	if(knh_isAuditLogging(pe)) {
		knh_uline_t uline = 0;
		if(ctx->ehdrNC != NULL) {
			if(ctx->gma != NULL && SP(ctx->gma)->uline != 0) {
				uline = SP(ctx->gma)->uline;
			}
			else {
				uline = knh_stack_uline(ctx, sfp);
			}
		}
		if(fmt[0] == '*') {
			knh_traceCFMT(ctx, pe, isThrowable, ns, event, uline, sfp, fmt+1, ap);
		}
		else if(fmt[0] == '!') {
			if(errno > 0) {
				if(errno != EACCES) pe = LOG_ALERT;
				knh_tracePERROR(ctx, pe, isThrowable, ns, event, uline, sfp, fmt+1, ap);
			}
			else {
				knh_traceCFMT(ctx, pe, isThrowable, ns, event, uline, sfp, fmt+1, ap);
			}
		}
		else {
			KNH_ASSERT(ctx->bufa != NULL);
			knh_traceKFMT(ctx, pe, isThrowable, ns, event, uline, sfp, fmt, ap);
		}
		if(pe == LOG_EMERG) {
			knh_exit(ctx, 0);
		}
	}
}

void knh_trace(CTX ctx, knh_sfp_t *sfp, int pe,const char *ns, const char *evt, int isThrowable, const char *fmt, ...)
{
	if(knh_isAuditLogging(pe)) {
		va_list ap;
		va_start(ap , fmt);
		knh_vtrace(ctx, sfp, pe, ns, evt, isThrowable, fmt, ap);
		va_end(ap);
	}
}

/* ------------------------------------------------------------------------ */

#ifndef LIBNAME
#define LIBNAME "konoha"
#endif

void THROW_Halt(CTX ctx, knh_sfp_t *sfp, const char *msg)
{
	KNH_THROW(ctx, sfp, LOG_CRIT, "InternalError!!", "InternalError!!: %s", msg);
}

void THROW_Arithmetic(CTX ctx, knh_sfp_t *sfp, const char *msg)
{
	KNH_THROW(ctx, sfp, LOG_CRIT, "Arithmetic!!", "Arithmetic!!: %s", msg);
}

void THROW_OutOfRange(CTX ctx, knh_sfp_t *sfp, knh_int_t n, size_t max)
{
	KNH_THROW(ctx, sfp, LOG_CRIT, "Model!!", "OutOfRange!!: %i not < %i", n, (knh_int_t)max);
}

void THROW_NoSuchMethod(CTX ctx, knh_sfp_t *sfp, knh_class_t cid, knh_methodn_t mn)
{
	KNH_THROW(ctx, sfp, LOG_CRIT, "Type!!", "NoSuchMethod!! %C.%M", cid, mn);
}

void THROW_ParamTypeError(CTX ctx, knh_sfp_t *sfp, size_t n, knh_methodn_t mn, knh_class_t reqt, knh_class_t cid)
{
	KNH_THROW(ctx, sfp, LOG_CRIT, "Type!!", "Type!!: %d of %M NOT %T", n, mn, cid);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
