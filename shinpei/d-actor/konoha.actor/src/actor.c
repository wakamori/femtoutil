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
//  kimio - Kimio Kuramitsu, Yokohama National University, Japan
//  shinpei_NKT - Shinpei Nakata, Yokohama National University, Japan
//  goccy - Masaaki Goshima, Yokohama National University, Japan
//  chen_ji - Takuma Wakamori, Yokohama National University, Japan
// **************************************************************************

//#include <konoha_actor.h>
#ifdef __cplusplus
extern "C" {
#endif

#define K_INTERNAL
#include <konoha1.h>

/* ======================================================================== */
// [private functions]

/* ------------------------------------------------------------------------ */
// [bytes]

static inline knh_bytes_t new_bytes2(const char *text, size_t len)
{
	knh_bytes_t v;
	v.text = text;
	v.len = len;
	return v;
}

static inline knh_bytes_t knh_bytes_next(knh_bytes_t v, int ch)
{
	size_t i;
	for(i = 0; i < v.len; i++) {
		if(v.utext[i] == ch) {
			v.text = v.text + (i+1);
			v.len = v.len - (i+1);
			break;
		}
	}
	return v;
}

static inline knh_index_t knh_bytes_index(knh_bytes_t v, int ch)
{
	size_t i;
	for(i = 0; i < v.len; i++) {
		if(v.utext[i] == ch) return (knh_index_t)i;
	}
	return -1;
}

/* ------------------------------------------------------------------------ */
// [actor]

typedef struct {
	knh_hObject_t h;
	knh_String_t *name;
	knh_String_t *host;
	int port;
} knh_Actor_t;

static void Actor_init(CTX ctx, knh_RawPtr_t *po)
{
	knh_Actor_t *actor = (knh_Actor_t *)po;
	actor->name = NULL;
	actor->host = NULL;
	actor->port = 0;
}

static void Actor_free(CTX ctx, knh_RawPtr_t *po)
{
	knh_Actor_t *actor = (knh_Actor_t *)po;
	if (actor->name != NULL) {
		KNH_FINALv(ctx, actor->name);
	}
	if (actor->host != NULL) {
		KNH_FINALv(ctx, actor->host);
	}
}

static knh_Actor_t *knh_Actor_new(CTX ctx, knh_bytes_t name, knh_bytes_t host, int port)
{
	knh_Actor_t *actor = (knh_Actor_t *)new_O(Actor, knh_getcid(ctx, STEXT("Actor")));
	KNH_INITv(actor->name, new_String2(ctx, CLASS_String, name.text, name.len, 0));
	KNH_INITv(actor->host, new_String2(ctx, CLASS_String, host.text, host.len, 0));
	actor->port = port;
	return actor;
}

/* ------------------------------------------------------------------------ */
// [memcached]

//static memcached_st *new_memcached(const char *host)
//{
//	int port = 11211;
//	memcached_return rc;
//	memcached_st *memc = memcached_create(NULL);
//	memcached_server_st *servers = memcached_server_list_append(NULL, host, port, &rc);
//	if (rc != MEMCACHED_SUCCESS) {
//		DBG_P( "%s\n", memcached_strerror(memc, rc));
//		return 0;
//	}
//	rc = memcached_server_push(memc, servers);//servers ==> memc
//	if (rc != MEMCACHED_SUCCESS) {
//		DBG_P( "%s\n", memcached_strerror(memc, rc));
//		return 0;
//	}
//	memcached_server_list_free(servers);
//	return memc;
//}
//
//static const char *memcached_getValue(memcached_st *memc, const char *key)
//{
//	memcached_return rc;
//	size_t val_length;
//	uint32_t flags;
//	const char *val = memcached_get(memc, key, strlen(key), &val_length, &flags, &rc);
//	if (rc != MEMCACHED_SUCCESS) {
//		DBG_P( "%s\n", memcached_strerror(memc, rc));
//		return NULL;
//	}
//	//DBG_P( "get value = [%s]\n", val);
//	return val;
//}
//
//static void memcached_setValue(memcached_st *memc, const char *key, const char *val)
//{
//	memcached_return rc = memcached_set(memc, key, strlen(key), val, strlen(val), (time_t)0, (uint32_t)0);
//	if (rc != MEMCACHED_SUCCESS) {// && rc != MEMCACHED_STORED && rc != MEMCACHED_NOTSTORED) {
//		//DBG_P( "%s\n", memcached_strerror(memc, rc));
//		return;
//	}
//	//DBG_P( "[set] : [%s] : [%s]\n", key, val);
//}

//static void memcached_addValue(memcached_st *memc, const char *key, const char *val)
//{
//	memcached_return rc = memcached_add(memc, key, strlen(key), val, strlen(val), (time_t)0, (uint32_t)0);
//	if (rc != MEMCACHED_SUCCESS) {// && rc != MEMCACHED_STORED && rc != MEMCACHED_NOTSTORED) {
//		//DBG_P( "%s\n", memcached_strerror(memc, rc));
//		return;
//	}
//	//DBG_P( "[add] : [%s] : [%s]\n", key, val);
//}

/* ------------------------------------------------------------------------ */
// [socket]

//static knh_bool_t SOCKET_exists(CTX ctx, knh_Path_t *pth)
//{
//	return 0; // dummy
//}
//
//static knh_io_t SOCKET_open(CTX ctx, knh_Path_t *pth, const char *mode)
//{
//	return IO_NULL; // Always opened by external
//}
//
//static knh_intptr_t SOCKET_read(CTX ctx, knh_io_t fd, char *buf, size_t bufsiz)
//{
//	return recv((int)fd, buf, bufsiz, 0);
//}
//
//static knh_intptr_t SOCKET_write(CTX ctx, knh_io_t fd, const char *buf, size_t bufsiz)
//{
//	return send((int)fd, buf, bufsiz, 0);
//}
//
//static void SOCKET_close(CTX ctx, knh_io_t fd)
//{
//	close((int)fd);
//}
//
//static knh_StreamDPI_t SOCKET_DSPI = {
//	K_DSPI_STREAM, "socket",  K_OUTBUF_MAXSIZ,
//	SOCKET_exists,
//	SOCKET_open, SOCKET_open, SOCKET_read, SOCKET_write, SOCKET_close,
//};
//
//static knh_io_t knh_Socket_open(CTX ctx, knh_sfp_t *sfp, const char *ip_or_host, int port)
//{
//	int sd = IO_NULL;
//	struct in_addr addr = {0};
//	struct sockaddr_in server = {0};
//	const char *errfunc = NULL;
//	if (port == 0) port = 80;
//	if ((int)(addr.s_addr = inet_addr(ip_or_host)) == -1) {
//		struct hostent *host = gethostbyname(ip_or_host);
//		if (host == NULL) {
//			errfunc = "gethostname";
//			goto L_PERROR;
//		}
//		memcpy(&addr, (struct in_addr *)*host->h_addr_list, sizeof(struct in_addr));
//	}
//
//	server.sin_family = AF_INET;
//	server.sin_addr = addr;
//	server.sin_port = htons(port);
//
//	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//		errfunc = "socket";
//		goto L_PERROR;
//	}
//
//	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
//		errfunc = "connect";
//		close(sd);
//		goto L_PERROR;
//	}
//
//	L_PERROR:;
//	if (errfunc != NULL) {
//		LOGDATA = {sDATA("host", ip_or_host), iDATA("port", port)};
//		LIB_Failed(errfunc, "Socket!!");
//		//DBG_P( "Socket!! %s\n", perror());
//		perror("socket!!");
//		sd = IO_NULL;
//	} else {
//		LOGDATA = {sDATA("host", ip_or_host), iDATA("port", port), __ERRNO__};
//		NOTE_OK("socket");
//	}
//	if (sd == IO_NULL) {
//		DBG_P( "ERROR : cannot open socket\n");
//	}
//	return (knh_io_t) sd;
//}
//
//static knh_InputStream_t *knh_Socket_getInputStream(CTX ctx, knh_io_t sd)
//{
//	return new_InputStreamDPI(ctx, sd, &SOCKET_DSPI, KNH_TNULL(Path));
//}
//
//static knh_OutputStream_t *knh_Socket_getOutputStream(CTX ctx, knh_io_t sd)
//{
//	return new_OutputStreamDPI(ctx, sd, &SOCKET_DSPI, KNH_TNULL(Path));
//}
//
//static knh_io_t knh_ServerSocket_open(CTX ctx, knh_sfp_t *sfp, int port, int backlog)
//{
//	//knh_Socket_t *ss = (knh_Socket_t*)sfp[0].o;
//	struct sockaddr_in addr = {0};
//	int optval = 1;
//	const char *errfunc = NULL;
//	const char *host = "127.0.0.1";
//	in_addr_t hostinfo;
//	
//	int fd = socket(AF_INET, SOCK_STREAM, 0);
//	if (fd  == -1) {
//		errfunc = "socket"; goto L_RETURN;
//	}
//
//	hostinfo = inet_addr(host);
//	if (hostinfo == INADDR_NONE) {
//		errfunc = "inet_addr"; goto L_RETURN;
//	}
//
//	addr.sin_family = AF_INET;
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	addr.sin_port = htons((short)port);
//	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
//
//	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
//		errfunc = "bind"; goto L_RETURN;
//	}
//	if (listen(fd, backlog) == -1) {
//		errfunc = "listen"; goto L_RETURN;
//	}
//
//	L_RETURN:
//	if(errfunc == NULL) {
//		//ss->sd = fd;
//		LOGDATA = {sDATA("host", host), iDATA("port", port), iDATA("max_connection", backlog)};
//		NOTE_OK("listen");
//	} else {
//		LOGDATA = {sDATA("host", host), iDATA("port", port), iDATA("max_connection", backlog), __ERRNO__};
//		LIB_Failed(errfunc, "Socket!!");
//		//knh_Object_toNULL(ctx, ss);
//	}
//	return fd;
//}
//
//static knh_intptr_t knh_ServerSocket_accept(CTX ctx, knh_sfp_t *sfp, knh_io_t sd)
//{
//    struct sockaddr_in client_address;
//    socklen_t client_len = sizeof(struct sockaddr_in);
//	knh_intptr_t fd = accept(sd, (struct sockaddr*)&client_address, &client_len);
//	if (fd == -1) {
//		LOGDATA = {__ERRNO__};
//		LIB_Failed("accept", "Socket!!");
//	} else {
//		LOGDATA = {__ERRNO__};
//		NOTE_OK("accept");
//	}
//	return fd;
//}
//
//static knh_bytes_t new_bytes(const char *c_buf)
//{
//	DBG_ASSERT(c_buf != NULL);
//	knh_bytes_t t;
//	t.ubuf = (knh_uchar_t*)c_buf;
//	t.len = knh_strlen(t.text);
//	return t;
//}

/* ------------------------------------------------------------------------ */
// [actor]

//static knh_MailBox_t *knh_MailBox_new(CTX ctx)
//{
//	knh_MailBox_t *mailbox = (knh_MailBox_t *)KNH_MALLOC(ctx, sizeof(knh_MailBox_t));
//	mailbox->end = 0;
//	mailbox->msgs = (knh_Message_t **)KNH_MALLOC(ctx, sizeof(knh_Message_t *) * MAX_MAIL_NUM);
//	int i = 0;
//	for (i = 0; i < MAX_MAIL_NUM; i++) {
//		mailbox->msgs[i] = (knh_Message_t *)KNH_MALLOC(ctx, sizeof(knh_Message_t));
//	}
//	return mailbox;
//}
//
//static inline int knh_MailBox_existsMessage(knh_MailBox_t *box)
//{
//	return box->end;
//}

//static knh_Message_t *knh_MailBox_popMessage(knh_MailBox_t *box)
//{
//	if (box->end == 0) return NULL;
//	knh_Message_t *msg = box->msgs[0];
//	int i = 0;
//	for (i = 0; i < MAX_MAIL_NUM - 1; i++) {
//		box->msgs[i] = box->msgs[i + 1];
//	}
//	//memmove(box->msgs[0], box->msgs[1], sizeof(knh_Message_t) * (MAX_MAIL_NUM - 1));
//	box->end--;
//	return msg;
//}

//static void knh_MailBox_pushMessage(knh_MailBox_t *box, knh_Message_t *m)
//{
//	knh_Message_t *msg = box->msgs[box->end];
//	//msg->actor_name = m->actor_name;
//	msg->mtd_name = m->mtd_name;
//	msg->msg = m->msg;
//	box->end++;
//}

//static void knh_Actor_addMessageToMailBox(knh_Actor_t *a, const char *mtd_name, Object *msg)
//{
//	knh_Message_t m;
//	//m.actor_name = actor_name;
//	m.mtd_name = mtd_name;
//	m.msg = msg;
//	knh_MailBox_pushMessage(a->mailbox, &m);
//}

//static void knh_Actor_invokeMethod(CTX ctx, knh_Actor_t *a, const char *mtd_name, Object *msg)
//{
//	//knh_MailBox_t *box = a->mailbox;
//	//knh_Message_t *msg = knh_MailBox_popMessage(box);
//	//const char *mtd_name = msg->mtd_name;
//	//Object *o = msg->msg;
//	knh_MethodInfo_t **info = a->mtd_info;
//	int i = 0;
//	for (i = 0; i < MAX_KMETHOD_NUM; i++) {
//		if (!strncmp(mtd_name, info[i]->mtd_name, sizeof(mtd_name))) {
//			//DBG_P( "find method!!\n");
//			BEGIN_LOCAL(ctx, lsfp, 6);
//			KNH_SETv(ctx, lsfp[5].o, msg);
//			KNH_SCALL(ctx, lsfp, 0, info[i]->mtd, 2);
//			END_LOCAL(ctx, lsfp);
//			break;
//		}
//	}
//}

//static int knh_Actor_getPortNum(memcached_st *memc)
//{
//	int port = 1234;
//	const char *val = memcached_getValue(memc, "port");
//	if (val == NULL) {
//		const char *key = "port";
//		const char *val = "1234";
//		memcached_setValue(memc, key, val);
//		return port;
//	}
//	port = atoi(val);
//	return port;
//}

//#define MAX_BUF_SIZE 128
//static void knh_Actor_addMemcached(CTX ctx, knh_Actor_t *a)
//{
//	const char *actor_name = DP(a)->actor_name;
//	knh_String_t *p = ctx->script->ns->path->urn;
//	const char *path = p->str.text;
//	memcached_st *memc = new_memcached("localhost");
//	int port = knh_Actor_getPortNum(memc);
//	port++;
//	char port_str[5] = {0};
//	snprintf(port_str, 5, "%d", port);
//	memcached_setValue(memc, "port", port_str);
//	char buf[MAX_BUF_SIZE] = {0};
//	if (sizeof(path) + 6 > MAX_BUF_SIZE) {
//		DBG_P( "ERROR: too long name\n");
//	}
//	snprintf(buf, MAX_BUF_SIZE, "%s:%s", path, port_str);
//	memcached_setValue(memc, actor_name, buf);
//	DP(a)->port = port;
//	DP(a)->path = path;
//}

//static const char *knh_Actor_getActorPathFromMemcached(const char *actor_name)
//{
//	memcached_st *memc = new_memcached("localhost");
//	return memcached_getValue(memc, actor_name);
//}


//static void knh_Actor_setMethodInfo(CTX ctx, knh_Actor_t *actor)
//{
//	knh_MethodInfo_t **info = actor->mtd_info;
//	knh_Array_t *mn_array = O_cTBL(ctx->script)->methods;
//	knh_Method_t **methods = mn_array->methods;
//	int i = 0;
//	while (methods[i] != NULL) {
//		const char *mn_name = knh_getmnname(ctx, methods[i]->mn);
//		//DBG_P( "mn_name = [%s]\n", mn_name);
//		(*info)->mtd_name = mn_name;
//		(*info)->mtd = methods[i];
//		info++;
//		i++;
//	}
//}

//static void knh_Actor_readMessage(CTX ctx, knh_Actor_t *a, knh_Object_t *o)
//{
//	//DBG_P( "readMessage\n");
//	knh_Object_t **v = ((knh_ObjectField_t *)o)->fields;
//	knh_String_t *s = (knh_String_t *)v[0];
//	const char *mtd_name = S_totext(s);
//	//DBG_P( "mtd_name = %s\n", mtd_name);
//	knh_Object_t *msg = v[1];
//	//DBG_P( "msg = %p\n", msg);
//	//DBG_P( "sname = %s\n", S_totext(msg->h.cTBL->sname));
//	knh_Actor_invokeMethod(ctx, a, mtd_name, msg);
//}

//static void knh_Actor_mainLoop(CTX ctx, knh_sfp_t *sfp, knh_Actor_t *a)
//{
//	const char *name = DP(a)->actor_name;
//	const char *path = DP(a)->path;
//	int port = DP(a)->port;
//	DBG_P( "======== <<< Actor Information >>> ========\n");
//	DBG_P( "name : %s\n", name);
//	DBG_P( "path : %s\n", path);
//	DBG_P( "port : %d\n", port);
//	DBG_P( "===========================================\n");
//	//int prev_pid = 0;
//	int status;
//	knh_io_t fd = knh_ServerSocket_open(ctx, sfp, port, 3);
//	while (true) {
//		knh_io_t sd = knh_ServerSocket_accept(ctx, sfp, fd);
//		int pid = fork();
//		//DBG_P( "==================\n");
//		//DBG_P( "pid = [%d]\n", pid);
//		if (pid == 0) {
//			//child
//			//DBG_P( "child\n");
//			knh_InputStream_t *ins = knh_Socket_getInputStream(ctx, sd);
//			knh_class_t cid = knh_getcid(ctx, new_bytes("ConnectionObject"));
//			const knh_ClassTBL_t *ct = ClassTBL(cid);
//			knh_Object_t *o = (knh_Object_t *)knh_InputStream_readObject(ctx, ins, ct);
//			//DBG_P( "mtd_name = %s\n", c->mtd_name);
//			//knh_Object_t *msg = c->msg;
//			knh_Actor_readMessage(ctx, a, o);
//			knh_InputStream_close(ctx, ins);
//			exit(0);
//		} else if (pid < 0) {
//			DBG_P( "fork error\n");
//		} else {
//			//DBG_P( "PARENT: pid = [%d]\n", pid);
//			waitpid(pid, &status, 0);
//			//prev_pid = pid;
//		}
//		close(sd);
//	}
//}

//static knh_Actor_t *knh_Actor_new(CTX ctx)
//{
//	knh_Actor_t *actor = (knh_Actor_t *)KNH_MALLOC(ctx, sizeof(knh_Actor_t));
//	knh_ActorEX_t *b = (knh_ActorEX_t *)KNH_MALLOC(ctx, sizeof(knh_ActorEX_t));
//	actor->mtd_info = (knh_MethodInfo_t **)KNH_MALLOC(ctx, sizeof(knh_MethodInfo_t *) * MAX_KMETHOD_NUM);
//	int i = 0;
//	for (i = 0; i < MAX_KMETHOD_NUM; i++) {
//		actor->mtd_info[i] = (knh_MethodInfo_t *)KNH_MALLOC(ctx, sizeof(knh_MethodInfo_t));
//	}
//	actor->mailbox = knh_MailBox_new(ctx);
//	actor->b = b;
//	return actor;
//}

//static bool knh_Actor_existsWorkingDir(const char *dir_name)
//{
//	DIR *dir = opendir(dir_name);
//	bool ret = (dir != NULL) ? true : false;
//	return ret;
//}

//static bool knh_Actor_changeToWorkingDir(const char *dir_name)
//{
//	int ret = chdir(dir_name);
//	return (ret == 0) ? true : false;
//}

//#define MAX_SCRIPT_SIZE 4096

////====================@implemented by yoan ======================>
//static void knh_Actor_addIndent(char* buf, const char* src)
//{
//	size_t i = 0, prev_count = 1, bracket_count = 1;
//	size_t c = 1, src_count = strlen(src);
//	char line[256], prev = 0, t = src[1];
//	while (c <= src_count) {
//		if (t == '{') {
//			bracket_count++;
//		} else if (t == '}') {
//			bracket_count--;
//		}
//		if (prev == '\n') {
//			memset(buf, '\t', prev_count);
//			buf += prev_count;
//			strncpy(buf, line, i);
//			buf += i;
//			memset(line, '\0', i);
//			i = 0;
//			prev_count = bracket_count;
//		}
//		line[i++] = t;
//		prev = t;
//		t = src[++c];
//	}
//}
////<===============================================================
//static const char *knh_Actor_getOriginalScript(CTX ctx, knh_Array_t *a, const char *actor_name)
//{
//	int i = 0;
//	int asize = a->size;
//	const char *script = (const char *)malloc(MAX_SCRIPT_SIZE);
//	memset((char *)script, 0, MAX_SCRIPT_SIZE);
//	char *scr = (char *)script;
//	char src_buf[MAX_SCRIPT_SIZE] = {0};
//	const char* pkg_name = "using konoha.actor.*;\n";
//	snprintf(scr, MAX_SCRIPT_SIZE, "%s", pkg_name);
//	scr += strlen(pkg_name);
//	for (i = 0; i < asize; i++) {
//		knh_Func_t *fo = (knh_Func_t *)a->list[i];
//		knh_Method_t *mtd = fo->mtd;
//		const char *mtd_name = knh_getmnname(ctx, mtd->mn);
//		const char *type_name = S_totext(ClassTBL(DP(mtd)->mp->p0.type)->sname);
//		const char *arg_name = S_totext(knh_getFieldName(ctx, DP(mtd)->mp->p0.fn));
//		const char *src = S_totext(fo->mtd->b->tsource->text);
//		knh_Actor_addIndent(src_buf, src);
//		int wcount = snprintf(scr, MAX_SCRIPT_SIZE - strlen(scr), "\nvoid %s(%s %s)\n{\n%s}\n", mtd_name, type_name, arg_name, src_buf);
//		if (wcount < 0) {
//			DBG_P( "ERROR: too large script size");
//		}
//		scr += wcount;
//		memset(src_buf, '\0', MAX_SCRIPT_SIZE);
//	}
//	snprintf(scr, MAX_SCRIPT_SIZE - strlen(scr), "Actor.act(\"%s\");", actor_name);
//	return script;
//}
//
//static void knh_Actor_activateActor(const char *dir_name, const char *script_name)
//{
//	char buf[128] = {0};
//	int pid = fork();
//	snprintf(buf, 128, "/usr/local/bin/konoha %s/%s", dir_name, script_name);
//	if (pid == 0) {
//		system(buf);
//		exit(0);
//	}
//}

/* ======================================================================== */
// [KMETHODS]

//## @Hidden Actor Actor.opLINK(String path, NameSpace _);
KMETHOD Actor_opLINK(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t host_port = knh_bytes_next(S_tobytes(sfp[1].s), ':');
	knh_bytes_t host;
	knh_index_t idx = knh_bytes_index(host_port, ':');
	if (idx == -1) {
		LOGDATA = {sDATA("path", host_port.text)};
		LIB_Failed("Actor.opLINK", "Script!!");
		knh_Object_toNULL(ctx, sfp[0].o);
		RETURN_(sfp[0].o);
	}
	host = new_bytes2(host_port.text, idx);
	knh_int_t port;
	if (!knh_bytes_parseint(knh_bytes_next(host_port, ':'), &port)) {
		LOGDATA = {sDATA("path", host_port.text)};
		LIB_Failed("Actor.opLINK", "Script!!");
		knh_Object_toNULL(ctx, sfp[0].o);
		RETURN_(sfp[0].o);
	}
	knh_Actor_t *actor = knh_Actor_new(ctx, STEXT(""), host, port);
	DBG_P( "[actor] name=%s", S_totext(actor->name));
	DBG_P( "[actor] host=%s", S_totext(actor->host));
	DBG_P( "[actor] port=%d", actor->port);
	RETURN_(actor);
}



//## String Actor.getName();
KMETHOD Actor_getName(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t*)sfp[0].o;
	RETURN_(actor->name);
}
//## String Actor.getHost();
KMETHOD Actor_getHost(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t*)sfp[0].o;
	RETURN_(actor->host);
}

