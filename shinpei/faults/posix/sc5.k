/*
 * 5. invalid mode
 * Shinpei Nakata (c) 2011
 */

using konoha.proc.*;
include "5-invalidmode.k"

boolean isFaultInject = false;

int main (String[] args)
{
	InvalidModeOpenFaultSetup fs = new InvalidModeOpenFaultSetup();
	String input_file = "hoge.txt";
	fs.setup([input_file]);
 	FaultInjector fi = new FaultInjector();
	InvalidModeOpenFault imoFault = new InvalidModeOpenFaultSetup(input_file);
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
