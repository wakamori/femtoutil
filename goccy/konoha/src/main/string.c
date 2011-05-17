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

#define USE_STEXT

#include"commons.h"
#include <errno.h>

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

int knh_bytes_parseint(knh_bytes_t t, knh_int_t *value)
{
	knh_uint_t n = 0, prev = 0, base = 10;
	size_t i = 0;
	if(t.len > 1) {
		if(t.utext[0] == '0') {
			if(t.utext[1] == 'x') {
				base = 16; i = 2;
			}
			else if(t.utext[1] == 'b') {
				base = 2;  i = 2;
			}
		}else if(t.utext[0] == '-') {
			base = 10; i = 1;
		}
	}
	for(;i < t.len; i++) {
		int c = t.utext[i];
		if('0' <= c && c <= '9') {
			prev = n;
			n = n * base + (c - '0');
		}else if(base == 16) {
			if('A' <= c && c <= 'F') {
				prev = n;
				n = n * 16 + (10 + c - 'A');
			}else if('a' <= c && c <= 'f') {
				prev = n;
				n = n * 16 + (10 + c - 'a');
			}else {
				break;
			}
		}else if(c == '_') {
			continue;
		}else {
			break;
		}
		if(!(n >= prev)) {
			*value = 0;
			return 0;
		}
	}
	if(t.utext[0] == '-') n = -((knh_int_t)n);
	*value = n;
	return 1;
}

int knh_bytes_parsefloat(knh_bytes_t t, knh_float_t *value)
{
#if defined(K_USING_NOFLOAT)
	{
		knh_int_t v = 0;
		knh_bytes_parseint(t, &v);
		*value = (knh_float_t)v;
	}
#else
	*value = strtod(t.text, NULL);
#endif
	return 1;
}


knh_index_t knh_bytes_indexOf(knh_bytes_t base, knh_bytes_t sub)
{
	const char *const str0 = base.text;  /* ide version */
	const char *const str1 = sub.text;
	knh_index_t len  = sub.len;
	knh_index_t loop = base.len - len;
	knh_index_t index = -1;
	if (loop >= 0) {
		knh_index_t i;
		const char *s0 = str0, *s1 = str1;
		const char *const s0end = s0 + loop;
		while(s0 <= s0end) {
			for (i = 0; i < len; i++) {
				if (s0[i] != s1[i]) {
					goto L_END;
				}
			}
			if (i == len) {
				return s0 - str0;
			}
			L_END:
			s0++;
		}
	}
	return index;
}

int knh_bytes_strcmp(knh_bytes_t v1, knh_bytes_t v2)
{
	int res1;
	if(v1.len < v2.len) {
		int res = knh_strncmp(v1.text, v2.text, v1.len);
		res1 = (res == 0) ? -1 : res;
	}
	else if(v1.len > v2.len) {
		int res = knh_strncmp(v1.text, v2.text, v2.len);
		res1 = (res == 0) ? 1 : res;
	}
	else {
		res1 = knh_strncmp(v1.text, v2.text, v1.len);
	}
	return res1;
}

/* ------------------------------------------------------------------------ */
/* These utf8 functions were originally written by Shinpei Nakata */

#define utf8_isLead(c)      ((c & 0xC0) != 0x80)
#define utf8_isTrail(c)     ((0x80 <= c) && (c <= 0xBF))
#define utf8_isSingleton(c) (c <= 0x7f)

//static knh_ushort_t utf8_getBytes0(knh_uchar_t c)
//{
//	return (utf8_isSingleton(c))?1:(c<=0x7f)?2:(c<=0xef)?3:(c<=0xf7)?4:(c<=0xfb)?5:6;
//}

static const knh_uchar_t _utf8len[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0,
};

#define utf8len(c)    _utf8len[(knh_uchar_t)c]


/* ------------------------------------------------------------------------ */

//knh_bool_t knh_bytes_checkENCODING0(knh_bytes_t s)
//{
//#ifdef K_USING_UTF8
//	size_t i, j;
//	knh_uchar_t ch;
//	size_t bytes = 0;
//	for (i=0; i < s.len; i++) {
//		ch = s.utext[i];
//		/* UTF8 must be in
//		 * single: 0x00 - 0x7f
//		 * lead: 0xC0 - 0xFD (actually, 0xC2-0xF4)
//		 * trail: 0x80 - 0xBF
//		 * consequently, invalid utf8 is ranged
//		 * 0xC0 - 0xC1, 0xF5 - 0xFF
//		 *  */
//		if (utf8_isSingleton(ch)) {
//		} else if (utf8_isLead(ch)) {
//			bytes = utf8_getBytes(ch);
//			for (j=1;j<bytes;j++)
//			{
//				ch = s.utext[i+j];
//				if (!utf8_isTrail(ch)) {
//					DBG_P("invalid UTF!");
//					return 0;
//				}
//			}
//			i += bytes;
//		}
//	}
//	return 1;
//#else
//	return 1;
//#endif
//}

