/*
 cgi.k : utilities for writing cgi with konoha
  shinpei_NKT(C)2011

 version:
  0.0.1 : first commit. (shinpei_NKT)
*/

class Cookie {
	Cookie () {

	}
};

class FieldStrage {
	Map storage;

	FieldStorage () {
		storage = {};
	}
	
	void readQuery(String str) {
		int i;
		String[] queries = str.split("=");
		int size = |queries|;
		int range;
		if (size % 2 == 0) {
			range = size /2;
		} else {
			range = (size + 1) / 2;
		}

		for (i = 0; i < range; i++) {
			storage[queries[2 * i]] = queries[2 * i + 1];
		}
	}
}

class Cgi {
	String method;
	FieldStrage fs;
	Cgi () {
		this.method = $env.REQUEST_METHOD;
		fs = new FieldStorage();
		fs.readQuery($env.QUERY_STRING);
	}

}


