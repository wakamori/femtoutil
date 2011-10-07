/*
 * 2. file permission
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "2-filepermission.k"

boolean isFaultInject = true;

int main (String[] args)
{
	FilePermissionFaultSetup fs = new FilePermissionFaultSetup();
	String input_file = "hoge.txt";
	fs.setup([input_file]);
 	FaultInjector fi = new FaultInjector();
	FilePermissionFault fpFault = new FilePermissionFault(input_file);
	fi.addFaultInstance(fpFault);
	
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
