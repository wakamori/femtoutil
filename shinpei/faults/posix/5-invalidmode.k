/*
 * 5. invalid mode
 * Shinpei Nakata (c) 2011
 * STATUS : TODO
 */

include "faultinjector.k";
include "faultsetup.k";

class InvalidModeOpenFaultSetup extends FaultSetup {
	InvalidModeOpenFaultSetup () {

	}
	@Override boolean setup (String[] args) {
		filename = args[0];
		createFile(filename, 1024);
		return true;
	}
}

class InvalidModeOpenFault extends FaultInstance {
	String filename;
	DeleteFileFault (String fname = "") {
		faultName = "FileDelete";
		filename = fname;
	}

	@Override boolean inject (void) {
		if (filename != "") {
			return System.unlink(filename);
		}
		return false;
	}
}
