/*
 * 3. itssymlink
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class ItsSymlinkFaultSetup extends FaultSetup {
	String linked_file;
	String link_file;

	ItsSymlinkFaultSetup () {
		linked_file = "";
		link_file = "";
	}

	@Override boolean setup (String[] args) {
		linked_file = args[0];
		link_file = args[1];
		this.createFile(linked_file, 1024);
		System.symlink(file:: linked_file, file:: link_file);
		return true;
	}

	@Override boolean wrapup () {
		return System.unlink(file:: link_file);
	}
}

class ItsSymlinkFault extends FaultInstance {
	String filename;
	ItsSymlinkFault (String fname = "") {
		faultName = "ItsSymlink";
		filename = fname;
	}

	@Override boolean inject (void) {
		if (filename != "") {
			return System.unlink(filename);
		}
		return false
	}
}