knh_bool_t knh_bytes_checkENCODING(knh_bytes_t v)
{
#ifdef K_USING_UTF8
	const unsigned char *s = v.utext;
	const unsigned char *e = s + v.len;
	while (s < e) {
		size_t ulen = utf8len(s[0]);
		switch(ulen) {
		case 1: s++; break;
		case 2:
			if(!utf8_isTrail(s[1])) return 0;
			s+=2; break;
		case 3:
			if(!utf8_isTrail(s[1])) return 0;
			if(!utf8_isTrail(s[2])) return 0;
			s+=3; break;
		case 4:
			if(!utf8_isTrail(s[1])) return 0;
			if(!utf8_isTrail(s[2])) return 0;
			if(!utf8_isTrail(s[3])) return 0;
			s+=4; break;
		case 5: case 6: case 0: default:
			return 0;
		}
	}
	return (s == e);
#else
	return 1;
#endif
}

size_t knh_bytes_mlen(knh_bytes_t v)
{
#ifdef K_USING_UTF8
	size_t size = 0;
	const unsigned char *s = v.utext;
	const unsigned char *e = s + v.len;
	while (s < e) {
		size_t ulen = utf8len(s[0]);
		size ++;
		s += ulen;
	}
	return size;
#else
	return v.len;
#endif
}

knh_bytes_t knh_bytes_mofflen(knh_bytes_t v, size_t moff, size_t mlen)
{
#ifdef K_USING_UTF8
	size_t i;
	const unsigned char *s = v.utext;
	const unsigned char *e = s + v.len;
	for(i = 0; i < moff; i++) {
		s += utf8len(s[0]);
	}
	v.ubuf = (knh_uchar_t*)s;
	for(i = 0; i < mlen; i++) {
		s += utf8len(s[0]);
	}
	KNH_ASSERT(s <= e);
	v.len = (const char*)s - v.text;
	return v;
#else
	return knh_bytes_subbytes(m, moff, mlen); /* if K_ENCODING is not set */
#endif
}

knh_int_t knh_uchar_toucs4(knh_utext_t *utf8)   /* utf8 -> ucs4 */
{
#if defined(K_USING_UTF8)
	knh_int_t ucs4 = 0;
	int i= 0;
	knh_uchar_t ret = 0;
	if (!utf8_isSingleton(utf8[0])) {
		knh_ushort_t length_utf8 = utf8len(utf8[i]);
		knh_uchar_t mask = (knh_uchar_t)(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);

		switch(length_utf8){
		case 2:
			/* 110xxxxx 10xxxxxx */
			TODO();
			break;
		case 3:
			/* format 1110xxxx 10xxxxxx 10xxxxxx */
			// first 4 bits
			ucs4 = 0;
			ret = utf8[0] & mask;
			ucs4 = ucs4 | ret;
			// second bit
			ucs4 = ucs4 << 6;
			mask = mask | 1 << 4 | 1 << 5;
			ret = utf8[1] & mask;
			ucs4 = ucs4  | ret;
			// third bit
			ucs4 = ucs4 << 6;
			ret = mask & utf8[2];
			ucs4 = ucs4 | ret;
			break;
		default:
			/* TODO: */
			break;
		}
	} else {
		/* ASCII, let it goes...*/
		ucs4 = utf8[0];
	}
	return ucs4;
#else
	return (knh_int_t)utf8[0];
#endif
}

/* ------------------------------------------------------------------------ */
/* ucs4 -> utf8 */

char *knh_format_utf8(char *buf, size_t bufsiz, knh_uint_t ucs4)
{
	/* TODO: here, we assume that BOM bigEndian
	 and only 3 bytes or 1 byte UTF
	 */
	knh_uint_t mask = 0x0;
	knh_uint_t byte1 = 0x7F;
	knh_uint_t byte2 = 0x7FF;
	knh_uint_t byte3 = 0xFFFF;

	char *ret = buf;
	unsigned char utf8[8];
	if (ucs4 <= byte1) {
		/* 7 bits */
		knh_snprintf(buf, bufsiz, "%c", (int)(0xffff & ucs4));
		ret = buf;
	} else if (ucs4 <= byte2) {
		/* cut last 6 bits */
		TODO();
		/* first 5 bits */
	} else if (ucs4 <= byte3) {
		/* cut last 6 bits */
		mask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3| 1 << 4 | 1 << 5;
		utf8[2] = (unsigned char)(ucs4 & mask);
		utf8[2] = utf8[2] | 1 << 7;
		/* cut next 6 bits */
		ucs4 = ucs4 >> 6;
		utf8[1] = (unsigned char)(ucs4 & mask);
		utf8[1] = utf8[1] | 1 << 7;
		/* first 4 bits */
		mask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
		ucs4 = ucs4 >> 6;
		utf8[0] = (unsigned char)(ucs4 & mask);
		utf8[0] = utf8[0] | 1 << 7 | 1 << 6 | 1 << 5;
		utf8[3] = '\0';
		knh_snprintf(buf, bufsiz, "%s", utf8);
	} else {
		TODO();
	}
	return ret;
}
/* ------------------------------------------------------------------------ */
/* [String] */

