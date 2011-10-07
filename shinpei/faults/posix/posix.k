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

include "lib:gluelink";

@Public @Native String System.getHostName();

// user, group
@Public @Native String System.getLogin();
@Public @Native int System.getUid();
@Public @Native int System.getEuid();
@Public @Native int System.getGid();
@Public @Native int System.getEgid();

// process 
@Public @Native int System.getPid();
@Public @Native int System.getPPid();
@Public @Native int System.sleep(int sec);
@Public @Native boolean System.usleep(int sec);

// file system
@Public @Native boolean System.link(Path path, Path path2);
@Public @Native boolean System.symlink(Path path, Path path2);
@Public @Native boolean System.rename(Path path, Path path2);
@Public @Native boolean System.unlink(Path path);

@Public @Native String System.getCwd();
@Public @Native boolean System.chdir(Path path);
@Public @Native boolean System.chroot(Path path);

// DIR
@Public @Native class Dir;
@Public @Native @Throwable Dir System.openDir(Path path);
@Public @Native @Iterative String Dir.readName();
@Public @Native void Dir.close();
@Public @Native @Iterative Map Dir.read();

// FILE
@Public @Native class File;
@Public @Native @Throwable File System.fopen(Path path, String mode);
@Public @Native int File.getc();
@Public @Native int File.read(Bytes buf, int offset, int len);
@Public @Native boolean File.putc(int ch);
@Public @Native int File.write(Bytes buf, int offset, int len);
@Public @Native void File.close();

//include func:SystemCONST;
