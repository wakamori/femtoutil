/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
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

@Native Map System.environ(void);
@Native Int System.chdir(String path);
@Native String System.getcwd(String vuf, Int size);
@Native String System.ctermid(String s);
@Native Int System.getegid(void);
@Native Int System.geteuid(void);
@Native Int System.getgid(void);
@Native Int System.getgroups(Int size, Int list[]);
@Native String System.getlogin(void) 
@Native Int System.getpgrp(void);
@Native Int System.getpid(void);
@Native Int System.getppid(void);
@Native Int System.getuid(void);
@Native String System.getenv(String name);
@Native Int System.putenv(String string);
@Native Int System.setegid(Int egid);
@Native Int System.seteuid(Int euid);
@Native Int System.setgid(Int gid);
@Native Int System.setgroups(Int size, Int *list)
@Native Int System.setpgrp(void);
@Native Int System.setpgid(Int pid, Int pgid);
@Native Int System.setreuid(Int ruid, Int euid);
@Native Int System.setregid(Int rgid, Int egid);
@Native Int System.getsid(Int pid);
@Native Int System.setsid(void);
@Native Int System.setuid(Int uid);
@Native String System.strerror(Int errnum);
@Native Int System.umask(Int mast);
@Native Int System.unsetenv(String name);
