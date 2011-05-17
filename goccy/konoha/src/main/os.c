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
#define USE_bytes_startsWith    1
#define USE_bytes_endsWith      1
#define USE_bytes_index         1

#define USE_cwb_open      1
#define USE_cwb_open2     1
#define USE_cwb_tobytes   1
#define USE_cwb_write     1
#define USE_cwb_putc      1

#include"commons.h"

#ifdef K_USING_POSIX_
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#ifdef K_USING_MACOSX_
#include <mach-o/dyld.h>
#endif
#endif/*K_USING_POSIX_*/

#ifdef K_USING_WINDOWS
#include<windows.h>
#endif

#ifdef K_USING_BTRON
#include<btron/file.h>
#include<btron/dynload.h>
#include<tstring.h>
#endif

#ifdef HAVE_LOCALE_H
#include<locale.h>
#endif

#ifdef HAVE_LOCALCHARSET_H
#include<localcharset.h>
#endif

#ifdef HAVE_LANGINFO_H
#include<langinfo.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#define K_PERROR_BEFORE_RETRUN

#if defined(K_USING_WINDOWS)
#define LIBNAME "Windows"
#else
#define LIBNAME "libc"

#if !defined(K_USING_POSIX_)
static int UnsupportedAPI(CTX ctx, const char *funcname)
{
	ctx->api->perror(ctx, NULL, K_PERROR_LIBNAME, funcname);
	return K_PERROR_OK;
}
#endif
#endif

/* ------------------------------------------------------------------------ */
/* sysinfo */

#ifdef K_USING_BSDSYSCTL_
static size_t sysctl_getint(const char *name, int defdata)
{
	int data = defdata;
	size_t len = sizeof(int);
	if(sysctlbyname(name, &data, &len, NULL, 0) == -1) {
		//fprintf(stderr, "sysctl: %s=FAILED\n", name);
		return (size_t)data;
	}
	//fprintf(stderr, "sysctl: %s=%d\n", name, data);
	return (size_t)data;
}

static const char *sysctl_getstring(const char *name, char *buffer, size_t len)
{
	size_t len2 = 0;
	if(sysctlbyname(name, NULL, &len2, NULL, 0) == -1) {
		//fprintf(stderr, "sysctl: %s=FAILED, len2=%d<%d\n", name, len2, len);
		return NULL;
	}
	if(len2 <= len) {
		sysctlbyname(name, buffer, &len2, NULL, 0);
		//fprintf(stderr, "sysctl: %s=%s\n", name, buffer);
	}
	else {
		//fprintf(stderr, "sysctl: %s=FAILED, len2=%d<%d\n", name, len2, len);
	}
	return (const char*)buffer;
}

static void setsysinfo_sysctl(knh_sysinfo_t *sysinfo)
{
	static char cpu_brand[128] = {0};
	static char hw_machine[80] = {0};
	static char hw_model[128] = {0};
	static char kern_ostype[40] = {0};
	static char kern_version[40] = {0};
	static char kern_desc[128] = {0};
	sysinfo->cpu_desc = sysctl_getstring("machdep.cpu.brand_string", cpu_brand, sizeof(cpu_brand));
	sysinfo->hw_machine = sysctl_getstring("hw.machine", hw_machine, sizeof(hw_machine));
	sysinfo->hw_model = sysctl_getstring("hw.model", hw_model, sizeof(hw_model));
	sysinfo->hw_ncpu = sysctl_getint("hw.ncpu", 1);
	sysinfo->hw_availcpu = sysctl_getint("hw.availcpu", sysinfo->hw_ncpu);
	sysinfo->hw_physmem = sysctl_getint("hw.physmem", 0);
	sysinfo->hw_usermem = sysctl_getint("hw.usermem", 0);
	sysinfo->hw_cpufrequency = sysctl_getint("hw.cpufrequency", 0);
	sysinfo->hw_l2cachesize = sysctl_getint("hw.l2cachesize", 0);
	sysinfo->kern_ostype = sysctl_getstring("kern.ostype", kern_ostype, sizeof(kern_ostype));
	sysinfo->kern_version = sysctl_getstring("kern.osrelease", kern_version, sizeof(kern_version));
	sysinfo->kern_desc = sysctl_getstring("kern.version", kern_desc, sizeof(kern_desc));
}
#endif/*#ifdef K_USING_BSDSYSCTL*/

