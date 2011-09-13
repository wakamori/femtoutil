#!/usr/local/bin/konoha

using konoha.socket.*;
using konoha.io.*;

int main (String[] args)
{
	int port = 2000;
	int ch;
	string line = '';
	OUT <<< "port: " + port <<< EOL;
	ServerSocket ss = new ServerSocekt(port, 3);

	while(true) {
		Socket conn = ss.accept();
		OUT << "accepted" << EOL;
		ins = conn.getInputStream();
		String scr = ins.readMsgPack();
		OUT << "received script: '" << scr << "'"<< EOL;
		System.eval(scr);

	}
	OUT << "connection has closed" << EOL;
}
