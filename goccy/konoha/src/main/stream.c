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

#ifndef K_INCLUDE_BUILTINAPI

#define USE_STEXT 1
#define USE_B     1
//#define USE_bytes_startsWith  1

#define USE_cwb_open      1
#define USE_cwb_size      1
#define USE_cwb_tobytes   1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [InputStream] */


KNHAPI2(knh_InputStream_t*) new_InputStreamDSPI(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi)
{
	knh_InputStream_t* in = new_(InputStream);
	DP(in)->fio  = fio;
	in->dspi = dspi;
	if(DP(in)->ba->bu.len == 0) {
		knh_Bytes_expands(ctx, DP(in)->ba, K_STREAM_BUFSIZ);
	}
	return in;
}

KNHAPI2(knh_InputStream_t*) new_InputStreamNULL(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon)
{
	knh_bytes_t path = S_tobytes(urn);
	const knh_StreamDSPI_t *dspi = knh_getStreamDSPI(ctx, ns, path);
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	if(dspi->realpath(ctx, ns, ph)) {
		knh_io_t fd = dspi->fopen(ctx, ph, mode, mon);
		if(fd != IO_NULL) {
			knh_InputStream_t *in = new_InputStreamDSPI(ctx, fd, dspi);
			KNH_SETv(ctx, DP(in)->urn, urn);
			KNH_SETv(ctx, DP(in)->mon, mon);
			if(DP(in)->ba->bu.len == 0) {
				knh_Bytes_expands(ctx, DP(in)->ba, K_STREAM_BUFSIZ);
			}
			knh_path_close(ctx, ph);
			return in;
		}
	}
	else {
		KNH_TODO("not found");
	}
	knh_path_close(ctx, ph);
	return NULL;
}

knh_InputStream_t* new_BytesInputStream(CTX ctx, knh_Bytes_t *ba)
{
	knh_InputStream_t* in = new_(InputStream);
	DBG_ASSERT(ba != ctx->bufa);
	DP(in)->fio = IO_BUF;
	KNH_SETv(ctx, DP(in)->ba, ba);
	DP(in)->posend = ba->bu.len;
	return in;
}

knh_InputStream_t* new_StringInputStream(CTX ctx, knh_String_t *str)
{
	knh_InputStream_t* in = new_(InputStream);
	DBG_ASSERT(IS_String(str));
	DP(in)->fio = IO_BUF;
	KNH_SETv(ctx, DP(in)->str, str);
	DP(in)->pos   = 0;
	DP(in)->posend   = S_size(str);
	return in;
}

void knh_InputStream_setpos(CTX ctx, knh_InputStream_t *in, size_t s, size_t e)
{
	DBG_ASSERT(e <= BA_size(DP(in)->ba));
	DBG_ASSERT(s <= e);
	DP(in)->pos   = s;
	DP(in)->posend = e;
	DP(in)->fio = IO_BUF;
	in->dspi = knh_getByteStreamDSPI();
}

/* ------------------------------------------------------------------------ */
static int readbuf(CTX ctx, knh_InputStream_t *in, knh_Bytes_t *ba)
{
	if(IS_Bytes(ba)) {
		long ssize = in->dspi->fread(ctx, DP(in)->fio, ba->bu.buf, ba->dim->capacity, DP(in)->mon);
		if(ssize > 0) {
			DP(in)->stat_size += ssize;
			DP(in)->pos = 0;
			DP(in)->posend = ssize;
			return 1;
		}
	}
	knh_InputStream_close(ctx, in);
	return 0;
}

int knh_InputStream_getc(CTX ctx, knh_InputStream_t *in)
{
	knh_Bytes_t *ba = DP(in)->ba;
	if(!(DP(in)->pos < DP(in)->posend)) {
		if(DP(in)->fio == IO_NULL || readbuf(ctx, in, ba) == 0) {
			return EOF;
		}
	}
	int ch = ba->bu.ubuf[DP(in)->pos++];
	if(ch == '\n') {
		in->uline += 1;
	}
	return ch;
}

knh_String_t* knh_InputStream_readLine(CTX ctx, knh_InputStream_t *in)
{
	knh_Bytes_t *ba = DP(in)->ba;
	if(!(DP(in)->pos < DP(in)->posend)) {
		if(DP(in)->fio == IO_NULL || readbuf(ctx, in, ba) == 0) {
			return KNH_TNULL(String);
		}
	}
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	while(1) {
		if(!(DP(in)->pos < DP(in)->posend)) {
			if(DP(in)->fio == IO_NULL || readbuf(ctx, in, ba) == 0) {
				knh_InputStream_close(ctx, in);
				if(knh_cwb_size(cwb) == 0) {
					return KNH_TNULL(String);
				}
				goto L_TOSTRING;
			}
		}
		size_t i, pos = DP(in)->pos, posend = DP(in)->posend;
		knh_uchar_t *p = ba->bu.ubuf;
		for(i = pos; i < posend; i++) {
			int ch = p[i];
			if(ch == '\n') {
				DP(in)->pos = i + 1;
				knh_bytes_t buf = {{ba->bu.text + pos}, i - pos};
				knh_Bytes_write(ctx, cwb->ba, buf);
				goto L_TOSTRING;
			}
		}
	}
	L_TOSTRING:;
	if(in->decNULL == NULL) {
		return knh_cwb_newString(ctx, cwb);
	}
	else {
		return knh_cwb_newStringDECODE(ctx, cwb, in->decNULL);
	}
}

void knh_InputStream_close(CTX ctx, knh_InputStream_t *in)
{
	if(DP(in)->fio != IO_NULL) {
		in->dspi->fclose(ctx, DP(in)->fio);
		if(DP(in)->fio != IO_BUF) {
//			DBG_P("stream in=%s", S_tochar(DP(in)->urn));
			knh_Bytes_dispose(ctx, DP(in)->ba);
		}
		DP(in)->fio = IO_NULL;
		in->dspi = knh_getDefaultStreamDSPI();
	}
}

int InputStream_isClosed(CTX ctx, knh_InputStream_t *in)
{
	return (DP(in)->fio == IO_NULL);
}

