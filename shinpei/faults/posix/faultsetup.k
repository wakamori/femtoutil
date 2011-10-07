/*
 * 1. deletefile
 * Shinpei Nakata (c) 2011
 */

class FaultSetup {
	String filename;

	FaultSetup () {
		filename = "";
	}

	void createFile (String fname, int size) {
		filename = fname;
		OutputStream ous = new OutputStream(filename,"w");
		int wrote_bytes = 0;
		while (wrote_bytes < size) {
			ous << "@" ;
			wrote_bytes++;
		}
		ous.close();
	}
}
