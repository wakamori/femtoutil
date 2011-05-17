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

#include"commons.h"

/* ************************************************************************ */

#ifdef K_USING_CURL
#include<curl/curl.h>
#endif/*K_USING_CURL*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_INCLUDE_BUILTINAPI

/* ------------------------------------------------------------------------ */
/* K_DSPI_PATH */

/* ------------------------------------------------------------------------ */
/* [PATH] */

static knh_bool_t NOPATH_hasType(CTX ctx, knh_class_t cid, void *thunk)
{
	return 0;
}
static knh_bool_t NOPATH_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	return 0;
}
static knh_Object_t* NOPATH_newObjectNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	return NULL/*(knh_Object_t*)s*/;
}

static const knh_PathDSPI_t PATH_NOPATH = {
	K_DSPI_PATH, "NOPATH", CLASS_Tvoid, CLASS_Tvoid, NULL,
	NOPATH_hasType, NOPATH_exists, NOPATH_newObjectNULL,
};

/* charset:UTF-8 */
static knh_bool_t CHARSET_hasType(CTX ctx, knh_class_t cid, void *thunk)
{
	return (cid == CLASS_StringEncoder || cid == CLASS_StringDecoder);
}

static knh_bool_t CHARSET_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	knh_bytes_t t = knh_bytes_next(path, ':');
	iconv_t ic = ctx->spi->iconv_open(t.text, K_ENCODING);
	if(ic != (iconv_t)(-1)) {
		ctx->spi->iconv_close(ic);
		return 1;
	}
	else if(knh_bytes_strcasecmp(t, STEXT(K_ENCODING)) == 0) {
		return 1;
	}
	return 0;
}

static knh_Object_t* CHARSET_newObjectNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	knh_bytes_t t = knh_bytes_next(S_tobytes(s), ':');
	if(cid == CLASS_StringEncoder) {
		return (knh_Object_t*)new_StringEncoderNULL(ctx, t);
	}
	if(cid == CLASS_StringDecoder) {
		return (knh_Object_t*)new_StringDecoderNULL(ctx, t);
	}
	return NULL;
}

static const knh_PathDSPI_t PATH_CHARSET = {
	K_DSPI_PATH, "charset", CLASS_Boolean, CLASS_Tvoid, NULL,
	CHARSET_hasType, CHARSET_exists, CHARSET_newObjectNULL,
};

/* ------------------------------------------------------------------------ */
/* Path to:md5 */
/* Path from:toLower */

#define IS_CONV(cid) (CLASS_Converter <= cid && cid <= CLASS_StringConverter)

static Object* new_ConverterNULL(CTX ctx, knh_class_t cid, knh_bytes_t path, const knh_ConvDSPI_t *dspi)
{
	knh_conv_t *conv = NULL;
	DBG_ASSERT(IS_CONV(cid));
	if((cid == CLASS_StringConverter && dspi->sconv == NULL) ||
		(cid == CLASS_StringEncoder && dspi->enc == NULL) ||
		(cid == CLASS_StringDecoder && dspi->dec == NULL) ||
		(cid == CLASS_Converter && dspi->conv == NULL)) {
		return NULL;
	}
	if(dspi->open != NULL) {
		conv = dspi->open(ctx, path.text, NULL);
		if(conv == NULL) {
			KNH_WARN(ctx, "unknown path='%s'", path.text);
			return NULL;
		}
	}
	{
		knh_Converter_t *c = new_O(Converter, cid);
		c->dspi = dspi;
		c->conv = conv;
		return (knh_Object_t*)c;
	}
}

static knh_bool_t TOPATH_hasType(CTX ctx, knh_class_t cid, void *thunk)
{
	return (IS_CONV(cid));
}
static knh_bool_t TOPATH_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	const knh_ConvDSPI_t *dspi = knh_NameSpace_getConvTODSPINULL(ctx, ns, knh_bytes_next(path, ':'));
	return (dspi != NULL);
}
static knh_Object_t* TOPATH_newObjectNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	knh_bytes_t path = knh_bytes_next(S_tobytes(s), ':');
	const knh_ConvDSPI_t *dspi = knh_NameSpace_getConvTODSPINULL(ctx, ns, path);
	KNH_ASSERT(dspi != NULL); // checked before
	if(IS_CONV(cid)) {
		return new_ConverterNULL(ctx, cid, path, dspi);
	}
	return NULL;
}

static knh_bool_t FROMPATH_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	const knh_ConvDSPI_t *dspi = knh_NameSpace_getConvFROMDSPINULL(ctx, ns, knh_bytes_next(path, ':'));
	return (dspi != NULL);
}

