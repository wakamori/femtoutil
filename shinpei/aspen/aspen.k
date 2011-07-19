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

	void run (String method, String command) {
		if (method == "GET") {
			print command;
			switch (command) {
			case "login":
				OUT <<< "hi" <<< EOL;
				break;

			}
		} else if (method == "POST") {
			print command;
			switch (command) {
			case "login":
				OUT <<< "hi" <<< EOL;
				break;
			}
		}
	}
}

int main (String[] args)
{
	OUT <<< "Content-Type: text/html;charset=UTF-8" <<< EOL;
	OUT <<< EOL;
	Cgi cgi = new Cgi();
	aspen = new Aspen();
	OUT <<< cgi.fs.storage["method"] <<< EOL;
	aspen.run(cgi.method, cgi.fs.storage["method"]);
	return 0;
}
