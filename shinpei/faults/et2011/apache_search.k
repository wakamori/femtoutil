using konoha.proc.*;

void apache_search() {
    p = new Proc(["/bin/pidof", "apache2"]);
    ins = p.getInputStream();
    while (p.isAlive()) {
		str = ins.readLine();
		OUT << str << EOL;
		if (|str| > 0) {
			pids = str.split(" ");
			foreach (String spid in pids) {
				procinfo = new ProcInfo((int)spid);
				if (procinfo.states["PPid"] == "1") {
					String procfilename = "/proc/" + spid + "/oom_adj";
					OUT << procfilename << EOL;
					ous = new OutputStream(file:: procfilename);
					ous.write((Bytes)"15");
					ous.close();
				}
			}
		}
	}
	OUT << "done... " << EOL;
}