static knh_Object_t* FROMPATH_newObjectNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	knh_bytes_t path = knh_bytes_next(S_tobytes(s), ':');
	const knh_ConvDSPI_t *dspi = knh_NameSpace_getConvFROMDSPINULL(ctx, ns, path);
	KNH_ASSERT(dspi != NULL); // checked before
	if(IS_CONV(cid)) {
		return new_ConverterNULL(ctx, cid, path, dspi);
	}
	return NULL;
}

static const knh_PathDSPI_t PATH_TOPATH = {
	K_DSPI_PATH, "to", CLASS_Converter, CLASS_Tvoid, NULL,
	TOPATH_hasType, TOPATH_exists, TOPATH_newObjectNULL,
};

static const knh_PathDSPI_t PATH_FROMPATH = {
	K_DSPI_PATH, "from", CLASS_Converter, CLASS_Tvoid, NULL,
	TOPATH_hasType, FROMPATH_exists, FROMPATH_newObjectNULL,
};

/* ------------------------------------------------------------------------ */
/* file:/usr/local */

static knh_bool_t FILE_hasType(CTX ctx, knh_class_t cid, void *thunk)
{
	return (cid == CLASS_InputStream || cid == CLASS_OutputStream);
}
static knh_bool_t FILE_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	knh_ospath(ctx, ph);
	knh_bool_t res = knh_path_isfile(ctx, ph);
	knh_path_close(ctx, ph);
	return res;
}

static knh_Object_t* FILE_newObjectNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	if(cid == CLASS_InputStream) {
		knh_InputStream_t *in = new_InputStreamNULL(ctx, ns, s, "r", ctx->mon);
		return (knh_Object_t*)in;
	}
	if(cid == CLASS_OutputStream) {
		knh_OutputStream_t *out = new_OutputStreamNULL(ctx, ns, s, "a", ctx->mon);
		return (knh_Object_t*)out;
	}
	return NULL;
}

static const knh_PathDSPI_t PATH_FILE = {
	K_DSPI_PATH, "file", CLASS_Boolean, CLASS_InputStream, NULL,
	FILE_hasType, FILE_exists, FILE_newObjectNULL,
};

static knh_bool_t LIB_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	void *p = NULL;
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	knh_bytes_t libname = knh_bytes_next(path, ':');
	knh_bytes_t funcname = knh_bytes_rnext(path, '.');
	int res = 0;
	knh_path_reduce(ctx, ph, '.');
	knh_path_append(ctx, ph, 0, K_OSDLLEXT);
	DBG_P("BEFORE: %s", P_text(ph) + ph->pbody);
	//knh_ospath(ctx, ph);
	p = knh_path_dlopen(ctx, ph);
	if(p == NULL && !knh_bytes_startsWith(libname, STEXT("lib"))) {
		knh_path_reset(ctx, ph, NULL, STEXT("lib"));
		knh_path_append(ctx, ph, 0, libname.text);
		//knh_path_reduce(ctx, ph, '.');
		knh_path_append(ctx, ph, 0, K_OSDLLEXT);
		DBG_P("BEFORE: %s", P_text(ph));
		//knh_ospath(ctx, ph);
		p = knh_path_dlopen(ctx, ph);
	}
	if(p != NULL) {
		if(funcname.len != libname.len) {
			void *f = knh_dlsym(ctx, LOG_DEBUG, p, funcname.text);
			res = (f != NULL);
		}
		knh_dlclose(ctx, p);
	}
	knh_path_close(ctx, ph);
	return res;
}

static const knh_PathDSPI_t PATH_LIB = {
	K_DSPI_PATH, "lib", CLASS_Boolean, CLASS_Tvoid, NULL,
	NOPATH_hasType, LIB_exists, NOPATH_newObjectNULL,
};

/* ------------------------------------------------------------------------ */
/* K_DSPI_STREAM */

static knh_bool_t NOFILE_realpath(CTX ctx, knh_NameSpace_t *ns, knh_path_t *ph)
{
	KNH_WARN(ctx, "unsupported scheme: %s", P_text(ph));
	return 0;
}
static knh_io_t NOFILE_open(CTX ctx, knh_path_t *ph, const char *mode, knh_Monitor_t *mon)
{
	return IO_NULL;
}
static knh_intptr_t NOFILE_read(CTX ctx, knh_io_t fd, char *buf, size_t bufsiz, knh_Monitor_t *mon)
{
	return 0;  // read nothing. this means that we reach the end of the stream.
}
static knh_intptr_t NOFILE_write(CTX ctx, knh_io_t fd, const char *buf, size_t bufsiz, knh_Monitor_t *mon)
{
	DBG_ASSERT(fd != IO_BUF);
	return bufsiz;
}
static knh_intptr_t BYTE_write(CTX ctx, knh_io_t fd, const char *buf, size_t bufsiz, knh_Monitor_t *mon)
{
	DBG_ASSERT(fd == IO_BUF);
	return 0; // write nothing. this means that we are unable to clear buffer
}
static void NOFILE_close(CTX ctx, knh_io_t fd)
{
}

