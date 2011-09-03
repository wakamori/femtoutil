#!/usr/local/bin/konoha
/* -------------------------------------------
 * laf.k : version 0.1.1
 *  log analysis framework
 *  shinpei(c)2011
 *
 * ------------------------------------------- */

using konoha.math.*;

include "logstream.k";
include "logdata.k";


/* ------------------------------------------- */
/* [command optiongs] */

Map<String, String> arg_options = {
	"-a" : "specify analyzer",
}

/* ------------------------------------------- */


class Laf {
	String input_filename;
	String analyzer_name;

	String prefix;
	String suffix;

	StreamAnalyzer ana;
	LogStream los;
	OutputStream ous;

	Laf () {
		input_filename = "log.txt";
		analyzer_name = "";
		prefix = "";
		suffix = ".csv";
	}

	boolean parseArguments (String[] args) {
		int argc = |args|;
		OUT <<< "LAF: v0.1.1, " <<< EOL;
		if (argc < 1) {
			OUT <<< """
 warining!! you need logfile name.
 Options""" <<< EOL;
			foreach(String opt in arg_options.keys()) {
				OUT <<< "  " <<< opt <<< ": "  <<< arg_options[opt] <<< EOL;
			}
			return false;
		} else { 
			if (argc % 2 == 0) {
				OUT <<< "warining!! you need logfile name." <<< EOL;
				return false;
			}
			int i;
			for(i = 0; i < argc; i++) {
				String arg = args[i];
				if (arg == "-a") {
					analyzer_name = args[i + 1];
					i++;
				}
			}
			input_filename = args[argc - 1];
		}
		OUT <<< "using file: " <<< input_filename <<<", start streaming..." <<< EOL;
		return true;
	}

	void prepare (StreamAnalyzer instance) {
		ana = instance;
	}

	void start ( ) {
		los = new LogStream(input_filename);
		prefix = ana.name + "_"; // use analyze name as prefix
		ous = new OutputStream(prefix + input_filename + suffix, "w");
		OUT <<< "output is " + prefix + input_filename + suffix <<< EOL;
		los.addStreamAnalyzer(ana);
		los.setOus(ous);
		los.doAnalyze();
		los.destroy();
	}
}

