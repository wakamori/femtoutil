/*
 * 2. file permission
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class FilePermissionFaultSetup extends FaultSetup {
	String input_file;
	FilePermissionFaultSetup () {
		input_file = "";
	}

	@Override boolean setup (String[] args) {
		input_file = args[0];
		this.createFile(input_file, 1024);
		return true;
	}


	@Override boolean wrapup() {
		System.chmod(file:: input_file, 0755);
		return System.unlink(file:: input_file);
	}
}


class FilePermissionFault extends FaultInstance {
	String filename;
	FilePermissionFault (String fname = "") {
		faultName = "FilePermission";
		filename = fname;
	}

	@Override boolean inject (void) {
		if (filename != "") {
			return System.chmod(file:: filename, 0000);
		}
		return false
	}
}