static const knh_StreamDSPI_t STREAM_NOFILE = {
	K_DSPI_STREAM, "NOFILE", NOFILE_realpath,
	NOFILE_open, NOFILE_open, NOFILE_read, K_PAGESIZE, NOFILE_write, NOFILE_close
};

static const knh_StreamDSPI_t STREAM_BYTE = {
	K_DSPI_STREAM, "BYTE", NOFILE_realpath,
	NOFILE_open, NOFILE_open, NOFILE_read, K_OUTBUF_MAXSIZ, BYTE_write, NOFILE_close
};

/* ------------------------------------------------------------------------ */

static knh_bool_t FILE_realpath(CTX ctx, knh_NameSpace_t *ns, knh_path_t *ph)
{
	knh_ospath(ctx, ph);
	return knh_path_isfile(ctx, ph);
}

static knh_io_t FILE_open(CTX ctx, knh_path_t *ph, const char *mode, knh_Monitor_t *mon)
{
	const char *fname = P_text(ph) + ph->pbody;
	return (knh_io_t)knh_fopen(ctx, LOG_ERR, fname, mode);
}

static knh_io_t NOFILE_wopen(CTX ctx, knh_path_t *ph, const char *mode, knh_Monitor_t *mon)
{
	KNH_WARN(ctx, "nofile path='%s', mode='%s'", P_text(ph), mode);
	return IO_NULL;
}
static knh_intptr_t FILE_read(CTX ctx, knh_io_t fio, char *buf, size_t bufsiz, knh_Monitor_t *mon)
{
	return knh_fread(ctx, buf, bufsiz, (FILE*)fio);
}

static knh_intptr_t FILE_write(CTX ctx, knh_io_t fio, const char *buf, size_t bufsiz, knh_Monitor_t *mon)
{
	size_t ssize = knh_fwrite(ctx, buf, bufsiz, (FILE*)fio);
	knh_fflush(ctx, (FILE*)fio);
	return ssize;
}

static void FILE_close(CTX ctx, knh_io_t fd)
{
	knh_fclose(ctx, (FILE*)fd);
}

static const knh_StreamDSPI_t STREAM_FILE = {
	K_DSPI_STREAM, "file", FILE_realpath,
	FILE_open, FILE_open, FILE_read, K_OUTBUF_MAXSIZ, FILE_write, FILE_close,
};

static const knh_StreamDSPI_t STREAM_STDIO = {
	K_DSPI_STREAM, "stdio", NOFILE_realpath,
	NOFILE_open, NOFILE_open, FILE_read, K_OUTBUF_MAXSIZ, FILE_write, NOFILE_close,
};

static knh_InputStream_t *new_InputStreamSTDIO(CTX ctx, FILE *fp, knh_String_t *enc)
{
	knh_InputStream_t* in = new_InputStreamDSPI(ctx, (knh_io_t)stdin, &STREAM_STDIO);
	KNH_SETv(ctx, DP(in)->urn, TS_DEVSTDIN);
	return in;
}

static knh_OutputStream_t *new_OutputStreamSTDIO(CTX ctx, FILE *fp, knh_String_t *enc)
{
#ifdef K_USING_NOFILE
	knh_OutputStream_t* o = new_OutputStream__FILE(ctx, TS_DEVSTDOUT, NULL, &STREAM_STDIO);
#else
	KNH_ASSERT(fp == stdout || fp == stderr);
	knh_OutputStream_t* o = new_OutputStreamDSPI(ctx, (knh_io_t)fp, &STREAM_STDIO);
	knh_String_t *urn = (fp == stdout) ? TS_DEVSTDOUT : TS_DEVSTDERR;
	KNH_SETv(ctx, DP(o)->urn, urn);
#endif
	OutputStream_setAutoFlush(o, 1);
	return o;
}

/* ------------------------------------------------------------------------ */

static knh_bool_t hasPKG(CTX ctx, knh_path_t *ph, knh_bytes_t tpath, knh_bytes_t path)
{
	knh_path_reset(ctx, ph, NULL, tpath);
	knh_path_append(ctx, ph, 1, path.text); // konoha.math
	knh_path_append(ctx, ph, 1, knh_bytes_rnext(path, '.').text); // math
	knh_path_append(ctx, ph, 0, ".k");
	knh_ospath(ctx, ph);
	return knh_path_isfile(ctx, ph);
}

