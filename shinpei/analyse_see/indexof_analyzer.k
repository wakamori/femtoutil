
class IndexOfStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	@Override void analyze (String line) {
		if (line.indexOf("[fastmalloc]") != -1) {
			fastmal++;
		}

		if (line.indexOf("[fastfree]") != -1) {
			fastfree++;
		}
		if (line.indexOf("[fastrealloc]") != -1) {
			fastreal++;
		}

		if (line.indexOf("[valloc]") != -1) {
			valloc++
		}

		if (line.indexOf("[vfree]") != -1) {
			vfree++;
		}

		if (line.indexOf("[new_Object]") != -1) {
			createobj++;
		}

		if (line.indexOf("[~Object]") != -1) {
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
