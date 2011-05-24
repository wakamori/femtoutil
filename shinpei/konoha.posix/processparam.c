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
//
// **************************************************************************

/* ************************************************************************ */
//#define USE_B
//#include<konoha1.h>

//#include<sys/types.h>
//#include<unistd.h>
//#include<grp.h>


//#ifdef __cplusplus
//extern "C" {
//#endif


//## @Native Map System.environ(void);
METHOD System_environ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = new_(Map);
	knh_NameSpace_t *ns = sfp[1].ns;
	const knh_MapDSPI_t *dspi = knh_NameSpace_getMapDSPI(ctx, ns, B("hash"));
	m->dspi =dspi->config(ctx, CLASS_String, CLASS_String);
	m->map = dspi->init(ctx, 0, dspi->name, NULL);

	extern char **environ;
	char **ret = environ;
	BEGIN_LOCAL(ctx, lsfp, 2);

	int i;

	for (i = 0; ret[i] != NULL; i++) {
		int count = 0;
		while (ret[i][count] != '=') {
		count++;
		}
		ret[i][count] = '\0';
		knh_String_t *key = new_String(ctx, ret[i]);
		knh_String_t *value = new_String(ctx, (ret[i] + count + 1));

		KNH_SETv(ctx, lsfp[0].s, key);
		KNH_SETv(ctx, lsfp[1].s, value);

		m->dspi->set(ctx, m->map, lsfp);
//		knh_Array_add(ctx, a, m);
	}
	END_LOCAL_(ctx, lsfp);
	RETURN_(m);
}

//## @Native Int System.chdir(String path);
METHOD System_chdir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *path = String_to(char*, sfp[1]);
	int ret = chdir(path);
	RETURNi_(ret);
}

//## @Native String System.getcwd(String vuf, Int size);
METHOD System_getcwd(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *buf = String_to(char*, sfp[1]);
	size_t size = Int_to(size_t, sfp[2]);
	RETURN_(new_String(ctx, getcwd(buf, size)));
}

//## @Native String System.ctermid(String s);
METHOD System_ctermid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *s = String_to(char*, sfp[1]);
	RETURN_(new_String(ctx, ctermid(s)));
}

//## @Native Int System.getgid(void);
METHOD System_getegid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getegid());
}

//## @Native Int System.geteuid(void);
METHOD System_geteuid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(geteuid());
}

//## @Native Int System.getgid(void);
METHOD System_getgid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getgid());
}

//## @Native Int getgroups(Int size. Int list[]);
METHOD System_getgroups(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int size = Int_to(int, sfp[1]);
	knh_Array_t *a = sfp[2].a;
	gid_t list[knh_Array_size(a)];
	int i;
	for (i = 0; i  < knh_Array_size(a); i++) {
		list[i] = (gid_t)a->ilist[i];
	}
	RETURNi_(getgroups(size, list));
}

//## @Native String System.getlogin(void);
METHOD System_getlogin(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_String(ctx, getlogin()));
}

//## @Native Int System.getpgrp(voie);
METHOD System_getpgrp(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getpgrp());
}

//## @Native Int System.getpid(void);
METHOD System_getpid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getpid());
}

//## @Native Int System.getppid(void);
METHOD System_getppid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getppid());
}

//## @Native Int System.getuid(void);
METHOD System_getuid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(getuid());
}

//## @Native String System.getenv(String name);
METHOD System_getenv(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *name = String_to(char*, sfp[1]);
	RETURN_(new_String(ctx, getenv(name)));
}

//## @Native Int System.putenv(String string);
METHOD System_putenv(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *string = String_to(char*, sfp[1]);
	RETURNi_(putenv(string));
}

//## @Native Int System.setegid(Int egid);
METHOD System_setegid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	gid_t  egid = Int_to(gid_t, sfp[1]);
	RETURNi_(setegid(egid));
}

//## @Native Int System.seteuid(Int euid);
METHOD System_seteuid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	uid_t euid = Int_to(uid_t, sfp[1]);
	RETURNi_(seteuid(euid));
}

//## @Native Int System.setgid(Int gid);
METHOD System_setgid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	gid_t gid = Int_to(gid_t, sfp[1]);
	RETURNi_(setgid(gid));
}

//## @Native Int System.setgroups(Int size, Int *list);
METHOD System_setgroups(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t size = Int_to(size_t, sfp[1]);
	knh_Array_t *a = sfp[2].a;
	gid_t list[knh_Array_size(a)];
	int i;
	for (i = 0; i < knh_Array_size(a); i++) {
		list[i] = (gid_t)a->ilist[i];
	}
	RETURNi_(setgroups(size, list));
}

//## @Native Int System.setpgrp(void);
METHOD System_setpgrp(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(setpgrp());
}

//## @Native int System.setpgid(Int pid, Int pgid);
METHOD System_setpgid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	pid_t pid = Int_to(pid_t, sfp[1]);
	pid_t pgid = Int_to(pid_t, sfp[2]);
	RETURNi_(setpgid(pid, pgid));
}

//## @Native Int System.setreuid(Int ruid, Int euid);
METHOD System_setreuid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	uid_t ruid = Int_to(uid_t, sfp[1]);
	uid_t euid = Int_to(uid_t, sfp[2]);
	RETURNi_(setreuid(ruid, euid));
}

//## @Native Int System.setregid(Int rgid, Int egid);
METHOD System_setregid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	gid_t rgid = Int_to(gid_t, sfp[1]);
	uid_t egid = Int_to(uid_t, sfp[2]);
	RETURNi_(setregid(rgid, egid));
}

//## @Native Int System.getsid(Int pid);
METHOD System_getsid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	pid_t pid = Int_to(pid_t, sfp[1]);
	RETURNi_(getsid(pid));
}

//## @Native Int System.setsid(void);
METHOD System_setsid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(setsid());
}

//## @Native Int System.setuid(Int uid);
METHOD System_setuid(CTX ctx, knh_sfp_t *sfp _RIX)
{
	uid_t uid = Int_to(uid_t, sfp[1]);
	RETURNi_(setuid(uid));
}

//## @Native String strerror(Int errnum);
METHOD System_strerror(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int errnum = Int_to(int, sfp[1]);
	RETURN_(new_String(ctx, strerror(errnum)));
}

//## @Native Int umask(Int mask);
METHOD System_umask(CTX ctx, knh_sfp_t *sfp _RIX)
{
	mode_t mask = Int_to(mode_t, sfp[1]);
	RETURNi_(umask(mask));
}

//## @Native 
//METHOD System_uname(CTX ctx, knh_sfp_t *sfp _RIX)

//## @Native Int System.unsetenv(String name);
METHOD System_unsetenv(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *name = String_to(char*, sfp[1]);
	RETURNi_(unsetenv(name));
}




//#ifdef _SETUP

//EXPORTAPI(const knh_PackageDef_t*) init(CTX ctx)
//{
//	static const knh_PackageDef_t pkgdef = KNH_PKGINFO("proc", "1.0", "Konoha POSIX Library", NULL);
//	return &pkgdef;
//}


//EXPORTAPI(void) SystemCONST(CTX ctx, const knh_PackageLoaderAPI_t *kapi, knh_NameSpace_t *ns)
//{
//	if(ns == NULL) {
//		kapi->loadIntData(ctx, ns, IntConstData);
//	}
//}

//#endif


/* ------------------------------------------------------------------------ */

//#ifdef __cplusplus
//}
//#endif
