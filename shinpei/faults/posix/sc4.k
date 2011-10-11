/*
 * 4. open directory
 * DOUBT - is this fault? isn't a bug?
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "4-opendirectory.k"

boolean isFaultInject = true;

int main (String[] args)
{
	OpenDirectoryFaultSetup fs = new ItsSymlinkFaultSetup();
	String dirname = "hoge_dir";
	fs.setup([dirname]);

	Proc p = new Proc(["/usr/local/bin/konoha", "-a", "sample-posix.k", dirname]);
	ins = p.getInputStream();
	while (p.isAlive()) {
		while ((line = ins.readLine()) != null) {
			OUT << line;
		}
	}

	fs.wrapup();
	return 0;
}
