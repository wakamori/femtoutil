include(CheckTypeSize)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckLibraryExists)
include(CheckCSourceCompiles)

cmake_minimum_required(VERSION 2.8)
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_INSTALL_PREFIX=$HOME
# -DCMAKE_VERBOSE_MAKEFILE=1

set(CMAKE_C_FLAGS_RELEASE "-Wall -O2 -g")
set(CMAKE_C_FLAGS_DEBUG   "-Wall -O0 -g3")
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

#if("${CMAKE_C_COMPILER}" STREQUAL "/usr/bin/gcc")
#	set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
#endif()

project(konoha)
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
set(KONOHA_VERSION  ${VERSION_MAJOR}.${VERSION_MINOR})

set(K_CONFIG_ARCH ${CMAKE_SYSTEM})
set(K_CONFIG_OS   ${CMAKE_SYSTEM_NAME})
set(K_CONFIG_CPU  ${CMAKE_SYSTEM_PROCESSOR})

set(SO_VERSION   ${VERSION_MAJOR}.${VERSION_MINOR})
set(ARCHIVE_NAME ${PACKAGE_STRING})

set(KONOHA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		${KONOHA_INCLUDE_DIRS}  ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${INCLUDE_DIRS})

add_definitions(-DHAVE_CONFIG_H)

set(KONOHA_SRC
	src/lang/typing.c
	src/lang/term.c
	src/lang/asm.c
	src/lang/script.c
	src/lang/vmcodelibs.c
	src/lang/message.c
	src/main/array.c
	src/main/bytes.c
	src/main/class.c
	src/main/closure.c
	src/main/context.c
	src/main/evidence.c
	src/main/exports.c
	src/main/map.c
	src/main/memory.c
	src/main/number.c
	src/main/os.c
	src/main/query.c
	src/main/runtime.c
	src/main/security.c
	src/main/semantics.c
	src/main/ffi.c
	src/main/stack.c
	src/main/stream.c
	src/main/string.c
	src/main/struct.c
	src/main/system.c
	src/main/thread.c
	src/ext/mt19937ar.c
)

