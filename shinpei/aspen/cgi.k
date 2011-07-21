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

class FieldStorage {
	Map storage;

	FieldStorage () {
		storage = {};
	}
	
	void readQuery(String str) {
		int i;
		String[] queries = str.split("=");
		OUT <<< str <<< "<BR>" <<< EOL;
		int size = |queries|;
		int range;
		if (size == 1) { return ; }
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
	String requestMethod;
	FieldStorage fs;
	Boolean modeCgi;
	int contentLength;

	Cgi () {
		if ($env.REQUEST_METHOD != null) {
			this.modeCgi = true;
			this.requestMethod = $env.REQUEST_METHOD;
			fs = new FieldStorage();
			fs.readQuery($env.QUERY_STRING);
		} else {
			modeCgi = false;
		}
	}

	void printEnvironment (void ) {
		// from http://rfs.jp/sb/perl/03/05.html
		String printHTML = "";
		str = "";
		OUT <<< "Environment variables <br>" <<< EOL;
		// Webserver
		str += "<hr>Webserver environment variables<br />";
		str += "<p>GATEWAY_INTERFACE=" + $env.GATEWAY_INTERFACE + "</p>";
		str += "<p>SERVER_ADMIN=" + $env.SERVER_ADMIN + "</p>";
		str += "<p>SERVER_NAME=" + $env.SERVER_NAME + "</p>";
		str += "<p>SERVER_PORT=" + $env.SERVER_PORT + "</p>";
		str += "<p>SERVER_PROTOCOL=" + $env.SERVER_PROTOCOL + "</p>";
		str += "<p>SERVER_SOFTWARE=" + $env.SERVER_SOFTWARE + "</p>";
		// CGI
		str += "<hr>CGI environment variables<br />";
		str += "<p>CONTENT_LENGTH=" + $env.CONTENT_LENGTH + "</p>";
		str += "<p>CONTENT_TYPE=" + $env.CONTENT_TYPE + "</p>";
		str += "<p>DOCUMENT_ROOT=" + $env.DOCUMENT_ROOT + "</p>";
		str += "<p>PATH=" + $env.PATH + "</p>";
		str += "<p>PATH_INFO=" + $env.PATH_INFO + "</p>";
		str += "<p>PATH_TRANSLATED=" + $env.PATH_TRANSLATED + "</p>";
		str += "<p>QUERY_STRING=" + $env.QUERY_STRING + "</p>";
		str += "<p>REQUEST_METHOD=" + $env.REQUEST_METHOD + "</p>";
		str += "<p>SCRIPT_FILENAME=" + $env.SCRIPT_FILENAME + "</p>";
		str += "<p>SCRIPT_NAME=" + $env.SCRIPT_NAME + "</p>";
		// Client
		str += "<hr>Client environment variables<br />";
		str += "<p>AUTH_TYPE=" + $env.AUTH_TYPE + "</p>";
		str += "<p>PATH_TRANSLATED=" + $env.PATH_TRANSLATED + "</p>"; // ??
		str += "<p>HTTP_FROM=" + $env.HTTP_FROM + "</p>";
		str += "<p>HTTP_ACCEPT=" + $env.HTTP_ACCEPT + "</p>";
		str += "<p>HTTP_USER_AGENT=" + $env.HTTP_USER_AGENT + "</p>";
		str += "<p>HTTP_REFERER=" + $env.HTTP_REFERER + "</p>";
		str += "<p>REMOTE_ADDR=" + $env.REMOTE_ADDR + "</p>";
		str += "<p>REMOTE_HOST=" + $env.REMOTE_HOST + "</p>";
		str += "<p>REMOTE_IDENT=" + $env.REMOTE_IDENT + "</p>";
		str += "<p>REMOTE_USER=" + $env.REMOTE_USER + "</p>";

		OUT <<< str <<< EOL;
	}


	void encodeHTTPString (String str) {

	}

	String decodeHTTPString (String str) {

	}
}
