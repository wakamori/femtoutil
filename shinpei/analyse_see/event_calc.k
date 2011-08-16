
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
