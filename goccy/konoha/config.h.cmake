/* Define to 1 if you have the <dlfcn.h> header file. */
#cmakedefine HAVE_DLFCN_H 1

/* Define to 1 if you have the <float.h> header file. */
#cmakedefine HAVE_FLOAT_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isascii' function. */
#cmakedefine HAVE_ISASCII 1

/* Define to 1 if you have the <langinfo.h> header file. */
#cmakedefine HAVE_LANGINFO_H 1

/* Define to 1 if you have the `dl' library (-ldl). */
#cmakedefine HAVE_LIBDL 1

/* Define to 1 if you have the `m' library (-lm). */
#cmakedefine HAVE_LIBM 1

/* Define to 1 if you have the `sqlite3' library (-lsqlite3). */
#cmakedefine HAVE_LIBSQLITE3 1

/* Define to 1 if you have the `iconv' library (-liconv). */
#cmakedefine HAVE_LIBICONV 1

/* Define to 1 if you have the `pthread' library (-lptread). */
#cmakedefine HAVE_PTHREAD 1

/* Define to 1 if you have the <limits.h> header file. */
#cmakedefine HAVE_LIMITS_H 1

/* Define to 1 if you have the <localcharset.h> header file. */
#cmakedefine HAVE_LOCALCHARSET_H 1

/* Define to 1 if you have the <locale.h> header file. */
#cmakedefine HAVE_LOCALE_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function. */
#cmakedefine HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#cmakedefine HAVE_MEMSET 1

/* Define to 1 if you have the `mlock' function. */
#cmakedefine HAVE_MLOCK 1

/* Define to 1 if you have the `mmap' function. */
#cmakedefine HAVE_MMAP 1

/* Define to 1 if you have the `posix_memalign' function. */
#cmakedefine HAVE_POSIX_MEMALIGN 1

/* Define to 1 if you have the `qsort_r' function. */
#cmakedefine HAVE_QSORT_R 1

/* Define to 1 if you have the `realpath' function. */
#cmakedefine HAVE_REALPATH 1

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H 1

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine HAVE_STRINGS_H 1

/* Define to 1 if you have the `sysctlbyname' function. */
#cmakedefine HAVE_SYSCTLBYNAME 1

/* Define to 1 if you have the <sys/mmam.h> header file. */
#cmakedefine HAVE_SYS_MMAM_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#cmakedefine HAVE_SYS_SYSCTL_H

/* Define to 1 if you have the <sys/time.h> header file. */
#cmakedefine HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H

/* Define to 1 if you have the <pcre.h> header file. */
#cmakedefine HAVE_PCRE_H 1

/* Define to 1 if you have the `libiconv_open' function. */
#cmakedefine HAVE_ICONV_LIBICONV_OPEN 1

/* Define to 1 if you have the `iconv_open' function. */
#cmakedefine HAVE_ICONV_ICONV_OPEN    1

/* Define to 1 if you have the `libiconv_open' function in libiconv. */
#cmakedefine HAVE_ICONV_LIBICONV_OPEN_SYM   1

/* Define to 1 if you have the `iconv_open' function in libiconv. */
#cmakedefine HAVE_ICONV_ICONV_OPEN_SYM   1

/* arch type */
#cmakedefine K_CONFIG_ARCH "${K_CONFIG_ARCH}"

/* Konoha cpu type */
#cmakedefine K_CONFIG_CPU "${K_CONFIG_CPU}"

/* Konoha os type */
#cmakedefine K_CONFIG_OS "${K_CONFIG_OS}"

/* Difining konoha library prefix with default value */
#cmakedefine K_PREFIX "${CMAKE_INSTALL_PREFIX}"

/* Enable debug mode */
#cmakedefine K_USING_DEBUG ${K_USING_DEBUG}

/* Enable int32 */
#undef K_USING_INT32

/* Enable mark & sweep mode */
#undef K_USING_MSGC

/* Using pthread detected */
#undef K_USING_PTHREAD

/* Enable referencing counting mode */
#undef K_USING_RCGC

/* Using sqlite3 */
#undef K_USING_SQLITE3

/* Using thread */
#undef K_USING_THREAD

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#cmakedefine PACKAGE_NAME "${PACKAGE_NAME}"

/* Define to the full name and version of this package. */
#cmakedefine PACKAGE_STRING "${PACKAGE_STRING}"

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#cmakedefine PACKAGE_VERSION "${PACKAGE_VERSION}"

/* The size of `int', as computed by sizeof. */
#cmakedefine SIZEOF_INT ${SIZEOF_INT}

/* The size of `long', as computed by sizeof. */
#cmakedefine SIZEOF_LONG ${SIZEOF_LONG}

/* The size of `void*', as computed by sizeof. */
#cmakedefine SIZEOF_VOIDP ${SIZEOF_VOIDP}

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Define to empty if `const' does not conform to ANSI C. */
#undef const