static char *append_string(char *p, const char *str, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++) p[i] = str[i];
	p[len-1] = ' ';
	return p + (len);
}

#define APPEND_OPTION(p, STR)  p = append_string(p, STR, sizeof(STR));

static void setkonohainfo(knh_sysinfo_t *sysinfo)
{
	static char options[512] = {0};
	static char date[40] = {0};
	char *p = options;
	sysinfo->konoha_type = K_PROGNAME;
	sysinfo->konoha_codename = K_CODENAME;
	sysinfo->konoha_version = K_VERSION;
	sysinfo->konoha_disttype = K_DISTTYPE;
	sysinfo->konoha_disturl  = K_DISTURL;
	sysinfo->konoha_major_version = K_MAJOR_VERSION;
	sysinfo->konoha_minor_version = K_MINOR_VERSION;
	sysinfo->konoha_patch_version = K_PATCH_VERSION;
#if defined(K_PATCH_LEVEL)
	sysinfo->konoha_patch_level = K_PATCH_LEVEL;
#elif defined(K_REVISION)
	sysinfo->konoha_patch_level = K_REVISION;
#endif
	sysinfo->konoha_cc = CC_TYPE;
	knh_snprintf(date, sizeof(date), "%s %s", __DATE__, __TIME__);
	sysinfo->konoha_builtdate = date;
	sysinfo->konoha_config = CC_PLATFORM;
	sysinfo->konoha_disttype = K_DISTTYPE;
	sysinfo->konoha_disturl = K_DISTURL;
	sysinfo->konoha_systembits = (sizeof(void*) * 8);
	sysinfo->konoha_options = (const char*)options;
	/* THESE OPTIONS MUST BE ORDERED ALPHABETICALLY */
#if defined(K_USING_FASTEST)
	APPEND_OPTION(p, "fastest");
#endif
#if defined(K_USING_ICONV)
	APPEND_OPTION(p, "iconv");
#endif
#if defined(K_USING_INT32)
	APPEND_OPTION(p, "int32");
#endif
//#if defined(K_USING_PREFETCH)
//	APPEND_OPTION(p, "prefetch");
//#endif
//#if defined(K_USING_READLINE)
//	APPEND_OPTION(p, "readline");
//#endif
#if defined(K_USING_RCGC)
	APPEND_OPTION(p, "rcgc");
#else
	APPEND_OPTION(p, "msgc");
#endif
#if defined(K_USING_THCODE_)
	APPEND_OPTION(p, "thcode");
#else
	APPEND_OPTION(p, "switch");
#endif
#if defined(K_USING_SQLITE3)
	APPEND_OPTION(p, "sqlite3");
#endif
#if defined(K_USING_SYSLOG)
	APPEND_OPTION(p, "syslog");
#endif
#if defined(K_USING_THREAD)
	APPEND_OPTION(p, "thread");
#endif
#if defined(K_CONFIG_OS)
	if(sysinfo->kern_ostype == NULL) sysinfo->kern_ostype = K_CONFIG_OS;
#endif
}

static const knh_sysinfo_t *sysinfo_ = NULL;

const knh_sysinfo_t* knh_getsysinfo(void)
{
	if(sysinfo_ == NULL) {
		static knh_sysinfo_t sysinfo = {0};
#ifdef K_USING_BSDSYSCTL_
		setsysinfo_sysctl(&sysinfo);
#endif
		setkonohainfo(&sysinfo);
		sysinfo_ = &sysinfo;
	}
	return sysinfo_;
}

static void dump_int(CTX ctx, knh_OutputStream_t *w, const char *name, knh_intptr_t data, int isALL)
{
	if(data == -1) {
		if(!isALL) return;
		if(ctx == NULL) {
			fprintf(stderr, "%s: UNDEFINED\n", name);
		}
		else {
			knh_printf(ctx, w, "%s: UNDEFINED\n", name);
		}
	}
	else {
		if(ctx == NULL) {
			fprintf(stderr, "%s: %ld\n", name, (long)data);
		}
		else {
			knh_printf(ctx, w, "%s: %d\n", name, data);
		}
	}
}