static void knh_String_checkASCII(knh_String_t *o)
{
	unsigned char ch = 0;
	long len = S_size(o);
	const knh_uchar_t *p = (const knh_uchar_t *) S_tochar(o);
#ifdef K_USING_FASTESTFASTMODE /* written by ide */
	int len = S_size(o), n = (len + 3) / 4;
	/* Duff's device */
	switch(len%4){
	case 0: do{ ch |= *p++;
	case 3:     ch |= *p++;
	case 2:     ch |= *p++;
	case 1:     ch |= *p++;
	} while(--n>0);
	}
#else
	const knh_uchar_t *const e = p + len;
	while(p < e) {
		int n = len % 8;
		switch(n) {
			case 0: ch |= *p++;
			case 7: ch |= *p++;
			case 6: ch |= *p++;
			case 5: ch |= *p++;
			case 4: ch |= *p++;
			case 3: ch |= *p++;
			case 2: ch |= *p++;
			case 1: ch |= *p++;
		}
		len -= n;
	}
#endif
	String_setASCII(o, (ch < 128));
}

/* ------------------------------------------------------------------------ */

KNHAPI2(knh_String_t*) new_String_(CTX ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL)
{
	knh_String_t *s = (knh_String_t*)new_hObject(ctx, cid);
	if(t.len + 1 < sizeof(void*) * 2) {
		s->str.ubuf = (knh_uchar_t*)(&(s->hashCode));
		s->str.len = t.len;
		knh_memcpy(s->str.ubuf, t.utext, t.len);
		s->str.ubuf[s->str.len] = '\0';
		String_setTextSgm(s, 1);
	}
	else {
		s->str.len = t.len;
		s->str.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, KNH_SIZE(s->str.len+1));
		knh_memcpy(s->str.ubuf, t.utext, t.len);
		s->str.ubuf[s->str.len] = '\0';
		s->hashCode = 0;
	}
	if(memoNULL != NULL && String_isASCII(memoNULL)) {
		String_setASCII(s, 1);
	}
	else {
		knh_String_checkASCII(s);
	}
	return s;
}

KNHAPI2(knh_String_t*) new_String(CTX ctx, const char *str)
{
	if(str == NULL || str[0] == 0) {
		return TS_EMPTY;
	}
	else {
		knh_bytes_t t = {{str}, knh_strlen(str)};
		return new_String_(ctx, CLASS_String, t, NULL);
	}
}

knh_String_t *new_TEXT(CTX ctx, knh_class_t cid, knh_TEXT_t text, int isASCII)
{
	knh_String_t *s = (knh_String_t*)new_hObject(ctx, cid);
	s->str.text = text;
	s->str.len = knh_strlen(text);
	s->hashCode = 0;
	String_setASCII(s, isASCII);
	String_setTextSgm(s, 1);
	return s;
}

/* ------------------------------------------------------------------------ */

int knh_bytes_strcasecmp(knh_bytes_t v1, knh_bytes_t v2)
{
	if(v1.len < v2.len) {
		int res = knh_strncasecmp(v1.text, v2.text, v1.len);
		return (res == 0) ? -1 : res;
	}
	else if(v1.len > v2.len) {
		int res = knh_strncasecmp(v1.text, v2.text, v2.len);
		return (res == 0) ? 1 : res;
	}
	else {
		return knh_strncasecmp(v1.text, v2.text, v1.len);
	}
}

/* ------------------------------------------------------------------------ */
/* regex */

static knh_regex_t* strregex_malloc(CTX ctx, knh_String_t *pattern)
{
	return (knh_regex_t*)pattern;
}
static int strregex_parsecflags(CTX ctx, const char *opt)
{
	return 0;
}
static int strregex_parseeflags(CTX ctx, const char *opt)
{
	return 0;
}
static int strregex_regcomp(CTX ctx, knh_regex_t *reg, const char *pattern, int cflags)
{
	return 0;
}
static size_t strregex_regerror(int errcode, knh_regex_t *reg, char *ebuf, size_t ebuf_size)
{
	ebuf[0] = 0;
	return 0;
}
static int strregex_regexec(CTX ctx, knh_regex_t *reg, const char *str, size_t nmatch, knh_regmatch_t p[], int flags)
{
	size_t e = 0;
	knh_String_t *ptn = (knh_String_t*)reg;
	const char *po = strstr(str, S_tochar(ptn));
	if(po != NULL) {
		p[e].rm_so = po - str;
		p[e].rm_eo = p[e].rm_so + S_size(ptn);
		p[e].rm_name.ubuf = NULL;
		p[e].rm_name.len = 0;
		e++;
	}
	DBG_ASSERT(e < nmatch);
	p[e].rm_so = -1;
	p[e].rm_eo = -1;
	p[e].rm_name.ubuf = NULL;
	p[e].rm_name.len = 0;
	return (po) ? 0 : -1;
}

