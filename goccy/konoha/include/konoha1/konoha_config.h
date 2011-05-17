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

#ifndef KONOHA_CONFIG_H_
#define KONOHA_CONFIG_H_

/* ------------------------------------------------------------------------ */
/* You are aggreed to use konoha under the following License */

#define K_UNDER_GPL      3
#define K_PREVIEW         1   // when released, comment this out

#ifdef K_UNDER_GPL
#define K_LICENSE         "GPL3.0"
#else
#define K_LICENSE         "Konoha NDA"
#endif

/* ------------------------------------------------------------------------ */
/* configuration */

/* language configuration */

//#define K_USING_FFIDSL          1
//#define K_USING_INT32         1
#define K_USING_FASTMALLOC      1
//#define K_USING_RCGC          1
//#define K_USING_SQLITE3       1

//#ifdef K_USING_DEBUG  /* for debug*/
//#define K_USING_RCGC   1
//#endif

/* optimization */
//#define K_USING_FASTEST        1
#define K_USING_PREFETCH       1
#define K_USING_UTF8           1

// functionality
#define K_USING_SECURITY_ALERT  1

/* statistics */
#define K_USING_STAT           1
//#define K_USING_DEBUG        1
//#define K_USING_VMCOUNT      1

#define K_DEOS_TRACE           "DEOS_TRACE"

/* ------------------------------------------------------------------------ */
/* Manifesto */

#define K_PROGNAME      "konoha"
#define K_MAJOR_VERSION  1
#define K_MINOR_VERSION  0
#define K_PATCH_VERSION  0
#define K_VERSION       "1.0"

#define LIBK_VERSION     "1.0"
#define K_CODENAME       "alpha2"
#define K_URL            "http://code.google.com/p/konoha"

#ifdef K_PREVIEW
#undef K_DISTTYPE
#define K_DISTTYPE "svn"
#define K_DISTURL  "http://konoha.googlecode.com/svn/trunk"
#else
#ifndef K_DISTTYPE
#define K_DISTTYPE  "source"
#define K_DISTURL  "http://konoha.sourceforge.jp/"
#endif
#endif

typedef struct knh_sysinfo_t {
	const char *hw_machine; /* e.g., i386 */
	const char *hw_model;   /* e.g., MacBookAir2,1 */
	size_t hw_ncpu;
	size_t hw_availcpu;
	size_t hw_physmem;
	size_t hw_usermem;
	size_t hw_cpufrequency;
	size_t hw_l2cachesize;
	const char *cpu_desc; /* Intel(R) Core(TM)2 Duo CPU     L9600  @ 2.13GHz */
	const char *kern_ostype;    /* e.g., "Darwin" */
	const char *kern_version;   /* e.g., "10.5.82 */
	const char *kern_desc;
	/* konoha original information */
	const char *konoha_type;
	const char *konoha_codename;
	const char *konoha_version;
	const char *konoha_cc;
	const char *konoha_config;
	const char *konoha_builtdate;
	size_t konoha_major_version;
	size_t konoha_minor_version;
	size_t konoha_patch_version;
	size_t konoha_patch_level;
	size_t konoha_systembits;          /* 32 or 64 */
	const char *konoha_disttype;
	const char *konoha_disturl;
	const char *konoha_options;
} knh_sysinfo_t ;

/* ------------------------------------------------------------------------ */
/* COMMON */

#if defined(__LP64__) || defined(__LLP64__) || defined(_WIN64)
#define K_USING_64_                   1
#define K_PAGESIZE                    4096 /*8192*/
#else
#define K_PAGESIZE                    4096
#endif

#define K_MTDCACHE_SIZE     337
#define K_TMAPCACHE_SIZE     91

#define K_FIELDSIZE                    64
#define K_STACKSIZE                  1024

#ifdef K_USING_UTF8
#define K_ENCODING                    "UTF-8"
#define K_USING_I18N                  1
#else
#define K_ENCODING                    "ASCII"
#endif

/* ------------------------------------------------------------------------ */
/* Compiler Check */
/* If you try gcc on new environments, check 'cpp -dM /dev/null' first. */