static void dump_string(CTX ctx, knh_OutputStream_t *w, const char *name, const char *data, int isALL)
{
	if(data == NULL) {
		if(!isALL) return;
		data = "";
	}
	if(ctx == NULL) {
		fprintf(stderr, "%s: %s\n", name, data);
	}
	else {
		knh_printf(ctx, w, "%s: %s\n", name, data);
	}
}

void dump_sysinfo(CTX ctx, knh_OutputStream_t *w, int isALL)
{
	const knh_sysinfo_t *sysinfo = knh_getsysinfo();
	dump_string(ctx, w, "konoha.type", sysinfo->konoha_type, isALL);
	dump_string(ctx, w, "konoha.version", sysinfo->konoha_version, isALL);
	dump_int(ctx, w, "konoha.patch.level", sysinfo->konoha_patch_level, isALL);
	dump_string(ctx, w, "konoha.options", sysinfo->konoha_options, isALL);
	dump_int(ctx, w, "konoha.systembits", sysinfo->konoha_systembits, isALL);
	dump_string(ctx, w, "konoha.config", sysinfo->konoha_config, isALL);
	dump_string(ctx, w, "konoha.built.cc", sysinfo->konoha_cc, isALL);
	dump_string(ctx, w, "konoha.built.date", sysinfo->konoha_builtdate, isALL);
	dump_string(ctx, w, "konoha.dist_type", sysinfo->konoha_disttype, isALL);
	dump_string(ctx, w, "konoha.dist_url", sysinfo->konoha_disturl, isALL);
	dump_string(ctx, w, "kern.ostype", sysinfo->kern_ostype, isALL);
	dump_string(ctx, w, "kern.version", sysinfo->kern_version, isALL);
	dump_string(ctx, w, "kern.desc", sysinfo->kern_desc, isALL);
	dump_string(ctx, w, "hw.machine", sysinfo->hw_machine, isALL);
	dump_string(ctx, w, "hw.model", sysinfo->hw_model, isALL);
	dump_string(ctx, w, "cpu.desc", sysinfo->cpu_desc, isALL);
	dump_int(ctx, w, "hw.ncpu", sysinfo->hw_ncpu, isALL);
	dump_int(ctx, w, "hw.availcpu", sysinfo->hw_availcpu, isALL);
	dump_int(ctx, w, "hw.physmem", sysinfo->hw_physmem, isALL);
	dump_int(ctx, w, "hw.usermem", sysinfo->hw_usermem, isALL);
	dump_int(ctx, w, "hw.cpufrequency", sysinfo->hw_cpufrequency, isALL);
	dump_int(ctx, w, "hw.l2cachesize", sysinfo->hw_l2cachesize, isALL);
}

/* ------------------------------------------------------------------------ */

knh_path_t* knh_path_open_(CTX ctx, const char *scheme, knh_bytes_t t, knh_path_t *ph)
{
	knh_Bytes_t *ba = ctx->bufa;
	ph->pstart = ba->bu.len;
	if(!(ba->bu.len + K_PATHMAX < ba->dim->capacity)) {
		knh_Bytes_expands(ctx, ba, K_PAGESIZE);  // K_PATHMAX < KPAGESIZE;
	}
	ba->bu.len += K_PATHMAX;
	knh_path_reset(ctx, ph, scheme, t);
	return ph;
}

knh_path_t* knh_path_open(CTX ctx, const char *scheme, const char *name, knh_path_t *ph)
{
	return knh_path_open_(ctx, scheme, B(name), ph);
}

knh_cwb_t* knh_cwb_copy(CTX ctx, knh_cwb_t *cwbbuf, knh_path_t *ph, int hasScheme)
{
	knh_cwb_t *cwb = knh_cwb_open(ctx, cwbbuf);
	knh_Bytes_ensureSize(ctx, cwb->ba, ph->plen + 1);
	knh_bytes_t t = {{P_text(ph)}, ph->plen};
	if(!hasScheme) {
		t.text = t.text + ph->pbody;
		t.len = t.len - ph->pbody;
	}
	knh_Bytes_write(ctx, cwb->ba, t);
	return cwb;
}

