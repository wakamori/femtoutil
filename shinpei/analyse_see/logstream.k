/* -------------------------------------------
 * LogStream.k
 * shinpei(c)2011
 *
 * ------------------------------------------- */



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
			foreach (String line in _ins) {
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