static knh_bool_t PKG_realpath(CTX ctx, knh_NameSpace_t *ns, knh_path_t *ph)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_copy(ctx, &cwbbuf, ph, 0/*hasScheme*/);
	int res = 1;
	char *epath = knh_getenv("KONOHA_PACKAGE");
	if(epath != NULL) {
		if(hasPKG(ctx, ph, B(epath), knh_cwb_tobytes(cwb))) {
			goto L_RETURN;
		}
	}
	knh_String_t *tpath = knh_getPropertyNULL(ctx, STEXT("konoha.package.path"));
	if(tpath != NULL) {
		if(hasPKG(ctx, ph, S_tobytes(tpath), knh_cwb_tobytes(cwb))) {
			goto L_RETURN;
		}
	}
	tpath = knh_getPropertyNULL(ctx, STEXT("user.package.path"));
	if(tpath != NULL) {
		if(hasPKG(ctx, ph, S_tobytes(tpath), knh_cwb_tobytes(cwb))) {
			goto L_RETURN;
		}
	}
	res = 0;
	L_RETURN:;
	knh_cwb_close(cwb);
	return res;
}

static knh_bool_t PKG_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	knh_bool_t res = PKG_realpath(ctx, ns, ph);
	knh_path_close(ctx, ph);
	return res;
}

static const knh_PathDSPI_t PATH_PKG = {
	K_DSPI_PATH, "pkg", CLASS_Boolean, CLASS_InputStream, NULL,
	NOPATH_hasType, PKG_exists, NOPATH_newObjectNULL,
};

static const knh_StreamDSPI_t STREAM_PKG = {
	K_DSPI_STREAM, "pkg", PKG_realpath,
	FILE_open, NOFILE_wopen, FILE_read, 0, NOFILE_write, FILE_close,
};

static knh_bool_t SCRIPT_realpath(CTX ctx, knh_NameSpace_t *ns, knh_path_t *ph)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_copy(ctx, &cwbbuf, ph, 0/*hasScheme*/);
	knh_path_reset(ctx, ph, NULL, S_tobytes(ns->rpath));
	if(!knh_path_isdir(ctx, ph)) {
		knh_path_reduce(ctx, ph, '/');
	}
	knh_path_append(ctx, ph, 1/*sep*/, knh_cwb_tochar(ctx, cwb));
	knh_cwb_close(cwb);
	knh_ospath(ctx, ph);
	return knh_path_isfile(ctx, ph);
}

static knh_bool_t SCRIPT_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	knh_bool_t res = SCRIPT_realpath(ctx, ns, ph);
	knh_path_close(ctx, ph);
	return res;
}

static const knh_PathDSPI_t PATH_SCRIPT = {
	K_DSPI_PATH, "script", CLASS_Boolean, CLASS_InputStream, NULL,
	FILE_hasType, SCRIPT_exists, FILE_newObjectNULL,
};

static const knh_StreamDSPI_t STREAM_SCRIPT = {
	K_DSPI_STREAM, "script", SCRIPT_realpath,
	FILE_open, NOFILE_wopen, FILE_read, K_OUTBUF_MAXSIZ, FILE_write, FILE_close,
};

static knh_bool_t START_realpath(CTX ctx, knh_NameSpace_t *ns, knh_path_t *ph)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_copy(ctx, &cwbbuf, ph, 0/*hasScheme*/);
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(!knh_bytes_endsWith(t, STEXT(".k"))) {
		knh_String_t *s = knh_getPropertyNULL(ctx, STEXT("konoha.script.path"));
		if(s != NULL) {
			knh_path_reset(ctx, ph, NULL, S_tobytes(s));
			knh_path_append(ctx, ph, 1/*sep*/, knh_cwb_tochar(ctx, cwb));
			knh_ospath(ctx, ph);
			if(knh_path_isfile(ctx, ph)) return 1;
		}
	}
	knh_path_reset(ctx, ph, NULL, S_tobytes(ns->rpath));
	if(!knh_path_isdir(ctx, ph)) {
		knh_path_reduce(ctx, ph, '/');
	}
	knh_path_append(ctx, ph, 1/*sep*/, knh_cwb_tochar(ctx, cwb));
	knh_cwb_close(cwb);
	knh_ospath(ctx, ph);
	return knh_path_isfile(ctx, ph);
}

static knh_bool_t START_exists(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	knh_bool_t res = START_realpath(ctx, ns, ph);
	knh_path_close(ctx, ph);
	return res;
}

static const knh_PathDSPI_t PATH_START = {
		K_DSPI_PATH, "start", CLASS_Boolean, CLASS_InputStream, NULL,
		FILE_hasType, START_exists, FILE_newObjectNULL,
};

static const knh_StreamDSPI_t STREAM_START = {
		K_DSPI_STREAM, "start", START_realpath,
		FILE_open, NOFILE_wopen, FILE_read, K_OUTBUF_MAXSIZ, FILE_write, FILE_close,
};

/* ------------------------------------------------------------------------ */

static void SYSLOG_UnknownPathType(CTX ctx, knh_bytes_t path)
{
	KNH_WARN(ctx, "undefined path='%s'", path.text);
}

