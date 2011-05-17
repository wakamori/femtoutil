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

#ifdef K_USING_PTHREAD
#include<pthread.h>
#endif

#ifdef K_USING_BTRON
#include<btron/proctask.h>
#include<btron/taskcomm.h>
#endif

#ifdef K_USED_TO_BE_ON_LKM
#include <linux/mutex.h>
#endif

#ifdef K_USING_WINTHREAD_
#include <process.h>
#endif

#ifdef K_USING_MINGW
#undef K_USING_WINTHREAD_
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [TLS] */

knh_thread_t knh_thread_self(void)
{
#if defined(K_USING_PTHREAD)
	return (knh_thread_t)pthread_self();
#elif defined(K_USING_BTRON)
	return b_get_tid();
#elif defined(K_USING_WINTHREAD_)
	return GetCurrentThread();
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

#ifdef K_USING_BTRON
typedef struct {
	void* (*func)(void*);
	void* arg;
} knh_thread_target_btron;

static void knh_thread_btronEntryPoint(knh_thread_target_btron* arg)
{
	knh_thread_target_btron target = *arg;
	free(arg);

	// FIXME: return value is ignored
	target.func(target.arg);

	// BTRON threads must terminate with b_ext_tsk;
	//       that's why we need this stub function
	b_ext_tsk();
}
#endif /* K_USING_BTRON */


/* ------------------------------------------------------------------------ */

int knh_thread_create(CTX ctx, knh_thread_t *thread, void *attr, knh_Fthread fgo, void * arg)
{
#if defined(K_USING_PTHREAD)
	return pthread_create((pthread_t*)thread, (pthread_attr_t*)attr, fgo, arg);
#elif defined(K_USING_BTRON)
	// FIXME: attr is ignored
	W err;
	knh_thread_target_btron* target =
		(knh_thread_target_btron*)malloc(sizeof(knh_thread_target_btron));
	if (target == NULL) {
		return -1;
	}
	target->func = fgo;
	target->arg = arg;
	err = b_cre_tsk((FP)knh_thread_btronEntryPoint, -1, (W)target);
	if (err < 0) {
		free(target);
		return -1;
	}
	*thread = err;
	return 0;
#elif defined(K_USING_WINTHREAD_)
	return _beginthread((void (__cdecl *)(void *))fgo, 0, NULL);
#else
	return -1;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_thread_detach(CTX ctx, knh_thread_t th)
{
#if defined(K_USING_PTHREAD)
	return pthread_detach((pthread_t)th);
#elif defined(K_USING_WINTHREAD_)
	TODO();
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_thread_join(CTX ctx, knh_thread_t thread, void **ret)
{
#if defined(K_USING_PTHREAD)
	return pthread_join((pthread_t)thread, ret);
#elif defined(K_USING_WINTHREAD_)
	return WaitForSingleObject((HANDLE)thread, INFINITE);
#else
	return -1;
#endif
}

/* ------------------------------------------------------------------------ */
/* @data */

typedef struct knh_threadcc_t {
	CTX ctx;
	knh_sfp_t *sfp;
} knh_threadcc_t ;

//static void *threading(void *p)
//{
//	knh_threadcc_t ta = *((knh_threadcc_t*)p);
//	CTX ctx = new_ThreadContext(ta.ctx);
//
//	knh_beginContext(ctx);
//	knh_sfp_t *lsfp = ctx->stack;
//
//	KNH_SETv(ctx, lsfp[0].o, new_ExceptionHandler(ctx));
//	KNH_TRY(ctx, L_CATCH, lsfp, 0);
//	{
//		knh_Method_t *mtd = ta.sfp[0].mtd;
//		DBG_ASSERT(IS_Method(mtd));
//		KNH_SETv(ctx, lsfp[1].o, mtd);
//		KNH_SETv(ctx, lsfp[2].o, ta.sfp[1].o);
//		lsfp[2].ndata = knh_Object_data(ta.sfp[1].o);
//		{
//			int i, args = knh_Method_psize(mtd);
//			for(i = 0; i < args; i++) {
//				KNH_SETv(ctx, lsfp[3+i].o, ta.sfp[2+i].o);
//				lsfp[3+i].ndata = knh_Object_data(ta.sfp[2+i].o);
//			}
//			KNH_SCALL(ctx, lsfp, 1, mtd, args);
//		}
//		goto L_FINALLY;
//	}
//	/* catch */
//L_CATCH:;
//		KNH_PRINT_STACKTRACE(ctx, lsfp, 0);
//
//L_FINALLY:
//		knh_Context_clearstack(ctx);
//		KONOHA_END(ctx);
//		knh_ThreadContext_dispose(ctx);
//		return NULL;
//}

/* ------------------------------------------------------------------------ */
// sfp |   0   |   1   |   2   |   3  |
//     |  self |  mtd  | arg1  | ...  |

//void knh_stack_threadRun(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_thread_t th;
//	knh_threadcc_t ta = {ctx, sfp + 1};
//	knh_thread_create(ctx, &th, NULL, threading, (void*)&ta);
//	//knh_thread_detach(ctx, th);
//}

/* ------------------------------------------------------------------------ */
/* [mutex] */

//create mutex 	pthread_mutex_init 	CreateMutex
//remove mutex lock 	pthread_mutex_lock 	WaitForSingleObject
//release mutex lock 	pthread_mutex_unlock 	ReleaseMutex
//destroy mutex 	pthread_mutex_destroy 	CloseHandle

int knh_mutex_init(knh_mutex_t *m)
{
#if defined(K_USING_PTHREAD)
	return pthread_mutex_init((pthread_mutex_t*)m, NULL);
#elif defined(K_USING_BTRON)
	W sem = b_cre_sem(1, SEM_EXCL|DELEXIT);
	if (sem < 0) {
		return -1;
	}
	*m = sem;
	return 0;
#elif defined(K_USED_TO_BE_ON_LKM)
	mutex_init((struct mutex *)m);
	return 0;
#elif defined(K_USING_WINTHREAD_)
	InitializeCriticalSection(m);
	// TODO STATUS_NO_MEMORY
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_mutex_lock(knh_mutex_t *m)
{
	//DBG_P("locking %p", m);
#if defined(K_USING_PTHREAD)
	int ret = pthread_mutex_lock((pthread_mutex_t*)m);
	return ret;
#elif defined(K_USING_BTRON)
	W err = b_wai_sem(*m, T_FOREVER);
	if (err < 0) {
		return -1;
	}
	return 0;
#elif defined(K_USED_TO_BE_ON_LKM)
	mutex_lock((struct mutex *) m);
	return 0;
#elif defined(K_USING_WINTHREAD_)
	EnterCriticalSection(m);
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_mutex_trylock(knh_mutex_t *m)
{
	//DBG_P("trylock %p", m);
#if defined(K_USING_PTHREAD)
	return pthread_mutex_trylock((pthread_mutex_t*)m);
#elif defined(K_USED_TO_BE_ON_LKM)
	return mutex_trylock((struct mutex *) m);
#elif defined(K_USING_WINTHREAD_)
	return TryEnterCriticalSection(m);
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_mutex_unlock(knh_mutex_t *m)
{
	//DBG_P("unlocking %p", m);
#if defined(K_USING_PTHREAD)
	int ret = pthread_mutex_unlock((pthread_mutex_t*)m);
	return ret;
#elif defined(K_USING_BTRON)
	W err = b_sig_sem(*m);
	if (err < 0) {
		return -1;
	}
	return 0;
#elif defined(K_USED_TO_BE_ON_LKM)
	mutex_unlock((struct mutex *) m);
	return 0;
#elif defined(K_USING_WINTHREAD_)
	LeaveCriticalSection(m);
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_mutex_destroy(knh_mutex_t *m)
{
	//DBG_P("destroying %p", m);
#if defined(K_USING_PTHREAD)
	return pthread_mutex_destroy((pthread_mutex_t*)m);
#elif defined(K_USING_BTRON)
	W err = b_del_sem(*m);
	if (err < 0) {
		return -1;
	}
	return 0;
#elif defined(K_USED_TO_BE_ON_LKM)
	return 0;
#elif defined(K_USING_WINTHREAD_)
	DeleteCriticalSection(m);
	return 0;
#else
	return 0;
#endif
}


knh_mutex_t *knh_mutex_malloc(CTX ctx)
{
	knh_mutex_t *m = (knh_mutex_t*)KNH_MALLOC(ctx, sizeof(knh_mutex_t));
	knh_bzero(m, sizeof(knh_mutex_t));
	if(knh_mutex_init(m) != 0) {
		KNH_PANIC(ctx, "uninitialized_mutex=%p", m);
	}
	return m;
}

void knh_mutex_free(CTX ctx, knh_mutex_t *m)
{
	if(knh_mutex_destroy(m) != 0) {
		KNH_WARN(ctx, "*unlocked_mutex=%p", m);
	}
	KNH_FREE(ctx, m, sizeof(knh_mutex_t));
}

/* ------------------------------------------------------------------------ */
/* [TLS] */

#ifndef CC_TYPE_TLS
#if defined(K_USING_PTHREAD)
static void destr(void *data)
{
	DBG_P("destruction data=%p", data);
}
#endif

/* ------------------------------------------------------------------------ */

int knh_thread_key_create(knh_thread_key_t *key)
{
#if defined(K_USING_PTHREAD)
	return pthread_key_create((pthread_key_t*)key, destr);
#elif defined(K_USING_WINTHREAD_)
	DWORD v = TlsAlloc();
	*key = (knh_thread_key_t)v;
	return (int)v;
#else
	return -1;
#endif
}

/* ------------------------------------------------------------------------ */

int thread_setspecific(knh_thread_key_t key, const void *data)
{
#if defined(K_USING_PTHREAD)
	return pthread_setspecific(key, data);
#elif defined(K_USING_WINTHREAD_)
	return TlsSetValue((DWORD)key, (LPVOID) data);
#else
	return -1;
#endif
}

/* ------------------------------------------------------------------------ */

void* knh_thread_getspecific(knh_thread_key_t key)
{
#if defined(K_USING_PTHREAD)
	return pthread_getspecific(key);
#elif defined(K_USING_WINTHREAD_)
	return (void*)TlsGetValue((DWORD)key);
#else
	return NULL;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_thread_key_delete(knh_thread_key_t key)
{
#if defined(K_USING_PTHREAD)
	return pthread_key_delete(key);
#elif defined(K_USING_WINTHREAD_)
	return TlsFree((DWORD) key);
#else
	return -1;
#endif
}
#endif


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