void knh_path_reset(CTX ctx, knh_path_t *ph, const char *scheme, knh_bytes_t t)
{
	knh_index_t idx = knh_bytes_index(t, ':');
	char *buf = P_buf(ph);
	if(t.len + 1 > K_PATHMAX) t.len = K_PATHMAX - 1;
	if(idx > 0 || scheme == NULL) {
		knh_memcpy(buf, t.text, t.len); buf[t.len] = 0;
		ph->pbody = idx + 1;
		ph->plen = t.len;
	}
	else {
		knh_snprintf(buf, K_PATHMAX, "%s:%s", scheme, t.text);
		ph->pbody = knh_strlen(scheme) + 1;
		ph->plen = t.len + ph->pbody;
	}
	DBG_ASSERT(buf[ph->plen] == 0);
	ph->isRealPath = 0;
}

void knh_path_close(CTX ctx, knh_path_t *ph)
{
	knh_Bytes_t *ba = ctx->bufa;
	KNH_ASSERT(ph->pstart < ba->bu.len);
	//DBG_P("ph='%s', size=%d", P_text(ph), ph->plen);
	knh_Bytes_clear(ba, ph->pstart);
	ph->pstart = 0;
	ph->pbody = 0;
	ph->plen = 0;
}

/* ------------------------------------------------------------------------ */

const char* knh_realpath(CTX ctx, const char *path, knh_path_t *ph)
{
	char *buf = P_buf(ph);
#if defined(K_USING_WINDOWS)
	char *ptr = _fullpath(buf, path, K_PATHMAX);
#elif defined(K_USING_POSIX_)
	char *ptr = realpath(path, buf);
#else
	char *ptr = NULL;
	KNH_TODO("realpath in your new environment");
#endif
	if(ptr != buf) {
		if(ptr != NULL) free(ptr);
		buf[0] = 0;
		return NULL;
	}
	ph->pbody = 0;
	ph->plen = knh_strlen(buf);
	ph->isRealPath = 1;
	return ptr;
}

const char* knh_ospath(CTX ctx, knh_path_t *ph)
{
	knh_uchar_t *ubuf = P_ubuf(ph);
	DBG_ASSERT(ubuf[ph->plen] == 0);
	if(!ph->isRealPath) {
		int hasUTF8 = 0;
		size_t i;
		for(i = ph->pbody; i < ph->plen; i++) {
			int ch = ubuf[i];
			if(ch == '/' || ch == '\\') {
				ubuf[i] = K_FILESEPARATOR;
			}
			if(ch > 127) hasUTF8 = 1;
		}
		if(hasUTF8) {
			KNH_TODO("multibytes file path");
		}
		{
			knh_cwb_t cwbbuf, *cwb = knh_cwb_copy(ctx, &cwbbuf, ph, 0);
			const char *p = knh_realpath(ctx, knh_cwb_tochar(ctx, cwb), ph);
			knh_cwb_close(cwb);
			return p; // p is NULL if not found;
		}
	}
	return P_text(ph) + ph->pbody;
}