static void strregex_regfree(CTX ctx, knh_regex_t *reg) { }

static const knh_RegexSPI_t REGEX_STR = {
	"strregex",
	strregex_malloc, strregex_parsecflags, strregex_parseeflags,
	strregex_regcomp, strregex_regexec, strregex_regerror,
	strregex_regfree
};

const knh_RegexSPI_t* knh_getStrRegexSPI(void)
{
	return &REGEX_STR;
}

knh_bool_t Regex_isSTRREGEX(knh_Regex_t *re)
{
	return (re->spi == &REGEX_STR);
}

#ifdef K_USING_PCRE
/* This part was implemented by Yutaro Hiraoka */

#include <pcre.h>

#define PCRE_MAX_ERROR_MESSAGE_LEN 512

typedef struct {
	pcre *re;
	const char *err;
	int erroffset;
} PCRE_regex_t;

static knh_regex_t* pcre_regex_malloc(CTX ctx, knh_String_t* s)
{
	PCRE_regex_t *preg = (PCRE_regex_t*) KNH_MALLOC(ctx,sizeof(PCRE_regex_t));
	return (knh_regex_t *) preg;
}

size_t pcre_regex_nmatchsize(knh_regex_t *reg)
{
	PCRE_regex_t *preg = (PCRE_regex_t*)reg;
	size_t capsize;
	pcre_fullinfo(preg->re, NULL, PCRE_INFO_CAPTURECOUNT, &capsize);
	return (capsize > 0) ? capsize+1 : K_REGEX_MATCHSIZE;
}

static int pcre_regex_parsecflags(CTX ctx, const char *option)
{
	int i, cflags = PCRE_UTF8;
	int optlen = strlen(option);
	for (i = 0; i < optlen; i++) {
		switch(option[i]) {
		case 'i': // caseless
			cflags |= PCRE_CASELESS;
			break;
		case 'm': // multiline
			cflags |= PCRE_MULTILINE;
			break;
		case 's': // dotall
			cflags |= PCRE_DOTALL;
			break;
		case 'x': //extended
			cflags |= PCRE_EXTENDED;
			break;
		default: break;
		}
	}
	return cflags;
}

static int pcre_regex_parseeflags(CTX ctx, const char *option)
{
	int i, eflags = 0;
	int optlen = strlen(option);
	for (i = 0; i < optlen; i++) {
		switch(option[i]){
		default: break;
		}
	}
	return eflags;
}

static size_t pcre_regex_regerror(int res, knh_regex_t *reg, char *ebuf, size_t ebufsize)
{
	PCRE_regex_t *pcre = (PCRE_regex_t*)reg;
	snprintf(ebuf, ebufsize, "[%d]: %s", pcre->erroffset, pcre->err);
	return 0;
}

static int pcre_regex_regcomp(CTX ctx, knh_regex_t *reg, const char *pattern, int cflags)
{
	PCRE_regex_t* preg = (PCRE_regex_t*)reg;
	preg->re = pcre_compile(pattern, cflags, &preg->err, &preg->erroffset, NULL);
	return (preg->re != NULL) ? 0 : -1;
}

static int pcre_regex_regexec(CTX ctx, knh_regex_t *reg, const char *str, size_t nmatch, knh_regmatch_t p[], int eflags)
{
	PCRE_regex_t *preg = (PCRE_regex_t*)reg;
	int res, nm_count, nvector[nmatch*3];
	nvector[0] = 0;
	size_t idx, matched;
	if (strlen(str) == 0) return -1;
	if ((res = pcre_exec(preg->re, NULL, str, strlen(str), 0, eflags, nvector, nmatch*3)) >= 0) {
		matched = (res > 0) ? res : nmatch;
		res = 0;
		for (idx = 0; idx < matched; idx++) {
			p[idx].rm_so = nvector[2*idx];
			p[idx].rm_eo = nvector[2*idx+1];
		}
		p[idx].rm_so = -1;
		
		pcre_fullinfo(preg->re, NULL, PCRE_INFO_NAMECOUNT, &nm_count);
		if (nm_count > 0) {
			unsigned char *nm_table;
			int nm_entry_size;
			pcre_fullinfo(preg->re, NULL, PCRE_INFO_NAMETABLE, &nm_table);
			pcre_fullinfo(preg->re, NULL, PCRE_INFO_NAMEENTRYSIZE, &nm_entry_size);
			unsigned char *tbl_ptr = nm_table;
			for (idx = 0; idx < nm_count; idx++) {
				int n_idx = (tbl_ptr[0] << 8) | tbl_ptr[1];
				unsigned char *n_name = tbl_ptr + 2;
				p[n_idx].rm_name.utext = n_name;
				p[n_idx].rm_name.len = strlen((char*)n_name);
				tbl_ptr += nm_entry_size;
			}
		}
	}
	return res;
}