void InputStream_setCharset(CTX ctx, knh_InputStream_t *in, knh_StringDecoder_t *c)
{
	if(in->decNULL == NULL) {
		if(c != NULL) {
			KNH_INITv(in->decNULL, c);
		}
	}
	else {
		if(c != NULL) {
			KNH_SETv(ctx, in->decNULL, c);
		}
		else {
			KNH_FINALv(ctx, in->decNULL);
			in->decNULL = c;
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [OutputStream] */

KNHAPI2(knh_OutputStream_t*) new_OutputStreamDSPI(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi)
{
	knh_OutputStream_t* w = new_(OutputStream);
	DP(w)->fio = fio;
	w->dspi = dspi;
	return w;
}

KNHAPI2(knh_OutputStream_t*) new_OutputStreamNULL(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon)
{
	knh_bytes_t path = S_tobytes(urn);
	const knh_StreamDSPI_t *dspi = knh_getStreamDSPI(ctx, ns, path);
	knh_path_t phbuf, *ph = knh_path_open_(ctx, NULL, path, &phbuf);
	if(dspi->realpath(ctx, ns, ph)) {
		knh_io_t fd = dspi->wopen(ctx, ph, mode, mon);
		if(fd != IO_NULL) {
			knh_OutputStream_t *out = new_OutputStreamDSPI(ctx, fd, dspi);
			KNH_SETv(ctx, DP(out)->urn, urn);
			KNH_SETv(ctx, DP(out)->mon, mon);
			knh_path_close(ctx, ph);
			return out;
		}
	}
	else {
		KNH_TODO("not found");
	}
	knh_path_close(ctx, ph);
	return NULL;
}

KNHAPI2(knh_OutputStream_t*) new_BytesOutputStream(CTX ctx, knh_Bytes_t *ba)
{
	knh_OutputStream_t* w = new_(OutputStream);
	DP(w)->fio = IO_BUF;
	w->dspi = knh_getByteStreamDSPI();
	KNH_SETv(ctx, DP(w)->ba, ba);
	return w;
}

/* ------------------------------------------------------------------------ */
/* [methods] */

KNHAPI2(void) knh_OutputStream_flush(CTX ctx, knh_OutputStream_t *w, int isNEWLINE)
{
	knh_Bytes_t *ba = DP(w)->ba;
	if(ba->bu.len > 0) {
		if(w->encNULL != NULL && OutputStream_hasUTF8(w)) {
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_StringEncoder_t *c = w->encNULL;
			KNH_ASSERT(ba != cwb->ba);
			c->dspi->enc(ctx, c->conv, BA_tobytes(ba), cwb->ba);
			ba = cwb->ba;
			if(w->dspi->fwrite(ctx, DP(w)->fio, (ba)->bu.text, (ba)->bu.len, DP(w)->mon) > 0) {
				knh_Bytes_clear(DP(w)->ba, 0);
			}
			knh_cwb_close(cwb);
			OutputStream_setUTF8(w, 0);
		}
		else if(w->dspi->fwrite(ctx, DP(w)->fio, (ba)->bu.text, (ba)->bu.len, DP(w)->mon) > 0) {
			knh_Bytes_clear(ba, 0);
		}
	}
}

KNHAPI2(void) knh_OutputStream_putc(CTX ctx, knh_OutputStream_t *w, int ch)
{
	knh_Bytes_t *ba = DP(w)->ba;
	knh_uchar_t ubuf[8] = {ch};
	knh_bytes_t t = {{(const char*)ubuf}, 1};
	knh_Bytes_write(ctx, ba, t);
	DP(w)->stat_size++;
}

KNHAPI2(void) knh_OutputStream_write(CTX ctx, knh_OutputStream_t *w, knh_bytes_t buf)
{
	knh_Bytes_t *ba = DP(w)->ba;
	knh_Bytes_write(ctx, ba, buf);
	if(ba->bu.len > w->dspi->wbufsiz) {
		knh_OutputStream_flush(ctx, w, 0);
	}
}

void knh_OutputStream_clear(CTX ctx, knh_OutputStream_t *w)
{
	knh_Bytes_clear(DP(w)->ba, 0);
}

void knh_OutputStream_close(CTX ctx, knh_OutputStream_t *w)
{
	if(DP(w)->fio != IO_NULL) {
		knh_OutputStream_flush(ctx, w, 1);
		w->dspi->fclose(ctx, DP(w)->fio);
		if(DP(w)->fio != IO_BUF) {
			DBG_P("stream out=%s", S_tochar(DP(w)->urn));
			knh_Bytes_dispose(ctx, DP(w)->ba);
		}
		DP(w)->fio = IO_NULL;
		w->dspi = knh_getDefaultStreamDSPI();
	}
}

int OutputStream_isClosed(knh_OutputStream_t *w)
{
	return (DP(w)->fio == IO_NULL);
}

/* ------------------------------------------------------------------------ */

void OutputStream_setCharset(CTX ctx, knh_OutputStream_t *w, knh_StringEncoder_t *c)
{
	if(w->encNULL == NULL) {
		KNH_INITv(w->encNULL, c);
	}
	else {
		KNH_SETv(ctx, w->encNULL, c);
	}
}

/* ------------------------------------------------------------------------ */
/* [knh_write] */

KNHAPI2(void) knh_write_BOL(CTX ctx, knh_OutputStream_t *w)
{
	knh_intptr_t i, n = DP(w)->indent;
	if(!OutputStream_isBOL(w)) {
		knh_write_EOL(ctx, w);
	}
	for(i = 0; i < n; i++) {
		knh_Bytes_write(ctx, DP(w)->ba, S_tobytes(DP(w)->TAB));
	}
	OutputStream_setBOL(w, 0);
}

KNHAPI2(void) knh_write_EOL(CTX ctx, knh_OutputStream_t *w)
{
	knh_Bytes_write(ctx, DP(w)->ba, S_tobytes(DP(w)->NEWLINE));
	if(OutputStream_isAutoFlush(w)) {
		knh_OutputStream_flush(ctx, w, 1);
	}
	OutputStream_setBOL(w, 1);
}

KNHAPI2(void) knh_write_TAB(CTX ctx, knh_OutputStream_t *w)
{
	knh_Bytes_write(ctx, DP(w)->ba, S_tobytes(DP(w)->TAB));
}

//void knh_write_begin(CTX ctx, knh_OutputStream_t *w, int ch)
//{
//	if(ch != 0) {
//		knh_putc(ctx, w, ch);
//		knh_write_EOL(ctx, w);
//	}
//	DP(w)->indent++;
//}
//
//void knh_write_end(CTX ctx, knh_OutputStream_t *w, int ch)
//{
//	DP(w)->indent--;
//	if(ch != 0) {
//		knh_write_BOL(ctx, w);
//		knh_putc(ctx, w, ch);
//	}
//}

/* ------------------------------------------------------------------------ */
/* [datatype] */

void knh_write_bool(CTX ctx, knh_OutputStream_t *w, int b)
{
	knh_String_t *s = (b) ? TS_true : TS_false;
	knh_Bytes_write(ctx, DP(w)->ba, S_tobytes(s));
}

void knh_write__p(CTX ctx, knh_OutputStream_t *w, void *ptr)
{
	char buf[K_INT_FMTSIZ];
	knh_snprintf(buf, sizeof(buf), "%p", ptr);
	knh_Bytes_write(ctx, DP(w)->ba, B(buf));
}

void knh_write_dfmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_intptr_t n)
{
	char buf[K_INT_FMTSIZ];
	knh_snprintf(buf, sizeof(buf), fmt, n);
	knh_Bytes_write(ctx, DP(w)->ba, B(buf));
}

void knh_write_ifmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_int_t n)
{
	char buf[K_INT_FMTSIZ];
	knh_snprintf(buf, sizeof(buf), fmt, n);
	knh_Bytes_write(ctx, DP(w)->ba, B(buf));
}

void knh_write_ffmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_float_t n)
{
	char buf[K_FLOAT_FMTSIZ];
	knh_snprintf(buf, sizeof(buf), fmt, n);
	knh_Bytes_write(ctx, DP(w)->ba, B(buf));
}

