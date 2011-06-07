/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c)  2010-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 * (2) Konoha Software Foundation License 1.0
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

// **************************************************************************
// LIST OF CONTRIBUTERS
//  goccy - Masaaki Goshima, Yokohama National University, Japan
//
// **************************************************************************

#include <konoha1.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_METHOD_NUM 16
#define MAX_MAIL_NUM 32

typedef struct _knh_MethodInfo_t {
	const char *mtd_name;
	knh_Method_t *mtd;
} knh_MethodInfo_t;

typedef struct knh_Message_t {
	//const char *actor_name;//for "sender" keyword
	const char *mtd_name;
	Object *msg;
} knh_Message_t;

typedef struct knh_MailBox_t {
	knh_Message_t **msgs;
	int end;
} knh_MailBox_t;

typedef struct knh_ActorEX_t {
	const char *actor_name;
	int port;//for serversocket
	const char *path;//path
} knh_ActorEX_t;

typedef struct _knh_Actor_t {
	knh_hObject_t h;
	knh_MethodInfo_t **mtd_info;
	knh_MailBox_t *mailbox;
	knh_ActorEX_t *b;
} knh_Actor_t;


EXPORTAPI(const knh_ClassDef_t *) CActor(CTX ctx)
{
	static knh_ClassDef_t cdef;
	cdef = *(knh_getDefaultClassDef());
	cdef.name = "CActor";
	//cdef.init = Actor_init;
	//cdef.reftrace = Actor_reftrace;
	//cdef.free = Actor_free;
	return (const knh_ClassDef_t*)&cdef;
}

knh_MailBox_t *knh_MailBox_new(CTX ctx)
{
	knh_MailBox_t *mailbox = (knh_MailBox_t *)KNH_MALLOC(ctx, sizeof(knh_MailBox_t));
	mailbox->end = 0;
	mailbox->msgs = (knh_Message_t **)KNH_MALLOC(ctx, sizeof(knh_Message_t *) * MAX_MAIL_NUM);
	int i = 0;
	for (i = 0; i < MAX_MAIL_NUM; i++) {
		mailbox->msgs[i] = (knh_Message_t *)KNH_MALLOC(ctx, sizeof(knh_Message_t));
	}
	return mailbox;
}

static void knh_Actor_init(CTX ctx, knh_Actor_t *actor)
{
	knh_ActorEX_t *b = (knh_ActorEX_t *)KNH_MALLOC(ctx, sizeof(knh_ActorEX_t));
	actor->mtd_info = (knh_MethodInfo_t **)KNH_MALLOC(ctx, sizeof(knh_MethodInfo_t *) * MAX_METHOD_NUM);
	int i = 0;
	for (i = 0; i < MAX_METHOD_NUM; i++) {
		actor->mtd_info[i] = (knh_MethodInfo_t *)KNH_MALLOC(ctx, sizeof(knh_MethodInfo_t));
	}
	actor->mailbox = knh_MailBox_new(ctx);
	actor->b = b;
}

#include <pthread.h>

typedef struct _knh_ThreadInfo_t {
	knh_Func_t *func;
	knh_Actor_t *a;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
} knh_ThreadInfo_t;


#define boolean int
#define true 1
#define false 0

static int knh_MailBox_existsMessage(knh_MailBox_t *box)
{
	return box->end;
}

static knh_Message_t *knh_MailBox_popMessage(knh_MailBox_t *box)
{
	if (box->end == 0) return NULL;
	knh_Message_t *msg = box->msgs[0];
	int i = 0;
	for (i = 0; i < MAX_MAIL_NUM - 1; i++) {
		box->msgs[i] = box->msgs[i + 1];
	}
	//memmove(box->msgs[0], box->msgs[1], sizeof(knh_Message_t) * (MAX_MAIL_NUM - 1));
	box->end--;
	return msg;
}

static void knh_MailBox_pushMessage(knh_MailBox_t *box, knh_Message_t *m)
{
	knh_Message_t *msg = box->msgs[box->end];
	//msg->actor_name = m->actor_name;
	msg->mtd_name = m->mtd_name;
	msg->msg = m->msg;
	box->end++;
}

static void knh_Actor_invokeMethod(CTX ctx, knh_Actor_t *a)
{
	knh_MailBox_t *box = a->mailbox;
	knh_Message_t *msg = knh_MailBox_popMessage(box);
	const char *mtd_name = msg->mtd_name;
	Object *o = msg->msg;
	knh_MethodInfo_t **info = a->mtd_info;
	int i = 0;
	for (i = 0; i < MAX_METHOD_NUM; i++) {
		if (!strncmp(mtd_name, info[i]->mtd_name, sizeof(mtd_name))) {
			//fprintf(stderr, "find method!!\n");
			BEGIN_LOCAL(ctx, lsfp, 5);
			KNH_SETv(ctx, lsfp[5].o, o);
			klr_setesp(ctx, lsfp + 6);
			KNH_SCALL(ctx, lsfp, 0, info[i]->mtd, 2);
			END_LOCAL_(ctx, lsfp);
			break;
		}
	}
}

