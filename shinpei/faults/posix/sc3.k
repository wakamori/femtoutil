/*
 * 3. file symlink
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "3-itssymlink.k"

boolean isFaultInject = true;

int main (String[] args)
{
	ItsSymlinkFaultSetup fs = new ItsSymlinkFaultSetup();
	String linked_file = "hoge_base.txt";
	String link_file = "hoge_link.txt";
	fs.setup([linked_file, link_file]);

 	FaultInjector fi = new FaultInjector();
	ItsSymlinkFault fpFault = new ItsSymlinkFault(linked_file);
	fi.addFaultInstance(fpFault);
	if (isFaultInject) {
		print "injecting...";
		fi.loadToLocalTarget();
	}
	Proc p = new Proc(["/usr/local/bin/konoha", "-a", "sample-posix.k", link_file]);
	ins = p.getInputStream();
	while (p.isAlive()) {
		while ((line = ins.readLine()) != null) {
			OUT << line;
		}
	}
	return 0;
}
