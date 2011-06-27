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
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <libmemcached/memcached.h>
#include <jni.h>

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


typedef struct _knh_ThreadInfo_t {
	knh_Func_t *func;
	knh_Actor_t *a;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
} knh_ThreadInfo_t;


#define boolean int
#define true 1
#define false 0

static inline int knh_MailBox_existsMessage(knh_MailBox_t *box)
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

static void knh_Actor_invokeMethod(CTX ctx, knh_Actor_t *a, const char *mtd_name, Object *msg)
{
	//knh_MailBox_t *box = a->mailbox;
	//knh_Message_t *msg = knh_MailBox_popMessage(box);
	//const char *mtd_name = msg->mtd_name;
	//Object *o = msg->msg;
	knh_MethodInfo_t **info = a->mtd_info;
	int i = 0;
	for (i = 0; i < MAX_METHOD_NUM; i++) {
		if (!strncmp(mtd_name, info[i]->mtd_name, sizeof(mtd_name))) {
			//fprintf(stderr, "find method!!\n");
			BEGIN_LOCAL(ctx, lsfp, 5);
			KNH_SETv(ctx, lsfp[5].o, msg);
			klr_setesp(ctx, lsfp + 6);
			KNH_SCALL(ctx, lsfp, 0, info[i]->mtd, 2);
			END_LOCAL_(ctx, lsfp);
			break;
		}
	}
}


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
	KONOHA_END(info->ctx);
}

METHOD CActor_startDeliver(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	knh_Func_t *func = sfp[1].fo;
	knh_ThreadInfo_t *th_info = (knh_ThreadInfo_t *)KNH_MALLOC(ctx, sizeof(knh_ThreadInfo_t));
	pthread_t th;
	th_info->ctx = (knh_context_t *)KNH_MALLOC(ctx, sizeof(knh_context_t));
	memcpy(th_info->ctx, ctx, sizeof(knh_context_t));
	th_info->func = func;
	th_info->a = a;
	th_info->sfp = (knh_sfp_t *)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * ctx->stacksize);
	knh_sfp_copy(th_info->ctx, th_info->sfp, sfp, th_info->ctx->stacksize);
	//pthread_create(&th, NULL, (void*(*)(void *))delivery_thread_func, (void *)th_info);
	//pthread_detach(th);
	RETURNvoid_();
}

METHOD CActor_startScheduler(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	while (true) {
		if (knh_MailBox_existsMessage(a->mailbox)) {
			//knh_Actor_invokeMethod(ctx, a);
		}
	}
}

METHOD CActor_fork(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(fork());
}

METHOD CActor_readMessage(CTX ctx, knh_sfp_t *sfp _RIX)
{
	//asm volatile("int3");
	//fprintf(stderr, "readMessage\n");
	knh_Actor_t *a = (knh_Actor_t *)sfp[0].o;
	knh_Object_t **v = ((knh_ObjectField_t *)sfp[1].o)->fields;
	knh_String_t *s = (knh_String_t *)v[0];
	const char *mtd_name = S_tochar(s);
	//fprintf(stderr, "mtd_name = %s\n", mtd_name);
	knh_Object_t *msg = v[1];
	//fprintf(stderr, "msg = %p\n", msg);
	//fprintf(stderr, "sname = %s\n", S_tochar(msg->h.cTBL->sname));
	knh_Actor_invokeMethod(ctx, a, mtd_name, msg);
}

METHOD CActor_exit(CTX ctx, knh_sfp_t *sfp _RIX)
{
	exit(0);
	RETURNvoid_();
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

#define MAX_SCRIPT_SIZE 4096
METHOD CActor_getOriginalScript(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[1].a;
	int i = 0;
	int asize = a->size;
	char script[MAX_SCRIPT_SIZE] = {0};
	char *scr = script;
	for (i = 0; i < asize; i++) {
		knh_Func_t *fo = (knh_Func_t *)a->list[i];
		knh_Method_t *mtd = fo->mtd;
		const char *mtd_name = knh_getmnname(ctx, mtd->mn);
		const char *type_name = S_tochar(ClassTBL(DP(mtd)->mp->p0.type)->sname);
		const char *arg_name = S_tochar(knh_getFieldName(ctx, DP(mtd)->mp->p0.fn));
		const char *src = S_tochar(fo->mtd->b->kcode->source);
		size_t size = strlen(mtd_name) + strlen(type_name) + strlen(arg_name) + strlen(src) + 4;
		if (size > MAX_SCRIPT_SIZE) fprintf(stderr, "ERROR: too large script size");
		snprintf(scr, size + 18, "void %s(%s %s)\n{\n%s\n}\n", mtd_name, type_name, arg_name, src);
		scr += size + 18;
	}
	RETURN_(new_String(ctx, script));
}

METHOD CActor_getScriptPath(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(ctx->script->ns->rpath);
}


METHOD CActor_existsWorkingDir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *dir_name = String_to(const char *, sfp[1]);
	DIR *dir = opendir(dir_name);
	int ret = (dir != NULL) ? 1 : 0;
	RETURNb_(ret);
}