void knh_write_flag(CTX ctx, knh_OutputStream_t *w, knh_flag_t flag)
{
	knh_uchar_t ubuf[8];
	knh_bytes_t t = {{(const char*)ubuf}, 1};
	knh_intptr_t i;
	knh_flag_t f = 1 << 15;
	for(i = 0; i < 16; i++) {
		if(i > 0 && i % 8 == 0) {
			ubuf[0] = ' ';
			knh_Bytes_write(ctx, DP(w)->ba, t);
		}
		ubuf[0] = ((f & flag) == f) ? '1' : '0';
		knh_Bytes_write(ctx, DP(w)->ba, t);
		f = f >> 1;
	}
}

KNHAPI2(void) knh_write_ascii(CTX ctx, knh_OutputStream_t *w, const char *text)
{
	knh_bytes_t t;
	t.text = text; t.len = knh_strlen(text);
	knh_Bytes_write(ctx, DP(w)->ba, t);
}

KNHAPI2(void) knh_write_utf8(CTX ctx, knh_OutputStream_t *w, knh_bytes_t t, int hasUTF8)
{
	if(hasUTF8 && w->encNULL != NULL && !OutputStream_hasUTF8(w)) {
		size_t i;
		for(i = 0; i < t.len; i++) {
			if(t.ubuf[i] > 127) {
				OutputStream_setUTF8(w, 1);
				break;
			}
		}
	}
	knh_Bytes_write(ctx, DP(w)->ba, t);
}

/* ------------------------------------------------------------------------ */

void knh_write_quote(CTX ctx, knh_OutputStream_t *w, int quote, knh_bytes_t t, int hasUTF8)
{
	knh_bytes_t sub = t;
	size_t i, s = 0;
	knh_putc(ctx, w, quote);
	for(i = 0; i < t.len; i++) {
		int ch = t.utext[i];
		if(ch == '\t' || ch == '\n' || ch == '\r' || ch == '\\' || ch == quote) {
			sub.utext = t.utext + s;
			sub.len = i - s;
			knh_write_utf8(ctx, w, sub, hasUTF8);
			s = i + 1;
			knh_putc(ctx, w, '\\');
			if(ch == '\t') {
				knh_putc(ctx, w, 't');
			}
			else if(ch == '\n') {
				knh_putc(ctx, w, 'n');
			}
			else if(ch == '\r') {
				knh_putc(ctx, w, 'r');
			}
			else {
				knh_putc(ctx, w, ch);
			}
		}
	}
	if (s < t.len) {
		sub.utext = t.utext + s;
		sub.len = t.len - s;
		knh_write_utf8(ctx, w, sub, hasUTF8);
	}
	knh_putc(ctx, w, quote);
}

void knh_write_cap(CTX ctx, knh_OutputStream_t *w, knh_bytes_t t, int hasUTF8)
{
	if(islower(t.utext[0])) {
		knh_putc(ctx, w, toupper(t.utext[0]));
		t.utext = t.utext+1; t.len = t.len -1;
	}
	knh_write_utf8(ctx, w, t, hasUTF8);
}

void knh_write_Object(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	if(level % 2 == 0) { // TYPED
		knh_write_ascii(ctx, w, CLASS__(O_cid(o)));
		knh_putc(ctx, w, ':');
	}
	if(Object_isNullObject(o)) {
		knh_write(ctx, w, STEXT("null"));
	}
	else {
		O_cTBL(o)->ospi->p(ctx, w, o, level);
	}
	if(IS_FMTdump(level)) {
		knh_write_EOL(ctx, w);
	}
}

void knh_write_InObject(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	if(level % 2 == 0) { // TYPED
		knh_write_ascii(ctx, w, CLASS__(O_cid(o)));
		knh_putc(ctx, w, ':');
	}
	if(Object_isNullObject(o)) {
		knh_write_ascii(ctx, w, "null");
	}
	else {
//		if(checkRecursiveCalls(ctx, sfp)) {
//			knh_write_dots(ctx, w);
//			goto L_CLOSE;
//		}
		O_cTBL(o)->ospi->p(ctx, w, o, level);
	}
}

/* ------------------------------------------------------------------------ */
/* [printf] */

static const char* knh_vprintf_parseindex(const char *p, int *index)
{
	const char *ptr = p+1;
	if(ptr[0] == '{' && isdigit(ptr[1]) && ptr[2] == '}') {
		*index = ptr[1] - '0';
		ptr += 3;
		return ptr;
	}
	return p;
}

#define VA_NOP                0
#define VA_DIGIT              1 /* intptr_t */
#define VA_LONG               2 /* knh_int_t */
#define VA_FLOAT              3
#define VA_CHAR               4
#define VA_POINTER            5
#define VA_OBJECT             6
#define VA_FIELDN             7
#define VA_CLASS              8
#define VA_TYPE               9
#define VA_METHODN            10
#define VA_BYTES              11

typedef struct {
	int atype;
	union {
		knh_intptr_t  dvalue;
		knh_uintptr_t uvalue;
		knh_int_t     ivalue;
		knh_float_t   fvalue;
		knh_float_t   evalue;
		void         *pvalue;
		char         *svalue;
		Object       *ovalue;
		knh_bytes_t   bvalue;
		knh_class_t     cid;
		knh_type_t      type;
		knh_fieldn_t    fn;
		knh_methodn_t   mn;
	};
} knh_valist_t;

/* ------------------------------------------------------------------------ */

