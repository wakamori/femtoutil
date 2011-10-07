/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "1-deletefile.k"
include "faultsetup.k"

boolean isFaultInject = false;

int main (String[] args)
{
	FaultSetup fs = new FaultSetup();
	String input_file = "hoge.txt";
	fs.createFile(input_file, 1024);
 	FaultInjector fi = new FaultInjector();
	DeleteFileFault fdFault = new DeleteFileFault(input_file);
	fi.addFaultInstance(fdFault);
	
	if (isFaultInject) {
		print "injecting...";
		fi.loadToLocalTarget();
	}
	Proc p = new Proc(["/usr/local/bin/konoha", "-a", "sample-posix.k", input_file]);
	ins = p.getInputStream();
	while (p.isAlive()) {
		while ((line = ins.readLine()) != null) {
			OUT << line;
		}
	}
	return 0;
}