#ifdef __GNUC__
#ifdef __llvm__ /* llvm-gcc */
#define		CC_TYPE "LLVM-GCC " __VERSION__
#else /* gnu gcc */
#define		CC_TYPE "GCC " __VERSION__
#define		CC_LABEL_AS_VALUE   1
#endif
//In most case, it makes slower a little
//#define		likely(x)       __builtin_expect(!!(x), 1)
//#define		unlikely(x)     __builtin_expect(!!(x), 0)
#define		CC_UNUSED           __attribute__((unused))
#ifdef 		K_USING_PREFETCH
#define			prefetch(...)   __builtin_prefetch(__VA_ARGS__)
#endif

#if defined(__i386__) || (defined(__x86_64__) && !defined(__amd64__))
#undef		fastcall
#define		fastcall __attribute__((fastcall))
#define		CC_FASTCALL_   fastcall
#else
#define		CC_FASTCALL_
#endif
	#ifdef __linux__
		#define CC_TYPE_TLS  __thread
	#endif
#endif/*__GNUC__*/

#ifdef _MSC_VER
#define 	CC_FASTCALL_  /* __fastcall */
#if _MSC_VER == 1500
#define CC_TYPE "Microsoft Visual C++ 9.0 (1500)"
#else
#define CC_TYPE "Microsoft Visual C++"
#endif
#define		CC_EXPORT __declspec(dllexport)
#define		CC_IMPORT __declspec(dllimport)
#define 	EXPORTAPI(T__)   CC_EXPORT T__ __cdecl

#define		CC_TYPE_TLS   __declspec( thread )
#define		_CRT_SECURE_NO_WARNINGS

#ifdef defined(_WIN64)
#define CC_PLATFORM "windows-x86_64"
#else
#define CC_PLATFORM "windows-i386"
#endif

#endif/* _MSC_VER */

#ifndef CC_TYPE
#define CC_TYPE "UNKNOWN CC (PLEASE ADD konoha_config.h)"
#endif

#if defined(CC_LABEL_AS_VALUE)
#define K_USING_THCODE_ 1
#else
#undef K_USING_THCODE_
#endif

#ifndef likely
#define likely(x)	   (x)
#define unlikely(x)	   (x)
#endif

#ifndef CC_PLATFORM
#ifdef K_CONFIG_OS
#define CC_PLATFORM K_CONFIG_OS "-" K_CONFIG_CPU
#else
#define CC_PLATFORM "SET ${host_os}-${host_cpu} BY YOURSELF"
#endif
#endif

#ifndef prefetch
#undef K_USING_PREFETCH
#define prefetch(...)
#endif

#ifndef CC_UNUSED
#define CC_UNUSED
#endif

#ifndef CC_EXPORT
#define CC_EXPORT
#endif

#ifndef KNHAPI2
#define KNHAPI2(T)        T
#define EXPORTAPI(T)      T
#endif

/* ------------------------------------------------------------------------ */
/* [API] */

/* officially supported */

#if defined(__linux__)
#define		K_USING_STDC_    1
#define 	K_USING_POSIX_   1
#define 	K_USING_LINUX_   1
#define     K_USING_SYSLOG   1
#define     K_USING_ICONV    1
#define     K_USING_SQLITE3  1
#define     K_USING_PCRE     1
#endif

#if defined(__APPLE__) /* MacOS X */
#define		K_USING_STDC_    1
#define 	K_USING_POSIX_      1
#define 	K_USING_BSD_        1
#define 	K_USING_MACOSX_     1
#define		K_OSDLLEXT ".dylib"
#define     K_USING_BSDSYSCTL_  1
#define     K_USING_SYSLOG      1
#define     K_USING_ICONV       1
#define     K_USING_SQLITE3  1
#define     K_USING_PCRE     1
#endif

#if defined(__WIN32__)  /* mingw */
#define		K_USING_STDC_       1
#define		K_USING_WIN32_      1
#define     K_USING_ICONV       1
#define     K_USING_SYSLOG       1   // if not available, report to konohaken
#define     K_USING_SQLITE3  1
#define     K_USING_PCRE     1
#if defined(K_USING_THREAD)
#define		K_USING_WINTHREAD_   1
#endif
#define 	K_OSLINEFEED "\r\n"
#define 	K_OSDLLEXT ".dll"
#define		K_FILESEPARATOR '\\'
#define     K_KONOHAFOLDER "Konoha"

#endif

