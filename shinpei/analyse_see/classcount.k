/* -------------------------------------------
 * classcount.k
 *  count each class usage
 *  shinpei(c)2011
 *
 * ------------------------------------------- */

class ClassCountStreamAnalyzer extends StreamAnalyzer {
	int createobj;
	int delobj;

	LogRawData[] log_array;
	Map<String,int> livetime;
	Map<String, int> classcount;
	int live_sumtime;
	String[] livelog;
	String[] livelog_with_class;
	Map<String, int> live_sumtime_class;

	Array<String> classnames;

	ClassCountStreamAnalyzer () {
		name ="classcount";
		description = "count class usage";
		livetime = {};
		classcount = {};
		livelog = [];
		livelog_with_class = [];
		live_sumtime = 0;
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
			live_sumtime_class[data.s_data] += livetime[data.s_address];
			livelog_with_class.add("class:" + data.s_data + "," + str);
			return;
		}
		if (line.startsWith("[CLASSNAME]") == true) {
			int offset = |"[CLASSNAME] T"|;
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
}