const knh_StreamDSPI_t *knh_getDefaultStreamDSPI(void)
{
	return &STREAM_NOFILE;
}

const knh_StreamDSPI_t *knh_getByteStreamDSPI(void)
{
	return &STREAM_BYTE;
}

const knh_StreamDSPI_t *knh_getStreamDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	const knh_StreamDSPI_t *p = (const knh_StreamDSPI_t*)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_STREAM, path);
	if(p == NULL) {
		SYSLOG_UnknownPathType(ctx, path);
		p = &STREAM_NOFILE;
	}
	return p;
}

#ifdef K_USING_CURL

typedef struct {
	CURL *curl;
	const char *charset;
	const char *contenttype;
	char *buffer;               /* buffer to store cached data*/
	long pos;
	long buffer_len;             /* currently allocated buffers length */
	long buffer_pos;             /* end of data in buffer*/
	int still_running;          /* Is background url fetch still in progress */
} CURLFILE;

/* This global variable was originated in a sample code in libcurl */
static CURLM *multi_handle = NULL;
static CURLM *emulti_handle = NULL;

static size_t write_callback(char *buffer, size_t size, size_t nitems, void *userp)
{
	char *newbuff;
	CURLFILE *url = (CURLFILE *)userp;
	size *= nitems;
	long rembuff = url->buffer_len - url->buffer_pos; /* remaining space in buffer */
	if(size > rembuff) {
		newbuff = realloc(url->buffer, url->buffer_len + (size - rembuff));
		if(newbuff == NULL) {
			size = rembuff;
		}
		else {
			url->buffer_len += size - rembuff;
			url->buffer = newbuff;
		}
	}
	memcpy(&url->buffer[url->buffer_pos], buffer, size);
	url->buffer_pos += size;
	if(url->contenttype == NULL){
		char *type = NULL;
		curl_easy_getinfo(url->curl, CURLINFO_CONTENT_TYPE, &type);
		if(type != NULL){
			char *charset = NULL;
			charset = strrchr(type, '=');
			if(charset != NULL){
				charset++;
				type = strtok(type, ";");
			}
			url->charset = (const char*)charset;
			url->contenttype = (const char*)type;
		}
	}
	return size;
}

static int fill_buffer(CURLFILE *file, int want, int waittime)
{
	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	struct timeval timeout;
	int rc;
	/* only attempt to fill buffer if transactions still running and buffer
	 * doesnt exceed required size already
	 */
	if((!file->still_running) || (file->buffer_pos > want))
		return 0;

	/* attempt to fill buffer */
	do {
		int maxfd = -1;
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);

		/* set a suitable timeout to fail on */
		timeout.tv_sec = 60; /* 1 minute */
		timeout.tv_usec = 0;

		/* get file descriptors from the transfers */
		curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

		/* In a real-world program you OF COURSE check the return code of the
           function calls.  On success, the value of maxfd is guaranteed to be
           greater or equal than -1.  We call select(maxfd + 1, ...), specially
           in case of (maxfd == -1), we call select(0, ...), which is basically
           equal to sleep. */

		rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
		switch(rc) {
		case -1: /* select error */
		case 0: break;
		default:
			/* timeout or readable/writable sockets */
			/* note we *could* be more efficient and not wait for
			 * CURLM_CALL_MULTI_PERFORM to clear here and check it on re-entry
			 * but that gets messy */
			while(curl_multi_perform(multi_handle, &file->still_running) == CURLM_CALL_MULTI_PERFORM);
			break;
		}
	} while(file->still_running && (file->buffer_pos < want));
	return 1;
}

/* use to remove want bytes from the front of a files buffer */
static int use_buffer(CURLFILE *file, int want)
{
	/* sort out buffer */
	if((file->buffer_pos - want) <= 0) {
		/* ditch buffer - write will recreate */
		if(file->buffer != NULL) free(file->buffer);
		file->buffer=NULL;
		file->buffer_pos=0;
		file->buffer_len=0;
	} else { /* move rest down make it available for later */
		memmove(file->buffer, &file->buffer[want], (file->buffer_pos - want));
		file->buffer_pos -= want;
	}
	return 0;
}

