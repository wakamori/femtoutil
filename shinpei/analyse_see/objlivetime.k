/* -------------------------------------------
 * objlivetime.k
 *  output live time of each objects
 *  shinpei(c)2011
 *
 * ------------------------------------------- */


class ObjData {
	int oidx;
	String ptr;
	int livetime;
	int cid;
}

CID_MAX = 100;
class ObjectLivetimeStreamAnalyzer extends StreamAnalyzer {
	Map<String,int> livetime;
	Array<String> classnames;
	Map<String, ObjData> map_objdata;
	Array<Array<ObjData>> a_objdata;
	int createcnt;
	int delcnt;

	ObjectLivetimeStreamAnalyzer () {
		name = "objlive";
		description = "analyze obj live span";
		livetime = {};
		classnames = [];
		map_objdata = {};
		createcnt = 0;
		delcnt = 0;
		// init class table
		for (i=0;i<CID_MAX;i++) a_objdata.add([]);
	}

	@Override void analyze (String line) {
		if (line.startsWith("[new]") == true) {
			createcnt++;
			int offset = |"[new] T"|;
			int lastidx = |line| - 1;
			String substr = line[offset to lastidx];
			String[] map = substr.split(",");
			int time = (int)map[0];
			String[] a = map[1].replace(" ", "").split("=");
			String ptr = a[1];
			a  = map[2].replace(" ", "").split("=");
			int cid = (int)a[1];
			if (cid > CID_MAX) {
				print "INVALID CID!!";
			}
			a = map[3].replace(" ", "").split("=");
			int idx = (int)a[1];
			livetime[ptr] = time;

			ObjData odata = new ObjData();
			odata.oidx = idx;
			odata.ptr = ptr;
			odata.cid = cid;
			map_objdata[ptr] = odata;
			return;
		}
		
		if (line.startsWith("[~Object]") == true) {
			delcnt++; 
			int offset = |"[~Object] T"|;
			int lastidx = |line| - 1;
			String substr = line[offset to lastidx];
			String[] map = substr.split(",");
			int deltime = (int)map[0];
			String[] a = map[1].replace(" ", "").split("=");
			String ptr = a[1];
			int newtime = livetime[ptr];
			if (deltime < newtime) {
				print "error!!";
			}
			ObjData odata = (ObjData)map_objdata[ptr];
			odata.livetime = deltime - newtime;

			objarray = a_objdata[odata.cid];
			if (objarray == null) {
				print "ERROR!";
				objarray = [];
				objarray.add(odata);
				a_objdata[odata.cid] = objarray;
			} else {
				objarray.add(odata);
			}
			return;
		}

		if (line.startsWith("[CLASSNAME]") == true) {
			int offset = |"[CLASSNAME] T"|;
			int lastidx = |line| - 1;
			String substr = line[offset to lastidx];
			String[] map = substr.split(",");
			String[] a = map[2].replace(" ", "").split("=");
			String cn = a[1];
			classnames.add(cn);
			return;
		}

	}
	
	@Override void doneStreamAnalyze (OutputStream ous) {
		OUT.println("cc:" + createcnt + ",dc:" + delcnt);
		OUT.println("done analyze, start post-process...");
		foreach (String ptr in map_objdata.keys()) {
			ObjData d =(ObjData) map_objdata[ptr];
			ous <<< d.oidx <<< "," <<< d.livetime <<< "," <<< classnames[d.cid] <<< EOL;
		}
		ous.flush();
	}

	@Override String explanation(void) {
		return """
analyzer for object livetime
""";

	}
}

