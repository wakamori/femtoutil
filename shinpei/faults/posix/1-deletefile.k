/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class DeleteFileFaultSetup extends FaultSetup {

	DeleteFileFaultSetup () {

	}

	@Override boolean setup(String[] args) {
		String input_file = args[0];
		this.createFile (input_file, 1024);
		return true;
	}
}


class DeleteFileFault extends FaultInstance {
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
