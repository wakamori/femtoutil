/******************************************************************
*KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
*
* Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
*           (c) 2008-      Konoha Software Foundation
* All rights reserved.
*
* You may choose one of the following two licenses when you use konoha.
* See www.konohaware.org/license.html for further information.
*
* (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
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

//include nativelink;

@Native Int System.access(String path, Int mode);
@Native Int System.chdir(String path);
@Native String System.getcwd();
@Native Int System.chroot(String path);
@Native Int System.chmod(String path, Int mode);
@Native Int System.chown(String path, Int owner, Int group);
@Native Int System.lchown(String path, Int owner, Int group);
@Native Int System.link(String oldpath, String newpath);
@Native Int System.listdir(String path);
@Native Int System.pathconf(String path, Int name);
@Native Int System.fpathconf(Int filedes, Int name);
//@Native String System.readlink(String path);
@Native Int System.remove(String path);
@Native Int System.rename(String oldname, String newname);
@Native Int System.rmdir(String path);
@Native String System.tempnam(String dir, String pfx);
@Native String System.tmpnam(String s);
@Native Int System.unlink(String path);

//include func:SystemCONST;
