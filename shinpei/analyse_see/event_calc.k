#!/usr/local/bin/konoha

using konoha.math.*;

/* ------------------------------------------- */
/* [parameters] */

CSV_MODE = true;
CLASS_MODE = false;

/* ------------------------------------------- */
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
			foreach(String line in _ins) {
				ret = a.analyze(line);
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


class StartsWithStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	@Override void analyze (String line) {
		if (line.startsWith("[fastmalloc]") == true) {
			fastmal++;
		}

		if (line.startsWith("[fastfree]") == true) {
			fastfree++;
		}
		if (line.startsWith("[fastrealloc]") == true) {
			fastreal++;
		}

		if (line.startsWith("[valloc]") == true) {
			valloc++
		}

		if (line.startsWith("[vfree]") == true) {
			vfree++;
		}

		if (line.startsWith("[new_Object]") == true) {
			createobj++;
		}

		if (line.startsWith("[~Object]") == true) {
			delobj++;
		}
	}
	
	@Override void doneStreamAnalyze (OutputStream ous) {
		ret = "fastmalloc: " + fastmal + "\n";
		ret += "fastfree: " + fastfree + "\n";
		ret += "fastrealloc: " + fastreal + "\n";
		ret += "valloc: " + valloc + "\n";
		ret += "vfree: " + vfree + "\n";
		ret += "new_Object: " + createobj + "\n";
		ret += "~Object: " + delobj + "\n";
		ous <<< ret;
		ous.flush();

		return ret;
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
	OUT <<< "LAF: v0.0, " <<< EOL;
	OUT <<< "using file: " <<< filename <<<", start streaming..." <<< EOL;
	String filter_name = "myfilter";
	
	OutputStream ous;
	ous = new OutputStream("res." + filename, "w");
	StreamAnalyzer ana = new StartsWithStreamAnalyzer();
	los = new LogStream(filename);
	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();
	return 0;
}