void knh_vprintf(CTX ctx, knh_OutputStream_t *w, const char *fmt, va_list ap)
{
	knh_valist_t args[10];
	const char *c = fmt;
	int i, ch, bindex = 0, bindex_max = 10;
	for(i = 0; i < bindex_max; i++) args[i].atype = 0;
	while((ch = *c) != '\0') {
		c++;
		if(ch == '%') {
			int index;
			ch = *c;
			if(ch == '%') {
				c++;
				continue;
			}
			index = bindex++;
			c = knh_vprintf_parseindex(c++, &index);
			//DBG_P("bindex=%d, index=%d", bindex, index);
			switch(ch) {
				case 'd': case 'u':
				args[index].atype = VA_DIGIT; break;
				case 'l': case 'i':
					args[index].atype = VA_LONG; break;
				case 'f': case 'e':
					args[index].atype = VA_FLOAT; break;
				case 's':
					args[index].atype = VA_CHAR; break;
				case 'p':
					args[index].atype = VA_POINTER; break;
				case 'L':
				case 'K': case 'k':
				case 'O': case 'o':
					args[index].atype = VA_OBJECT; break;
				case 'N': case 'F':
					args[index].atype = VA_FIELDN; break;
				case 'M':
					args[index].atype = VA_METHODN; break;
				case 'C':
					args[index].atype = VA_CLASS; break;
				case 'T':
					args[index].atype = VA_TYPE; break;
				case 'B':
					args[index].atype = VA_BYTES; break;
				// TODO
				// we should care if "fmt" has "%%".
				// sometimes, next args is NULL.
				case '%':
					index--;
					c++;
				default:
					bindex--;
			}
			if(bindex == 10) {
				DBG_ASSERT(bindex < 10);
				break;
			}
		}
	}

	for(i = 0; i < 10; i++) {
		switch(args[i].atype) {
		case VA_DIGIT:
			args[i].dvalue = (knh_intptr_t)va_arg(ap, knh_intptr_t); break;
		case VA_LONG:
			args[i].ivalue = (knh_int_t)va_arg(ap, knh_int_t); break;
		case VA_FLOAT:
#if defined(K_USING_NOFLOAT)
			args[i].fvalue = (knh_float_t)va_arg(ap, knh_float_t);
#else
			args[i].fvalue = (knh_float_t)va_arg(ap, double);
#endif
			break;
		case VA_CHAR:
			args[i].svalue = (char*)va_arg(ap, char*); break;
		case VA_POINTER:
			args[i].pvalue = (void*)va_arg(ap, void*); break;
		case VA_OBJECT:
			args[i].ovalue = (Object*)va_arg(ap, Object*); break;
		case VA_FIELDN:
			args[i].fn = (knh_fieldn_t)va_arg(ap, int/*knh_fieldn_t*/); break;
		case VA_METHODN:
			args[i].mn = (knh_methodn_t)va_arg(ap, int/*knh_methodn_t*/); break;
		case VA_CLASS:
			args[i].cid = (knh_class_t)va_arg(ap, int/*knh_class_t*/); break;
		case VA_TYPE:
			args[i].type = (knh_type_t)va_arg(ap, int/*knh_type_t*/); break;
		case VA_BYTES:
			args[i].bvalue = (knh_bytes_t)va_arg(ap, knh_bytes_t); break;
		default:
			bindex_max = i;
			goto L_FORMAT;
		}
	}

	L_FORMAT: {
		knh_bytes_t b;
		c = fmt;
		bindex = 0;
		b.text = c;
		b.len = 0;
		while((ch = *c) != '\0') {
			c++;
			if(ch == '\\') {
				if(b.len > 0) {
					knh_write_utf8(ctx, w, b, 1);
				}
				ch = *c;
				switch(ch) {
					case '\0' : return ;
					case 'n': knh_write_EOL(ctx, w); break;
					case 't': knh_write_TAB(ctx, w); break;
					default:
						knh_putc(ctx, w, '\\');
						knh_putc(ctx, w, ch);
				}
				b.text = c;
				b.len = 0;
			}
			else if(ch == '%') {
				if(b.len > 0) {
				  knh_write_utf8(ctx, w, b, 1);
				}
				ch = *c;
				if(ch == '%') {
					knh_putc(ctx, w, '%');
					c++;
					b.text = c;
					b.len = 0;
					continue;
				}
				int index = bindex++;
				c = knh_vprintf_parseindex(++c, &index);

				switch(ch) {
					case '\0' : return ;
					case 'd':
						DBG_ASSERT(args[index].atype == VA_DIGIT);
						knh_write_dfmt(ctx, w, K_INTPTR_FMT, args[index].dvalue);
						break;
					case 'u':
						DBG_ASSERT(args[index].atype == VA_DIGIT);
						knh_write_dfmt(ctx, w, K_INTPTR_UFMT, args[index].uvalue);
						break;
					case 'l': case 'i' :
						DBG_ASSERT(args[index].atype == VA_LONG);
						knh_write_ifmt(ctx, w, K_INT_FMT, args[index].ivalue);
						break;
					case 'f':
						DBG_ASSERT(args[index].atype == VA_FLOAT);
						knh_write_ffmt(ctx, w, K_FLOAT_FMT, args[index].fvalue);
						break;
					case 'e':
						DBG_ASSERT(args[index].atype == VA_FLOAT);
						knh_write_ffmt(ctx, w, K_FLOAT_FMTE, args[index].fvalue);
						break;
					case 's':
						DBG_ASSERT(args[index].atype == VA_CHAR);
						knh_write(ctx, w, B(args[index].svalue));
						break;
					case 'p':
						DBG_ASSERT(args[index].atype == VA_POINTER);
						knh_write__p(ctx, w, args[index].pvalue);
						break;
					case 'L':
					case 'O': case 'o':
						DBG_ASSERT(args[index].atype == VA_OBJECT);
						knh_write_Object(ctx, w, args[index].ovalue, FMT_s);
						break;
					case 'K': case 'k':
						DBG_ASSERT(args[index].atype == VA_OBJECT);
						knh_write_Object(ctx, w, args[index].ovalue, FMT_line);
						break;
					case 'N': case 'F':
						DBG_ASSERT(args[index].atype == VA_FIELDN);
						knh_write_ascii(ctx, w, FN__(args[index].fn));
						break;
					case 'M':
						DBG_ASSERT(args[index].atype == VA_METHODN);
						knh_write_mn(ctx, w, args[index].mn);
						break;
					case 'C':
						DBG_ASSERT(args[index].atype == VA_CLASS);
						knh_write_type(ctx, w, args[index].cid);
						break;
					case 'T':
						DBG_ASSERT(args[index].atype == VA_TYPE);
						knh_write_type(ctx, w, args[index].type);
						break;
					case 'B':
						DBG_ASSERT(args[index].atype == VA_BYTES);
						knh_write(ctx,w, args[index].bvalue);
						break;
					case '%':
						index--;
						bindex--;
					default:
						//knh_putc(ctx, w, '%');
						knh_putc(ctx, w, ch);
				}
				b.text = c;
				b.len = 0;
				if(!(bindex <= bindex_max)) {
					DBG_ASSERT(bindex <= bindex_max);
					break;
				}
			}
			else {
				b.len = b.len+1;
			}
		}
		if(b.len > 0) {
		  knh_write_utf8(ctx, w, b, 1);
		}
	}
}