static void pcre_regex_regfree(CTX ctx, knh_regex_t *reg)
{
	PCRE_regex_t *preg = (PCRE_regex_t*)reg;
	pcre_free(preg->re);
	KNH_FREE(ctx, preg, sizeof(PCRE_regex_t));
}

static const knh_RegexSPI_t REGEX_PCRE = {
	"pcre",
	pcre_regex_malloc,
	pcre_regex_parsecflags,
	pcre_regex_parseeflags,
	pcre_regex_regcomp,
	pcre_regex_regexec,
	pcre_regex_regerror,
	pcre_regex_regfree
};

#endif/*K_USING_PCRE*/

const knh_RegexSPI_t* knh_getRegexSPI(void)
{
#ifdef K_USING_PCRE
	return &REGEX_PCRE;
#else
	return &REGEX_STR;
#endif
}

/* ------------------------------------------------------------------------ */

static knh_conv_t* strconv_open(CTX ctx, const char* to, const char *from)
{
	iconv_t rc = ctx->spi->iconv_open(to, from);
	if(rc == (iconv_t)-1){
		KNH_WARN(ctx, "%s: unknown codec from=%s to=%s", ctx->spi->iconvspi, from, to);
		return NULL;
	}
	return (knh_conv_t*)rc;
}

static knh_bool_t strconv(Ctx *ctx, knh_conv_t *iconvp, knh_bytes_t from, knh_Bytes_t *to)
{
	char buffer[4096], *ibuf = (char*)from.ubuf;
	size_t ilen = from.len, rsize = 0;//, ilen_prev = ilen;
	iconv_t cd = (iconv_t)iconvp;
	knh_bytes_t bbuf = {{(const char*)buffer}, 0};
	while(ilen > 0) {
		char *obuf = buffer;
		size_t olen = sizeof(buffer);
		size_t rc = ctx->spi->iconv(cd, &ibuf, &ilen, &obuf, &olen);
		olen = sizeof(buffer) - olen; rsize += olen;
		if(rc == (size_t)-1 && errno == EILSEQ) {
			KNH_WARN(ctx, "%s: invalid sequence", ctx->spi->iconvspi);
			return 0;
		}
		bbuf.len = olen;
		knh_Bytes_write(ctx, to, bbuf);
	}
	return 1;
}
static void strconv_close(CTX ctx, knh_conv_t *conv)
{
	ctx->spi->iconv_close((iconv_t)conv);
}

static knh_ConvDSPI_t SCONV = {
	K_DSPI_CONVTO, "md5",
	strconv_open, // open,
	strconv,  // byte->byte     :conv
	strconv,  // String->byte   :enc
	strconv,   // byte->String   :dec
	NULL,  // String->String :sconv
	strconv_close,
	NULL
};

knh_StringDecoder_t* new_StringDecoderNULL(CTX ctx, knh_bytes_t t)
{
	if(knh_bytes_strcasecmp(t, STEXT(K_ENCODING)) == 0) {
		return KNH_TNULL(StringDecoder);
	}
	else {
		iconv_t id = ctx->spi->iconv_open(K_ENCODING, t.text);
		if(id != (iconv_t)(-1)) {
			knh_StringDecoder_t *c = new_(StringDecoder);
			c->conv = (knh_conv_t*)id;
			c->dspi = &SCONV;
			return c;
		}
	}
	return NULL;
}

