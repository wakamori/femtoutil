/******************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 ** Copyright (c)  2010-      Konoha Team konohaken@googlegroups.com
 ** All rights reserved.
 **
 ** You may choose one of the following two licenses when you use konoha.
 ** See www.konohaware.org/license.html for further information.
 **
 ** (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 ** (2) Konoha Software Foundation License 1.0
 * *
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 ** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 ** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 ** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 ** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 ** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **
 *****************************************************************************/

// **************************************************************************
// LIST OF CONTRIBUTERS
//  kimio - Kimio Kuramitsu, Yokohama National University, Japan
//
// **************************************************************************

/* ************************************************************************ */

/*
#include<konoha1.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

*/
/* ======================================================================== */
/* ConstData */
/*
   static knh_IntData_t IntConstData[] = {
   {"System.F_OK", F_OK},
   {"System.R_OK", R_OK},
   {"System.W_OK", W_OK},
   {"System.X_OK", X_OK},
   {"System.S_ISUID",S_ISUID},
   {"System.S_ISGID",S_ISGID},
//	{"System.S_ENFMT",S_ENFMT},
{"System.S_ISVTX",S_ISVTX},
{"System.S_IREAD",S_IREAD},
{"System.S_IWRITE",S_IWRITE},
{"System.S_IEXEC",S_IEXEC},
{"System.S_IRWXU",S_IRWXU},
{"System.S_IRUSR",S_ISUID},
{"System.S_IWUSR",S_IWUSR},
{"System.S_IXUSR",S_IXUSR},
{"System.S_IRWXG",S_IRWXG},
{"System.S_IRGRP",S_IRGRP},
{"System.S_IWGRP",S_IWGRP},
{"System.S_IXGRP",S_IXGRP},
{"System.S_IRWXO",S_IRWXO},
{"System.S_IROTH",S_IROTH},
{"System.S_IWOTH",S_IWOTH},
{"System.S_IXOTH",S_IXOTH},
{"System.TMP_MAX",TMP_MAX},
{NULL}  // end of const data
};
*/
/* ------------------------------------------------------------------------ */
/* [API] */

//## @Native Int System.access(String path, Int mode);

METHOD System_access(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *path = String_to(char *, sfp[1]);
	int mode = Int_to(int, sfp[2]);
	RETURNi_(access(path, mode));
}

//## @Native Int System.chdir(String path);

METHOD System_chdir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(chdir(path));
}


//## @Native String System.getCwd();
METHOD System_getCwd(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char tmpbuf[FILEPATH_BUFSIZ];
	getcwd(tmpbuf, sizeof(tmpbuf));
	RETURN_(new_String(ctx, (const char*)tmpbuf));
}

//## @Native Int System.chroot(String path);

METHOD System_chroot(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(chroot(path));
}

//## @Native Int System.chmod(String path, Int mode);

METHOD System_chmod(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	int mode = Int_to(int, sfp[2]);
	RETURNi_(chmod(path, mode));
}

//## @Native Int System.chown(String path, Int owner, Int group);

METHOD System_chown(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	int owner = Int_to(int, sfp[2]);
	int group = Int_to(int, sfp[3]);
	RETURNi_(chown(path, owner, group));
}

//## @Native Int System.lchown(String path, Int owner, Int group);

METHOD System_lchown(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	int owner = Int_to(int, sfp[2]);
	int group = Int_to(int, sfp[3]);
	RETURNi_(lchown(path, owner, group));
}

//## @Native Int System.link(String oldpath, String newpath);

METHOD System_link(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *oldpath = String_to(const char *, sfp[1]);
	const char *newpath = String_to(const char *, sfp[2]);
	RETURNi_(link(oldpath, newpath));
}

//## @Native Int System.listdir(String path);

METHOD System_listdir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(listdir(path));
}

//## @Native Int System.lstat(String restrictpath, struct stat *restrict buf);
//
//METHOD System_lstat(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	const char *restrictpath = String_to(const char *, sfp[1]);
//	struct stat *restrict buf = String_to(struct stat *restrict, sfp[2]);
//	RETURNi_(lstat(restrictpath, buf));
//}

//## @Native Int System.pathconf(String path, Int name);

METHOD System_pathconf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	int name = Int_to(int, sfp[2]);
	RETURNi_(pathconf(path, name));
}

//## @Native Int System.fpathconf(int filedes, Int name);

METHOD System_fpathconf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int filedes = Int_to(int, sfp[1]);
	int name = Int_to(int, sfp[2]);
	RETURNi_(fpathconf(filedes, name));
}

//## @Native Int System.readlink(String path, String buf, Int bufsize);
//
//METHOD System_readlink(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//const char *restrict path = String_to(const char *restrict, sfp[1]);
//char *restrict buf = String_to(char *restrict, sfp[2]);
//int bufsize = Int_to(int, sfp[3]);
//RETURNi_(readlink(path, buf, bufsize));
//}

//## @Native Int System.remove(String path);

METHOD System_remove(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(remove(path));
}

//## @Native Int System.rename(String oldname, String newname);

METHOD System_rename(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *oldname = String_to(const char *, sfp[1]);
	const char *newname = String_to(const char *, sfp[2]);
	RETURNi_(rename(oldname, newname));
}

//## @Native Int System.rmdir(String path);

METHOD System_rmdir(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(rmdir(path));
}

//## @Native String System.tempnam(String dir, String pfx);

METHOD System_tempnam(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *dir = String_to(const char *, sfp[1]);
	const char *pfx = String_to(const char *, sfp[2]);
	RETURN_(new_String(ctx ,tempnam(dir, pfx)));
}

//## @Native String System.tmpnam(String s);

METHOD System_tmpnam(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char *s = String_to(char *, sfp[1]);
	RETURN_(new_String(ctx ,tmpnam(s)));
}

//## @Native Int System.unlink(String path);

METHOD System_unlink(CTX ctx, knh_sfp_t *sfp _RIX)
{
	const char *path = String_to(const char *, sfp[1]);
	RETURNi_(unlink(path));
}
/*
#ifdef _SETUP

EXPORTAPI(const knh_PackageDef_t*) init(CTX ctx)
{
static const knh_PackageDef_t pkgdef = KNH_PKGINFO("posix2", "1.0", "Konoha POSIX Library 2", NULL);
return &pkgdef;
}

EXPORTAPI(void) SystemCONST(CTX ctx, const knh_PackageLoaderAPI_t *kapi, knh_NameSpace_t *ns)
{
if(ns == NULL) {
kapi->loadIntData(ctx, ns, IntConstData);
}
}

#endif*/ /* _SETUP */