#include <libmemcached/memcached.h>

static memcached_st *new_memcached(const char *host)
{
	int port = 11211;
	memcached_return rc;
	memcached_st *memc = memcached_create(NULL);
	memcached_server_st *servers = memcached_server_list_append(NULL, host, port, &rc);
	if (rc != MEMCACHED_SUCCESS) {
		fprintf(stderr, "%s\n", memcached_strerror(memc, rc));
		return 0;
	}
	rc = memcached_server_push(memc, servers);//servers ==> memc
	if (rc != MEMCACHED_SUCCESS) {
		fprintf(stderr, "%s\n", memcached_strerror(memc, rc));
		return 0;
	}
	memcached_server_list_free(servers);
	return memc;
}

static const char *memcached_getValue(memcached_st *memc, const char *key)
{
	memcached_return rc;
	size_t val_length;
	uint32_t flags;
	const char *val = memcached_get(memc, key, strlen(key), &val_length, &flags, &rc);
	if (rc != MEMCACHED_SUCCESS) {
		fprintf(stderr, "%s\n", memcached_strerror(memc, rc));
		return NULL;
	}
	//fprintf(stderr, "get value = [%s]\n", val);
	return val;
}

static void memcached_setValue(memcached_st *memc, const char *key, const char *val)
{
	memcached_return rc = memcached_set(memc, key, strlen(key), val, strlen(val), (time_t)0, (uint32_t)0);
	if (rc != MEMCACHED_SUCCESS) {// && rc != MEMCACHED_STORED && rc != MEMCACHED_NOTSTORED) {
		//fprintf(stderr, "%s\n", memcached_strerror(memc, rc));
		return;
	}
	//fprintf(stderr, "[set] : [%s] : [%s]\n", key, val);
}

static void memcached_addValue(memcached_st *memc, const char *key, const char *val)
{
	memcached_return rc = memcached_add(memc, key, strlen(key), val, strlen(val), (time_t)0, (uint32_t)0);
	if (rc != MEMCACHED_SUCCESS) {// && rc != MEMCACHED_STORED && rc != MEMCACHED_NOTSTORED) {
		//fprintf(stderr, "%s\n", memcached_strerror(memc, rc));
		return;
	}
	//fprintf(stderr, "[add] : [%s] : [%s]\n", key, val);
}

static int knh_Actor_getPortNum(memcached_st *memc)
{
	int port = 1234;
	const char *val = memcached_getValue(memc, "port");
	if (val == NULL) {
		const char *key = "port";
		const char *val = "1234";
		memcached_setValue(memc, key, val);
		return port;
	}
	port = atoi(val);
	return port;
}

#define MAX_BUF_SIZE 128
static void knh_Actor_addMemcached(CTX ctx, knh_Actor_t *a)
{
	const char *actor_name = DP(a)->actor_name;
	knh_String_t *p = ctx->script->ns->rpath;
	const char *path = p->str.text;
	memcached_st *memc = new_memcached("localhost");
	int port = knh_Actor_getPortNum(memc);
	port++;
	char port_str[5] = {0};
	snprintf(port_str, 5, "%d", port);
	memcached_setValue(memc, "port", port_str);
	char buf[MAX_BUF_SIZE] = {0};
	if (sizeof(path) + 6 > MAX_BUF_SIZE) {
		fprintf(stderr, "ERROR: too long name\n");
	}
	snprintf(buf, MAX_BUF_SIZE, "%s:%s", path, port_str);
	memcached_setValue(memc, actor_name, buf);
	DP(a)->port = port;
	DP(a)->path = path;
	RETURNvoid_();
}

static const char *knh_Actor_getActorPathFromMemcached(const char *actor_name)
{
	memcached_st *memc = new_memcached("localhost");
	return memcached_getValue(memc, actor_name);
}

METHOD CActor_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t *)sfp[0].o;
	knh_Actor_init(ctx, actor);
	DP(actor)->actor_name = String_to(const char *, sfp[1]);
	knh_Actor_addMemcached(ctx, actor);
	RETURN_(actor);
}

METHOD CActor_getPathByActorName(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *actor_name = String_to(const char *, sfp[1]);
	const char *path = knh_Actor_getActorPathFromMemcached(actor_name);
	RETURN_(new_String(ctx, path));
}

METHOD CActor_getPort(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	RETURNi_(DP(a)->port);
}

METHOD CActor_getPath(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	RETURN_(new_String(ctx, DP(a)->path));
}

