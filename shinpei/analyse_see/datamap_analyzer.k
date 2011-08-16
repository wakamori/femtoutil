class DataMapStreamAnalyzer extends StreamAnalyzer {
	int fastmal;
	int fastfree;
	int fastreal;
	int valloc;
	int vfree;
	int createobj;
	int delobj;

	LogRawData[] log_array;
	Map<int,int> objmap;

	@Override void analyze (String line) {
		/* first make class */
		LogRawData data;
/*		if (line.startsWith("[fastmalloc] ") == true) {
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
		
		if (line.startsWith("[fastfree] ") == true) {
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

		if (line.startsWith("[fastrealloc] ") == true) {
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

		if (line.startsWith("[valloc]") == true) {
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

		if (line.startsWith("[vfree]") == true) {
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
*/
		if (line.startsWith("[new_Object]") == true) {
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

		if (line.startsWith("[~Object]") == true) {
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

		//log_arra  --> malloc map

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
