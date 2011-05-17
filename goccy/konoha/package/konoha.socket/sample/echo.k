using konoha.socket.*;

void main (String[] args) {
	int port = (|args| < 2) ? 12345 : (int) args[1];
	OUT <<< "port=" + port <<< EOL;
	ServerSocket s = new ServerSocket(port, 2);

	while(true) {
		Socket csock = s.accept();
		OUT <<< "accepted" <<< EOL;
		i = csock.getInputStream();
		String str = i.readLine();
		out = csock.getOutputStream();
		OUT <<< str <<< EOL;
		out <<< str <<< EOL;
		out.flush();
		csock.close();
	}
	s.close();
}