check_include_files(malloc.h HAVE_MALLOC_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(float.h HAVE_FLOAT_H)
check_include_files(langinfo.h HAVE_LANGINFO_H)
check_include_files(localcharset.h HAVE_LOCALCHARSET_H)
check_include_files(limits.h HAVE_LIMITS_H)
check_include_files(locale.h HAVE_LOCALE_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files("sys/time.h" HAVE_SYS_TIME_H)
check_include_files("sys/param.h" HAVE_SYS_PARAM_H)
check_include_files("sys/stat.h" HAVE_SYS_STAT_H)
check_include_files("sys/sysctl.h" HAVE_SYS_SYSCTL_H)
check_include_files("sys/types.h" HAVE_SYS_SYSCTL_H)

find_path(HAVE_PCRE_H  pcre.h  PATHS ${include_locations})
find_path(HAVE_ICONV_H iconv.h PATHS ${include_locations})
find_path(HAVE_SYS_MMAM_H mman.h PATHS ${include_locations} PATH_SUFFIXES sys)
find_path(HAVE_SYS_STAT_H stat.h PATHS ${include_locations} PATH_SUFFIXES sys)
find_path(HAVE_UNISTD_H unistd.h ${include_locations})

find_library(HAVE_LIBDL      NAMES dl)
find_library(HAVE_LIBM       NAMES m)
find_library(HAVE_LIBICONV   NAMES iconv)
find_library(HAVE_PTHREAD    NAMES pthread)
find_library(HAVE_LIBSQLITE3 NAMES sqlite3)
find_library(HAVE_LIBPCRE    NAMES pcre)

check_type_size("void *" SIZEOF_VOIDP)
check_type_size(long     SIZEOF_LONG)
check_type_size(int      SIZEOF_INT)
check_type_size(float    SIZEOF_FLOAT)
check_type_size(double   SIZEOF_DOUBLE)

set(_HEADERS stdio.h stdlib.h string.h ctype.h)
if(UNIX)
	set(_HEADERS ${_HEADERS} sys/mman.h unistd.h)
endif(UNIX)

if(APPLE)
	set(_HEADERS ${_HEADERS} mach-o/dyld.h)
endif(APPLE)

if(CYGWIN)
endif(CYGWIN)

if(MSVC)
endif(MSVC)

check_symbol_exists(malloc         "${_HEADERS}" HAVE_MALLOC)
check_symbol_exists(isascii        "${_HEADERS}" HAVE_ISASCII)
check_symbol_exists(memset         "${_HEADERS}" HAVE_MEMSET)
check_symbol_exists(realpath       "${_HEADERS}" HAVE_REALPATH)
check_symbol_exists(qsort_r        "${_HEADERS}" HAVE_QSORT_R)
check_symbol_exists(posix_memalign "${_HEADERS}" HAVE_POSIX_MEMALIGN)
check_symbol_exists(mmap           "${_HEADERS}" HAVE_MMAP)
check_symbol_exists(sysctlbyname   "${_HEADERS}" HAVE_SYSCTLBYNAME)
check_symbol_exists(mlock          "${_HEADERS}" HAVE_MLOCK)
check_symbol_exists(snprintf       "${_HEADERS}" HAVE_SNPRINTF)

#set(CMAKE_REQUIRED_LIBRARIES HAVE_LIBDL ${HAVE_LIBDL})
macro(CHECK_PROTOTYPE _SYMBOL _HEADER _RESULT)
	set(_INCLUDE_FILE)
	foreach(it ${_HEADER})
		set(_INCLUDE_FILE "${_INCLUDE_FILE}#include <${it}>\n")
	endforeach(it)
	SET(_CHECK_SOURCE_CODE "
${_INCLUDE_FILE}
void cmakeRequireSymbol(int dummy,...){(void)dummy;}
int main()
{
#ifndef ${_SYMBOL}
#ifndef _MSC_VER
  cmakeRequireSymbol(0,&${_SYMBOL});
#else
  char i = sizeof(&${_SYMBOL});
#endif
#endif
	return 0;
}
")
	check_c_source_compiles("${_CHECK_SOURCE_CODE}" ${_RESULT})
endmacro(CHECK_PROTOTYPE)


set(ExtraLibs)
set(MessageLibs)

set(_HEADERS_SAVE ${_HEADERS})

if(HAVE_LIBSQLITE3)
	set(ExtraLibs ${ExtraLibs} ${HAVE_LIBSQLITE3})
	set(MessageLibs ${MessageLibs} sqlite3)
endif(HAVE_LIBSQLITE3)

if(HAVE_LIBDL)
	set(ExtraLibs ${ExtraLibs} ${HAVE_LIBDL})
	set(MessageLibs ${MessageLibs} dl)
endif()

if(HAVE_LIBICONV)
	set(ExtraLibs ${ExtraLibs} ${HAVE_LIBICONV})
	set(MessageLibs ${MessageLibs} iconv)
	if(HAVE_ICONV_H)
		check_prototype(libiconv_open iconv.h HAVE_ICONV_LIBICONV_OPEN)
		check_prototype(iconv_open    iconv.h HAVE_ICONV_ICONV_OPEN)
		check_library_exists(iconv libiconv_open HAVE_LIBICONV
				HAVE_ICONV_LIBICONV_OPEN_SYM)
		check_library_exists(iconv iconv_open HAVE_LIBICONV
				HAVE_ICONV_ICONV_OPEN_SYM)
#		set(_HEADERS ${_HEADERS} HAVE_ICONV_H)
#		check_symbol_exists(libiconv_open "${_HEADERS}"
#				HAVE_ICONV_LIBICONV_OPEN_SYM)
#		set(_HEADERS ${_HEADERS_SAVE})
	endif()
endif(HAVE_LIBICONV)

if(HAVE_PTHREAD)
	set(ExtraLibs ${ExtraLibs} ${HAVE_PTHREAD})
	set(MessageLibs ${MessageLibs} pthread)
endif(HAVE_PTHREAD)

if(HAVE_LIBPCRE)
	if(HAVE_PCRE_H)
		set(ExtraLibs ${ExtraLibs} ${HAVE_LIBPCRE})
		set(MessageLibs ${MessageLibs} pcre)
	endif(HAVE_PCRE_H)
endif(HAVE_LIBPCRE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(K_USING_DEBUG 1)
endif()

find_package(Subversion)
if(Subversion_FOUND)
	Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR}/include ER)
	set(SUBVERSION_REVISION ${ER_WC_REVISION})
	add_definitions(-DK_REVISION=${SUBVERSION_REVISION})
endif(Subversion_FOUND)

if(HAVE_ICONV_ICONV_OPEN_SYM)
	add_definitions(-DLIBICONV_PLUG)
endif()

option(USE_LLVM "Use llvm" OFF)
if(USE_LLVM)
	find_program(LLVM_CONFIG NAMES llvm-config DOC "llvm-config")
	execute_process(
			COMMAND ${LLVM_CONFIG} --cppflags
			OUTPUT_VARIABLE LLVM_CFLAGS
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)
	execute_process(
			COMMAND ${LLVM_CONFIG} --cxxflags
			OUTPUT_VARIABLE LLVM_CXXFLAGS
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)
	execute_process(
			COMMAND ${LLVM_CONFIG} --ldflags
			OUTPUT_VARIABLE LLVM_LDFLAGS
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)
	execute_process(
		COMMAND ${LLVM_CONFIG} --libs core jit native linker ipo
		OUTPUT_VARIABLE LLVM_JIT_LIBS
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${LLVM_CFLAGS})
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS})
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_LD_FLAGS} "${LLVM_LDFLAGS} ${LLVM_JIT_LIBS}")
	set(KONOHA_SRC ${KONOHA_SRC} src/lang/llvm.cpp)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake 
		${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_library(konoha SHARED ${KONOHA_SRC})
target_link_libraries(konoha ${ExtraLibs})
set_target_properties(konoha PROPERTIES VERSION ${PACKAGE_VERSION} SOVERSION ${SO_VERSION})

add_executable(konoha1 src/konoha.c)
target_link_libraries(konoha1 konoha)
set_target_properties(konoha1 PROPERTIES OUTPUT_NAME konoha)

file(GLOB dirs "${CMAKE_CURRENT_SOURCE_DIR}/package/konoha.*")
foreach(dir ${dirs})
	add_subdirectory(${dir})
endforeach()
add_subdirectory(dist/script)


install(TARGETS konoha1 konoha 
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		)

install(DIRECTORY include/ DESTINATION include 
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN ".svn" EXCLUDE
		)


# test
enable_testing()
set(ktest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/konoha)
set(ac_dir ${CMAKE_CURRENT_SOURCE_DIR}/ac)
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/ac/*.k")
foreach(f ${files})
	string(REGEX REPLACE ".*/([^ ]+).k" "\\1" res "${f}" )
	add_test(NAME ${res} COMMAND ${ktest} --utest "file:${f}")
endforeach()

#set(CPACK_SOURCE_GENERATOR "TGZ;DEB")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${ARCHIVE_NAME})
set(CPACK_PACKAGE_NAME "Konoha")
set(CPACK_PACKAGE_VENDOR "Konoha")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Konoha - Statically-typed scripting language")
set(CPACK_PACKAGE_VERSION       "konoha-${VERSION_MAJOR}.${VERSION_MINOR}-alpha1")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
include(CPack)

MESSAGE(STATUS)
MESSAGE(STATUS "---------------------------------------------------------------------------" )
MESSAGE(STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Change a value with: cmake -D<Variable>=<Value>" )
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if(NOT HAVE_PCRE_H)
			MESSAGE(STATUS "pcre.h is not found")
	endif(NOT HAVE_PCRE_H)
	MESSAGE(STATUS "iconv.h=${HAVE_ICONV_H}")
	MESSAGE(STATUS "iconv.dylib=${HAVE_LIBICONV}")
endif()

MESSAGE(STATUS)
foreach(lib ${MessageLibs})
	MESSAGE(STATUS "using ${lib}")
endforeach()
MESSAGE(STATUS "---------------------------------------------------------------------------" )
MESSAGE(STATUS)