knh_StringEncoder_t* new_StringEncoderNULL(CTX ctx, knh_bytes_t t)
{
	if(knh_bytes_strcasecmp(t, STEXT(K_ENCODING)) == 0) {
		return KNH_TNULL(StringEncoder);
	}
	else {
		iconv_t id = ctx->spi->iconv_open(K_ENCODING, t.text);
		if(id != (iconv_t)(-1)) {
			knh_StringEncoder_t *c = new_(StringEncoder);
			c->conv = (knh_conv_t*)id;
			c->dspi = &SCONV;
			return c;
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_cwb_newStringDECODE(CTX ctx, knh_cwb_t *cwb, knh_StringDecoder_t *c)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	LOCAL_NEW(ctx, lsfp, 0, knh_String_t*, s, knh_cwb_newString(ctx, cwb));
	if(!String_isASCII(s)) {
		c->dspi->dec(ctx, c->conv, S_tobytes(s), cwb->ba);
		s = knh_cwb_newString(ctx, cwb);
		KNH_SETv(ctx, lsfp[0].o, KNH_NULL); //
	}
	END_LOCAL(ctx, lsfp, s);
	return s;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#else /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.opEXISTS(NameSpace ns);

static METHOD String_opEXISTS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t path = S_tobytes(sfp[0].s);
	DBG_ASSERT(IS_NameSpace(sfp[1].ns));
	const knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, sfp[1].ns, path);
	knh_bool_t tf = 0;
	if(dspi != NULL) tf = dspi->exists(ctx, sfp[1].ns, path, dspi->thunk);
	RETURNb_(tf);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Private method dynamic String.path(String scheme, NameSpace ns, Class c);

static METHOD String_path(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_class_t cid = (sfp[3].c)->cid;
	knh_bytes_t path = S_tobytes(sfp[0].s);
	knh_bytes_t qpath = S_tobytes(sfp[1].s);
	const knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, sfp[2].ns, qpath);
	knh_String_t* spath = sfp[0].s;
	Object *v = NULL;
	int isTRIM = 0;
	if(!knh_bytes_startsWith(path, qpath)) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_Bytes_write(ctx, cwb->ba, qpath);
		knh_Bytes_write(ctx, cwb->ba, path);
		spath = knh_cwb_newString(ctx, cwb);
		KNH_SETv(ctx, sfp[rix].s, spath);
		path = S_tobytes(spath);
		isTRIM = 1;
	}
	KNH_ASSERT(dspi != NULL);
	KNH_ASSERT(dspi->hasType(ctx, cid, dspi->thunk));
	if(cid == CLASS_Boolean) {
		sfp[rix].bvalue = dspi->exists(ctx, sfp[2].ns, path, dspi->thunk);
		v = sfp[rix].bvalue ? KNH_TRUE : KNH_FALSE;
	}
	else if(cid == CLASS_String) {
		v = UPCAST(spath);
	}
	else {
		v = dspi->newObjectNULL(ctx, sfp[2].ns, cid, spath, dspi->thunk);
		if(v == NULL) {
			KNH_SYSLOG(ctx, sfp, LOG_WARNING, "MissingPath", "qpath='%B', path='%B' for %C", qpath, path, cid);
			v = KNH_NULVAL(cid);
		}
	}
	RETURN_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.equals(String s);

static METHOD String_equals(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_((S_size(sfp[0].s) == S_size(sfp[1].s) &&
		knh_bytes_strcmp(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)) == 0));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.startsWith(String s);

static METHOD String_startsWith(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_bytes_startsWith(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.endsWith(String s);

static METHOD String_endsWith(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_bytes_endsWith(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.concat(Object value, ...);
//## @Const method String String.opADD(dynamic value);

static METHOD String_concat(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int i, ac = knh_stack_argc(ctx, sfp);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	for(i = 0; i < ac; i++) {
		if(IS_bString(sfp[i].o)) {
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(sfp[i].s));
		}
		else {
			knh_write_Object(ctx, cwb->w, sfp[i].o, FMT_s);
		}
	}
	RETURN_(knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.indexOf(String s);

static METHOD String_indexOf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	if (IS_NULL(sfp[1].o)) RETURNi_(-1);
	knh_bytes_t delim = S_tobytes(sfp[1].s);
	knh_index_t loc = knh_bytes_indexOf(base, delim);
	if(delim.len == 0) loc--;
	if (loc >= 0 && !String_isASCII(sfp[0].s)) {
		base.len = (size_t)loc;
		loc = knh_bytes_mlen(base);
	}
	RETURNi_(loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.lastIndexOf(String s);

static METHOD String_lastIndexOf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	if (IS_NULL(sfp[1].o)) RETURNi_(-1);
	knh_bytes_t delim = S_tobytes(sfp[1].s);
	knh_index_t loc = base.len - delim.len;
	if(delim.len == 0) loc--;
	for(; loc >= 0; loc--) {
		if(base.utext[loc] == delim.utext[0]) {
			knh_bytes_t sub = {{base.text + loc}, delim.len};
			if(knh_bytes_strcmp(sub, delim) == 0) break;
		}
	}
	if (loc >= 0 && !String_isASCII(sfp[0].s)) {
		base.len = (size_t)loc;
		loc = knh_bytes_mlen(base);
	}
	RETURNi_(loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.search(Regex re);

static METHOD String_search(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Regex_t *re = sfp[1].re;
	if(IS_NULL(re) || S_size(re->pattern) == 0) RETURNi_(-1);
	knh_index_t loc = -1;
	const char *str = S_tochar(sfp[0].s);  // necessary
	knh_regmatch_t pmatch[1];
	int res = re->spi->regexec(ctx, re->reg, str, 1, pmatch, re->eflags);
	if(res == 0) {
		loc = pmatch[0].rm_so;
		if (!String_isASCII(sfp[0].s) && loc != -1) {
			knh_bytes_t base = {{str}, loc};
			loc = knh_bytes_mlen(base);
		}
	}
//	else {
//		char ebuf[K_ERRBUFSIZE];
//		re->spi->regerror(res, re->reg, ebuf, K_ERRBUFSIZE);
//		ctx->api->ebilog(ctx, re->spi->name, "regexec", LOG_WARNING, "errmsg='%s'", ebuf);
//	}
	RETURNi_(loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method String[] String.match(Regex re);

size_t pcre_regex_nmatchsize(knh_regex_t *reg);

static METHOD String_match(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s0 = sfp[0].s;
	knh_Regex_t *re = sfp[1].re;
	knh_Array_t *a = NULL;
	if(IS_NULL(re) || S_size(re->pattern) == 0) {
		a = new_Array(ctx, CLASS_String, 0);
	}
	else {
		const char *str = S_tochar(sfp[0].s);  // necessary
		size_t nmatch = pcre_regex_nmatchsize(re->reg);
		knh_regmatch_t pmatch[nmatch];
		int res = re->spi->regexec(ctx, re->reg, str, nmatch, pmatch, re->eflags);
		a = new_Array(ctx, CLASS_String, nmatch);
		if(res == 0) {
			knh_bytes_t sub = S_tobytes(s0);
			int i;
			for(i = 0; i < nmatch; i++) {
				if(pmatch[i].rm_so == -1) break;
				//DBG_P("[%d], rm_so=%d, rm_eo=%d", i, pmatch[i].rm_so, pmatch[i].rm_eo);
				sub.text = str + pmatch[i].rm_so;
				sub.len = pmatch[i].rm_eo - pmatch[i].rm_so;
				knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, s0));
			}
		}
//		else {
//			char ebuf[K_ERRBUFSIZE];
//			re->spi->regerror(res, re->reg, ebuf, K_ERRBUFSIZE);
//			ctx->api->ebilog(ctx, re->spi->name, "regexec", LOG_WARNING, "errmsg='%s'", ebuf);
//		}
	}
	RETURN_(a);
}

///* ------------------------------------------------------------------------ */
//
//static int knh_bytes_equals_(knh_bytes_t base, size_t s, knh_bytes_t target)
//{
//	size_t i;
//	for(i = 1; i < target.len; i++) {
//		if(base.utext[s+i] != target.utext[i]) return 0;
//	}
//	return 1;
//}

/* ------------------------------------------------------------------------ */
//## @Const method String String.replace(Regex re, String s);

static METHOD String_replace(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s0 = sfp[0].s;
	knh_Regex_t *re = sfp[1].re;
	knh_String_t *s = s0;
	if(!IS_NULL(re) && S_size(re->pattern) > 0) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_bytes_t tos = S_tobytes(sfp[2].s);
		const char *str = S_tochar(s0);  // necessary
		const char *estr = str + S_size(s0);
		knh_regmatch_t pmatch[K_REGEX_MATCHSIZE];
		while (str < estr) {
			int res = re->spi->regexec(ctx, re->reg, str, K_REGEX_MATCHSIZE, pmatch, re->eflags);
			if (res == 0) {
				knh_bytes_t sub = {{str},  pmatch[0].rm_so};
				knh_Bytes_write(ctx, cwb->ba, sub);
				knh_Bytes_write(ctx, cwb->ba, tos);
				str += (pmatch[0].rm_eo);
			}
			else {
				knh_bytes_t sub = {{str}, knh_strlen(str)};
				knh_Bytes_write(ctx, cwb->ba, sub);
				break;
			}
		}
		s = knh_cwb_newString(ctx, cwb);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */

static knh_Array_t *knh_String_toCharArray(CTX ctx, knh_String_t *bs, int istrim)
{
	knh_bytes_t base = S_tobytes(bs);
	if(String_isASCII(bs)) {
		size_t i, n = base.len;
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.utext[i])) continue;
			knh_bytes_t sub = {{base.text + i}, 1};
			knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, bs));
		}
		return a;
	}
	else {
		size_t i, n = knh_bytes_mlen(base);
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.utext[i])) continue;
			knh_bytes_t sub = knh_bytes_mofflen(base, i, 1);
			knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, bs));
		}
		return a;
	}
}

