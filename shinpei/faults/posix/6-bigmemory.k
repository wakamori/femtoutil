/*
 * 6. big memory
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
		return true;
	}

	@Override boolean wrapup() {
		return true;
	}
}

clsas 