KNHAPI2(void) knh_printf(CTX ctx, knh_OutputStream_t *w, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	knh_vprintf(ctx, w, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#else /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */
//## method InputStream InputStream.new(String urn, String mode, NameSpace ns, Monitor mon);

static METHOD InputStream_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_InputStream_t *in = sfp[0].in;
	knh_bytes_t path = S_tobytes(sfp[1].s);
	const char *mode = IS_NULL(sfp[2].s) ? "r" : S_tochar(sfp[2].s);
	knh_NameSpace_t *ns = sfp[3].ns;
	knh_path_t phbuf, *ph = knh_path_open_(ctx, "script", path, &phbuf);
	DBG_ASSERT(IS_NameSpace(ns));
	path.text = P_text(ph); path.len = ph->plen;
	in->dspi = knh_getStreamDSPI(ctx, ns, path);
	if(in->dspi->realpath(ctx, ns, ph)) {
		KNH_SETv(ctx, DP(in)->mon, sfp[4].mon);
		DP(in)->fio = in->dspi->fopen(ctx, ph, mode, DP(in)->mon);
		if(DP(in)->fio != IO_NULL) {
			KNH_SETv(ctx, DP(in)->urn, sfp[1].s);
			knh_Bytes_ensureSize(ctx, DP(in)->ba, K_PAGESIZE);
			goto L_RETURN;
		}
	}
	knh_Object_toNULL(ctx, in);
	in->dspi = knh_getDefaultStreamDSPI();
	L_RETURN:;
	knh_path_close(ctx, ph);
	RETURN_(in);
}

/* ------------------------------------------------------------------------ */
//## method Int InputStream.getChar();

static METHOD InputStream_getChar(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(knh_InputStream_getc(ctx, sfp[0].in));
}

///* ------------------------------------------------------------------------ */
////## method Int InputStream.read(Bytes buf, Int offset, Int length);
//
//static METHOD InputStream_read(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_Bytes_t *ba = sfp[1].ba;
//	knh_bytes_t buf = BA_tobytes(ba);
//	size_t offset = Int_to(size_t,sfp[2]);
//	size_t len    = Int_to(size_t,sfp[3]);
//	if(unlikely(offset > buf.len)) {
//		THROW_OutOfRange(ctx, sfp, offset, buf.len);
//	}
//	buf = knh_bytes_last(buf, offset);
//	if(len != 0) {
//		knh_Bytes_ensureSize(ctx, ba, offset + len);  // DONT USE ensureSize
//		buf.len = len;
//		buf.ubuf = ba->bu.ubuf;
//	}
//	RETURNi_(knh_InputStream_read(ctx, sfp[0].in, (char*)buf.ubuf, buf.len));
//}

///* ------------------------------------------------------------------------ */
//
//size_t knh_InputStream_read(CTX ctx, knh_InputStream_t *in, char *buf, size_t bufsiz)
//{
//	if(InputStream_isFILE(in)) {
//		return in->dspi->fread(ctx, DP(in)->fd, buf, bufsiz);
//	}
//	else {
//		size_t inbufsiz = (DP(in)->bufend - DP(in)->bufpos);
//		if(bufsiz <= inbufsiz) {
//			knh_memcpy(buf, DP(in)->buf, bufsiz);
//			DP(in)->bufpos += bufsiz;
//			{
//				DP(in)->size += bufsiz;
//			}
//			return bufsiz;
//		}
//		// XXX when both InputStream.read and InputStream.readLine method call,
//		// it seams strange. so, move DP(o)->buf's pointer to bufpos.
//		knh_memcpy(buf, DP(in)->buf + DP(in)->bufpos, inbufsiz);
//		DP(in)->bufpos += inbufsiz;
//		DP(in)->size += bufsiz;
//		buf += inbufsiz;
//		size_t s = in->dspi->fread(ctx, DP(in)->fd, buf+inbufsiz, bufsiz-inbufsiz);
//		DP(in)->size += s;
//		return s + inbufsiz;
//	}
//}

/* ------------------------------------------------------------------------ */
//## method Boolean InputStream.isClosed();

static METHOD InputStream_isClosed_(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(InputStream_isClosed(ctx, sfp[0].in));
}

/* ------------------------------------------------------------------------ */
//## method void InputStream.close();

static METHOD InputStream_close(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_InputStream_close(ctx, sfp[0].in);
}

/* ------------------------------------------------------------------------ */
//## method StringDecoder InputStream.setCharset(StringDecoder c);

static METHOD InputStream_setCharset_(CTX ctx, knh_sfp_t *sfp _RIX)
{
	InputStream_setCharset(ctx, sfp[0].in, (knh_StringDecoder_t*)sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## method String InputStream.readLine();

static METHOD InputStream_readLine(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(knh_InputStream_readLine(ctx, sfp[0].in));
}

///* ------------------------------------------------------------------------ */
////## method dynamic InputStream.readData();
//
//static METHOD InputStream_readData(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	Object *v = knh_InputStream_readData(ctx, sfp[0].in);
//	RETURN_(v);
//}


/* ------------------------------------------------------------------------ */
/* [iterators] */

static ITRNEXT knh_InputStream_nextLine(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	int ch;
	knh_Iterator_t *it = sfp[0].it;
	knh_InputStream_t *in = (knh_InputStream_t*)DP(it)->source;
	knh_cwb_t cwbbuf;
	knh_cwb_t *cwb = knh_cwb_open(ctx, &cwbbuf);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == 13) continue;
		if(ch == 10) {
			//ITRNEXT_(new_String__cwbconv(ctx, cwb, DP(in)->bconv));
			ITRNEXT_(knh_cwb_newString(ctx, cwb));
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	if(knh_cwb_size(cwb) != 0) {
		ITRNEXT_(knh_cwb_newString(ctx, cwb));
	}
	ITREND_();
}

/* ------------------------------------------------------------------------ */
//## @Final mapper InputStream String..;

static TYPEMAP knh_InputStream_String__(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_Iterator(ctx, CLASS_String, sfp[0].o, knh_InputStream_nextLine));
}

/* ------------------------------------------------------------------------ */
/* [OutputStream] */

//## method OutputStream OutputStream.new(String urn, String mode, NameSpace ns, Monitor mon);

static METHOD OutputStream_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_bytes_t path = S_tobytes(sfp[1].s);
	const char *mode = IS_NULL(sfp[2].s) ? "w" : S_tochar(sfp[2].s);
	knh_NameSpace_t *ns = sfp[3].ns;
	knh_path_t phbuf, *ph = knh_path_open_(ctx, "script", path, &phbuf);
	path.text = P_text(ph); path.len = ph->plen;
	w->dspi = knh_getStreamDSPI(ctx, ns, path);
	if(w->dspi->realpath(ctx, ns, ph)) {
		KNH_SETv(ctx, DP(w)->mon, sfp[4].mon);
		DP(w)->fio = w->dspi->fopen(ctx, ph, mode, DP(w)->mon);
		if(DP(w)->fio != IO_NULL) {
			KNH_SETv(ctx, DP(w)->urn, sfp[1].s);
			goto L_RETURN;
		}
	}
	knh_Object_toNULL(ctx, w);
	w->dspi = knh_getDefaultStreamDSPI();
	L_RETURN:;
	knh_path_close(ctx, ph);
	RETURN_(w);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.writeASCII(Int ch);

static METHOD OutputStream_writeASCII(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_putc(ctx, w, (int)(sfp[1].ivalue));
	if(DP(w)->ba->bu.len > w->dspi->wbufsiz) {
		knh_OutputStream_flush(ctx, w, 0);
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.write(Bytes buf, Int offset, Int length);

static METHOD OutputStream_write(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t t = BA_tobytes(sfp[1].ba);
	size_t offset = (sfp[2].ivalue == 0) ? 0 : knh_array_index(ctx, sfp, Int_to(size_t, sfp[2]), t.len);
	size_t len = (sfp[3].ivalue == 0) ? (t.len - offset) : Int_to(size_t, sfp[3]);
	if(offset + len > t.len) len = t.len - offset;
	t.utext = &(t.utext[offset]);
	t.len = len;
	knh_OutputStream_write(ctx, sfp[0].w, t);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method Boolean OutputStream.isClosed();

static METHOD OutputStream_isClosed_(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(OutputStream_isClosed(sfp[0].w));
}

///* ------------------------------------------------------------------------ */
//static void _OutputStream_writeLine(CTX ctx, knh_OutputStream_t *w, knh_String_t *s)
//{
//	if(s == TS_EOL) {
//		knh_write_EOL(ctx, w);
//	}
//	else if(s == TS_BEGIN) {
//		DP(w)->indent++;
//	}
//	else if(s == TS_END) {
//		DP(w)->indent--;
//	}
//	else {
//		knh_OutputStream_writeLine(ctx, w, S_tobytes(s), 0);
//	}
//}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.print(Object value, ...);

static METHOD OutputStream_print(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_write_Object(ctx, w, v[i].o, FMT_s);
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.println(dynamic value, ...);

static METHOD OutputStream_println(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_write_Object(ctx, w, v[i].o, FMT_s);
	}
	knh_write_EOL(ctx, w);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.send(String s, ...);

static METHOD OutputStream_send(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_String_t *s = v[i].s;
		if(s == TS_EOL) {
			knh_write_EOL(ctx, w);
		}
		else {
			knh_write_utf8(ctx, w, S_tobytes(s), !String_isASCII(s));
		}
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.writeData(Object data, ...);

static METHOD OutputStream_writeData(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_write_Object(ctx, w, v[i].o, FMT_data);
		knh_write_EOL(ctx, w);
	}
	RETURNvoid_();
}

//======================================  MSGPACK  =======================================//

#ifdef K_USING_MSGPACK
#include <msgpack.h>
#endif

static void *knh_msgpack_init(CTX ctx, knh_packer_t *pk)
{
#ifdef K_USING_MSGPACK
	pk->sbuffer = msgpack_sbuffer_new();
	pk->pk = msgpack_packer_new(pk->sbuffer, msgpack_sbuffer_write);
#endif
	return pk;
}

static void knh_msgpack_flushfree(CTX ctx, knh_packer_t *pk)
{
#ifdef K_USING_MSGPACK
	msgpack_sbuffer *sbuffer = pk->sbuffer;
	pk->w->dspi->fwrite(ctx, DP(pk->w)->fio, sbuffer->data, sbuffer->size, NULL);
	fprintf(stderr, "send packing data!!\n");
#endif
}

static void knh_msgpack_null(CTX ctx, void *pk)
{

}

static void knh_msgpack_bool(CTX ctx, void *_pk, int b)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	(b) ? msgpack_pack_true(pk->pk) : msgpack_pack_false(pk->pk);
	fprintf(stderr, " %d", b);
#endif
}

static void knh_msgpack_int(CTX ctx, void *_pk, knh_int_t i)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_int64(pk->pk, i);
	fprintf(stderr, " %d", i);
#endif
}

static void knh_msgpack_float(CTX ctx, void *_pk, knh_float_t f)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_double(pk->pk, f);
	fprintf(stderr, " %f", f);
#endif
}

static void knh_msgpack_string(CTX ctx, void *_pk, const char *str, size_t len)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_raw(pk->pk, len + 1);
	msgpack_pack_raw_body(pk->pk, str, len + 1);
	fprintf(stderr, " %s", str);
#endif
}

static void knh_msgpack_raw(CTX ctx, void *_pk, const char *str, size_t len)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_raw(pk->pk, len + 1);
	msgpack_pack_raw_body(pk->pk, str, len + 1);
	fprintf(stderr, " %s", str);
#endif
}

static void knh_msgpack_putc(CTX ctx, void *pk, int ch)
{

}

static void knh_msgpack_array(CTX ctx, void *_pk, size_t array_size)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_array(pk->pk, array_size);
	fprintf(stderr, "[ ");//, array_size);
#endif
}