//## int Actor.getPort();
KMETHOD Actor_getPort(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t *)sfp[0].o;
	RETURNi_(actor->port);
}

//## boolean Actor.setStore(StoreActor store);
KMETHOD Actor_setStore(CTX ctx, knh_sfp_t *sfp _RIX)
{
//	KNH_TODO("Actor.setStore");
}

//## boolean Actor.sendMethod(Func<dynamic=>dynamic> method);
/* KMETHOD Actor_sendMethod(CTX ctx, knh_sfp_t *sfp _RIX) */
/* { */
/* 	KNH_TODO("Actor.sendMethod"); */
/* } */

//## boolean Actor.eval(String script);
/* KMETHOD Actor_eval(CTX ctx, knh_sfp_t *sfp _RIX) */
/* { */
/* 	KNH_TODO("Actor.eval"); */
/* } */

//## void startActor(int port);
/* KMETHOD Script_startActor(CTX ctx, knh_sfp_t *sfp _RIX) */
/* { */
/* 	KNH_TODO("startActor"); */
/* } */

//# StoreActor StoreActor.new(String name, String host, int port);
KMETHOD StoreActor_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Actor_t *actor = (knh_Actor_t *)sfp[0].o;
	if (actor->name == NULL) {
		KNH_INITv(actor->name, sfp[1].s);
	} else {
		KNH_SETv(ctx, actor->name, sfp[1].s);
	}
	if (actor->host == NULL) {
		KNH_INITv(actor->host, sfp[2].s);
	} else {
		KNH_SETv(ctx, actor->host, sfp[2].s);
	}
	actor->port = Int_to(int, sfp[3]);
	DBG_P( "[actor] name=%s", S_totext(actor->name));
	DBG_P( "[actor] host=%s", S_totext(actor->host));
	DBG_P( "[actor] port=%d", actor->port);
	RETURN_(actor);
}


