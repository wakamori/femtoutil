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
#define USE_cwb_open      1

#include"commons.h"

#if K_USING_WIN32_
#include <winsock2.h>
#define _WIN32_DCOM

#ifdef K_USING_MINGW
#include <windows.h>
#include <winsock.h>
#include <process.h>
#else
#include <comdef.h>
#include <Wbemidl.h>
#include "atlstr.h"
#endif

#pragma comment(lib, "wbemuuid.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/sysctl.h>
#include <unistd.h>
#endif
#ifdef K_USING_MACOSX_
#include <sys/sysctl.h>
#endif
#ifdef K_USING_LINUX_
#include <sys/sysinfo.h>
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define CPU_NAME "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
#define CPU_NAME "i386"
#elif defined(__amd64__) || defined(_M_AMD64)
#define CPU_NAME "amd64"
#else
#define CPU_NAME "unknown"
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [security alert] */

#ifdef K_USING_SECURITY_ALERT
#define UPDATE_HOST "konoha.sourceforge.jp"
#define PORT 80
#define BUF_LEN 4096
#ifdef K_PREVIEW
#define UPDATE_PATH "/cgi-bin/security-alert/server?dist=%s&ver=%s&arch=%s&cpu=%s&rev=%d&clock=%u&mem=%u&ncpu=%d&preview=yes"
#else
#define UPDATE_PATH "/cgi-bin/security-alert/server?dist=%s&ver=%s&arch=%s&cpu=%s&rev=%d&clock=%u&mem=%u&ncpu=%d"
#endif

///* ------------------------------------------------------------------------ */
//
//#define CHANGE_COLOR(out, color) fprintf(out, "\x1b[%dm", color)
//
//enum {
//	BLACK = 30,
//	RED,
//	GREEN,
//	YELLOW,
//	BLUE,
//	MAGENTA,
//	SYAN,
//	GRAY,
//	DEFAULT,
//	WHITE,
//};

