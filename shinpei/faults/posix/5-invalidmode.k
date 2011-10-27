/*
 * 5. invalid mode
 * Shinpei Nakata (c) 2011
 * STATUS : TODO.. we need to modify sample-posix.k
 * and it seems a user-bug.
 */

include "faultinjector.k";
include "faultsetup.k";

class InvalidModeOpenFaultSetup extends FaultSetup {
	String input_file;

	InvalidModeOpenFaultSetup () {
		input_file = "";
	}

	@Override boolean setup (String[] args) {
		input_file = args[0];
		createFile(input_file, 1024);
		return true;
	}
	@Override boolean wrapup() {

	}
}

class InvalidModeOpenFault extends FaultInstance {
	String filename;
	DeleteFileFault (String fname = "") {
		faultName = "InvalidModeOpen";
		filename = fname;
	}

	@Override boolean inject (void) {
		if (filename != "") {
			return System.unlink(filename);
		}
		return false;
	}
}
