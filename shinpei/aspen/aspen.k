#!/usr/local/bin/konoha
/*
 aspen.k : small konoha invoke script
  shinpei_NKT(C)2011

 version:
  0.0.1 : first commit. (shinpei_NKT)
*/

include "cgi.k"

int main (String[] args)
{
	OUT <<< "Content-Type: text/html;charset=UTF-8" <<< EOL;
	OUT <<< EOL;
	Cgi cgi = new Cgi();
	OUT <<< cgi.method <<< EOL;
	print cgi.fs.storage;
//	OUT <<< cgi.fs.storage["method"] <<< EOL;
	return 0;
}
