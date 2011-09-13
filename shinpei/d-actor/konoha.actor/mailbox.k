using konoha.io.*;
using konoha.socket.*;


String[] inbox;
// we only suppose 
// mailbox.k $(port)
void main (String[] args)
{
	int argc = |args|;
	int port = 2000;
	if (argc > 0) {
		port = (int)args[0];
	}

	ServerSocket ss = new ServerSocket(port, 3);
	while (true) {
		Socket conn = ss.accept();
		InputStream ins = conn.getInputStream();
		String msg = ins.readMsgPack();
		OUT << msg << EOL;
	}
}