static knh_io_t CURL_open(Ctx *ctx, knh_bytes_t path, const char *mode, knh_Monitor_t *mon)
{
	//TODO mode treats as method
	CURLFILE *file = malloc(sizeof(CURLFILE));
	if(file == NULL) return (knh_io_t)NULL;
	memset(file, 0, sizeof(CURLFILE));
	file->curl = curl_easy_init();
	curl_easy_setopt(file->curl, CURLOPT_URL, path.ubuf);
	curl_easy_setopt(file->curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(file->curl, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(file->curl, CURLOPT_WRITEFUNCTION, write_callback);
	if(!multi_handle) multi_handle = curl_multi_init();
	curl_multi_add_handle(multi_handle, file->curl);

	/* lets start the fetch */
	while(curl_multi_perform(multi_handle, &file->still_running) == CURLM_CALL_MULTI_PERFORM );

	if((file->buffer_pos == 0) && (!file->still_running)) {
		/* if still_running is 0 now, we should return NULL */
		/* make sure the easy handle is not in the multi handle anymore */
		curl_multi_remove_handle(multi_handle, file->curl);
		/* cleanup */
		curl_easy_cleanup(file->curl);
		free(file);
		file = NULL;
	}
	return (knh_io_t)file;
}

//new OutputStream(http://...)
static knh_io_t CURL_init(Ctx *ctx, knh_bytes_t path, const char *mode, knh_Monitor_t *mon)
{
	return (knh_io_t)NULL;
}

static void CURL_close(Ctx *ctx, knh_io_t fd)
{
	CURLFILE *file = (CURLFILE*)fd;
	curl_multi_remove_handle(multi_handle, file->curl);
	curl_easy_cleanup(file->curl);
	if(file->buffer) free(file->buffer);
	free(file);
}

static int url_feof(CURLFILE *file)
{
	int ret=0;
	if((file->buffer_pos == 0) && (!file->still_running))
		ret = 1;
	return ret;
}

static knh_intptr_t CURL_read(Ctx *ctx, knh_io_t fd, char *buf, size_t bufsiz)
{
	CURLFILE *file = (CURLFILE*)fd;
	if(file == NULL) return 0;
	fill_buffer(file, bufsiz, 1);
	if(!file->buffer_pos) return 0;
	/* ensure only available data is considered */
	if(file->buffer_pos < bufsiz) bufsiz = file->buffer_pos;
	/* xfer data to caller */
	memcpy(buf, file->buffer, bufsiz);
	use_buffer(file, bufsiz);
	return bufsiz;
}

static knh_intptr_t CURL_write(Ctx *ctx, knh_io_t fd, const char *buf, size_t bufsiz)
{
	return 0;
}

static int CURL_feof(Ctx *ctx, knh_io_t fd)
{
	return url_feof((CURLFILE*)fd);
}

static int CURL_getc(Ctx *ctx, knh_io_t fd)
{
	CURLFILE *file = (CURLFILE*)fd;
	if(!file->buffer_pos || file->buffer_pos < file->buffer_len)
		fill_buffer(file, 1024, 1);
	return file->buffer[file->buffer_pos++];
}

static const char* CURL_getContentType(Ctx *ctx, knh_io_t fd)
{
	CURLFILE *file = (CURLFILE*)fd;
	if(file != NULL){
		if(file->contenttype != NULL){
			return file->contenttype;
		}
	}
	return NULL;
}

static const char* CURL_getCharset(Ctx *ctx, knh_io_t fd)
{
	CURLFILE *file = (CURLFILE*)fd;
	if(file != NULL){
		if(file->charset != NULL){
			return file->charset;
		}
	}
	return NULL;
}

static knh_StreamDSPI_t STREAM_CURL = {
		K_DSPI_STREAM, "curl", 0,
		CURL_open, CURL_init, CURL_read, CURL_write, CURL_close,
		CURL_feof, CURL_getc, CURL_getContentType, CURL_getCharset
};

static knh_bool_t CURL_exists(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t path, void *thunk)
{
	CURLFILE *file = knh_malloc(ctx, sizeof(CURLFILE));
	if(file == NULL) return PATH_unknown;
	memset(file, 0, sizeof(CURLFILE));
	file->curl = curl_easy_init();
	curl_easy_setopt(file->curl, CURLOPT_URL, path.ubuf);
	curl_easy_setopt(file->curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(file->curl, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(file->curl, CURLOPT_WRITEFUNCTION, write_callback);

	emulti_handle = curl_multi_init();
	curl_multi_add_handle(emulti_handle, file->curl);

	/* lets start the fetch */
	while(curl_multi_perform(emulti_handle, &file->still_running) == CURLM_CALL_MULTI_PERFORM );
	if((file->buffer_pos == 0) && (!file->still_running)) {
		/* if still_running is 0 now, we should return NULL */
		/* make sure the easy handle is not in the multi handle anymore */
		curl_multi_remove_handle(emulti_handle, file->curl);
		/* cleanup */
		curl_easy_cleanup(file->curl);
		knh_free(ctx, file, sizeof(CURLFILE));
		emulti_handle = NULL;
		file = NULL;
		return PATH_unknown;
	}
	emulti_handle = NULL;
	knh_free(ctx, file, sizeof(CURLFILE));
	return (knh_bool_t)1;
}

static knh_bool_t CURL_hasType(Ctx *ctx, knh_class_t cid, void *thunk)
{
	return (cid == CLASS_InputStream);
}

static Object* CURL_newObjectNULL(Ctx *ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_String_t *s, void *thunk)
{
	return (Object*)s;
}

static knh_PathDSPI_t PATH_CURL = {
		K_DSPI_PATH, "curl", CLASS_InputStream, CLASS_Tvoid,
		CURL_hasType, CURL_exists, CURL_newObjectNULL
};

#endif/*K_USING_CURL*/

/* ------------------------------------------------------------------------ */
/* K_DSPI_QUERY */

static void NOP_qfree(knh_qcur_t *qcur)
{
}
static knh_qconn_t *NOP_qopen(CTX ctx, knh_bytes_t url)
{
	return NULL;
}
static knh_qcur_t *NOP_query(CTX ctx, knh_qconn_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
	return NULL;
}
static void NOP_qclose(CTX ctx, knh_qconn_t *hdr)
{
}
static int NOP_qnext(CTX ctx, knh_qcur_t *qcur, struct knh_ResultSet_t *rs)
{
	return 0;  /* NOMORE */
}

static const knh_QueryDSPI_t QUERY_NOP = {
	K_DSPI_QUERY, "NOP",
	NOP_qopen, NOP_query, NOP_qclose, NOP_qnext, NOP_qfree
};

#ifdef K_USING_SQLITE3

#include<sqlite3.h>

static void knh_sqlite3_perror(CTX ctx, sqlite3 *db, int r)
{
	const char *msg = "sqlite3";
	if(r == SQLITE_PERM || r == SQLITE_AUTH) {
		msg = "Security";
	}
	KNH_WARN(ctx, "sqlite3_error='%s'", sqlite3_errmsg(db));
}

static knh_qconn_t *SQLITE3_qopen(CTX ctx, knh_bytes_t url)
{
	sqlite3 *db = NULL;
	url = knh_bytes_next(url, ':');
	int r = sqlite3_open(url.text, &db);
	if (r != SQLITE_OK) {
		return NULL;
	}
	return (knh_qconn_t*)db;
}

static int SQLITE3_qnext(CTX ctx, knh_qcur_t *qcur, struct knh_ResultSet_t *rs)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)qcur;
	int r = sqlite3_step(stmt);
	if(SQLITE_ROW == r) {
		size_t i;
		knh_ResultSet_initData(ctx, rs);
		for(i = 0; i < DP(rs)->column_size; i++) {
			int type = sqlite3_column_type(stmt, i);
			switch(type) {
				case SQLITE_INTEGER: {
					ResultSet_setInt(ctx, rs, i, (knh_int_t)sqlite3_column_int64(stmt, i));
					break;
				}
				case SQLITE_FLOAT: {
					ResultSet_setFloat(ctx, rs, i, (knh_float_t)sqlite3_column_double(stmt, i));
					break;
				}
				case SQLITE_TEXT: {
					knh_bytes_t t = {{(const char*)sqlite3_column_text(stmt,i)}, sqlite3_column_bytes(stmt, i)};
					ResultSet_setText(ctx, rs, i, t);
					break;
				}
				case SQLITE_BLOB: {
					knh_bytes_t t = {{(const char*)sqlite3_column_blob(stmt,i)}, sqlite3_column_bytes(stmt, i)};
					ResultSet_setBlob(ctx, rs, i, t);
					break;
				}
				case SQLITE_NULL:
				default: {
					ResultSet_setNULL(ctx, rs, i);
				}
			}
		}
		return 1;
	}
	else if (r != SQLITE_DONE) {
	   //
	}
	return 0;  /* NOMORE */
}

static knh_qcur_t *SQLITE3_query(CTX ctx, knh_qconn_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
	if(rs == NULL) {
		int r = sqlite3_exec((sqlite3*)hdr, sql.text, NULL, NULL, NULL);
		if(r != SQLITE_OK) {
			knh_sqlite3_perror(ctx, (sqlite3*)hdr, r);
		}
		return NULL;
	}
	else {
		sqlite3_stmt *stmt = NULL;
		sqlite3_prepare((sqlite3*)hdr, sql.text, sql.len, &stmt, NULL);
//	if (r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		DBG_P("msg='%s', sqlite3_errmsg((sqlite3)hdr));
//		return NULL;
//	}
//		r = sqlite3_reset(stmt);
//	if(r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		return NULL;
//	}
		size_t column_size = (size_t)sqlite3_column_count(stmt);
		//DBG_P("column_size=%d", column_size);
		knh_ResultSet_initColumn(ctx, rs, column_size);
		if(column_size > 0) {
			size_t i;
			for(i = 0; i < DP(rs)->column_size; i++) {
				char *n = (char*)sqlite3_column_name(stmt, i);
				//DBG_P("(%d) name = '%s'", i, n);
				if(n != NULL) {
					ResultSet_setName(ctx, rs, i, new_S(ctx, B(n)));
				}
			}
		}
		return (knh_qcur_t*)stmt;
	}
}

static void SQLITE3_qclose(CTX ctx, knh_qconn_t *hdr)
{
	sqlite3_close((sqlite3*)hdr);
}

static void SQLITE3_qfree(knh_qcur_t *qcur)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)qcur;
	sqlite3_finalize(stmt);
}

