/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

include "faultinjector.k";
using konoha.posix.*;

class DeleteFileFault extends FaultInstance {
	String filename;
	DeleteFileFault (String fname = "") {
		faultName = "FileDelete";
		filename = fname;
	}

	@Override boolean inject (void) {
		print filename;
		if (filename != "") {
			return System.unlink(filename);
		}
		return false;
	}
}
