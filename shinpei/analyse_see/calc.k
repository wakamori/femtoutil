

#!/usr/local/bin/konoha

using konoha.math.*;

include "logstream.k";
include "logdata.k";

/* ------------------------------------------- */
/* [parameters] */

CSV_MODE = true;
CLASS_MODE = true;
/* ------------------------------------------- */

class DataMapStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	LogRawData[] log_array;
	Map<String,int> livetime;
	Map<String, int> classcount;
	int live_sumtime;
	float variance_live;
	String[] livelog;
	String[] livelog_with_class;
	Map<String, int> live_sumtime_class;

	Array<String> classnames;

	DataMapStreamAnalyzer () {
		livetime = {};
		classcount = {};
		livelog = [];
		livelog_with_class = [];
		live_sumtime = 0;
		variance_live = 0.0;
		live_sumtime_class = {};
		classnames = [];
	}

	@Override void analyze (String line) {
		/* first make class */
		if (line.startsWith("[new]") == true) {
			data = new LogRawData("createobj");
			int offset = |"[new] T"|;
			int lastidx = |line| - 1;
			substr = line[offset to lastidx];
			String[] map = substr.split(",");
			data.time = (int)map[0];
			String[] a = map[1].replace(" ", "").split("=");
			data.s_address = a[1];
			a  = map[2].replace(" ", "").split("=");
			data.s_data = a[1];
			a = map[3].replace(" ", "").split("=");
			data.idx = (int)a[1];
			createobj++;
			String s = data.s_address;
			String classname = data.s_data;
			int idx = data.idx;
			livetime[s] = data.time;
			classcount[classname] += 1;
			return;
		}
		
		if (line.startsWith("[~Object]") == true) {
			data = new LogRawData("deleteobj");
			int offset = |"[~Object] T"|;
			int lastidx = |line| - 1;
			substr = line[offset to lastidx];
			map = substr.split(",");
			data.time = (int)map[0];
			String[] a = map[1].replace(" ", "").split("=");
			data.s_address = a[1];
			a  = map[2].replace(" ", "").split("=");
			data.s_data = a[1];

			delobj++;
			livetime[data.s_address] -= data.time;
			livetime[data.s_address] *= (-1);
			str = (String)(livetime[data.s_address]);
			if (CLASS_MODE != true) {
				live_sumtime += livetime[data.s_address];
				livelog.add(str);
			} else {

				live_sumtime_class[data.s_data] += livetime[data.s_address];
				livelog_with_class.add("class:" + data.s_data + "," + str);
			}
			return;
		}
		if (line.startsWith("[[") == true) {
			int offset = |"[[CLASSNAME]] T"|;
			int lastidx = |line| - 1;
			substr = line[offset to lastidx];
			map = substr.split(",");
			a = map[2].replace(" ", "").split("=");
			String cn = a[1];
			classnames.add(cn);
			return;
		}

	}
	
	@Override void doneStreamAnalyze (OutputStream ous) {
		OUT.println("done analyze, start post-process...");
		if (CLASS_MODE == true) {
			Map<String, float> class_ave = {};
			Map<String, float> class_variance = {};
			/* calculate class average */
			String[] classes = classcount.keys();
			int classnum = |classes|;
			for (i = 0; i < classnum; i++) {
				cn = classes[i];
				class_ave[cn] = live_sumtime_class[cn] / classcount[cn];
				class_variance[cn] = 0.0;
			}
			
			for (int i = 0; i < |livelog_with_class|; i++) {
				str = livelog_with_class[i];
				substr = str[6 to (|str|-1)];
				sa = substr.split(",");
				cn = sa[0]; // classname;
				time = (int)sa[1]; // time;
				class_variance[cn] += Math.pow((time - class_ave[cn]), 2.0);
			}
			ous <<< "classname,count,average,variance" <<< EOL;
			ous.flush();
			for (i = 0; i < classnum; i++) {
				cn = classes[i];
				variance =  class_variance[cn] / (classcount[cn] - 1);
				ous <<< classnames[(int)cn] <<< "," <<< classcount[cn] <<< "," <<< class_ave[cn] <<< "," <<< variance <<< EOL;
			}
			
			ous.flush();
			OUT.println("post-process done. bye...");
			return;
		} 

		if (CSV_MODE == true) {
			ous <<< createobj <<< "," ;
			float average_live = live_sumtime / createobj;
			ous <<< average_live <<< ",";
			for (int i = 0; i < |livelog|; i++) {
				time = (int)livelog[i];
				variance_live += Math.pow((time - average_live), 2.0);
			}
			variance_live /= (createobj - 1);
			ous <<< variance_live <<< ",";
			float standard_deviation = Math.sqrt(variance_live);
			ous <<< standard_deviation <<<  EOL;
			ous.flush();
		} else {
			ous <<< "Created object: " <<< createobj <<< EOL;
			float average_live = live_sumtime / createobj;
			ous <<< "Avarage  live-time: " <<< average_live <<< EOL;
			for (int i = 0; i < |livelog|; i++) {
				time = (int)livelog[i];
				variance_live += Math.pow((time - average_live), 2.0);
			}
			variance_live /= (createobj - 1);
			ous <<< "Variance: " <<< variance_live <<< EOL;
			float standard_deviation = Math.sqrt(variance_live);
			ous <<< "Deviation(std): " <<< standard_deviation <<< EOL;
			ous.flush();
		}
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
	String prefix = "res.";
	String suffix = ".txt";
	if (CLASS_MODE == true) {
		prefix = "class.";
	}
	if (CSV_MODE == true) {
		suffix = ".csv";
	}
//	print prefix + filename + suffix;
	ous = new OutputStream(prefix + filename + suffix, "w");

//	Filter filter = new MyFilter(filter_name);
	StreamAnalyzer ana = new DataMapStreamAnalyzer();
	los = new LogStream(filename);
//	los.addFilter(filter);
	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();
	return 0;
}