//static void getmessage(int fd, char* path)
//{
//	char buf[BUF_LEN];
//	knh_snprintf(buf, sizeof(buf),
//		"GET %s HTTP/1.0\r\n"
//		"Host: %s:%d\r\n\r\n", path, UPDATE_HOST, PORT);
//	DBG_(if(knh_isSystemVerbose()) {
//		fprintf(stdout, "Sending to %s\n--->\n%s\n<---\n", UPDATE_HOST, buf);
//	})
//	if(send(fd, buf, strlen(buf), 0) != -1) {
//		knh_bzero(buf, sizeof(buf));
//		if(recv(fd, buf, sizeof(buf), 0) == -1) {
//			const char *version_str;
//			if ((version_str = strstr(buf, "\r\n\r\nMESSAGE:")) != 0 ){
//				size_t len;
//				char *str;
//				version_str += strlen("\r\n\r\nMESSAGE:");
//				len = strlen(version_str) + 1;
//				str = (char *)malloc(strlen(version_str));
//				strncpy(str, version_str, len);
//				knh_setSecurityAlertMessage(str, 1);
//			}
//		}
//	}
//	else {
//		DBG_(if(knh_isSystemVerbose()) {
//			fprintf(stdout, "** FAILED **\n");
//		})
//	}
//}
//
//static void serverconnect(char *path)
//{
//	struct hostent *servhost;
//	struct sockaddr_in server = {0};
//	int fd;
//
//#ifdef K_USING_WIN32_
//	WSADATA data;
//	WSAStartup(MAKEWORD(2,0), &data);
//#endif
//	servhost = gethostbyname(UPDATE_HOST);
//	if (servhost == NULL) {
//		DBG_P("[%s] Failed to convert Name to IPAdress \n", UPDATE_HOST);
//		return;
//	}
//	server.sin_family = AF_INET;
//	memcpy(&server.sin_addr, servhost->h_addr,  servhost->h_length);
//	server.sin_port = htons(PORT);
//	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//		DBG_P("[%s] Failed to make socket\n", UPDATE_HOST);
//		return;
//	}
//	if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
//		DBG_P("[%s] Failed to connect\n", UPDATE_HOST);
//		return;
//	}
//	getmessage(fd, path);
//
//#ifdef K_USING_WIN32_
//	closesocket(fd);
//	WSACleanup();
//#else
//	close(fd);
//#endif
//	return;
//}
//
///* ------------------------------------------------------------------------ */
//
//static int getncpu(void)
//{
//	int ncpu;
//#ifdef K_USING_MACOSX_
//	int mib[2] = { CTL_HW, HW_NCPU };
//	size_t len = sizeof(int);
//	if (sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1) {
//		/* Error */ ncpu = 1;
//	}
//#elif defined(K_USING_WIN32_)
//	CAtlString strMessage;
//	SYSTEM_INFO sysInfo;
//	GetNativeSystemInfo(&sysInfo);
//	ncpu = (int)sysInfo.dwNumberOfProcessors;
//#elif defined(K_USING_LINUX_)
//	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
//#else
//	TODO();
//#endif
//	return ncpu;
//}
//
///* ------------------------------------------------------------------------ */
//
//#define ONE_MB (1024 * 1024)
//static unsigned int getmem(void)
//{
//	unsigned int mem = 0;
//#ifdef K_USING_MACOSX_
//	int mem_sels[2] = { CTL_HW, HW_PHYSMEM };
//	size_t length = sizeof(int);
//	sysctl(mem_sels, 2, &mem, &length, NULL, 0);
//	mem = (unsigned int) mem / ONE_MB;
//#elif defined(K_USING_WIN32_) && !defined(K_USING_MINGW)
//	MEMORYSTATUSEX stat;
//	GlobalMemoryStatusEx(&stat);
//	mem = (unsigned int) stat.ullTotalPhys / ONE_MB;
//#elif defined(K_USING_LINUX_)
//	mem = (unsigned int) getpagesize() * sysconf (_SC_PHYS_PAGES) / ONE_MB;
//#else
//	TODO();
//#endif
//	return mem;
//}
//
///* ------------------------------------------------------------------------ */
//
//static unsigned int getclock(void)
//{
//	unsigned int clock;
//#ifdef K_USING_MACOSX_
//	int cpu_sels[2] = { CTL_HW, HW_CPU_FREQ };
//	size_t len = sizeof(int);
//	sysctl(cpu_sels, 2, &clock, &len, NULL, 0);
//	clock = (unsigned int) clock / (1000 * 1000);
//#elif defined(K_USING_WIN32_) && !defined(K_USING_MINGW)
//	HRESULT hres;
//	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
//	hres = CoInitializeSecurity(
//		NULL,
//		-1,                          // COM authentication
//		NULL,                        // Authentication services
//		NULL,                        // Reserved
//		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
//		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
//		NULL,                        // Authentication info
//		EOAC_NONE,                   // Additional capabilities
//		NULL                         // Reserved
//		);
//	IWbemLocator *pLoc = NULL;
//	hres = CoCreateInstance(
//		CLSID_WbemLocator,
//		0,
//		CLSCTX_INPROC_SERVER,
//		IID_IWbemLocator, (LPVOID *) &pLoc);
//	IWbemServices *pSvc = NULL;
//	hres = pLoc->ConnectServer(
//		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
//		NULL,                    // User name. NULL = current user
//		NULL,                    // User password. NULL = current
//		0,                       // Locale. NULL indicates current
//		NULL,                    // Security flags.
//		0,                       // Authority (e.g. Kerberos)
//		0,                       // Context object
//		&pSvc                    // pointer to IWbemServices proxy
//		);
//	hres = CoSetProxyBlanket(
//		pSvc,                        // Indicates the proxy to set
//		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
//		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
//		NULL,                        // Server principal name
//		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
//		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
//		NULL,                        // client identity
//		EOAC_NONE                    // proxy capabilities
//		);
//	IEnumWbemClassObject* pEnumerator = NULL;
//	hres = pSvc->ExecQuery(
//		bstr_t("WQL"),
//		bstr_t("SELECT * FROM Win32_Processor"),
//		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
//		NULL,
//		&pEnumerator);
//	IWbemClassObject *pclsObj;
//	ULONG uReturn = 0;
//
//	while (pEnumerator) {
//		HRESULT hr = pEnumerator->Next(
//			WBEM_INFINITE,
//			1,
//			&pclsObj,
//			&uReturn
//			);
//		if (uReturn == 0) { break; }
//		VARIANT vtProp;
//		hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
//		clock = (unsigned int)vtProp.bstrVal;
//		VariantClear(&vtProp);
//		pclsObj->Release();
//	}
//	pSvc->Release();
//	pLoc->Release();
//	pEnumerator->Release();
//	CoUninitialize();
//#elif defined(K_USING_LINUX_)
//	char buf[64] = {'\0'}, *data = buf;
//	const char *cpumhz = "cpu MHz";
//	size_t len = strlen(cpumhz);
//	FILE *fp = fopen("/proc/cpuinfo","r");
//	while (fgets( buf, 64, fp ) != NULL){
//		if ( strncmp(buf, cpumhz, len) == 0){
//			data = strstr(buf, cpumhz);
//			break;
//		}
//	}
//	while (!isdigit(*data)) {
//		data++;
//	}
//	clock = (unsigned int) atoi(data) / 1000;
//	fclose(fp);
//
//#else
//	TODO();
//#endif
//	return clock;
//}