//KMETHOD Actor_act(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	knh_Actor_t *actor = knh_Actor_new(ctx);
//	DP(actor)->actor_name = String_to(const char *, sfp[1]);
//	knh_Actor_addMemcached(ctx, actor);
//	knh_Actor_setMethodInfo(ctx, actor);
//	//knh_Actor_mainLoop(ctx, sfp, actor);
//	RETURNvoid_();
//}

//KMETHOD Actor_send(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	const char *target_name = String_to(const char *, sfp[1]);
//	const char *mtd_name = String_to(const char *, sfp[2]);
//	knh_Object_t *msg = sfp[3].o;
//	const char *path = knh_Actor_getActorPathFromMemcached(target_name);
//	//DBG_P( "path = [%s]\n", path);
//	int size = strlen(path) + 1;
//	int i = 0;
//	for (i = 0; i < size; i++) {
//		if (path[i] == ':') {
//			break;
//		}
//	}
//	//DBG_P( "port = %s\n", path + i + 1);
//	int port = atoi(path + i + 1);
//	knh_io_t sd = knh_Socket_open(ctx, sfp, "127.0.0.1", port);
//	knh_OutputStream_t *ous = knh_Socket_getOutputStream(ctx, sd);
//	knh_class_t cid = knh_getcid(ctx, new_bytes("ConnectionObject"));
//	knh_Object_t *o = new_Object_init2(ctx, ClassTBL(cid));
//	Object **v = (Object **)o->ref;
//	KNH_SETv(ctx, v[0], new_String(ctx, mtd_name));
//	KNH_SETv(ctx, v[1], msg);
//	knh_OutputStream_writeObject(ctx, ous, o);
//	knh_OutputStream_close(ctx, ous);
//	close(sd);
//}