static const knh_QueryDSPI_t QUERY_SQLITE3 = {
	K_DSPI_QUERY, "sqlite3",
	SQLITE3_qopen, SQLITE3_query, SQLITE3_qclose, SQLITE3_qnext, SQLITE3_qfree
};

#endif/*K_USING_SQLITE3*/

/* ------------------------------------------------------------------------ */

const knh_QueryDSPI_t *knh_getQueryDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path)
{
	if(path.len == 0) {
		return &QUERY_NOP;
	}
	else {
		const knh_QueryDSPI_t *p = (const knh_QueryDSPI_t *)knh_NameSpace_getDSPINULL(ctx, ns, K_DSPI_QUERY, path);
		if(p == NULL) {
			SYSLOG_UnknownPathType(ctx, path);
			p = &QUERY_NOP;
		}
		return p;
	}
}
/* ------------------------------------------------------------------------- */

static knh_bool_t tolowercase(CTX ctx, knh_conv_t *cv, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	size_t i, s = BA_size(tobuf);
	knh_Bytes_write(ctx, tobuf, t);
	knh_bytes_t tt = {{tobuf->bu.text + s}, BA_size(tobuf) - s};
	for(i = 0; i < tt.len; i++) {
		int ch = tt.utext[i];
		if('A' <= ch && ch <= 'Z') {
			tt.ubuf[i] = ch - 'A' + 'a';
		}
	}
	return 1;
}

