/*
 * 4. open directory
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class OpenDirectoryFaultSetup extends FaultSetup {
	String dirname;
	OpenDirectoryFaultSetup () {
		dirname = "";
	}
	@Override boolean setup(String[] args) {
		dirname = args[0];
		// mkdir
		Proc p = new Proc(["/bin/mkdir", dirname]);
		return true;
	}

	@Override boolean wrapup() {
		Proc p = new Proc(["/bin/rm", "-rf", dirname]);
		return true;
	}
}
