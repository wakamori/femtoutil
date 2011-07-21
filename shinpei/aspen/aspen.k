#!/usr/local/bin/konoha
/*
 aspen.k : small konoha invoke script
  shinpei_NKT(C)2011

 version:
  0.0.1 : first commit. (shinpei_NKT)
*/

include "cgi.k";

class Aspen {
	String query;

	Aspen() {
		quert = "";
	}

	void run (String method, String command, Map query) {
		if (method == "GET") {
			print command;
			switch (command) {
			case "login":
				onLogin(query);
				break;
			}
		} else if (method == "POST") {
			OUT <<< "POST" <<< EOL
			switch (command) {
			case "login":
				onLogin(query);
				break;
			}
		}
	}

	void onLogin(Map q) {
		OUT <<< q["password"] <<< EOL;

	}
}

int main (String[] args)
{
	OUT <<< "Content-Type: text/html;charset=UTF-8" <<< EOL;
	OUT <<< EOL;
	Cgi cgi = new Cgi();
	cgi.printEnvironment();
	aspen = new Aspen();
	if (cgi.modeCgi) {
		OUT <<< cgi.fs.storage["method"] <<< EOL;
		aspen.run(cgi.requestMethod, cgi.fs.storage["method"], cgi.fs.storage);
	}
	return 0;
}
