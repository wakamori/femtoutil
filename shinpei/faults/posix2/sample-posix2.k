/*
 * sample-posix, fopen
 * Shinpei Nakata (c) 2011
 */

using konoha.posix.*;
include "libfi.k";

int main (String[] args) {
	Path fpth = null;
	String filename = "";
	if (|args| > 0) {
		filename = args[0];
		fpth = file::filename;
	}
	faultinjection(12);
	DFile f = System.fopen(fpth, "r");
	faultremove();
	int bufsiz = 128;
	Bytes buf = new byte[bufsiz];
	int total_readsize = 0;
	while (total_readsize < 1024) {
		int readsize = 0;
		readsize = f.read(buf, 0, bufsiz);
		if (readsize == 0) break;
		total_readsize += readsize;
		print buf.decode();
	}
	return 0;
}