//KMETHOD Actor_spawn(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	const char *actor_name = String_to(const char *, sfp[1]);
//	knh_Array_t *func_list = sfp[2].a;
//	const char *path = S_totext(ctx->script->ns->path->urn);
//	int i, len = strlen(path);
//	char dir[len];
//	char cmd[len + 6];
//	memset(dir, 0, len);
//	memset(cmd, 0, len + 6);
//	for (i = 0; i < len - 2; i++) {
//		dir[i] = path[i];
//	}
//	if (!knh_Actor_existsWorkingDir(dir)) {
//		snprintf(cmd, len + 6, "mkdir %s", dir);
//		system(cmd);
//	}
//	if (!knh_Actor_changeToWorkingDir(dir)) {
//		DBG_P( "ERROR: cannnot change directory\n");
//	}
//	int actor_name_size = strlen(actor_name) + 1;
//	char script_name[actor_name_size + 2];
//	snprintf(script_name, actor_name_size + 2, "%s.k", actor_name);
//	//DBG_P( "script_name = [%s]\n", script_name);
//	FILE *fp;
//	if ((fp = fopen(script_name, "w+")) == NULL) {
//		DBG_P( "ERROR: cannot write to file\n");
//		exit(EXIT_FAILURE);
//	}
//	const char *script = knh_Actor_getOriginalScript(ctx, func_list, actor_name);
//	//DBG_P( "script = [%s]\n", script);
//	fwrite(script, 1, strlen(script), fp);
//	fclose(fp);
//	knh_Actor_activateActor(dir, script_name);
//}

