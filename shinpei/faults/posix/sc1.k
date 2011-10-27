/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "1-deletefile.k"

boolean isFaultInject = true;

int main (String[] args)
{
	DeleteFileFaultSetup fs = new DeleteFileFaultSetup();
	String input_file = "hoge.txt";
	fs.setup([input_file]);
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
	fs.wrapup();
	return 0;
}
