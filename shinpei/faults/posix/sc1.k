/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

include "1-deletefile.k"

int main (String[] args)
{
	FaultInjector fi = new FaultInjector();
	DeleteFileFault fdFault = new DeleteFileFault("hoge");
	fi.addFaultInstance(fdFault);
	//wait timinig
	fi.loadToLocalTarget();
}