/* ------------------------------------------------------------------------ */

//static void *knh_checkSecurityAlert(void * ptr)
//{
//	char path[BUF_LEN] = {'\0'};
//	unsigned int clock = getclock();
//	unsigned int mem   = getmem();
//	int ncpu  = getncpu();
//	knh_snprintf(path, 512, UPDATE_PATH,
//				 K_DISTTYPE, K_VERSION, CC_PLATFORM,
//		     CPU_NAME, K_REVISION, clock, mem, ncpu);
//	DBG_P("Path == [%s] \n", path);
//	serverconnect(path);
//	return NULL;
//}

void knh_askSecurityAlert(CTX ctx)
{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_Bytes_write(ctx, cwb->ba, S_tobytes(knh_getPropertyNULL(ctx, STEXT("user.path"))));
//	knh_Bytes_putc(ctx, cwb->ba, '/');
//	knh_Bytes_write(ctx, cwb->ba, STEXT("ALLOWED_SECURITY_ALERT"));
//	if(!knh_path_isfile(ctx, cwb)) {
//		char buf[80];
//		fprintf(stdout,
//"IMPORTANT: For improving Konoha experience and delivering security updates,\n"
//"Konoha development team is collecting the following information:\n"
//"\tversion: version=%s distribution=%s revision=%d\n"
//"\tsystem: %s %dbits LANG=%s\n"
//"DO YOU ALLOW? [y/N]: ",
//		K_VERSION, K_DISTTYPE, (int)K_REVISION, CC_PLATFORM, (int)(sizeof(void*) * 8), knh_getenv("LANG"));
//		if(fgets(buf, sizeof(80), stdin) != NULL) {
//			if((buf[0] == 'y' || buf[0] == 'Y') && (buf[1] == 0 || buf[1] == '\n' || buf[1] == '\r')) {
//				FILE *fp = fopen(knh_cwb_tochar(ctx, cwb), "a");
//				if(fp != NULL) {
//					fclose(fp);
//				}
//				fprintf(stdout, "Thank you for using Konoha!!\n");
//				goto L_CHECK;
//			}
//		}
//		knh_cwb_close(cwb);
//		return;
//	}
//	L_CHECK:;
//	knh_cwb_close(cwb);
//	{
//		knh_thread_t th;
//		knh_thread_create(ctx, &th, NULL, knh_checkSecurityAlert, (void*)ctx);
//		knh_thread_detach(ctx, th);
//	}
}
#endif

/* ------------------------------------------------------------------------ */
/* [password] */

//static int secureMode = 0;

/* ------------------------------------------------------------------------ */

//void knh_setSecureMode(void)
//{
//	secureMode = 1;
//}

/* ------------------------------------------------------------------------ */

//knh_bool_t knh_isTrustedPath(CTX ctx, knh_bytes_t path)
//{
//	DBG_P("check: %s", (char*)path.buf);
//	if(knh_bytes_startsWith(path, STEXT("http:"))) {
//		return 0;
//	}
//	return (secureMode != 1);
//}

/* ------------------------------------------------------------------------ */
/* [password] */

//const char* knh_getPassword(CTX ctx, knh_bytes_t url)
//{
//	return "password";
//}

/* ------------------------------------------------------------------------ */
/* [Trusted] */

void knh_checkSecurityManager(CTX ctx, knh_sfp_t *sfp)
{
//	/* VERY SLOW */
//	knh_sfp_t *sp = sfp - 2;
//	while(ctx->stack < sp) {
//		if(IS_Method(sp[0].mtd)) {
//			if(!URI_ISTRUSTED(DP(sp[0].mtd)->uri)) {
//				char buf[FILEPATH_BUFSIZ];
//				knh_snprintf(buf, sizeof(buf), "Security!!: untrusted domain='%s'", URI__(DP(sp[0].mtd)->uri));
//				KNH_THROW__T(ctx, buf);
//			}
//		}
//		sp--;
//	}
}

/* ------------------------------------------------------------------------ */

//knh_bool_t knh_isTrustedHost(CTX ctx, knh_bytes_t host)
//{
//	TODO();
//	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
//	int res = knh_DictMap_index__b(DP(ctx->sys)->trustedHostDictMap, host);
//	if(res != -1) return 1;
//	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//	return 0;
//}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
