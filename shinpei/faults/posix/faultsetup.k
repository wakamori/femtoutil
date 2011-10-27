/*
 * faultsetup.k
 * Shinpei Nakata (c) 2011
 */

using konoha.posix.*;

class FaultSetup {
	String filename;
	@Virtual boolean setup(String[] args);
	@Virtual boolean wrapup();
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

	void createFileWithPermission(String fname, int mode) {
		filename = fname;
		OutputStream ous = new OutputStream(filename, "w");
		int size = 1024
		createFile(fname, size);
		System.chmod(file:: fname, mode);
	}
}
