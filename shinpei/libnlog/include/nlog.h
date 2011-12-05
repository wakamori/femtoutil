/*****************************
 *
 * libnlog 
 * shinpei nakata(c)2011
 *
 *****************************/

#ifndef __NLOG_H__ 
#define __NLOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#if ! defined(RTLD_NEXT)
#  define RTLD_NEXT ((void *) -1L)
#endif


#include <malloc.h>

static void *(*malloc_orig) (size_t size, const void *) = NULL;
void *nlog_malloc (size_t size, const void *caller);

typedef const char * nlog_t;

#define LOG_END     ((const char*)0L)
#define LOGT_s      1
#define LOG_s(key,value)  ((const char*)LOGT_s), key, value
#define LOGT_i      2
#define LOG_i(key, value) ((const char*)LOGT_i), key, ((const char*)(intptr_t)(value))
#define LOGT_p      3
#define LOG_p(key, value) ((const char*)LOGT_p), key, ((const char*)(value))

typedef struct nlog2_t {
  int type;
  const char *key;
  const char *value;
} nlog2_t;


#endif
