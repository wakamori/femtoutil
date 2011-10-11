/*
 * 3. itssymlink
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
include "faultsetup.k";

class ItsSymlinkFaultSetup extends FaultSetup {
	ItsSymlinkFaultSetup () { }
	@Overrite boolean setup (String[] args) {
		String linked_file = args[0];
		String link_file = args[1];
		this.createFile(linked_file, 1024);
		System.symlink(file:: linked_file, file:: link_file);
		return true;
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
