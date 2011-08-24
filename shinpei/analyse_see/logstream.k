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
	String name;
	String description;

	@Virtual void analyze (String line);
	@Virtual void doneStreamAnalyze(OutputStream ous);
	@Virtual String explanation();
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
		StreamAnalyzer a = _analyzers[0];
		String ret, line;
		OUT.println("analyzer: " + a.name);
		OUT.println("explanation: " + a.description);
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