METHOD CActor_getName(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	RETURN_(new_String(ctx, DP(a)->actor_name));
}

METHOD CActor_setMethodInfo(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t *)sfp[0].o;
	knh_MethodInfo_t **info = actor->mtd_info;
	knh_Array_t *mn_array = O_cTBL(ctx->script)->methods;
	knh_Method_t **methods = mn_array->methods;
	int i = 0;
	while (methods[i] != NULL) {
		const char *mn_name = knh_getmnname(ctx, methods[i]->mn);
		//fprintf(stderr, "mn_name = [%s]\n", mn_name);
		(*info)->mtd_name = mn_name;
		(*info)->mtd = methods[i];
		info++;
		i++;
	}
	RETURNvoid_();
}

#ifdef K_USING_THREAD
static void knh_sfp_copy(CTX ctx, knh_sfp_t *dst, knh_sfp_t *src, size_t size)
{
	size_t i;
	for (i = 0; i < size; i++) {
		dst[i].ndata = src[i].ndata;
		KNH_SETv(ctx, dst[i].o, src[i].o);
	}
}

static void delivery_thread_func(void *arg)
{
	knh_ThreadInfo_t *info = (knh_ThreadInfo_t *)arg;
	knh_context_t *ctx = info->ctx;
	knh_sfp_t *sfp = info->sfp;
	knh_Func_t *f = info->func;
	knh_Actor_t *a = info->a;
	KONOHA_BEGIN(ctx);
	ctx->stack = sfp;
	ctx->freeMemoryList = NULL;
	//BEGIN_LOCAL(ctx, lsfp, 5);
	KNH_SETv(ctx, sfp[5].o, UPCAST(a));
	//KNH_SETv(ctx, lsfp[5].o, UPCAST(a));
	klr_setesp(ctx, sfp + 6);
	//klr_setesp(ctx, lsfp + 6);
	KNH_SCALL(ctx, sfp, 0, f->mtd, 1);
	//KNH_SCALL(ctx, lsfp, 0, f->mtd, 2);
	//END_LOCAL_(ctx, lsfp);
	fprintf(stderr, "end of thread_func\n");
	KONOHA_END(info->ctx);
}
#endif

METHOD CActor_startDeliver(CTX ctx, knh_sfp_t *sfp _RIX)
{
	//asm volatile("int3");
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	knh_Func_t *func = sfp[1].fo;
#ifdef K_USING_THREAD
	knh_ThreadInfo_t *th_info = (knh_ThreadInfo_t *)KNH_MALLOC(ctx, sizeof(knh_ThreadInfo_t));
	pthread_t th;
	th_info->ctx = (knh_context_t *)KNH_MALLOC(ctx, sizeof(knh_context_t));
	memcpy(th_info->ctx, ctx, sizeof(knh_context_t));
	th_info->func = func;
	th_info->a = a;
	th_info->sfp = (knh_sfp_t *)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * ctx->stacksize);
	knh_sfp_copy(th_info->ctx, th_info->sfp, sfp, th_info->ctx->stacksize);
	pthread_create(&th, NULL, (void *)delivery_thread_func, (void *)th_info);
	pthread_detach(th);
#else
	#include <unistd.h>
	int pid;
	if ((pid = fork()) == 0) {
		RETURNvoid_();
	} else if (pid > 0) {
		BEGIN_LOCAL(ctx, lsfp, 5);
		KNH_SETv(ctx, lsfp[5].o, UPCAST(a));
		klr_setesp(ctx, lsfp + 6);
		KNH_SCALL(ctx, lsfp, 0, func->mtd, 2);
		wait(0);
		END_LOCAL_(ctx, lsfp);
	}
#endif
	//fprintf(stderr, "return to main thread\n");
	RETURNvoid_();
}

METHOD CActor_startScheduler(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	//fprintf(stderr, "start Scheduler\n");
	while (true) {
		if (knh_MailBox_existsMessage(a->mailbox)) {
			knh_Actor_invokeMethod(ctx, a);
		}
	}
}

METHOD CActor_addMessageToMailBox(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	//const char *actor_name = String_to(const char *, sfp[1]);
	const char *mtd_name = String_to(const char *, sfp[1]);
	Object *msg = sfp[2].o;
	knh_Message_t m;
	//m.actor_name = actor_name;
	m.mtd_name = mtd_name;
	m.msg = msg;
	knh_MailBox_pushMessage(a->mailbox, &m);
}

EXPORTAPI(const knh_PackageDef_t*) init(CTX ctx)
{
	static const knh_PackageDef_t pkgdef = KNH_PKGINFO("actor", "0.0", "Konoha Actor Library", NULL);
	return &pkgdef;
}

//#endif
/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