//KMETHOD CActor_sendToScalaActor(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	JNIEnv *jnienv;
//	JavaVM *javavm;
//	JavaVMInitArgs vm_args;
//
//	JavaVMOption options[1];
//	options[0].optionString = "-Djava.class.path=.:/usr/local/scala/lib/scala-library.jar";
//	vm_args.version = JNI_VERSION_1_2;
//	vm_args.options = options;
//	vm_args.nOptions = 1;
//	vm_args.ignoreUnrecognized = true;
//
//	int result = JNI_CreateJavaVM(&javavm, (void **)&jnienv, &vm_args);
//
//	if (result != 0) {
//		DBG_P( "ERROR : [%d] cannot create JavaVM.\n", result);
//	}
//
//	DBG_P( "search Class\n");
//	jclass cls = jnienv->FindClass("ScalaActor");
//	if (cls == 0) {
//		DBG_P( "cannot find ScalaActor\n");
//	}
//	DBG_P( "get Method from Hoge\n");
//	//jmethodID mid = jnienv->GetStaticMethodID(cls, "sendInt", "(Ljava/lang/String;Ljava/lang/String;II)V");
//	jmethodID mid = jnienv->GetStaticMethodID(cls, "sendInt", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/Object;)V");
//	if (mid == 0) {
//		DBG_P( "cannot find sendInt()\n");
//	}
//	printf("call Method.\n");
//	const char *actor_name = String_to(const char *, sfp[1]);
//	const char *host_name = String_to(const char *, sfp[2]);
//	int port = Int_to(int, sfp[3]);
//	int msg = Int_to(int, sfp[4]);
//
//	jstring target = jnienv->NewStringUTF(actor_name);
//	jstring host = jnienv->NewStringUTF(host_name);
//	//jobject obj = jnienv->NewObject(jnienv, targetClass, mid);
//	jnienv->CallStaticVoidMethod(cls, mid, target, host, port, msg);
//	//jobject objResult = jnienv->CallStaticObjectMethod(cls, mid);
//	jthrowable throwResult = jnienv->ExceptionOccurred();
//	if (throwResult != NULL) {
//		DBG_P( "Exception!!\n");
//		jnienv->ExceptionDescribe();
//		jnienv->ExceptionClear();
//	}
//
//	//if (objResult == NULL) {
//	//DBG_P( "ERROR : objResult == NULL\n");
//	//} else {
//	//jstring strResult = (jstring)objResult;
//	//DBG_P( "[%s]\n", jnienv->GetStringUTFChars(strResult, NULL));
//	//}
//	DBG_P( "delete JavaVM\n");
//	result = javavm->DestroyJavaVM();
//	if (result != 0) {
//		DBG_P( "ERROR [%d] : cannot delte JavaVM\n", result);
//	}
//	RETURNvoid_();
//}

/* ======================================================================== */
// [DEFAPI]

DEFAPI(void) defActor(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Actor";
	cdef->init = Actor_init;
	cdef->free = Actor_free;
	knh_NameSpace_setLinkClass(ctx, ctx->share->rootns, STEXT("actor"), ClassTBL(knh_getcid(ctx, STEXT("Actor"))));
}

DEFAPI(void) defStoreActor(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "StoreActor";
	cdef->init = Actor_init;
	cdef->free = Actor_free;
}

#ifdef _SETUP
DEFAPI(const knh_PackageDef_t*) init(CTX ctx, knh_LoaderAPI_t *kapi)
{
	RETURN_PKGINFO("konoha.actor");
}
#endif /* _SETUP */

#ifdef __cplusplus
}
#endif