knh_String_t* knh_path_newString(CTX ctx, knh_path_t *ph, int hasScheme)
{
	knh_uchar_t *ubuf = P_ubuf(ph);
	if(ph->isRealPath) {
		int hasUTF8 = 0;
		size_t i;
		for(i = ph->pbody; i < ph->plen; i++) {
			int ch = ubuf[i];
			if(ch == '\\') {
				ubuf[i] = '/';
			}
			if(ch > 127) hasUTF8 = 1;
		}
		if(hasUTF8) {
			KNH_TODO("multibytes file path");
		}
	}
	knh_bytes_t t = {{P_text(ph)}, ph->plen};
	if(!hasScheme) {
		t.text = t.text + ph->pbody;
		t.len = t.len - ph->pbody;
	}
	DBG_P("ph='%s', size=%d", P_text(ph), ph->plen);
	return new_String_(ctx, CLASS_String, t, NULL);
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_path_isfile(CTX ctx, knh_path_t *ph)
{
	knh_bool_t res = 1;
	const char *phname = P_text(ph) + ph->pbody;
	if(phname[0] == 0) return 0;
#if defined(K_USING_WINDOWS)
	DWORD attr = GetFileAttributesA(phname);
	if(attr == -1 || (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) res = 0;
#elif defined(K_USING_POSIX_)
	struct stat buf;
	if(stat(phname, &buf) == -1) res = 0;
	else res = S_ISREG(buf.st_mode);
#else
	FILE* in = fopen(phname,"r");
	if(in == NULL)  res = 0;
	else fclose(in);
#endif
	if(res == 0) {
		DBG_P("isfile='%s' NOTFOUND", P_text(ph));
	}
	return res;
}

knh_bool_t knh_path_isdir(CTX ctx, knh_path_t *ph)
{
	const char *pname = P_text(ph) + ph->pbody;
	if(pname[0] == 0) return 0;
#if defined(K_USING_WINDOWS)
	DWORD attr = GetFileAttributesA(pname);
	if(attr == -1) return 0;
	return ((attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#elif defined(K_USING_POSIX_)
	struct stat buf;
	if(stat(pname, &buf) == -1) return 0;
	return S_ISDIR(buf.st_mode);
#else
	// avoid "unused variable" warning unused variable
	(void)phname;
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_mkdir0(CTX ctx, knh_path_t *ph)
{
	const char *pname = P_text(ph) + ph->pbody;
#if defined(K_USING_WINDOWS)
	CreateDirectory(pname, NULL);
#elif defined(K_USING_POSIX_)
	return (mkdir(pname, 0777) != -1);
#else
	return 0;
#endif
}

knh_bool_t knh_path_mkdir(CTX ctx, knh_path_t *ph)
{
	knh_uchar_t *ubuf = P_ubuf(ph);
	size_t i;
	for(i = ph->pbody; i < ph->plen; i++) {
		int ch = ubuf[i];
		if(ch == K_FILESEPARATOR) {
			int res = 0;
			ubuf[i] = 0;
			if(!knh_path_isdir(ctx, ph)) {
				res = knh_mkdir0(ctx, ph);
			}
			ubuf[i] = ch;
			if(res == -1) return 0;
		}
	}
	return knh_mkdir0(ctx, ph);
}

/* ------------------------------------------------------------------------ */
/* [homepath] */

const char* knh_path_reduce(CTX ctx, knh_path_t *ph, int ch)
{
	knh_uchar_t *ubuf = P_ubuf(ph);
	long i;
	DBG_ASSERT((int)ph->plen > 0);
	if(ph->isRealPath && ch == '/' && ch != K_FILESEPARATOR) {
		ch = K_FILESEPARATOR;
	}
	for(i = ph->plen - 1; i >= ph->pbody; i--) {
		if(ubuf[i] == ch) {
			ph->plen = i;
			ubuf[ph->plen] = 0;
			return (const char*)ubuf + i + 1;
		}
	}
	return NULL;
}

void knh_path_append(CTX ctx, knh_path_t *ph, int issep, const char *name)
{
	char *buf = P_buf(ph), *p = (char*)name;
	size_t i, plen = ph->plen;
	DBG_ASSERT((int)plen > 0);
	if(issep && plen > 0 && (buf[plen-1] != '/' && buf[plen-1] != K_FILESEPARATOR)) {
		buf[plen] = ph->isRealPath ? K_FILESEPARATOR : '/';
		plen++;
		ph->plen = plen;
	}
	for(i = plen; *p != 0; i++) {
		if(!(i + 1< K_PATHMAX)) {
			break;
		}
		buf[i] = *p; p++;
		if(ph->isRealPath && (buf[i] == '\\' || buf[i] == '/')) {
			buf[i] = K_FILESEPARATOR;
		}
	}
	ph->plen = i;
	buf[ph->plen] = 0;
	//DBG_P("APPEND_END=%s + %s (%d)", buf, name, ph->plen);
}

/* Linux, MacOSX */
// $konoha.home.path /usr/local/konoha
// $konoha.bin.path  /usr/local/bin/konoha
// $konoha.package.path {$konoha.home.path}/package
// $konoha.script.path  {$konoha.home.path}/script

#define SETPROP(K, V)  DictMap_set_(ctx, sysprops, new_T(K), UPCAST(V));

void knh_System_initPath(CTX ctx, knh_System_t *o)
{
	knh_DictMap_t *sysprops = DP(o)->props;
	knh_path_t phbuf, *ph = knh_path_open(ctx, NULL, ".", &phbuf);
	knh_bytes_t home = {{NULL}, 0}, user = {{NULL}, 0};

	// current working directory
	knh_ospath(ctx, ph);
	KNH_SETv(ctx, ctx->share->rootns->rpath, knh_path_newString(ctx, ph, 0));

	home.text = (const char*)knh_getenv("KONOHAHOME");
#if defined(K_KONOHAHOME)
	if(home.text == NULL) {
		home = STEXT(K_KONOHAHOME);
	}
#endif
	if(home.text != NULL) {
		home.len = knh_strlen(home.text);
		SETPROP("konoha.home.path", new_T(home.text));
	}
#if defined(K_USING_WINDOWS)
	{
		char buf[FILEPATH_BUFSIZ];
		int bufsiz = FILEPATH_BUFSIZ;
		HMODULE h = LoadLibrary(NULL);
		GetModuleFileNameA(h, buf, bufsiz);
		ph = knh_path_open_(ctx, NULL, B(buf), &phbuf);
		SETPROP("konoha.bin.path", knh_path_newString(ctx, ph, 0));
		if(homepath == NULL) {
			knh_String_t *s;
			GetModuleFileNameA(h, buf, bufsiz);
			knh_path_reduce(ctx, ph, '/');
			s = knh_path_newString(ctx, ph);
			SETPROP("konoha.home.path", UPCAST(s));
			home = S_tobytes(shome);
		}
	}
#elif defined(K_USING_LINUX_)
	// @url(http://shinh.skr.jp/binary/b2con.html)
	// http://doc.trolltech.com/3.3/qapplication.html#applicationDirPath
	{
		char buf[FILEPATH_BUFSIZ];
		int bufsiz = FILEPATH_BUFSIZ;
		size_t size = readlink("/proc/self/exe", buf, bufsiz);
		ph = knh_path_open_(ctx, NULL, new_bytes2(buf, size), &phbuf);
		SETPROP("konoha.bin.path", knh_path_newString(ctx, ph, 0));
		if(home.text == NULL) {
			knh_String_t *s;
			knh_path_reduce(ctx, ph, '/');
			knh_path_reduce(ctx, ph, '/');
			knh_path_append(ctx, ph, 1/*isSep*/, "konoha");
			s = knh_path_newString(ctx, ph, 0/*hasScheme*/);
			SETPROP("konoha.home.path", UPCAST(s));
			home = S_tobytes(s);
		}
	}
#elif defined(K_USING_MACOSX_)
	ph = knh_path_open(ctx, NULL, _dyld_get_image_name(0), &phbuf);
	knh_ospath(ctx, ph);
	SETPROP("konoha.bin.path", knh_path_newString(ctx, ph, 0/*hasScheme*/));
	if(home.text == NULL) {
		knh_String_t *s;
		knh_path_reduce(ctx, ph, '/');
		knh_path_reduce(ctx, ph, '/');
		knh_path_append(ctx, ph, 1/*isSep*/, "konoha");
		s = knh_path_newString(ctx, ph, 0/*hasScheme*/);
		SETPROP("konoha.home.path", UPCAST(s));
		home = S_tobytes(s);
	}
#else
	home = STEXT("/opt/konoha");
	SETPROP("konoha.home.path", new_T("/opt/konoha"));
#endif
	DBG_ASSERT(home.utext != NULL);

	/* $konoha.package.path {$konoha.home.path}/package */
	knh_path_reset(ctx, ph, NULL, home);
	knh_path_append(ctx, ph, 1/*sep*/, "package");
	knh_path_append(ctx, ph, 1/*sep*/, LIBK_VERSION);
	SETPROP("konoha.package.path", knh_path_newString(ctx, ph, 0/*hasScheme*/));

	/* $konoha.script.path {$konoha.home.path}/script */
	knh_path_reset(ctx, ph, NULL, home);
	knh_path_append(ctx, ph, 1/*sep*/, "script");
	knh_path_append(ctx, ph, 1/*sep*/, LIBK_VERSION);
	SETPROP("konoha.script.path", knh_path_newString(ctx, ph, 0/*hasScheme*/));

#if defined(K_USING_WINDOWS)
	user.text = knh_getenv("USERPROFILE");
#else
	user.text = knh_getenv("HOME");
#endif
	if(user.text != NULL) {
		/* $user.path */
		user.len = knh_strlen(user.text);
		knh_path_reset(ctx, ph, NULL, user);
		knh_path_append(ctx, ph, 1/*sep*/, K_KONOHAFOLDER);
		SETPROP("user.path", knh_path_newString(ctx, ph, 0));
		knh_ospath(ctx, ph);
		knh_path_mkdir(ctx, ph);

		knh_path_reset(ctx, ph, NULL, user); /* user.package.path */
		knh_path_append(ctx, ph, 1/*sep*/, K_KONOHAFOLDER);
		knh_path_append(ctx, ph, 1/*sep*/, "package");
		knh_path_append(ctx, ph, 1/*sep*/, LIBK_VERSION);
		SETPROP("user.package.path", knh_path_newString(ctx, ph, 0));
	}
	knh_path_close(ctx, ph);
}

/* ------------------------------------------------------------------------ */
/* [dlopen] */

void *knh_dlopen(CTX ctx, const char* path)
{
	const char *func = __FUNCTION__;
	void *handler = NULL;
#if defined(K_USING_WINDOWS)
	func = "LoadLibrary";
	handler = (void*)LoadLibraryA((LPCTSTR)path);
#elif defined(K_USING_POSIX_)
	func = "dlopen";
	handler = dlopen(path, RTLD_LAZY);
#else

#endif
	if(handler == NULL) {
		KNH_WARN(ctx, "%s cannot open path='%s'", func, path);
	}
	else {
		KNH_INFO(ctx, "%s opened path='%s' HANDLER=%p", func, path, handler);
	}
	return handler;
}

void *knh_path_dlopen(CTX ctx, knh_path_t *ph)
{
	knh_bytes_t t = {{P_text(ph)}, ph->plen};
	if(!knh_bytes_endsWith(t, STEXT(K_OSDLLEXT))) {
		knh_path_append(ctx, ph, 0/*sep*/, K_OSDLLEXT);
	}
	return knh_dlopen(ctx, P_text(ph) + ph->pbody);
}

void *knh_dlsym(CTX ctx, int pe, void* handler, const char* symbol)
{
#if defined(K_USING_WINDOWS)
	void *p = GetProcAddress((HMODULE)handler, (LPCSTR)symbol);
	if(p == NULL) {
		KNH_SYSLOG(ctx, NULL, pe, "GetProcAddress", "func='%s', ERR='NotFound'", symbol);
	}
	return p;
#elif defined(K_USING_POSIX_)
	void *p = dlsym(handler, symbol);
	//DBG_P("handler=%p,%p, symbol='%s' ERR='%s'", handler, p, symbol, dlerror());
	if(p == NULL) {
		KNH_SYSLOG(ctx, NULL, pe, "dlsym", "symbol='%s', ERR='%s'", symbol, dlerror());
	}
	return p;
#else
#endif
	return NULL;
}

int knh_dlclose(CTX ctx, void* hdr)
{
#if defined(K_USING_WINDOWS)
    return (int)FreeLibrary((HMODULE)hdr);
#elif defined(K_USING_POSIX_)
    return dlclose(hdr);
#elif defined(K_USING_BTRON)
    return b_dlclose((W)hdr);
#else
    return 0;
#endif
}

/* ------------------------------------------------------------------------ */
/* [charset] */

#if defined(K_USING_WINDOWS)
#define HAVE_LOCALCHARSET_H 1
static char *locale_charset(void)
{
	static char codepage[64];
	knh_snprintf(codepage, sizeof(codepage), "CP%d", (int)GetACP());
	return codepage;
}
#endif

/* ------------------------------------------------------------------------ */

const char *knh_getSystemEncoding(void)
{
	const char *enc = knh_getenv("KONOHAENC");
	if(enc != NULL) {
		return enc;
	}
#if defined(K_OSENCODING)
	return K_OSENCODING;
#elif defined(HAVE_LOCALCHARSET_H)
	return (char*)locale_charset();
#else
//	char *enc = knh_getenv("LC_CTYPE");
//	if(enc != NULL) {
//		return (char*)enc;
//	}
	return K_ENCODING;
#endif
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
