

#!/usr/local/bin/konoha

using konoha.math.*;

include "logstream.k";
include "logdata.k";

/* ------------------------------------------- */
/* [parameters] */

CSV_MODE = true;

/* ------------------------------------------- */

class ObjData {
	int oidx;
	String ptr;
	int livetime;
	int cid;
}

class ObjectLivetimeStreamAnalyzer extends StreamAnalyzer {

	LogRawData[] logdata;
	Map<String, ObjData> map_objdata;
	Map<String,int> livetime;
	Array<String> classnames;

	ObjectLivetimeStreamAnalyzer () {
		livetime = {};
		logdata = [];
		classnames = [];
		map_objdata = {};
	}

	@Override void analyze (String line) {
		
		
	}
	
	@Override void doneStreamAnalyze (OutputStream ous) {
		OUT.println("done analyze, start post-process...");
		foreach (String ptr in map_objdata.keys()) {
			ObjData d =(ObjData) map_objdata[ptr];
			ous <<< d.oidx <<< "," <<< d.livetime <<< "," <<< classnames[d.cid] <<< EOL;
		}
		ous.flush();
	}
}

/* calc.k */
class MyFilter extends StreamFilter {
	@Override String filter (String line) {
		return "";
	}
}

int main (String[] args)
{
	String filename = "log.txt";
	if (|args| < 1) {
		OUT <<< "warining!! you need logfile name." <<< EOL;
		OUT <<< "usage : konoha calk.k $(logfile)" <<< EOL;
	} else {
		filename = args[0];
	}
	OUT <<< "LAF: v0.1, " <<< EOL;
	OUT <<< "using file: " <<< filename <<<", start streaming..." <<< EOL;

	String filter_name = "myfilter";
	OutputStream ous;
	String prefix = "objlive.";
	String suffix = ".txt";

	if (CSV_MODE == true) {
		suffix = ".csv";
	}
//	print prefix + filename + suffix;

	ous = new OutputStream(prefix + filename + suffix, "w");
	StreamAnalyzer ana = new ObjectLivetimeStreamAnalyzer();
	los = new LogStream(filename);
//	los.addFilter(filter);
	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();
	return 0;
}
