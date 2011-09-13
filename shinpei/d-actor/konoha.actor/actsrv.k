#!/usr/local/bin/konoha

using konoha.socket.*;
using konoha.io.*;

//DEBUG_MODE = true;
DEBUG_MODE = false;

int main (String[] args)
{
	int argc = |args|;
	int port = 2000;
	if (argc > 0) {
		port = (int)args[0];
	}
	int ch;
	string line = '';
	OUT <<< "port: " + port <<< EOL;
	ServerSocket ss = new ServerSocekt(port, 3);

	while(true) {
		Socket conn = ss.accept();
		OUT << "accepted" << EOL;
		ins = conn.getInputStream();
		String scr = ins.readMsgPack();
		if (DEBUG_MODE) 
			OUT << "(DEBUG)received script: '" << scr << "'"<< EOL;
		System.eval(scr);

	}
	OUT << "connection has closed" << EOL;
}