/* ------------------------------------------------------------------------ */

static knh_bytes_t knh_bytes_trim(knh_bytes_t t)
{
	while(isspace(t.utext[0])) {
		t.utext++;
		t.len--;
	}
	if(t.len == 0) return t;
	while(isspace(t.utext[t.len-1])) {
		t.len--;
		if(t.len == 0) return t;
	}
	return t;
}

/* ------------------------------------------------------------------------ */
//## @Const method String[] String.split(Regex re);

static METHOD String_split(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s0 = sfp[0].s;
	knh_Regex_t *re = sfp[1].re;
	knh_Array_t *a = NULL;
	KNH_GC(ctx);
	if(IS_NULL(re) || S_size(re->pattern) == 0) {
		a = knh_String_toCharArray(ctx, s0, 0);
	}
	else {
		const char *str = S_tochar(s0);  // necessary
		const char *estr = str + S_size(s0);
		knh_regmatch_t pmatch[K_REGEX_MATCHSIZE];
		a = new_Array(ctx, CLASS_String, 0);
		while (str < estr) {
			int res = re->spi->regexec(ctx, re->reg, str, K_REGEX_MATCHSIZE, pmatch, re->eflags);
			if(res == 0) {
				knh_bytes_t sub = {{str},  pmatch[0].rm_so};
				knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, s0));
				str += (pmatch[0].rm_eo);
			}
			else {
				knh_bytes_t sub = {{str}, knh_strlen(str)};
				knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, s0));
				break;
			}
		}
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## @Const method Bytes Bytes.(Converter enc);

