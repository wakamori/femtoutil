/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class DeleteFileFaultSetup extends FaultSetup {

	String input_file;
	DeleteFileFaultSetup () {
		input_file = "";
	}

	@Override boolean setup(String[] args) {
		input_file = args[0];
		this.createFile (input_file, 1024);
		return true;
	}

	@Override boolean wrapup() {
		return System.unlink(file:: input_file);
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
