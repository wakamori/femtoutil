
class ProcInfo {
    int mypid;
    Map states;
    ProcInfo(int pid) {
	states = {};
	mypid = pid;
	procfilename = "/proc/" + (String)pid + "/status";
	ins = new InputStream(file:: procfilename);
	foreach (String line in ins) {
	    m = line.split(":");
	    key = m[0].trim();
	    value = m[1].trim();
	    states[key] = value;
	}
    }
}

      
