#!/usr/local/bin/konoha

using konoha.math.*;

include "logstream.k";
include "logdata.k";
include "objlivetime.k";
include "classcount.k"
/* ------------------------------------------- */
/* [parameters] */

CSV_MODE = true;
CLASS_MODE = false;
OBJLIVE_MODE = true;

/* ------------------------------------------- */
/* [command optiongs] */

Map<String, String> arg_options = {
	"-a" : "specify analyzer",
}


/* ------------------------------------------- */


int main (String[] args)
{
	String filename = "log.txt";
	String analyzername = "";
	int argc = |args|;
	OUT <<< "LAF: v0.1, " <<< EOL;
	if (argc < 1) {
		OUT <<< """
warining!! you need logfile name.
 Usage : konoha laf.k [options] $logfile

 Options""" <<< EOL;
		foreach(String opt in arg_options.keys()) {
			OUT <<< "  " <<< opt <<< ": "  <<< arg_options[opt] <<< EOL;
		}
		return 0;
	} else { 
		if (argc % 2 == 0) {
			OUT <<< "warining!! you need logfile name." <<< EOL;
			return 0;
		}
		int i;
		for(i=0;i<argc;i++) {
			String arg = args[i];
			if (arg == "-a") {
				analyzername = args[i+1];
				i++;
			}
		}
		filename = args[argc-1];
	}
	OUT <<< "using file: " <<< filename <<<", start streaming..." <<< EOL;


	LogStream los = new LogStream(filename);
	StreamAnalyzer ana;
	String prefix = "";
	String suffix = ".txt";


	if (CLASS_MODE == true) {
		ana = new ClassCountStreamAnalyzer();
	}
	if (OBJLIVE_MODE == true) {
		ana = new ObjectLivetimeStreamAnalyzer();
	}

	if (CSV_MODE == true) {
		suffix = ".csv";
	}

	prefix = a.name; // use analyze name as prefix
	OutputStream ous = new OutputStream(prefix + filename + suffix, "w");

//	Filter filter = new MyFilter(filter_name);
//	los.addFilter(filter);

	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();

	return 0;
}