#if defined(_MSC_VER)
#define		K_USING_STDC_        1
#define		K_USING_WIN32_       1
#define     K_USING_ICONV        1
#define     K_USING_SYSLOG       1   // if not available, report to konohaken
#define     K_USING_SQLITE3  1
#define     K_USING_PCRE     1
#if defined(K_USING_THREAD)
#define		K_USING_WINTHREAD_   1
#endif
#if defined(_WIN64)
#define		K_USING_WIN64_   1
#endif

#define 	K_OSLINEFEED "\r\n"
#define 	K_OSDLLEXT ".dll"
#define		K_FILESEPARATOR '\\'
#define     K_KONOHAFOLDER "Konoha"
#endif/* _MSC_VER*/

/* experimetally supported */

#if defined(__FreeBSD__) /* FreeBSD */
#define		K_USING_STDC_       1
#define 	K_USING_POSIX_      1
#define 	K_USING_BSD_        1
#define     K_USING_BSDSYSCTL_  1
#define     K_USING_ICONV       1
#define     K_USING_SYSLOG   1
#endif

#if defined(__OpenBSD__) /* FreeBSD */
#define		K_USING_STDC_       1
#define 	K_USING_POSIX_      1
#define 	K_USING_BSD_        1
#define     K_USING_BSDSYSCTL_  1
#define     K_USING_ICONV       1
#define     K_USING_SYSLOG   1
#endif

#if defined(__CYGWIN__)
#define		K_USING_STDC_        1
#define		K_USING_POSIX_       1
#define		K_USING_CYGWIN_      1
#define     K_USING_SYSLOG       1
#endif

#if defined(TBMX1)
#define		K_USING_STDC_        1
#define		K_USING_TKERNEL_     1
#define		K_USING_TSHELL_      1
#endif


/* THIS_MUST_BE_LAST_ONE */

#if !defined(K_USING_STDC_) && (defined(__unix__) || defined(__unix) || defined(unix))
#include <sys/param.h>
#define		K_USING_STDC_		1
#define		K_USING_POSIX_		1
#endif

#if !defined(K_USING_STDC_)
#define		K_USING_STDC_        1
#define		K_USING_UNKNOWNOS_   1
#endif


#ifndef K_OSENCODING
#define K_OSENCODING "UTF-8"
#endif

#ifndef K_OSLINEFEED
#define K_OSLINEFEED "\n"
#endif

#ifndef K_OSDLLEXT
#define K_OSDLLEXT ".so"
#endif

#ifndef K_FILESEPARATOR
#define K_FILESEPARATOR '/'
#endif

#ifndef K_KONOHAFOLDER
#define     K_KONOHAFOLDER ".konoha"
#endif

/* ------------------------------------------------------------------------ */
/* memory */

#if !defined(L1_CACHE_BYTES)
#define L1_CACHE_BYTES   32
#endif

/* ------------------------------------------------------------------------ */
/* api */

#ifdef _MSC_VER
#define knh_snprintf(b,n,fmt, ...)     _snprintf_s(b,n, _TRUNCATE, fmt, ## __VA_ARGS__)
#else
#define knh_snprintf(b,n,fmt, ...)     snprintf(b,n,fmt, ## __VA_ARGS__)
#endif

#define knh_strlen(c)                  strlen(c)
#define knh_memcpy(d, s, n)            memcpy(d,s,n)
#define knh_memmove(d, s, n)           memmove(d, s, n)
#define knh_bzero(s, n)                memset(s,0,n)
#define knh_strcmp(s1, s2)             strcmp(s1,s2)
#define knh_strncmp(s1, s2, n)         strncmp((const char*)(s1),(const char*)(s2),n)

#ifndef _MSC_VER
#define knh_strncasecmp(s1, s2, n)     strncasecmp((const char*)(s1),(const char*)(s2),n)
#else
#define knh_strncasecmp(s1, s2, n)     _strnicmp((const char*)(s1),(const char*)(s2),n)
#endif

#define knh_getenv(n)                  getenv(n)
#define  knh_qsort(b,s,w,f)            qsort(b,s,w,f)

//void knh_qsort_r (void *const pbase, size_t total_elems, size_t size,
//        void* thunk,int (*cmp)(void* ,const void*,const void*));

/* ------------------------------------------------------------------------ */

#endif /*KONOHA_CONFIG_H_*/