METHOD CActor_changeToWorkingDir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *dir_name = String_to(const char *, sfp[1]);
	int ret = chdir(dir_name);
	ret = (ret == 0) ? 1 : 0;
	RETURNb_(ret);
}

static void addIndent(char* buf, const char* src)
{
	size_t j,i = 0, prev_count = 1, bracket_count = 1;
 	size_t c = 1, src_count = strlen(src);
 	char line[256], prev = 0, t = src[1];
 	char* tmp_buf = buf;
 	while (c <= src_count) {
 		if (t == '{') {
 			bracket_count++;
 		} else if (t == '}') {
			bracket_count--;
 		}
 		if (prev == '\n') {
 			memset(buf, '\t', prev_count);
 			buf += prev_count;
 			strncpy(buf, line, i);
 			buf += i;
 			memset(line, '\0', i);
 			i = 0;
 			prev_count = bracket_count;
 		}
 		line[i++] = t;
 		prev = t;
 		t = src[++c];
 	}
}

METHOD CActor_activateActor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *script_name = String_to(const char *, sfp[1]);
	int pid;
	char buf[128] = {0};
	if ((pid = fork()) == 0) {
		snprintf(buf, 128, "konoha %s", script_name);
		//system(buf);
		//exec();
	} else {
		//snprintf(buf, 128, "kill -9 %d", pid);
		//system(buf);
		//exec();
	}
}


METHOD CActor_sendToScalaActor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	JNIEnv *jnienv;
    JavaVM *javavm;
    JavaVMInitArgs vm_args;

    JavaVMOption options[1];
    options[0].optionString = "-Djava.class.path=.:/usr/local/scala/lib/scala-library.jar";
    vm_args.version = JNI_VERSION_1_2;
    vm_args.options = options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = true;

    int result = JNI_CreateJavaVM(&javavm, (void **)&jnienv, &vm_args);
	
    if (result != 0) {
        fprintf(stderr, "ERROR : [%d] cannot create JavaVM.\n", result);
    }

    fprintf(stderr, "search Class\n");
    jclass cls = jnienv->FindClass("ScalaActor");
    if (cls == 0) {
        fprintf(stderr, "cannot find ScalaActor\n");
    }
    fprintf(stderr, "get Method from Hoge\n");
    //jmethodID mid = jnienv->GetStaticMethodID(cls, "sendInt", "(Ljava/lang/String;Ljava/lang/String;II)V");
	jmethodID mid = jnienv->GetStaticMethodID(cls, "sendInt", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/Object;)V");
    if (mid == 0) {
        fprintf(stderr, "cannot find sendInt()\n");
    }
    printf("call Method.\n");
	const char *actor_name = String_to(const char *, sfp[1]);
	const char *host_name = String_to(const char *, sfp[2]);
	int port = Int_to(int, sfp[3]);
	int msg = Int_to(int, sfp[4]);
	
	jstring target = jnienv->NewStringUTF(actor_name);
	jstring host = jnienv->NewStringUTF(host_name);
	//jobject obj = jnienv->NewObject(jnienv, targetClass, mid);
    jnienv->CallStaticVoidMethod(cls, mid, target, host, port, msg);
	//jobject objResult = jnienv->CallStaticObjectMethod(cls, mid);
    jthrowable throwResult = jnienv->ExceptionOccurred();
    if (throwResult != NULL) {
        fprintf(stderr, "Exception!!\n");
        jnienv->ExceptionDescribe();
        jnienv->ExceptionClear();
    }

	/*
    if (objResult == NULL) {
		fprintf(stderr, "ERROR : objResult == NULL\n");
    } else {
        jstring strResult = (jstring)objResult;
        fprintf(stderr, "[%s]\n", jnienv->GetStringUTFChars(strResult, NULL));
    }
    */
    fprintf(stderr, "delete JavaVM\n");
    result = javavm->DestroyJavaVM();
    if (result != 0) {
        fprintf(stderr, "ERROR [%d] : cannot delte JavaVM\n", result);
    }
	RETURNvoid_();
}

static void CActor_init(CTX ctx, knh_RawPtr_t *po)
{

}

static void CActor_free(CTX ctx, knh_RawPtr_t *po)
{

}

DEFAPI(void) defCActor(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "CActor";
	cdef->init = CActor_init;
	cdef->free = CActor_free;
}

DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	RETURN_PKGINFO("konoha.actor");
}

#ifdef __cplusplus
}
#endif
/* ------------------------------------------------------------------------ */