static void knh_msgpack_map(CTX ctx, void *_pk, size_t map_size)
{
#ifdef K_USING_MSGPACK
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_map(pk->pk, map_size);
	fprintf(stderr, "{ ");
#endif
}

#define MAX_MSG_BUFFER_SIZE 1024
static void knh_msgapck_setUnpacker(msgpack_unpacker *upk, msgpack_unpacked *result, char *buf)
{
	msgpack_unpacker_init(upk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);
	msgpack_unpacker_reserve_buffer(upk, MAX_MSG_BUFFER_SIZE);
	memcpy(msgpack_unpacker_buffer(upk), buf, MAX_MSG_BUFFER_SIZE);
	msgpack_unpacker_buffer_consumed(upk, MAX_MSG_BUFFER_SIZE);
	msgpack_unpacked_init(result);
}

static Object *knh_msgpack_getObject(CTX ctx, knh_ClassTBL_t *c, msgpack_unpacked *result, bool isRecursive);
static knh_Array_t *knh_msgpack_getArray(CTX ctx, knh_ClassTBL_t *c, msgpack_object_kv *map)
{
	knh_Array_t *o;
	//fprintf(stderr, "[");
	msgpack_object _value = map->val;
	knh_class_t cid = _value.via.u64;
	map++;
	msgpack_object _key = map->key;
	const char *key = _key.via.raw.ptr; //key is "abody"
	//fprintf(stderr, "key = [%s]\n", key);
	_value = map->val;
	msgpack_object *a = _value.via.array.ptr;
	size_t asize = _value.via.array.size;
	const msgpack_object *array_end = _value.via.array.ptr + asize;
	switch (cid) {
	case CLASS_Int: case CLASS_Float: case CLASS_Boolean:
	case CLASS_String:
		o = new_Array(ctx, cid, asize);
		break;
	default:
		//Object
		o = new_Array(ctx, c->p1, asize);
		c = ClassTBL(c->p1);
		break;
	}
	int i = 0;
	BEGIN_LOCAL(ctx, lsfp, 1);
	for (; a < array_end; a++) {
		Object *elem = knh_msgpack_getObject(ctx, c, (msgpack_unpacked *)a, 1);
		knh_class_t cid = O_cTBL(elem)->cid;
		switch (cid) {
		case CLASS_Int:
			lsfp[0].ivalue = ((knh_Int_t *)elem)->n.ivalue;
			o->api->add(ctx, o, lsfp);
			break;
		case CLASS_Float:
			lsfp[0].fvalue = ((knh_Float_t *)elem)->n.fvalue;
			o->api->add(ctx, o, lsfp);
			break;
		case CLASS_Boolean:
			lsfp[0].bvalue = ((knh_Boolean_t *)elem)->n.bvalue;
			o->api->add(ctx, o, lsfp);
			break;
		default:
			knh_Array_add(ctx, o, elem);
			break;
		}
		i++;
	}
	END_LOCAL_(ctx, lsfp);
	//fprintf(stderr, "]\n");
	return o;
}