static knh_bool_t touppercase(CTX ctx, knh_conv_t *cv, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	size_t i, s = BA_size(tobuf);
	knh_Bytes_write(ctx, tobuf, t);
	knh_bytes_t tt = {{tobuf->bu.text + s}, BA_size(tobuf) - s};
	for(i = 0; i < tt.len; i++) {
		int ch = tt.utext[i];
		if('a' <= ch && ch <= 'z') {
			tt.ubuf[i] = ch - 'a' + 'A';
		}
	}
	return 1;
}

static const knh_ConvDSPI_t TO_lower = {
	K_DSPI_CONVTO, "lower",
	NULL, tolowercase, tolowercase, tolowercase, tolowercase, NULL, NULL,
};

static const knh_ConvDSPI_t TO_upper = {
	K_DSPI_CONVTO, "upper",
	NULL, touppercase, touppercase, touppercase, touppercase, NULL, NULL,
};

void knh_loadScriptDriver(CTX ctx, knh_NameSpace_t *ns)
{
	const knh_PackageLoaderAPI_t *api = knh_getPackageLoaderAPI();
	api->addPathDSPI(ctx, ns, NULL, &PATH_NOPATH);
	api->addPathDSPI(ctx, ns, "charset", &PATH_CHARSET);
	api->addPathDSPI(ctx, ns, "to", &PATH_TOPATH);
	api->addPathDSPI(ctx, ns, "from", &PATH_FROMPATH);
	api->addPathDSPI(ctx, ns, "file", &PATH_FILE);
	api->addPathDSPI(ctx, ns, "lib", &PATH_LIB);
	api->addPathDSPI(ctx, ns, "pkg", &PATH_PKG);
	api->addPathDSPI(ctx, ns, "script", &PATH_SCRIPT);
	api->addPathDSPI(ctx, ns, "start", &PATH_START);
	api->addConvDSPI(ctx, ns, "lower", &TO_lower);
	api->addConvDSPI(ctx, ns, "upper", &TO_upper);
	api->addStreamDSPI(ctx, ns, NULL, &STREAM_NOFILE);
	api->addStreamDSPI(ctx, ns, "file", &STREAM_FILE);
	api->addStreamDSPI(ctx, ns, "pkg", &STREAM_PKG);
	api->addStreamDSPI(ctx, ns, "script", &STREAM_SCRIPT);
	api->addStreamDSPI(ctx, ns, "start",  &STREAM_START);
	api->addQueryDSPI(ctx, ns, NULL, &QUERY_NOP);
#ifdef K_USING_SQLITE3
	api->addQueryDSPI(ctx, ns, "sqlite3", &QUERY_SQLITE3);
#endif
#ifdef K_USING_CURL
	api->addPathDSPI(ctx, ns, "http", &PATH_CURL);
	api->addStreamDSPI(ctx, ns, "http", &STREAM_CURL);
#endif
}

/* ------------------------------------------------------------------------ */
#endif/* K_INCLUDE_BUILTINAPI*/

#ifdef __cplusplus
}
#endif