static METHOD Bytes_convert(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->conv(ctx, c->conv, BA_tobytes(sfp[0].ba), cwb->ba);
	knh_Bytes_t *ba = new_Bytes(ctx, NULL, knh_cwb_size(cwb));
	knh_Bytes_write(ctx, ba, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## @Const method Bytes String.encode(StringEncoder enc);

static METHOD String_encode(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	//fprintf(stderr, "%s, c=%p, c->dspi=%p, c->dspi->enc=****\n", CLASS__(c->h.cid), c, c->dspi);
	c->dspi->enc(ctx, c->conv, S_tobytes(sfp[0].s), cwb->ba);
	knh_Bytes_t *ba = new_Bytes(ctx, NULL, knh_cwb_size(cwb));
	knh_Bytes_write(ctx, ba, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## @Const method String Bytes.decode(StringDecoder c);

static METHOD Bytes_toString(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->dec(ctx, c->conv, BA_tobytes(sfp[0].ba), cwb->ba);
	RETURN_(knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.(StringConverter c);
//## @Const method String String.convert(StringConverter c);

static METHOD String_convert(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	if (!IS_NULL(c)) {
		c->dspi->sconv(ctx, c->conv, S_tobytes(sfp[0].s), cwb->ba);
	}
	RETURN_(knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.trim();

static METHOD String_trim(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t t = S_tobytes(sfp[0].s);
	knh_bytes_t t2 = knh_bytes_trim(t);
	knh_String_t *s = sfp[0].s;
	if(t.len > t2.len) {
		s = new_String_(ctx, CLASS_String, t2, s);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Regex.opHAS(String s);

static METHOD Regex_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Regex_t *re = sfp[0].re;
	knh_regmatch_t pmatch[1];
	const char *str = S_tochar(sfp[1].s);
	int res = re->spi->regexec(ctx, re->reg, str, 1, pmatch, re->eflags);
	RETURNb_(res == 0);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String Int;

static TYPEMAP String_Int(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_int_t v = 0;
	if(!knh_bytes_parseint(S_tobytes(sfp[K_TMRIDX].s), &v)) {
		KNH_SETv(ctx, sfp[rix].o, KNH_NULVAL(CLASS_Int));
	}
	RETURNi_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String Float;

static TYPEMAP String_Float(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_float_t v = 0.0;
	if(!knh_bytes_parsefloat(S_tobytes(sfp[K_TMRIDX].s), &v)) {
		KNH_SETv(ctx, sfp[rix].o, KNH_NULVAL(CLASS_Float));
	}
	RETURNf_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String Bytes;

static TYPEMAP String_Bytes(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_t *b = new_Bytes(ctx, "UTF8", S_size(sfp[K_TMRIDX].s) + 1);
	knh_Bytes_write(ctx, b, S_tobytes(sfp[K_TMRIDX].s));
	RETURN_(b);
}

/* ------------------------------------------------------------------------ */

static ITRNEXT String_nextChar(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *itr = ITR(sfp);
	knh_String_t *s = (knh_String_t*)DP(itr)->source;
	knh_bytes_t base = S_tobytes(s);
	size_t pos = (size_t)DP(itr)->mitr.index;
	if(pos < knh_bytes_mlen(base)) {
		knh_bytes_t sub = knh_bytes_mofflen(base, pos, 1);
		s = new_String_(ctx, CLASS_String, sub, s);
		DP(itr)->mitr.index = pos + 1;
		ITRNEXT_(s);
	}
	ITREND_();
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String String..;
//## method String.. String.opITR();

static TYPEMAP String_Iterator(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_Iterator(ctx, CLASS_String, sfp[0].o, String_nextChar));
}

/* ------------------------------------------------------------------------ */
//## mapper Bytes InputStream;

static TYPEMAP Bytes_InputStream(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_BytesInputStream(ctx, sfp[0].ba));
}

/* ------------------------------------------------------------------------ */

#endif/*K_INCLUDE_BUILTINAPI*/
