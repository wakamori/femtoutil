#!/usr/local/bin/konoha
/* from see2.k */

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


class StreamFilter extends Filter {

}

class SamplingFilter extends Filter {

}

class StreamAnalyzer {
	@Virtual void analyze (String line);
	@Virtual String doneStreamAnalyze(OutputStream ous);
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


class MyStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	@Override void analyze (String line) {
		if (line.search("[fastmalloc]") != -1) {
			fastmal++;
		}

		if (line.search("[fastfree]") != -1) {
			fastfree++;
		}
		if (line.search("[fastrealloc]") != -1) {
			fastreal++;
		}

		if (line.search("[valloc]") != -1) {
			valloc++
		}

		if (line.search("[vfree]") != -1) {
			vfree++;
		}

		if (line.search("[new_Object]") != -1) {
			createobj++;
		}

		if (line.search("[~Object]") != -1) {
			delobj++;
		}
	}
	
	@Override String doneStreamAnalyze (OutputStream ous) {
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

class AnalyzeData;

class LogRawData extends AnalyzeData {
	String event;
	String time;
	int address;
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

class HisStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	LogRawData[] log_array;

	@Override void analyze (String line) {
		/* first make class */
		LogRawData data;

		if (line.indexOf("[fastmalloc] ") != -1) {
			data = new LogRawData("fastmalloc");
			int offset = |"[fastmalloc] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.i_data = (int)(map[2][6 to]);
			fastmal++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[fastfree] ") != -1) {
			data = new LogRawData("fastfree");
			int offset = |"[fastfree] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.i_data = (int)(map[2][6 to]);
			fastfree++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[fastrealloc] ") != -1) {
			data = new LogRawData("realloc");
			int offset = |"[fastrealloc] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.i_data = (int)(map[2][6 to]);
			fastreal++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[valloc]") != -1) {
			data = new LogRawData("valloc");
			int offset = |"[valloc] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.i_data = (int)(map[2][6 to]);
			valloc++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[vfree]") != -1) {
			data = new LogRawData("vfree");
			int offset = |"[vfree] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.i_data = (int)(map[2][6 to]);
			vfree++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[new_Object]") != -1) {
			data = new LogRawData("creatobj");
			int offset = |"[new_Object] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.s_data = (map[2][7 to]);
			createobj++;
			log_array.add(data);
			return;
		}

		if (line.indexOf("[~Object]") != -1) {
			data = new LogRawData("creatobj");
			int offset = |"[~Object] T"|;
			substr = line[offset to];
			map = substr.split(",");
			data.time = map[0][1 to ];
			data.address = (int)(map[1][5 to]);
			data.s_data = (map[2][7 to]);
			delobj++;
			log_array.add(data);
			return;
		}
	}
	
	@Override String doneStreamAnalyze (OutputStream ous) {
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
	if (|args| < 1) {
		OUT <<< "warining!! you need logfile name." <<< EOL;
		OUT <<< "usage : konoha calk.k $(logfile)" <<< EOL;
		return 0;
	}
	String filename = args[0];
	String filter_name = "myfilter";
	ous = new OutputStream("result.txt", "w");
//	Filter filter = new MyFilter(filter_name);
	StreamAnalyzer ana = new MyStreamAnalyzer();
	los = new LogStream(filename);
//	los.addFilter(filter);
	los.addStreamAnalyzer(ana);
	los.setOus(ous);
	los.doFilter();
	los.destroy();
	return 0;
}
