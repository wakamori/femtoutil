/*
 * 2. deletefile
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class FilePermissionFaultSetup extends FaultSetup {
	FilePermissionFaultSetup () {

	}

	@Override boolean setup (String[] args) {
		String input_file = args[0];
		this.createFile(input_file, 1024);
		return true;
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