typedef struct _knh_FieldInfo_t {
	int isFind;
	int flocate;
} knh_FieldInfo_t;

static void knh_msgpack_setFieldInfo(CTX ctx, knh_FieldInfo_t *info, knh_ClassTBL_t *c, const char *key, Object *value)
{
	const knh_ClassTBL_t *tbl = c;
	int i = 0;
	for (i = 0; i < tbl->fsize; i++) {
		knh_fields_t *field = tbl->fields + i;
		knh_String_t *fname = knh_getFieldName(ctx, field->fn);
		knh_type_t type = field->type;
		switch (type) {
		case CLASS_Boolean: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Boolean) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT()) i++;
			break;
		}
		case CLASS_Int: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Int) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT()) i++;
			break;
		}
		case CLASS_Float: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Float) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT()) i++;
			break;
		}
		case CLASS_String: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_String) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			break;
		}
		default:
			//Array or Object
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->bcid == CLASS_Array) {
				info->isFind = 1;
				info->flocate = i;
			} else if (!strncmp(key, fname->str.text, fname->str.len) &&
					   O_cTBL(value)->p1 == c->cid) {
				info->isFind = 1;
				info->flocate = i;
			}
			break;
		}
	}
L_END:;
}

static Object *knh_msgpack_getClassObject(CTX ctx, knh_ClassTBL_t *c, msgpack_object_kv *map, const msgpack_object_kv *map_end)
{
	Object *o;
	msgpack_object _value = map->val;
	const char *class_name = _value.via.raw.ptr;
	if (!strncmp(class_name, (char *)c->sname->str.text, sizeof(class_name))) {
		o = (Object *)new_Object_init2(ctx, c);
		knh_FieldInfo_t info = {0, 0};
		map++;
		for (; map < map_end; map++) {
			const char *var_name = map->key.via.raw.ptr;
			msgpack_object val = map->val;
			Object *target = knh_msgpack_getObject(ctx, c, (msgpack_unpacked *)&val, 1);
			if (target == NULL) {
				KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot create object from : \"%s\"", var_name);
			}
			knh_msgpack_setFieldInfo(ctx, &info, c, var_name, target);
			if (!info.isFind) {
				KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot find field variable name : \"%s\"", var_name);
				break;
			}
			Object **v = (Object **)o->ref;
			switch (O_cTBL(target)->cid) {
			case CLASS_Int: {
				intptr_t ivalue = (intptr_t)((knh_Int_t *)target)->n.ivalue;
				v[info.flocate] = (Object *)ivalue;
				break;
			}
			case CLASS_Float: {
				knh_float_t fvalue = ((knh_Float_t *)target)->n.fvalue;
				v[info.flocate] = (Object *)&fvalue;
				break;
			}
			case CLASS_Boolean: {
				knh_int_t bvalue = ((knh_Boolean_t *)target)->n.bvalue;
				v[info.flocate] = (Object *)&bvalue;
				break;
			}
			default:
				//Object
				KNH_SETv(ctx, v[info.flocate], target);
				break;
			}
		}
	} else {
		KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot find class name : \"%s\"", class_name);
	}
	return o;
}

static Object *knh_msgpack_getObject(CTX ctx, knh_ClassTBL_t *c, msgpack_unpacked *result, bool isRecursive)
{
	msgpack_object obj = (isRecursive) ? *(msgpack_object *)result : result->data;
	Object *o;
	switch (obj.type) {
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		o = (Object *)new_Int(ctx, CLASS_Int, obj.via.u64);
		break;
	case MSGPACK_OBJECT_DOUBLE:
		o = (Object *)new_Float(ctx, CLASS_Float, obj.via.dec);
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		if (obj.via.boolean) {
			o = (Object *)new_Boolean(ctx, KNH_TRUE);
			((knh_Boolean_t *)o)->n.bvalue = 1;
		} else {
			o = (Object *)new_Boolean(ctx, KNH_FALSE);
			((knh_Boolean_t *)o)->n.bvalue = 0;
		}
		break;
	case MSGPACK_OBJECT_RAW:
		o = (Object *)new_String(ctx, obj.via.raw.ptr);
		break;
	case MSGPACK_OBJECT_MAP:
		if (obj.via.map.size == 0) break;
		msgpack_object_kv *map = obj.via.map.ptr;
		msgpack_object_kv *map_end = obj.via.map.ptr + obj.via.map.size;
		msgpack_object _key = map->key;
		const char *key = _key.via.raw.ptr;
		if (!strncmp(key, "ks:atype", sizeof("ks:atype"))) {
			//Array
			o = (Object *)knh_msgpack_getArray(ctx, c, map);
		} else if (!strncmp(key, "ks:class", sizeof("ks:class"))) {
			//Object
			o = (Object *)knh_msgpack_getClassObject(ctx, c, map, map_end);
		}
		break;
	default:
		KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "undefined msgpack object");
		break;
	}
	return o;
}

static knh_type_t knh_msgpack_unpack(CTX ctx, knh_ClassTBL_t *c, struct knh_InputStream_t *in, knh_sfp_t *sfp)
{
	char buf[MAX_MSG_BUFFER_SIZE] = {0};
	msgpack_unpacker upk;
	msgpack_unpacked result;
	in->dspi->fread(ctx, DP(in)->fio, buf, MAX_MSG_BUFFER_SIZE, NULL);
	knh_msgapck_setUnpacker(&upk, &result, buf);
	msgpack_unpacker_next(&upk, &result);
	Object *o = knh_msgpack_getObject(ctx, c, &result, 0);
	KNH_SETv(ctx, sfp[0].o, o);
	return O_cTBL(o)->cid;
}

static knh_PackSPI_t pack = {
	"msgpack",
	knh_msgpack_init,
	knh_msgpack_flushfree,
	knh_msgpack_null,
	knh_msgpack_bool,
	knh_msgpack_int,
	knh_msgpack_float,
	knh_msgpack_string,
	knh_msgpack_raw,
	knh_msgpack_putc,
	knh_msgpack_array,
	NULL,
	knh_msgpack_map,
	NULL,
	knh_msgpack_unpack,
};

