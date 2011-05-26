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


#include <konoha1.h>

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------*/

METHOD System_confstr(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int name = Int_to(int, sfp[1]);
	char *buf = string_to(char*, sfp[2]);
	size_t len = Int_to(size_t, sfp[3]);
	RETURNi_(confstr(name, buf, len));
}

//METHOD System_confstr_names

METHOD System_getloadavg(CTX ctx, knh_sfp_t *sfp _RIX)
{
	double *loadavg = Float_to(double*, sfp[1]);
	int nelem = Int_to(int, sfp[2]);
	RETURNi_(getloadavg(loadavg, nelem));
}

METHOD System_sysconf(CTX ctx, knh_sfp_t *sfp _RIX)
{
	int name = Int_to(long, sfp[1]);
	RETURNi_(sysconf(name));
}

//METHOD System_sysconf_names

#ifdef _SETUP

static knh_StringData_t StringConstData[] = {
	{"System.CURDIR", "."}, /* todo */
	{"System.PARTDIR", ".."}, /* todo */
	{"System.SEP", "/"}, /* todo */
	{"System.ALTSEP", ""},
	{"System.EXTSEP", ""},
	{"System.PATHSEP", ""},
	{"System.DEFPATH", ""},
	{"System.LINESEP", ""},
	{"System.DEVNULL", ""}
};

EXPORTAPI(const knh_PackageDef_t*) init(CTX ctx)
{
	static const knh_PackageDef_t pkgdef = KNH_PKGINFO("sysinfo","1.0", "Konoha POSIX Library", NULL);
	return &pkgdef;
}

EXPORTAPI(void) SystemCONST(CTX ctx, const knh_PackageLoaderAPI_t *kapi, knh_NameSpace_t *ns)
{
	if (ns == NULL) {
		kapi->loadStringData(ctx, ns, StringConstData);
	}
}

#endif
