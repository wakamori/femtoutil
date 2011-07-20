#define K_INTERNAL
#define USE_new_bytes 1
#include <konoha1.h>
#include <dirent.h>
#include <unistd.h>
#include <libmemcached/memcached.h>
#include <jni.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>

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
	volatile int end;
} knh_MailBox_t;

typedef struct knh_ActorEX_t {
	const char *actor_name;
	int port;//for serversocket
	const char *path;//path
} knh_ActorEX_t;

typedef struct _knh_Actor_t {
	knh_MethodInfo_t **mtd_info;
	knh_MailBox_t *mailbox;
	knh_ActorEX_t *b;
} knh_Actor_t;

knh_Object_t *knh_InputStream_readObject(CTX ctx, knh_InputStream_t *in, const knh_ClassTBL_t *ct);
void knh_OutputStream_writeObject(CTX ctx, knh_OutputStream_t *ous, knh_Object_t *o);