static knh_PackSPI_t *knh_getPackSPI()
{
	return &pack;
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.writeObject(Object data, NameSpace ns);

static METHOD OutputStream_writeObject(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	Object *o = sfp[1].o;
	//knh_NameSpace_t *ns = sfp[2].ns; //needs getPackSPI
	knh_PackSPI_t *packspi = knh_getPackSPI();
	knh_packer_t packer = {w, NULL, NULL};
	knh_packer_t *pkr = packspi->pack_init(ctx, &packer);
	if (O_cTBL(o)->ospi->wdata != NULL) {
		O_cTBL(o)->ospi->wdata(ctx, pkr, o, packspi);
	} else if (O_cTBL(o)->bcid == CLASS_Array) {
		O_cTBL(o)->ospi->wdata(ctx, pkr, o, packspi);
	}
	packspi->pack_flushfree(ctx, pkr);
	fprintf(stderr, "\n");
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method dyn InputStream.readObject(Class c, NameSpace ns);

static METHOD InputStream_readObject(CTX ctx, knh_sfp_t *sfp _RIX)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	//BEGIN_LOCAL(ctx, lsfp, 2);
	knh_InputStream_t *in = sfp[0].in;
	knh_Class_t *class = sfp[1].c;
	//knh_NameSpace_t *ns = sfp[2].ns; //needs getPackSPI
	knh_PackSPI_t *packspi = knh_getPackSPI();
	knh_type_t type = packspi->unpack(ctx, (knh_ClassTBL_t *)class->cTBL, in, lsfp);
	Object *o;
	if (type != TYPE_void) {
		KNH_SETv(ctx, o, lsfp[0].o);
	} else {
		KNH_SETv(ctx, o, KNH_NULL);
	}
	END_LOCAL(ctx, lsfp, o);
	RETURN_(o);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.flush();

static METHOD OutputStream_flush(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_flush(ctx, sfp[0].w, 0);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.reset();

static METHOD OutputStream_reset(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_clear(DP(sfp[0].w)->ba, 0);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.close();

static METHOD OutputStream_close(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_close(ctx, sfp[0].w);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method StringEncoder OutputStream.setCharset(StringEncoder c);

static METHOD OutputStream_setCharaset(CTX ctx, knh_sfp_t *sfp _RIX)
{
	OutputStream_setCharset(ctx, sfp[0].w, (knh_StringEncoder_t*)sfp[1].s);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
/* [format] */

static const char *newfmt(char *buf, size_t bufsiz, knh_bytes_t fmt, const char *t)
{
	char *p = buf + (fmt.len - 1);
	strncpy(buf, fmt.text, bufsiz);
	strncpy(p, t, bufsiz - fmt.len);
	DBG_P("FMT='%s'", buf);
	return (const char*)buf;
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const @FastCall method String Int.format(String fmt);

static METHOD Int_format(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	L_RETRY:;
	int ch = fmt.utext[fmt.len - 1];
	if(fmt.utext[0] == '%' && (ch == 'u' || ch == 'd' || ch == 'x')) {
		char fmtbuf[40], buf[80];
		const char *ifmt = (ch == 'd') ? K_INT_FMT : ((ch == 'x') ? K_INT_XFMT : K_UINT_FMT);
		knh_snprintf(buf, sizeof(buf), newfmt(fmtbuf, sizeof(fmtbuf), fmt, ifmt + 1), sfp[0].ivalue);
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_WARN(ctx, "invalid format: %s", fmt.text);
	}
	fmt = STEXT("%d");
	goto L_RETRY;
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const @FastCall method String Float.format(String fmt);

static METHOD Float_format(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	L_RETRY:;
	int ch = fmt.utext[fmt.len - 1];
	if(fmt.utext[0] == '%' && (ch == 'f' || ch == 'e')) {
		char fmtbuf[40], buf[80];
		const char *ifmt = (ch == 'f') ? K_FLOAT_FMT : K_FLOAT_FMTE;
		knh_snprintf(buf, sizeof(buf), newfmt(fmtbuf, sizeof(fmtbuf), fmt, ifmt + strlen(ifmt)-1), sfp[0].fvalue);
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_WARN(ctx, "invalid format: %s", fmt.text);
	}
	fmt = STEXT("%f");
	goto L_RETRY;
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const @FastCall method String String.format(String fmt);

static METHOD String_format(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	if(fmt.utext[0] == '%' && fmt.utext[fmt.len-1] == 's') {
		char buf[256];
		knh_snprintf(buf, sizeof(buf), fmt.text, S_tochar(sfp[0].s));
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_WARN(ctx, "invalid format: %s", fmt.text);
	}
	RETURN_(sfp[0].s);
}

/* ------------------------------------------------------------------------ */
/* %empty */

//## method void Object.%empty();
static METHOD Object__empty(CTX ctx, knh_sfp_t *sfp _RIX)
{
	if(CTX_isDebug(ctx)) {
		KNH_INFO(ctx, "*%%empty(%s)", O__(sfp[1].o));
	}
}

/* ------------------------------------------------------------------------ */
/* %s */

//## method void Object.%s();
static METHOD Object__s(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_Object(ctx, sfp[0].w, sfp[1].o, FMT_s);
}

/* ------------------------------------------------------------------------ */
/* %s */

//## method void Object.%k();
static METHOD Object__k(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_Object(ctx, sfp[0].w, sfp[1].o, FMT_line);
}

/* ------------------------------------------------------------------------ */
/* %s */

//## method void Object.%data();
static METHOD Object__data(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_Object(ctx, sfp[0].w, sfp[1].o, FMT_data);
}

/* ------------------------------------------------------------------------ */
/* %s */

//## method void Object.%dump();
static METHOD Object__dump(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_Object(ctx, sfp[0].w, sfp[1].o, FMT_dump);
}

/* ------------------------------------------------------------------------ */
//## method void Boolean.%s();
//## method void Boolean.%k();
//## method void Boolean.%data();
//## method void Boolean.%dump();

static METHOD Boolean__s(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_bool(ctx, sfp[0].w, sfp[1].bvalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%s();
//## method void Int.%d();

static METHOD Int__s(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ifmt(ctx, sfp[0].w, K_INT_FMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%s();
//## method void Float.%f();

static METHOD Float__s(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ffmt(ctx, sfp[0].w, K_FLOAT_FMT, sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%k();
//## method void Int.%data();
//## method void Int.%dump();

static METHOD Int__k(CTX ctx, knh_sfp_t *sfp _RIX)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, O_cid(sfp[1].o));
	knh_write_intx(ctx, sfp[0].w, u, sfp[1].ivalue);
#else
	knh_write_ifmt(ctx, sfp[0].w, K_INT_FMT, sfp[1].ivalue);
#endif
}

/* ------------------------------------------------------------------------ */
//## method void Float.%k();
//## method void Float.%data();
//## method void Float.%dump();

static METHOD Float__k(CTX ctx, knh_sfp_t *sfp _RIX)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, O_cid(sfp[1].o));
	knh_write_floatx(ctx, sfp[0].w, u, sfp[1].fvalue);
#else
	knh_write_ffmt(ctx, sfp[0].w, K_FLOAT_FMT, sfp[1].fvalue);
#endif
}

#endif /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
