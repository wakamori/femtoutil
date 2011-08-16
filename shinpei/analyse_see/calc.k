#!/usr/local/bin/konoha

using konoha.math.*;

class Filter {
	String name;
	Func filterFunc;

	Filter(String fname, Func f = null) {
		_name = fname;
		_filterFunc = f;
	}

	@Virtual String filter (String line) {
		return "answer\n";
	}

}

class StreamFilter extends Filter;

class SamplingFilter extends Filter;

class StreamAnalyzer {
	@Virtual void analyze (String line);
	@Virtual void doneStreamAnalyze(OutputStream ous);
}

class LogStream {
	@Private InputStream ins;
	@Private OutputStream ous;
	String filename;
	Array<Filter> filters;
	Array<StreamAnalyzer> analyzers;

	LogStream(String fn) {
		_filename = fn;
		_ins = new InputStream(_filename, "r");
		_filters = null;
		_analyzers = null;
	}

	void setOus (OutputStream o) {
		_ous = o;
	}

	void addFilter (Filter f) {
		_filters.add(f);
	}

	void addStreamAnalyzer (StreamAnalyzer a) {
		_analyzers.add(a);
	}

	String nextLine () {
		return _ins.readLine();

	}

	void doFilter(String filterName) {
//		Filter f = _filters[0];
		StreamAnalyzer a = _analyzers[0];
		String ret, line;

		if (a != null) {
			while (true) {
				line = nextLine();
				if (line != null) {
					//ret = f.filter(line);
					ret = a.analyze(line);
				} else {
					break;
				}
			}
			a.doneStreamAnalyze(_ous);
		}
	}

	void destroy () {
		ins.close();
		ous.close();
	}
}


class AnalyzeData;

class LogRawData extends AnalyzeData {
	String event;
    int time;
	int address;
	String s_address;
	int i_data;
	String s_data;

	LogRawData(String ev) {
		_event = ev;
	}
}

class FastMallocData extends LogRawData;
class FastFreeData extends LogRawData;
class FastReallocData extends LogRawData;
class VallocData extends LogRawData;
class VfreeData extends LogRawData;
class CreateObjectData extends LogRawData;
class DeleteObjectData extends LogRawData;

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
	float live_sumtime;
	float variance_live;
	String[] livelog;
	DataMapStreamAnalyzer () {
		livetime = {};
		classcount = {};
		livelog = [];
		live_sumtime = 0.0;
		variance_live = 0.0;
	}

	@Override void analyze (String line) {
		/* first make class */
		if (line.startsWith("[new_Object]") == true) {
			data = new LogRawData("createobj");
			int offset = |"[new_Object] T"|;
			int lastidx = |line| - 1;
			substr = line[offset to lastidx];
			String[] map = substr.split(",");
			data.time = (int)map[0];
			String[] a = map[1].replace(" ", "").split("=");
			data.s_address = a[1];
			a  = map[2].replace(" ", "").split("=");
			data.s_data = a[1];
			createobj++;
			String s = data.s_address;
			String classname = data.s_data;
			livetime[s] = data.time
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
			live_sumtime += livetime[data.s_address];
			livelog.add(str);
//			livelog.add("livetime:class: " + data.s_data + ", time=" + str);
			return;
		}
	}
	
	@Override void doneStreamAnalyze (OutputStream ous) {
		print "done analyze, start post process...";
		float average_live = live_sumtime / createobj;
		OUT<<< "Avarage  live-time: " <<< average_live <<< EOL;
		print classcount;
		for (int i = 0; i < |livelog|; i++) {
			time = (int)livelog[i];
			variance_live += Math.pow((time - average_live), 2.0);
		}
		variance_live /= (createobj - 1);
		OUT <<< "Variance: " <<< variance_live <<< EOL;
		float standard_deviation = Math.sqrt(variance_live);
		OUT <<< "Deviation(std): " <<< standard_deviation <<< EOL;
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
	ous = new OutputStream("result.txt", "w");
//	Filter filter = new MyFilter(filter_name);
	StreamAnalyzer ana = new DataMapStreamAnalyzer();
//	StreamAnalyzer ana = new StartsWithStreamAnalyzer();
//	StreamAnalyzer ana = new IndexOfStreamAnalyzer();
	los = new LogStream(filename);
//	los.addFilter(filter);
	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();
	return 0;
}
